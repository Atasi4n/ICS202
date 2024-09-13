#include <stdio.h>

int var1, var2;

// Prototipos de las funciones
int suma(int var1, int var2);
int resta(int var1, int var2);
int mult(int var1, int var2);
int div(int var1, int var2);

int main() {
    // Imprime un mensaje en la consola
    printf("Esta es una calculadora\n");

    // Inicializa las variables
    var1 = 6;
    var2 = 2;

    // Imprime los resultados de las operaciones
    printf("%d mas %d = %d\n", var1, var2, suma(var1, var2));
    printf("%d menos %d = %d\n", var1, var2, resta(var1, var2));
    printf("%d por %d = %d\n", var1, var2, mult(var1, var2));
    printf("%d entre %d = %d\n", var1, var2, div(var1, var2));

    return 0;  // Indica que el programa terminó exitosamente
}

// Funciones para las operaciones
int suma(int var1, int var2) {
    return var1 + var2;
}

int resta(int var1, int var2) {
    return var1 - var2;
}

int mult(int var1, int var2) {
    return var1 * var2;
}

int div(int var1, int var2) {
    return var1 / var2;
}

