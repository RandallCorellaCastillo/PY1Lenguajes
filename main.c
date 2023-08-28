#include "Tools.c"

//gcc -o main main.c cJSON/cJSON.c -I cJSON -lm
//C:\\Users\\fredd\\Downloads\\test.txt

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
void viewLoan();
void loanMaturity();
void giveBackBook();


void advancedSeek();

void advanceSeek_C(char* info, char* typeOY, int flagName, int flagAuthor, int flagSum);
void advanceSeek_E(char* info, char* typeOY, int flagName, int flagAuthor, int flagSum);

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
    printf("3. Historial de prestamos\n");
    printf("4. Vencimiento de prestamos\n");
    printf("6. Volver\n");
    printf("Ingrese su opcion: ");
    scanf("%d", &option);
    printf("\n");

    getchar(); 
    if (option == 1) {
        return catalogManagement();
    } else if (option == 2) {
        return userManagement();
    } else if (option == 3) {
        return viewLoan();
    } else if (option == 4) {
        return loanMaturity();
    } 
    else if (option == 6) {
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
        advancedSeek();
    } else if (option == 3) {
        loan();
    } else if (option == 4) {
        giveBackBook();
    } else if (option == 5) {
        mainMenu();
    } else {
        printf("Opcion incorrecta, intentalo de nuevo\n");
        getchar(); 
        fflush(stdin);
        return generalMenu();
    };
    
};

/*
loan
This option is responsible to create new loan.

@return return generalMenu()
*/

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
        printf("las fechas de inicio no debe ser mayor o igual a la de finalizacion.\n");
        return loan();

    }
    saveLoan(userName, id, Sdate, Edate);
    modifyCatalogDisp(id);
    return generalMenu();
    

}

/*
getNameLoan
This option is responsible to valid username.

@return return username if valid, otherwise return the same funct.
*/

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

/*
getIdloan

This option is responsible to valid book id of loan.

@return return id if valid, otherwise return the same funct.
*/
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

/*
getDateLoan
This option is responsible to valid date.

@param info type of date, "start" or "end"
This option is responsible to valid date.

@return return date if valid, otherwise return the same funct.
*/

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


/*
view Loan
*
This option is responsible display the loans info.

@return return is none, info is displayed on console.
*/


void viewLoan() {

    char Sdate[100];
    char Edate[100];
    strcpy(Sdate, getDateLoan("inicio"));
    strcpy(Edate, getDateLoan("finalizacion"));
    //calcular las fechas en valores numéricos.
    struct tm fecha1 = {0}; //
    struct tm fecha2 = {0}; // 
    struct tm fecha3 = {0}; // 
    // Utilizamos sscanf para extraer los valores numéricos de la cadena
    
    sscanf(Sdate, "%d/%d/%d", &fecha1.tm_mday, &fecha1.tm_mon, &fecha1.tm_year);
    sscanf(Edate, "%d/%d/%d", &fecha2.tm_mday, &fecha2.tm_mon, &fecha2.tm_year);

    fecha1.tm_mon -= 1; // Ajustamos el mes a 0-11
    fecha1.tm_year -= 1900; // Ajustamos el año a contar desde 1900
    
    fecha2.tm_mon -= 1;
    fecha2.tm_year -= 1900;
    
    time_t tiempo_fecha1 = mktime(&fecha1);
    time_t tiempo_fecha2 = mktime(&fecha2);

    if(!(tiempo_fecha1 < tiempo_fecha2)) {
        printf("las fechas de inicio no debe ser mayor o igual a la de finalizacion.\n");
        return viewLoan();
    }

    //tiempo_fecha1 < tiempo_fecha2
    char* buffer = readJson("prestamos.json");
    //open the json object.
    cJSON *json = cJSON_Parse(buffer);
    int arraySize = cJSON_GetArraySize(json);
    //for each item in json.
    printf("====================================================Prestamos=========================================================.\n");
    for (int i = 0; i < arraySize; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);

        cJSON *id = cJSON_GetObjectItemCaseSensitive(item, "id");
        //id
        cJSON *usuario = cJSON_GetObjectItemCaseSensitive(item, "usuario");
        //Titulo.
        cJSON *idE = cJSON_GetObjectItemCaseSensitive(item, "idEjemplar");
        //author.
        cJSON *start = cJSON_GetObjectItemCaseSensitive(item, "inicio");
        //year
        cJSON *end = cJSON_GetObjectItemCaseSensitive(item, "fin");
        //gender
        cJSON *state = cJSON_GetObjectItemCaseSensitive(item, "estado");

        sscanf(end->valuestring, "%d/%d/%d", &fecha3.tm_mday, &fecha3.tm_mon, &fecha3.tm_year);

        fecha3.tm_mon -= 1; // Ajustamos el mes a 0-11
        fecha3.tm_year -= 1900; // Ajustamos el año a contar desde 1900
        time_t tiempo_fecha3 = mktime(&fecha3);

        if(tiempo_fecha3 >= tiempo_fecha1 && tiempo_fecha3 <= tiempo_fecha2) {
            char stateR[100];
            if(state->valueint == 1) strcpy(stateR, "Activo");
            else strcpy(stateR, "Finalizado");
            printf("ID Prestamo: %d\nUsuario: %s\nID ejemplar: %d\nFecha de inicio: %s\nFecha de finalizacion: %s\nEstado: %s\n", 
            id->valueint, usuario->valuestring, idE->valueint, start->valuestring, end->valuestring, stateR);
            printf("==============================================================================================================.\n");
        }
    }
    //close the object
    cJSON_Delete(json);
    return operativeMenu();
}


