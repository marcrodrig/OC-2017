section .data
	salto_linea db 10
	mostrarSeparador db ": "
	largoMostrarSeparador equ $-mostrarSeparador
	modoMostrarAyuda db 'Sintaxis: enum [-h] | archivo_entrada [archivo_salida]',10,'-h: muestra ayuda',10,'archivo_entrada: ruta al archivo de entrada',10,'archivo_salida: ruta al archivo de salida',10,0
	largoModoAyuda equ $-modoMostrarAyuda
	mostrarLineas db 10,10,"Cantidad de líneas: "
	largoMostrarLineas equ $-mostrarLineas
	descriptor_entrada dd 1
	descriptor_salida dd 1

section .bss
;reservo un buffer de 1 byte para recorrer el archivo
	buffer resb 1

section .text
 global _start

;Comienzo del programa, tomo los argumentos y los comienza a analizar.
_start:
        pop ecx ; ecx contiene a ARGC, número de argumentos
        cmp ecx, 3
        jg errorParametros ; Si ingresan mas de 4 parametros hay un error de cantidad de argumentos
        cmp ecx, 2
        jl errorParametros
        jmp evaluar ;caso en donde tengo 2 o 3 argumentos

;Dependiendo del parámetro ingresado, irá a la respectiva acción
;si ingresa -h , irá a modoAyuda
;si ingresa archivo_entrada, irá a abrirArchivoEntrada
evaluar:
        pop ebx ; nombre del programa
        pop ebx ; argumento
        ;evaluo el argumento para determinar acción
        push ebx
        mov al, byte[ebx] ; primer caracter del argumento
        cmp al, '-'
        jne abrirArchivoEntrada
        inc ebx
        mov al, byte[ebx]
        cmp al, 'h'
	    je modoAyuda
        jmp errorParametros

;Terminación anormal por error en los parámetros.
errorParametros:
        mov eax, 1 ; sys_exit
        mov ebx, 3 ; terminación con error
        int 80h    ; invovación del servicio del SO

;write:
;@param: ecx: referencia al buffer que aloja los datos a escribir
;@param: edx: largo en bytes a escribir
;@param ebx: destino
;@return: eax: bytes escritos
write:
       mov eax, 4 ; indica syscall a realizar (sys_write)
       mov ebx, [descriptor_salida]
       int 80h
       ret

;open:
;@param: ebx: nombre del archivo
;@return eax: descriptor del archivo, -1 si no se lo puedo abrir
open:
       mov eax, 5 ; open
       mov ecx, 0x00000000 ; read mode, O_RDONLY (se abre para solo lectura)
       int 80h
       ret

abrirArchivoEntrada:
        push ecx
        call open
        pop ecx
        cmp eax,0
        jle errorArchivoEntrada
        mov [descriptor_entrada],eax ;en eax estaba el descriptor del archivo de entrada
        mov [buffer], byte 0
    	mov esi, 1 ;contador de líneas
        cmp ecx, 3
        je crearArchivoSalida
    	jmp procesar

crearArchivoSalida:
        pop ebx ;desapilo archivo_entrada
        pop ebx ;archivo_salida
        mov eax, 8 ; creat
        mov ecx, 00400 ;S_IRUSR (S_IREAD): 00400 el usuario tiene permiso de lectura
        int 80h
        cmp eax, 0
        jle errorArchivoSalida
        mov [descriptor_salida], eax ;en eax estaba el descriptor del archivo de salida
        jmp procesar

modoAyuda:
	mov ecx, modoMostrarAyuda
	mov edx, largoModoAyuda
	call write
	mov ebx, 0 ;Terminación normal
	mov eax,1
	int 80h

;Terminación anormal por error en el archivo de entrada.
errorArchivoEntrada:
        mov eax, 1 ; sys_exit
        mov ebx, 1 ; terminación con error
        int 80h    ; invocación del servicio del SO

;Terminación anormal por error en el archivo de salida.
errorArchivoSalida:
        mov eax, 1 ; sys_exit
        mov ebx, 2 ; terminación con error
        int 80h    ; invocación del servicio del SO

terminar:
	mov ecx, mostrarLineas
    mov edx, largoMostrarLineas
	call write
	jmp procesar

success:
        mov ecx, salto_linea ; salto de línea
        mov edx, 1
        call write

        mov ebx,0  ;Terminación normal
        mov eax,1
        int 80h

analizarCaracter:
	cmp edi, 1 ;si es fin de archivo, no evaluar caracteres
	jz success
	mov eax,3 	;read
	mov ebx, [descriptor_entrada]
	mov ecx ,buffer
	mov edx,1 ;largo del buffer
	int 80h
	push eax
	push ebx
	push edx
	cmp eax, 0 ;chequeo fin del archivo
	jz finDeArchivo

	call write
	pop edx
	pop ebx
	pop eax
	cmp [buffer], byte 10 ;chequeo salto de línea
	jnz analizarCaracter
	je nuevaLinea

nuevaLinea:
        inc esi ; incrementa el contador de líneas
        jmp procesar

finDeArchivo:
	mov edi,1 ;indica fin de archivo
	mov eax,6 	;close
	mov ebx,[descriptor_entrada]
	int 80h
	jmp terminar

procesar:
	mov eax, esi  ; cant lineas
	push byte 36	;Apilo un caracter delimitador ($)

;Rutinas apilar y desapilar para mostrar en base decimal el registro eax que contiene la cantidad de líneas
apilar:
	mov edx, 0
	mov ecx, 10
	div ecx
	add edx,48
	push edx
	cmp eax,0
	jnz apilar

desapilar:
	mov ecx,esp
    mov edx,1
	call write
	pop ecx
	cmp [esp], byte 36	;chequeo  con el delimitador '$'
	jz escribirSeparador
	jmp desapilar

escribirSeparador:
	cmp edi,1
	je success
	mov ecx, mostrarSeparador
	mov edx, largoMostrarSeparador
	call write
	jmp analizarCaracter
