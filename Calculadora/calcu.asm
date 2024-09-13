section .data
    msg_suma db "Suma: ", 0
    msg_resta db "Resta: ", 0
    msg_mult db "Multiplicacion: ", 0
    fmt db "%d", 0xA, 0            ; Formato para imprimir enteros + salto de línea
    len_fmt equ $ - fmt             ; Longitud del formato
    len_msg_suma equ $ - msg_suma   ; Longitud del mensaje de suma
    len_msg_resta equ $ - msg_resta ; Longitud del mensaje de resta
    len_msg_mult equ $ - msg_mult   ; Longitud del mensaje de multiplicación

section .bss
    buffer resb 10                 ; Buffer para imprimir resultados

section .text
    global _start

_start:
    ; Llamada a la función suma
    mov eax, 6                     ; Cargar var1 en eax
    mov ebx, 2                     ; Cargar var2 en ebx
    add eax, ebx                  ; Sumar eax y ebx (eax = var1 + var2)
    call int_to_string             ; Convertir el resultado a cadena
    mov eax, 4                    ; syscall: sys_write
    mov ebx, 1                    ; file descriptor: stdout
    mov ecx, msg_suma             ; mensaje
    mov edx, len_msg_suma         ; longitud del mensaje
    int 0x80                      ; llamada al sistema
    mov ecx, buffer               ; resultado
    mov edx, eax                  ; longitud del resultado
    int 0x80                      ; llamada al sistema

    ; Llamada a la función resta
    mov eax, 6                     ; Cargar var1 en eax
    mov ebx, 2                     ; Cargar var2 en ebx
    sub eax, ebx                  ; Restar ebx de eax (eax = var1 - var2)
    call int_to_string             ; Convertir el resultado a cadena
    mov eax, 4                    ; syscall: sys_write
    mov ebx, 1                    ; file descriptor: stdout
    mov ecx, msg_resta            ; mensaje
    mov edx, len_msg_resta        ; longitud del mensaje
    int 0x80                      ; llamada al sistema
    mov ecx, buffer               ; resultado
    mov edx, eax                  ; longitud del resultado
    int 0x80                      ; llamada al sistema

    ; Llamada a la función multiplicación
    mov eax, 6                     ; Cargar var1 en eax
    mov ebx, 2                     ; Cargar var2 en ebx
    imul eax, ebx                 ; Multiplicar eax y ebx (eax = var1 * var2)
    call int_to_string             ; Convertir el resultado a cadena
    mov eax, 4                    ; syscall: sys_write
    mov ebx, 1                    ; file descriptor: stdout
    mov ecx, msg_mult             ; mensaje
    mov edx, len_msg_mult         ; longitud del mensaje
    int 0x80                      ; llamada al sistema
    mov ecx, buffer               ; resultado
    mov edx, eax                  ; longitud del resultado
    int 0x80                      ; llamada al sistema

    ; Salida del programa
    mov eax, 1                    ; syscall: sys_exit
    xor ebx, ebx                  ; código de salida 0
    int 0x80                      ; llamada al sistema

; Convertir un entero a una cadena
int_to_string:
    mov ecx, buffer               ; dirección del buffer
    mov edx, 10                   ; divisor
    xor ebx, ebx                  ; Limpiar ebx (divisor)
    div edx                       ; dividir eax entre 10
    add dl, '0'                   ; convertir el residuo a carácter ASCII
    mov [ecx], dl                ; guardar el carácter en el buffer
    inc ecx                      ; mover el puntero del buffer
    test eax, eax                ; comprobar si hay más dígitos
    jnz int_to_string            ; repetir si es necesario
    mov byte [ecx], 0            ; agregar el carácter nulo al final
    ret