/*
Loan maturity
*
This option is responsible display the loans maturity info.

@return return is none, info is displayed on console.
*/

void loanMaturity() {

    time_t currentTime;
    time(&currentTime);

    // Convert the time to a struct tm
    struct tm *fechaActual = localtime(&currentTime);

    fechaActual->tm_mon = fechaActual->tm_mon;
    fechaActual->tm_year = fechaActual->tm_year;

    time_t tiempo_fechaActual = mktime(fechaActual);
    struct tm fecha2 = {0}; //
    
    //tiempo_fecha1 < tiempo_fecha2
    char* buffer = readJson("prestamos.json");
    //open the json object.
    cJSON *json = cJSON_Parse(buffer);
    int arraySize = cJSON_GetArraySize(json);
    //for each item in json.
    for (int i = 0; i < arraySize; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);

        cJSON *id = cJSON_GetObjectItemCaseSensitive(item, "id");
        //id
        cJSON *usuario = cJSON_GetObjectItemCaseSensitive(item, "usuario");
        //Titulo.
        cJSON *idE = cJSON_GetObjectItemCaseSensitive(item, "idEjemplar");
        //author.
        cJSON *start = cJSON_GetObjectItemCaseSensitive(item, "inicio");
        //year
        cJSON *end = cJSON_GetObjectItemCaseSensitive(item, "fin");
        //gender
        cJSON *state = cJSON_GetObjectItemCaseSensitive(item, "estado");

        sscanf(end->valuestring, "%d/%d/%d", &fecha2.tm_mday, &fecha2.tm_mon, &fecha2.tm_year);

        fecha2.tm_mon -= 1; // Ajustamos el mes a 0-11
        fecha2.tm_year -= 1900; // Ajustamos el año a contar desde 1900
        time_t tiempo_fecha2 = mktime(&fecha2);


        //printf("Fecha actual: %11d, Fecha prestamo: %11d", tiempo_fechaActual, tiempo_fecha2);

        if(tiempo_fechaActual > tiempo_fecha2) {
            printf("====================================================Prestamo Vencido=========================================================.\n");
            printf("ID Prestamo: %d\nUsuario: %s\nID ejemplar: %d\nFecha de inicio: %s\nFecha de finalizacion: %s\n", 
            id->valueint, usuario->valuestring, idE->valueint, start->valuestring, end->valuestring);
        } else {
            fecha2.tm_mday -= 3;
            tiempo_fecha2 = mktime(&fecha2);

            if(tiempo_fechaActual > tiempo_fecha2) {
                printf("====================================================Prestamo A Vencer=========================================================.\n");
                printf("ID Prestamo: %d\nUsuario: %s\nID ejemplar: %d\nFecha de inicio: %s\nFecha de finalizacion: %s\n", 
                id->valueint, usuario->valuestring, idE->valueint, start->valuestring, end->valuestring);
            }
        }


    }
    //close the object
    cJSON_Delete(json);
    printf("\n");
    return operativeMenu();
}


/*
give back books
*
This option is responsible for give back the book on loan.

@return return is none, info is saved on Json.
*/

void giveBackBook() {
    char idTemp[100];
    fflush(stdin);
    printf("\nIngrese el ID de su prestamo: ");
    fgets(idTemp, sizeof(idTemp), stdin); //Lee la cadena 
    idTemp[strlen(idTemp) - 1] = '\0';

    if (numberOnString(idTemp) == 0) {
        printf("\nERROR-> El ID no debe contener letras\n");
        return generalMenu();
    };

    int idLoan = atoi(idTemp);
    char *date = getDateLoan("finalizacion");
    checkLoans(idLoan, date);
    return generalMenu();
};


/*
advance seek
*
This option is responsible for requesting the parameters for the search.

@return return is none, info is displayed on console.
*/

void advancedSeek() {
    char type[100];
    char oper[100];
    char text[100];
    fflush(stdin);
    printf("Ingrese el texto a buscar:\n");

    fgets(text, sizeof(text), stdin); // Lee la cadena con espacios
    fflush(stdin);
    text[strlen(text) - 1] = '\0';

    strcpy(type, getTypeSeek());
    strcpy(oper, getOperSeek());

    fflush(stdin);
    printf("¿Desea buscar por nombre? En caso de que no, presione enter: ");
    int name = getchar();
    fflush(stdin);
    printf("¿Desea buscar por autor? En caso de que no, presione enter: ");
    int author = getchar();
    fflush(stdin);
    printf("¿Desea buscar por resumen? En caso de que no, presione enter: ");
    int sumary = getchar();

    int flagN = 1;
    int flagA = 1;
    int flagS = 1;
    if (name == '\n') flagN = 0;
    if (author == '\n') flagA = 0;
    if (sumary == '\n') flagS = 0;


    //text[strlen(text) - 1] = '\0';

    if (strcmp(type, "C") == 0 ) advanceSeek_C(text, oper, flagN, flagA, flagS);
    else advanceSeek_E(text, oper, flagN, flagA, flagS);
    return generalMenu();
}

