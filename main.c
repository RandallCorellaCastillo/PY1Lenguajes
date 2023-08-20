#include <stdio.h>
#include "cJSON.h"
#include <stdlib.h>
#include <string.h>

//Declaracion de funciones
void mainMenu();
void operativeMenu();
void generalMenu();


//Inicio del programa
int main() {
    mainMenu();
    return 0;
};

void mainMenu() {
    int option;

    printf("1. Menu operativo\n");
    printf("2. Menu general\n");
    printf("3. Salir\n");
    printf("Ingrese su opcion: ");
    scanf("%d", &option);
    printf("\n");


    if (option == 1) {
        return operativeMenu();
    } else if (option == 2) {
        return generalMenu();
    } else if (option == 3) {
        return;
    } else {
        printf("Opcion incorrecta, intentalo de nuevo\n");
    }
};

void operativeMenu() {
    return;
};

void generalMenu() {
    return;
};