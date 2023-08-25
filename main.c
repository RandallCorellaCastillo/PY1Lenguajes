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

int validateStringOnDate(char*);
void saveLoan(char*, int , char*, char*);
int checkNameUsers(char*);
void simpleSeek();
void loan();
int validateDate(char*);
int getIdloan();
char* getDateLoan();
char* getNameLoan();


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


void generalMenu() {
    int option;
    printf("Opciones Generales\n");
    printf("1. Búsqueda simple\n");
    printf("2. Búsqueda avanzada\n");
    printf("3. Préstamo\n");
    printf("4. Devolución\n");
    printf("5. Volver\n");
    printf("Ingrese su opcion: ");
    scanf("%d", &option);
    printf("\n");
    getchar(); 

    if (option == 1) {
        simpleSeek();
        generalMenu();
        printf("\n");

    } else if (option == 2) {

    } else if (option == 3) {
        loan();
    } else if (option == 4) {
        //loan();
    } else if (option == 5) {
        mainMenu();
    } else {
        printf("Opcion incorrecta, intentalo de nuevo\n");
        getchar(); 
        fflush(stdin);
        return generalMenu();
    };
    
};


void loan() {
    fflush(stdin);
    char userName[100];
    char Sdate[100];
    char Edate[100];
    int id;
    //Name
    strcpy(userName, getNameLoan());
    //ID
    id = getIdloan();

    strcpy(Sdate, getDateLoan("inicio"));
    strcpy(Edate, getDateLoan("finalizacion"));

    struct tm fecha1 = {0}; //
    struct tm fecha2 = {0}; // 
    // Utilizamos sscanf para extraer los valores numéricos de la cadena
    
    sscanf(Sdate, "%d/%d/%d", &fecha1.tm_mday, &fecha1.tm_mon, &fecha1.tm_year);
    sscanf(Edate, "%d/%d/%d", &fecha2.tm_mday, &fecha2.tm_mon, &fecha2.tm_year);

    fecha1.tm_mon -= 1; // Ajustamos el mes a 0-11
    fecha1.tm_year -= 1900; // Ajustamos el año a contar desde 1900
    
    fecha2.tm_mon -= 1;
    fecha2.tm_year -= 1900;
    
    time_t tiempo_fecha1 = mktime(&fecha1);
    time_t tiempo_fecha2 = mktime(&fecha2);

    if(tiempo_fecha1 < tiempo_fecha2) {printf("Prestamo realizado con exito.\n");}
    else {
        printf("las fechas de inicio no debe ser mayor o igual a la de finalizacion.");
    }
    saveLoan(userName, id, Sdate, Edate);
    modifyCatalogDisp(id);
    return generalMenu();
    

}

char* getNameLoan() {
    char userName[100];
    printf("Ingrese el nombre de usuario:\n");
    fgets(userName, sizeof(userName), stdin); // Lee la cadena con espacios
    fflush(stdin);

    userName[strlen(userName) - 1] = '\0';

    if (checkNameUsers(userName) != 1) { 
        printf("\nEl usuario no existe.\n");
        return getNameLoan();
    }
    char* res;
    int largo = strlen(userName);
    res = (char*) malloc (largo * sizeof(char));
    strcpy(res, userName);

    return res;
}

int getIdloan() {
    fflush(stdin);
    int id;

    printf("\nIngrese el id del libro que solicita:\n");
    if (scanf("%d", &id) != 1) {
        printf("No ingreso un numero valido.\n");
        return getIdloan();
    }

    if(sizeOfCatalog() < id) {
        printf("El ejemplar no existe\n");
        return getIdloan();
    }

    if(!verifyDisp(id)) {
        printf("El ejemplar seleccionado esta actualmente ocupado.\n");
        return getIdloan();
    }

    fflush(stdin);
    return id;
}

char* getDateLoan(char* info) {
    char date[100];
    fflush(stdin);
    //date
    printf("\nIngrese la fecha de %s con el formato 'dd/mm/yyyy':\n", info);
    fgets(date, sizeof(date), stdin); // Lee la cadena con espacios

    date[strlen(date) - 1] = '\0';
    if(!validateStringOnDate(date)){
        printf("La fecha no puede contener caracteres.");
        return getDateLoan(info);
    }
    if(!validateDate(date)) {printf("\nFecha invalida.\n");return getDateLoan(info);}
    //clear
    
    char* res;
    int largo = strlen(date);
    res = (char*) malloc (largo * sizeof(char));
    strcpy(res, date);
    fflush(stdin);
    return res;
}



//C:\\Users\\fredd\\Downloads\\test.txt