void advanceSeek_C(char* info, char* typeOY, int flagName, int flagAuthor, int flagSum) {
    char* buffer = readJson("catalog.json");
    //open the json object.
    cJSON *json = cJSON_Parse(buffer);
    int arraySize = cJSON_GetArraySize(json);
    //for each item in json.
    printf("==============================================================================================================.\n");
    for (int i = 0; i < arraySize; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);
        //id
        cJSON *id = cJSON_GetObjectItemCaseSensitive(item, "id");
        //Titulo.
        cJSON *title = cJSON_GetObjectItemCaseSensitive(item, "nombre");
        //author.
        cJSON *author = cJSON_GetObjectItemCaseSensitive(item, "autor");
        //sumary
        cJSON *sumary = cJSON_GetObjectItemCaseSensitive(item, "sinopsis");
        //
        char *resT = strstr(title->valuestring, info);
        char *resA = strstr(author->valuestring, info);
        char *resS = strstr(sumary->valuestring, info);
        char disp[20];

        if(verifyDisp(id->valueint)) {
            strcpy(disp, "Disponible");
        } else {
            strcpy(disp, "Ocupado");
        }

        int flag = 1; 
        if(strcmp(typeOY, "Y") == 0) {
            if (resA == NULL && flagAuthor == 1) flag = 0;
            if (resT == NULL && flagName == 1) flag = 0;
            if (resS == NULL && flagSum == 1) flag = 0;
            if(flag == 1) {
                printf("ID: %d, Titulo: %s, reseña: %s, Estado: %s\n",id->valueint, title->valuestring, sumary->valuestring, disp);
                printf("==============================================================================================================.\n");
            }
        } else {
            flag = 0;
            if (resA != NULL && flagAuthor == 1) flag = 1;
            if (resT != NULL && flagName == 1) flag = 1;
            if (resS != NULL && flagSum == 1) flag = 1;

            if(flag == 1) {
                printf("ID: %d, Titulo: %s, reseña: %s, Estado: %s\n",id->valueint, title->valuestring, sumary->valuestring, disp);
                printf("==============================================================================================================.\n");
            }
        }

    }
    //close the object
    cJSON_Delete(json);
}


void advanceSeek_E(char* info, char* typeOY, int flagName, int flagAuthor, int flagSum) {
    char* buffer = readJson("catalog.json");
    //open the json object.
    cJSON *json = cJSON_Parse(buffer);
    int arraySize = cJSON_GetArraySize(json);
    //for each item in json.
    //printf("Flags %d, %d, %d\n", flagAuthor, flagSum, flagName);
    printf("==============================================================================================================.\n");
    for (int i = 0; i < arraySize; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);
        //id
        cJSON *id = cJSON_GetObjectItemCaseSensitive(item, "id");
        //Titulo.
        cJSON *title = cJSON_GetObjectItemCaseSensitive(item, "nombre");
        //author.
        cJSON *author = cJSON_GetObjectItemCaseSensitive(item, "autor");
        //sumary
        cJSON *sumary = cJSON_GetObjectItemCaseSensitive(item, "sinopsis");
        //

        int comparacionT = strcmp(title->valuestring, info);
        int comparacionA = strcmp(author->valuestring, info);
        int comparacionS = strcmp(sumary->valuestring, info);

        char disp[20];

        if(verifyDisp(id->valueint)) {
            strcpy(disp, "Disponible");
        } else {
            strcpy(disp, "Ocupado");
        }

        int flag = 1; 
        if(strcmp(typeOY, "Y") == 0) {
            if (comparacionA != 0 && flagAuthor == 1) flag = 0;
            if (comparacionT != 0  && flagName == 1) flag = 0;
            if (comparacionS != 0  && flagSum == 1) flag = 0;
            if(flag == 1) {
                printf("ID: %d, Titulo: %s, reseña: %s, Estado: %s\n",id->valueint, title->valuestring, sumary->valuestring, disp);
                printf("==============================================================================================================.\n");
            }
        } else {
            flag = 0;
            if (comparacionT == 0  && flagName == 1) flag = 1;
            if (comparacionA == 0  && flagAuthor == 1) flag = 1;
            if (comparacionS == 0 && flagSum == 1) flag = 1;

            if(flag == 1) {
                printf("ID: %d, Titulo: %s, reseña: %s, Estado: %s\n",id->valueint, title->valuestring, sumary->valuestring, disp);
                printf("==============================================================================================================.\n");
            }
        }

    }
    //close the object
    cJSON_Delete(json);
}