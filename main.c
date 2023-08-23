#include "Tools.c"

//gcc -o main main.c cJSON/cJSON.c -I cJSON -lm

//Declaracion de funciones
void mainMenu();
void operativeMenu();
void generalMenu();
void userManagement();
int numberOnString(char*);
void catalogManagement();
int processTokens(char*);


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

    getchar();

    if (option == 1) {
        return operativeMenu();
    } else if (option == 2) {
        return generalMenu();
    } else if (option == 3) {
        return;
    } else {
        printf("Opcion incorrecta, intentalo de nuevo\n");
        getchar();  
        return mainMenu();
    };
};

void operativeMenu() {
    int option;

    printf("MENU OPERATIVO\n");
    printf("1. Gestion de catalogo\n");
    printf("2. Gestion de usuarios\n");
    printf("6. Volver\n");
    printf("Ingrese su opcion: ");
    scanf("%d", &option);
    printf("\n");

    getchar(); 

    if (option == 1) {
        return catalogManagement();
    } else if (option == 2) {
        return userManagement();
    } else if (option == 6) {
        return mainMenu();
    } else {
        printf("Opcion incorrecta, intentalo de nuevo\n");
        getchar(); 
        return operativeMenu();
    };
};

void generalMenu() {
    return;
};


void userManagement() {
    char name[100];
    char id[100];

    printf("Ingresa la cedula de tu nuevo usuario: ");
    fgets(id, 100, stdin);

    id[strlen(id) - 1] = '\0';

    if (strlen(id) != 7 || numberOnString(id) == 0) { 
        printf("\nERROR: tu cedula debe tener !7! !NUMEROS!\n");
        return userManagement();
    };


    printf("\nIngresa el nombre de tu nuevo usuario: ");
    fgets(name, 100, stdin);

    name[strlen(name) - 1] = '\0';

    if (correctString(name) == 0) {
        printf("\nERROR: tu nombre no puede contener numeros y debe contener espacios!!\n");
        return userManagement();
    };

    if (checkUsers(id) == 0) {
        printf("\nERROR: su cedula coincide con un usuario registrado\n");
        return userManagement();
    };

    saveUsers(id, name);

    return operativeMenu();
};

void catalogManagement() {
    char url[200];
    printf("Ingresa la direccion de tu archivo: ");
    fgets(url, 200, stdin);
    url[strlen(url) - 1] = '\0';
    FILE *fp = fopen(url, "r");

    if (fp == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return operativeMenu();
    };

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    char buffer[size];
    fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    const char delimiter[] = "\n";
    char *token = strtok(buffer, delimiter);

    int flag = 0;

    while (token != NULL) {
        char *temp = (char *)malloc(strlen(token) + 1);
        strcpy(temp, token);

        if (processTokens(temp)) {
            flag = 1;
        };

        free(temp);
        token = strtok(NULL, delimiter);
    };

    if (flag == 1) {
        printf("\nLibros que cumplan los parametros agregados con exito\n\n");
    } else {
        printf("\nNingun libro cumplio los parametros, no se actualizo el catalogo\n\n");
    };

    return operativeMenu();
};


//C:\\Users\\fredd\\Downloads\\test.txt