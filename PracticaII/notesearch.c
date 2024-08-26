#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>  // Para funciones como getuid(), open(), read(), lseek(), close()
#include "hacking.h"

#define FILENAME "/var/notes"

// Prototipos de funciones
int print_notes(int, int, char *);
int find_user_note(int, int);
int search_note(char *, char *);
void fatal(char *);

int main(int argc, char *argv[]) {
    int userid, printing = 1, fd; 
    char searchstring[100];
    
    if (argc > 1) {                   
        // Si hay un argumento,
        strcpy(searchstring, argv[1]); // este es el string de búsqueda
    } else {
        // si no,
        searchstring[0] = 0;           // la cadena de búsqueda está vacía
    }
    
    userid = getuid();
    fd = open(FILENAME, O_RDONLY);     // Abrir el archivo para lectura
    if (fd == -1)
        fatal("en main() al abrir el archivo para lectura");

    while (printing)
        printing = print_notes(fd, userid, searchstring);

    printf("-------[ end of note data ]-------\n");
    close(fd);

    return 0;
}

// Función para imprimir las notas para un UID dado que coincidan
// con una cadena de búsqueda opcional; 
// devuelve 0 al final del archivo, 1 si aún hay más notas.
int print_notes(int fd, int uid, char *searchstring) {
    int note_length;
    char note_buffer[100];

    note_length = find_user_note(fd, uid);
    if (note_length == -1)  // Si se alcanzó el final del archivo,
        return 0;           // salir

    read(fd, note_buffer, note_length); // Leer datos de la nota
    note_buffer[note_length] = 0;       // Terminar la cadena

    if (search_note(note_buffer, searchstring)) // Si se encuentra la cadena de búsqueda
        printf("%s", note_buffer);              // imprimir la nota

    return 1;  // Indica que hay más notas para imprimir
}

// Función para encontrar la siguiente nota para un userID dado;
// devuelve -1 si se alcanza el final del archivo;
// de lo contrario, devuelve la longitud de la nota encontrada.
int find_user_note(int fd, int user_uid) {
    int note_uid = -1;
    unsigned char byte;
    int length = 0;

    while (note_uid != user_uid) {  // Bucle hasta encontrar una nota para user_uid
        if (read(fd, &note_uid, 4) != 4) // Leer datos del UID
            return -1;  // Si no se leen 4 bytes, devolver código de fin de archivo

        if (read(fd, &byte, 1) != 1) // Leer el separador de nueva línea
            return -1;

        length = 0;
        while (byte != '\n') {  // Calcular cuántos bytes hasta el final de la línea
            if (read(fd, &byte, 1) != 1) // Leer un solo byte
                return -1;  // Si no se lee el byte, devolver código de fin de archivo
            length++;
        }
    }

    lseek(fd, length * -1, SEEK_CUR); // Rebobinar la lectura del archivo en la longitud de bytes
    printf("[DEBUG] found a %d byte note for user id %d\n", length, note_uid);
    return length;
}

// Función para buscar una nota por una palabra clave;
// devuelve 1 si se encuentra una coincidencia, 0 si no se encuentra.
int search_note(char *note, char *keyword) {
    int i, keyword_length, match = 0;
    keyword_length = strlen(keyword);

    if (keyword_length == 0)  // Si no hay cadena de búsqueda,
        return 1;              // siempre "coincide"

    for (i = 0; i < strlen(note); i++) { // Iterar sobre los bytes en la nota
        if (note[i] == keyword[match])  // Si el byte coincide con la palabra clave,
            match++;   // prepararse para verificar el siguiente byte
        else {        // de lo contrario,
            if (note[i] == keyword[0]) // si ese byte coincide con el primer byte de la palabra clave,
                match = 1;  // comenzar el conteo de coincidencias en 1
            else
                match = 0;  // De lo contrario, es cero
        }

        if (match == keyword_length) // Si hay una coincidencia completa,
            return 1;   // devolver coincidente
    }

    return 0;  // Devolver no coincidente
}

void fatal(char *message) {
    perror(message);
    exit(1);
}
