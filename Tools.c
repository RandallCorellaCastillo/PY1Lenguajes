#include <stdio.h>
#include "cJSON.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

void getTicket(char*, char*);

/*
    Struct to save name of books or persons and how many aparations each have
*/
struct Stats {
    char *name;
    int aparations;
};


int compare(char* val1, char* val2) {
    //compare the char*
    int result = strcmp(val1, val2);
    return result;
}

/*
Save info in Json
*
@param data text to save.
@param filename is the root of Json

@return return is none, info is saved on json.
*/
void saveToJson(const char *data, const char *filename) {
    //open file.
    FILE *file = fopen(filename, "w");
    //
    if (!file) {
        perror("Error al abrir el archivo para escribir");
    }
    //close and write data in file.
    fprintf(file, "%s\n", data);
    //close.
    fclose(file);
}

/*
read Json 
*
@param URL is the root of Json

@return return the json info.
*/
char* readJson(char* URL){
    // open the file
    FILE *fp = fopen(URL, "r");
    //size of file.
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    //buffer
    char buffer[size];
   
    fread(buffer, 1, sizeof(buffer), fp);
    //close the file.
    fclose(fp);

    char *text = (char *)malloc(sizeof(buffer) * sizeof(char));
    strcpy(text, buffer);

    return text;
}

/*
verify book disp.
*
@param idSeek is the id of book.


@return return 1 if valid, otherwise 0.
*/
int verifyDisp(int idSeek) {
    FILE *file = fopen("catalog.json", "r");

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *fileContent = (char *)malloc(fileSize + 1);
    fread(fileContent, 1, fileSize, file);

    fclose(file);

    fileContent[fileSize] = '\0';

    // Analizar el contenido JSON
    cJSON *jsonArray = cJSON_Parse(fileContent);

    cJSON *item = cJSON_GetArrayItem(jsonArray, idSeek);

    cJSON *disp = cJSON_GetObjectItemCaseSensitive(item, "disponibilidad");
    
    if(disp->valueint == 0) return 0;
    else return 1;

    free(jsonArray);

}


/*
commun seek of books.
*
@param info text to seek.

@return ireturn is none, info is displayed on console.
*/
void seekCommun(char* info) {

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
        //year
        cJSON *year = cJSON_GetObjectItemCaseSensitive(item, "año de publicacion");
        //gender
        cJSON *gender = cJSON_GetObjectItemCaseSensitive(item, "genero");
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

        if(resA != NULL || resT != NULL || resS != NULL) {
            printf("ID: %d, Titulo: %s, reseña: %s, Estado: %s\n", 
            id->valueint, title->valuestring, sumary->valuestring, disp);
            printf("==============================================================================================================.\n");
        }
    }
    //close the object
    cJSON_Delete(json);
}


/*
validate number on string.
*
@param text text to validate.

@return return 1 if valid, otherwise 0.
*/
int numberOnString(char* text) {
    int i = 0;
    //find letters in number.
    while (text[i] != '\0') {
        if (!isdigit(text[i])) {
            return 0;
        };
        i++;
    };   
    return 1;
};

/*
validate string.
*
@param text text to validate.

@return return 1 if valid,  otherwise 0.
*/
int correctString(char* text) {
    int i = 0;
    //find number in text.
    while (text[i] != '\0') {
        if (isdigit(text[i])) {
            return 0;
        };

        i++;
    };  

    if (strchr(text, ' ') == NULL) {
        return 0;
    } else {
        char* i = strchr(text, ' ');
        if (i[1] == '\0' || i[3] == '\0' || i[2] == '\0') {
            return 0;
        };
    };

    return 1;
};

/*
save user.
*
@param id user id.
@param name user name.

@return return is none, info is saved on Json.
*/

void saveUsers (char* id, char* name) {
    FILE *file = fopen("users.json", "r");

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *fileContent = (char *)malloc(fileSize + 1);
    fread(fileContent, 1, fileSize, file);

    fclose(file);

    fileContent[fileSize] = '\0';

    // Analizar el contenido JSON
    cJSON *jsonArray = cJSON_Parse(fileContent);
    free(fileContent);

    // Crear un nuevo objeto y agregarlo al arreglo
    cJSON *newObj = cJSON_CreateObject();
    cJSON_AddStringToObject(newObj, "identificacion", id);
    cJSON_AddStringToObject(newObj, "nombre", name);
    cJSON_AddItemToArray(jsonArray, newObj);

    // Convertir el JSON a una cadena
    char *json_str = cJSON_Print(jsonArray);

    // Guardar en archivo
    saveToJson(json_str, "users.json");

    // Liberar la memoria y objetos
    cJSON_Delete(jsonArray);
    free(json_str);
};


/*
validate user.
*
@param id is userId.

@return return 1 valid, otherwise 0.
*/
int checkUsers(char* id) {
    FILE *fp = fopen("users.json", "r");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char buffer[size];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);
    int arraySize = cJSON_GetArraySize(json);

    for (int i = 0; i < arraySize; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);
        cJSON *idJ = cJSON_GetObjectItemCaseSensitive(item, "identificacion");

        if (strcmp(idJ->valuestring, id) == 0) {
            return 0;
        };
    };
    cJSON_Delete(json);
    return 1;
};

/*
save books.
*
@param name is name book.
@param autor is author name.
@param year publish year.
@param gender gender of book.
@param synopsis summary.
@param quan cuantity.

@return return is none, info is saved on Json.
*/
void saveBooks(char* name, char* autor, char* year, char* gender, char* synopsis, char* quan) {
    FILE *file = fopen("catalog.json", "r");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    char *fileContent = (char *)malloc(fileSize + 1);
    fread(fileContent, 1, fileSize, file);
    fclose(file);
    fileContent[fileSize] = '\0';

    // Analizar el contenido JSON
    cJSON *jsonArray = cJSON_Parse(fileContent);
    free(fileContent);

    int len = cJSON_GetArraySize(jsonArray);
    int howMany = atoi(quan);

    // Crear un nuevo objeto y agregarlo al arreglo
    for (int k = 0; k < howMany; k++) {
        cJSON *newObj = cJSON_CreateObject();
        cJSON_AddNumberToObject(newObj, "id", len);
        cJSON_AddStringToObject(newObj, "nombre", name);
        cJSON_AddStringToObject(newObj, "autor", autor);
        cJSON_AddStringToObject(newObj, "año de publicacion", year);
        cJSON_AddStringToObject(newObj, "genero", gender);
        cJSON_AddStringToObject(newObj, "sinopsis", synopsis);
        cJSON_AddNumberToObject(newObj, "disponibilidad", 1);
        cJSON_AddItemToArray(jsonArray, newObj);
        len++;
    };

    // Convertir el JSON a una cadena
    char *json_str = cJSON_Print(jsonArray);
    // Guardar en archivo
    saveToJson(json_str, "catalog.json");
    cJSON_Delete(jsonArray);
    free(json_str);
};

/*
validate book name.
*
@param name to validate.

@return return 1 if valid, otherwise 0.
*/


int checkBook(char* name) {
    FILE *fp = fopen("catalog.json", "r");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char buffer[size];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);
    int arraySize = cJSON_GetArraySize(json);

    for (int i = 0; i < arraySize; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);
        cJSON *nameJ = cJSON_GetObjectItemCaseSensitive(item, "nombre");

        if (strcmp(nameJ->valuestring, name) == 0) {
            return 0;
        };
    };
    cJSON_Delete(json);
    return 1;
};


/*
process books and validate.
*
@param token to validate and save.

@return return 1 if valid, otherwise 0.
*/

int processTokens(char *token) {
    char* caracteristics[5];
    int i = 0;

    const char *delimiter2 = "|";
    char *saveptr;
    char *subtoken = strtok_r(token, delimiter2, &saveptr);

    while (subtoken != NULL) {
        caracteristics[i] = subtoken;
        subtoken = strtok_r(NULL, delimiter2, &saveptr);
        i++;
    };

    if (i != 6) {
        printf("\nERROR-> Toda linea debe seguir el formato especificado");
        return 0;
    };

    if (checkBook(caracteristics[0]) == 0) {
        printf("\nERROR-> ");
        printf("%s", caracteristics[0]);
        printf(" <-ya existe en nuestro catalogo\n");
        return 0;
    };

    if (numberOnString(caracteristics[2]) == 0 && strlen(caracteristics[2]) != 4) {
        printf("\nERROR-> El ano de publicacion debe contener 4 digitos y no contener letras");
        return 0;
    };

    if (numberOnString(caracteristics[5]) == 0) {
        printf("\nERROR-> la cantidad a agregar debe ser un numero, osease no debe contener letras");
        return 0;
    };

    saveBooks(caracteristics[0], caracteristics[1], caracteristics[2], caracteristics[3], caracteristics[4], caracteristics[5]);
    return 1;
};

/*
validate date.
*
@param date to validate.

@return return 1 if valid, otherwise 0.
*/

int validateDate(char *date) {
    int dia, mes, anio;
    // Intentamos extraer los valores de día, mes y año de la cadena
    sscanf(date, "%d/%d/%d", &dia, &mes, &anio);

    if (sscanf(date, "%d/%d/%d", &dia, &mes, &anio) != 3 || dia > 31 || mes > 12 || anio > 2050) {
         return 0;
    } else {
       return 1;
    }
}

/*
validate the name.
*
@param name to validate.

@return return 1 if valid, otherwise 0.
*/

int checkNameUsers(char* name) {
    FILE *fp = fopen("users.json", "r");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char buffer[size];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);
    int arraySize = cJSON_GetArraySize(json);

    for (int i = 0; i < arraySize; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);
        cJSON *idJ = cJSON_GetObjectItemCaseSensitive(item, "nombre");
        if (strcmp(idJ->valuestring, name) == 0) {
            return 1;
        };
    };
    cJSON_Delete(json);
    return 0;
};

/*
Display commun seek info.

@return return is none, info is displayed on console.
*/
void simpleSeek() {
    char text[100];
    printf("Ingrese el texto a buscar:\n");
    scanf("%s", text);
    printf("\n");
    seekCommun(text);
    printf("\n");

}

/*
validate the string.
*
@param text to validate.

@return return 1 if valid, otherwise 0.
*/

int validateStringOnDate(char* text) {
    char com = '/';
    int i = 0;
    //find letters in number.
    while (text[i] != '\0') {
        if (!isdigit(text[i]) && text[i] != com) {
            return 0;
        };
        i++;
    };   
    return 1;
};

/*
save loans.
*
@param UserL user Name.
@param id is book id.
@param initDate is start date.
@param endDate is end date.
*
@return return is none, but info is saved in JSON.
*/
void saveLoan(char* UserL, int id, char* initDate, char* endDate) {
    FILE *file = fopen("prestamos.json", "r");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    char *fileContent = (char *)malloc(fileSize + 1);
    fread(fileContent, 1, fileSize, file);
    fclose(file);
    fileContent[fileSize] = '\0';

    // Analizar el contenido JSON
    cJSON *jsonArray = cJSON_Parse(fileContent);
    int arraySize = cJSON_GetArraySize(jsonArray);
    free(fileContent);

    // Crear un nuevo objeto y agregarlo al arreglo
    cJSON *newObj = cJSON_CreateObject();
    cJSON_AddNumberToObject(newObj, "id", arraySize);
    cJSON_AddStringToObject(newObj, "usuario", UserL);
    cJSON_AddNumberToObject(newObj, "idEjemplar", id);
    cJSON_AddStringToObject(newObj, "inicio", initDate);
    cJSON_AddStringToObject(newObj, "fin", endDate);
    cJSON_AddNumberToObject(newObj, "estado", 1);
    cJSON_AddItemToArray(jsonArray, newObj);

    // Convertir el JSON a una cadena
    char *json_str = cJSON_Print(jsonArray);
    // Guardar en archivo
    saveToJson(json_str, "prestamos.json");
    cJSON_Delete(jsonArray);
    free(json_str);
};

/*
Modify book disp.
*
@param idSeek is book id.
*
@return return is none, but info is saved in JSON.
*/

void modifyCatalogDisp(int idSeek) {
    FILE *file = fopen("catalog.json", "r");

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *fileContent = (char *)malloc(fileSize + 1);
    fread(fileContent, 1, fileSize, file);

    fclose(file);

    fileContent[fileSize] = '\0';

    // Analizar el contenido JSON
    cJSON *jsonArray = cJSON_Parse(fileContent);

    cJSON_GetArrayItem(jsonArray, idSeek);

    cJSON_SetNumberValue(cJSON_GetObjectItem(cJSON_GetArrayItem(jsonArray, idSeek), "disponibilidad"), 0);

    //id
    char *json_str = cJSON_Print(jsonArray);

    saveToJson(json_str, "catalog.json");

    free(json_str);
    free(jsonArray);
}


/*
Modify book disp.
*
@param idSeek is book id.
*
@return return is none, but info is saved in JSON.
*/

void modifyCatalogDispV2(int idSeek) {
    FILE *file = fopen("catalog.json", "r");

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *fileContent = (char *)malloc(fileSize + 1);
    fread(fileContent, 1, fileSize, file);

    fclose(file);

    fileContent[fileSize] = '\0';

    // Analizar el contenido JSON
    cJSON *jsonArray = cJSON_Parse(fileContent);

    cJSON_GetArrayItem(jsonArray, idSeek);

    cJSON_SetNumberValue(cJSON_GetObjectItem(cJSON_GetArrayItem(jsonArray, idSeek), "disponibilidad"), 1);

    //id
    char *json_str = cJSON_Print(jsonArray);

    saveToJson(json_str, "catalog.json");

    free(json_str);
    free(jsonArray);
}

/*
Save loans earnings.
*
@param earning is loan earning.
@param date the date of get earnings
*
@return return is none, but info is saved in JSON.
*/
void saveEarnings(int earning, char* date) {
    FILE *file = fopen("earnings.json", "r");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    char *fileContent = (char *)malloc(fileSize + 1);
    fread(fileContent, 1, fileSize, file);
    fclose(file);
    fileContent[fileSize] = '\0';

    // Analizar el contenido JSON
    cJSON *jsonArray = cJSON_Parse(fileContent);
    int arraySize = cJSON_GetArraySize(jsonArray);
    free(fileContent);

    // Crear un nuevo objeto y agregarlo al arreglo
    cJSON *newObj = cJSON_CreateObject();
    cJSON_AddNumberToObject(newObj, "id", arraySize);
    cJSON_AddNumberToObject(newObj, "ganancia", earning);
    cJSON_AddStringToObject(newObj, "fecha", date);
    cJSON_AddItemToArray(jsonArray, newObj);

    // Convertir el JSON a una cadena
    char *json_str = cJSON_Print(jsonArray);
    // Guardar en archivo
    saveToJson(json_str, "earnings.json");
    cJSON_Delete(jsonArray);
    free(json_str);
};



/*
Calculate the size of the catalog.
*
@return returns the cuantity of books.
*/

int sizeOfCatalog() {
    FILE *fp = fopen("catalog.json", "r");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char buffer[size];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);
    int arraySize = cJSON_GetArraySize(json);
    free(json);
    return arraySize;
}

/*
This first line is a brief description.
*
Loans status
*
@param idLoan id of the loan.
@param date end date of the loan.
*
@return return is none, loan info is displayed on the console.
*/

void checkLoans(int idLoan, char* date) {
    FILE *fp = fopen("prestamos.json", "r");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char buffer[size];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);
    int arraySize = cJSON_GetArraySize(json);
    cJSON *tempLoan = cJSON_GetArrayItem(json, idLoan);

    if (tempLoan == NULL) {
        printf("\nERROR-> El ID ingresado no corresponde a ningun prestamo\n\n");
        return;
    };

    cJSON *tempDate = cJSON_GetObjectItemCaseSensitive(tempLoan, "fin");
    cJSON *tempEs = cJSON_GetObjectItemCaseSensitive(tempLoan, "estado");
    cJSON *tempIdEj = cJSON_GetObjectItemCaseSensitive(tempLoan, "idEjemplar");

    if (strcmp(tempDate->valuestring, date) != 0) {
        printf("\nERROR-> La fecha ingresada no coindice con el prestamo que esta buscando\n\n");
        return;
    };

    if (tempEs->valueint == 0) {
        printf("\nERROR-> Este prestamo ya fue cancelado\n\n");
        return;
    };

    cJSON *tempDate2 = cJSON_GetObjectItemCaseSensitive(tempLoan, "inicio");
    cJSON_SetNumberValue(cJSON_GetObjectItem(cJSON_GetArrayItem(json, idLoan), "estado"), 0);
    modifyCatalogDispV2(tempIdEj->valueint);

    char *json_str = cJSON_Print(json);

    saveToJson(json_str, "prestamos.json");

    cJSON_Delete(json);
    free(json_str);
    getTicket(tempDate2->valuestring, tempDate->valuestring);
    //return;
};


/*
calculate the payment of the loan.
*
@param dateStart start date of the loan.
@param dateEnd end date of the loan.
*
@return return is none, the info of payments is save in JSON.
*/
void getTicket(char* dateStart, char* dateEnd) {
    struct tm date1 = {0};
    struct tm date2 = {0};
    struct tm currentDate = {0};
    char dateStr[11]; 

    int day1, month1, year1, day2, month2, year2;
    
    sscanf(dateStart, "%d/%d/%d", &day1, &month1, &year1);
    sscanf(dateEnd, "%d/%d/%d", &day2, &month2, &year2);
    
    date1.tm_mday = day1;
    date1.tm_mon = month1 - 1;
    date1.tm_year = year1 - 1900;
    
    date2.tm_mday = day2;
    date2.tm_mon = month2 - 1;
    date2.tm_year = year2 - 1900; 

    time_t currentTime;
    time(&currentTime); 
    struct tm *localTime = localtime(&currentTime);
    strftime(dateStr, sizeof(dateStr), "%d/%m/%Y", localTime);

    currentDate.tm_year = localTime->tm_year;
    currentDate.tm_mon = localTime->tm_mon;
    currentDate.tm_mday = localTime->tm_mday;

    time_t time1 = mktime(&date1);
    time_t time2 = mktime(&date2);
    time_t time3 = mktime(&currentDate);

    double differenceInSeconds = difftime(time1, time2);
    int differenceInDaysOnTime = differenceInSeconds / (60 * 60 * 24);
    differenceInSeconds = difftime(time2, time3);
    int differenceInDaysOffTime = differenceInSeconds / (60 * 60 * 24);
    
    if (differenceInDaysOnTime < 0) {
        differenceInDaysOnTime = differenceInDaysOnTime * -1;
    };

    if (differenceInDaysOffTime < 0) {
        differenceInDaysOffTime = differenceInDaysOffTime * -1;
    } else {
        differenceInDaysOnTime = differenceInDaysOnTime - differenceInDaysOffTime;
        differenceInDaysOffTime = 0;
    };

    //si es negativo se paso si no esta on time

    //Calculamos el monto a pagar
    //Primeramente calculemos el costo del prestamo como tal segun la duracion que se establecio
    int ticket = 0;
    
    if (differenceInDaysOnTime <= 7) {
        ticket = differenceInDaysOnTime * 150;
    };
    if (differenceInDaysOnTime >= 8 && differenceInDaysOnTime <= 15) {
        ticket = differenceInDaysOnTime * 125;
    };
    if (differenceInDaysOnTime >= 16) {
        ticket = differenceInDaysOnTime * 100;
    };

    //Proseguimos con el calcule de dia extras en caso de haberlos
    if (differenceInDaysOffTime <= 7) {
        ticket = ticket + (differenceInDaysOffTime * 75);
    };
    if (differenceInDaysOffTime >= 8 && differenceInDaysOffTime <= 15) {
        ticket = ticket + (differenceInDaysOffTime * 50);
    };
    if (differenceInDaysOffTime >= 16) {
        ticket = ticket + (differenceInDaysOffTime * 25);
    };

    printf("\nAVISO-> ");
    printf("%d", ticket);
    printf("<- Es el monto a pagar en TECDOLLARS\n\n");

    saveEarnings(ticket, dateStr);
    return;
};

/*
get info of type.
*
Select 2 types of seek, Exact or containe.
*
No Param.
*
@return return the type of seek selected.
*/
char* getTypeSeek() {
    fflush(stdin);
    char type[100];
    printf("Seleccione la forma 'Exacta'(E) o 'Contiene'(C): ");
    fgets(type, 100, stdin);
    char *text = (char *)malloc(sizeof(type) * sizeof(char));
    strcpy(text, type);
    text[strlen(text) - 1] = '\0';

    if(!(strcmp(text, "E") == 0 || strcmp(text, "C") == 0)) return getTypeSeek();

    return text;
}
/*
get info of operator
*
@return return the opertor selected.
*/
char* getOperSeek() {
    fflush(stdin);
    char type[100];
    printf("Seleccione la forma para los operandos 'Y' o 'O': ");
    fgets(type, 100, stdin);
    char *text = (char *)malloc(sizeof(type) * sizeof(char));
    strcpy(text, type);
    text[strlen(text) - 1] = '\0';

    if(!(strcmp(text, "Y") == 0 || strcmp(text, "O") == 0)) return getOperSeek();

    return text;
}

/*
get the number of different books on the catalog
*
@return return the number of diffenent books.
*/
int countDifBooks() {
    FILE *fp = fopen("catalog.json", "r");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    char buffer[size];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    cJSON *jsonBooks = cJSON_Parse(buffer);
    int arraySize = cJSON_GetArraySize(jsonBooks);
    int count = 0;

    for (int i = 0; i < arraySize; i++) {
        int temp = i;

        if (temp == 0) {
            count++;
        } else {
            cJSON *item = cJSON_GetArrayItem(jsonBooks, temp);
            cJSON *item2 = cJSON_GetArrayItem(jsonBooks, temp - 1);
            cJSON *idJ = cJSON_GetObjectItemCaseSensitive(item, "nombre");
            cJSON *idJ2 = cJSON_GetObjectItemCaseSensitive(item2, "nombre");

            if (strcmp(idJ->valuestring, idJ2->valuestring) != 0) {
                count++;
            };
        };
    };

    cJSON_Delete(jsonBooks);
    return count;
};

/*
Compare 2 values and the number returned depends of the comparition
*
@return -1 if the firts value is bigger than the second
@return 1 if the second value is bigger than the first
@return 0 if both are equals
*/
int compareItems(const void *a, const void *b) {
    int valueA = ((struct Stats *)a)->aparations;
    int valueB = ((struct Stats *)b)->aparations;

    if (valueA > valueB) return -1;   
    if (valueA < valueB) return 1;    
    return 0;
};

/*
get the count of the loans for each book and prints the top 3 des
*
@return
*/
void getDifBooks() {
    cJSON *jsonBooks = cJSON_Parse(readJson("catalog.json"));
    int arraySize = cJSON_GetArraySize(jsonBooks);
    int count = 0;
    struct Stats books[countDifBooks()]; 

    for (int i = 0; i < arraySize; i++) {
        int temp = i;

        if (temp == 0) {
            cJSON *item = cJSON_GetArrayItem(jsonBooks, temp);
            cJSON *idJ = cJSON_GetObjectItemCaseSensitive(item, "nombre");
            books[count].name = (char*) malloc (strlen(idJ->valuestring) * sizeof(char*));
            strcpy(books[count].name, idJ->valuestring);
            books[count].aparations = 0;
            count++;
        } else {
            cJSON *item = cJSON_GetArrayItem(jsonBooks, temp);
            cJSON *item2 = cJSON_GetArrayItem(jsonBooks, temp - 1);
            cJSON *idJ = cJSON_GetObjectItemCaseSensitive(item, "nombre");
            cJSON *idJ2 = cJSON_GetObjectItemCaseSensitive(item2, "nombre");

            if (strcmp(idJ->valuestring, idJ2->valuestring) != 0) {
                books[count].name = (char*) malloc (strlen(idJ->valuestring) * sizeof(char*));
                strcpy(books[count].name, idJ->valuestring);
                books[count].aparations = 0;
                count++;
            };
        };
    };

    cJSON *jsonLoans = cJSON_Parse(readJson("prestamos.json"));
    arraySize = cJSON_GetArraySize(jsonLoans);

    
    for (int i = 0; i < arraySize; i++) {
        cJSON *item = cJSON_GetArrayItem(jsonLoans, i);
        cJSON *idJ = cJSON_GetObjectItemCaseSensitive(item, "idEjemplar");

        cJSON *book = cJSON_GetArrayItem(jsonBooks, idJ->valueint);
        cJSON *nameBook = cJSON_GetObjectItemCaseSensitive(book, "nombre");

        for (int j = 0; j < countDifBooks(); j++) {
            if (strcmp(books[j].name, nameBook->valuestring) == 0) {
                books[j].aparations++;
                break;
            };
        };
    };

    size_t numItems = sizeof(books) / sizeof(books[0]);
    qsort(books, numItems, sizeof(struct Stats), compareItems);

    for (int j = 0; j < 3; j++) {
        printf("El libro ");
        printf("%s", books[j].name);
        printf(" se ha prestado un total de ");
        printf("%d", books[j].aparations);
        printf(" veces\n");
    };

    cJSON_Delete(jsonBooks);
    cJSON_Delete(jsonLoans);
};

void userXLoan() {
    cJSON *jsonUsers = cJSON_Parse(readJson("users.json"));
    cJSON *jsonLoans = cJSON_Parse(readJson("prestamos.json"));
    int arraySize = cJSON_GetArraySize(jsonUsers);
    struct Stats users[arraySize];

    for (int i = 0; i < arraySize; i++) {
        cJSON *item = cJSON_GetArrayItem(jsonUsers, i);
        cJSON *idJ = cJSON_GetObjectItemCaseSensitive(item, "nombre");
        users[i].name = (char*) malloc (strlen(idJ->valuestring) * sizeof(char*));
        strcpy(users[i].name, idJ->valuestring);
        users[i].aparations = 0;
    };

    arraySize = cJSON_GetArraySize(jsonLoans);

    for (int i = 0; i < arraySize; i++) {
        cJSON *item = cJSON_GetArrayItem(jsonLoans, i);
        cJSON *idJ = cJSON_GetObjectItemCaseSensitive(item, "usuario");

        for (int j = 0; j < countDifBooks(); j++) {
            if (strcmp(users[j].name, idJ->valuestring) == 0) {
                users[j].aparations++;
                break;
            };
        };
    };

    size_t numItems = sizeof(users) / sizeof(users[0]);
    qsort(users, numItems, sizeof(struct Stats), compareItems);

    for (int j = 0; j < 3; j++) {
        printf("El usuario ");
        printf("%s", users[j].name);
        printf(" ha requerido prestamos de libros ");
        printf("%d", users[j].aparations);
        printf(" veces\n");
    };

    cJSON_Delete(jsonUsers);
    cJSON_Delete(jsonLoans);
};

void earningsPerMonth() {
    struct Stats months[] = {
        {"Enero", 0},
        {"Febrero", 0},
        {"Marzo", 0},
        {"Abril", 0},
        {"Mayo", 0},
        {"Junio", 0},
        {"Julio", 0},
        {"Agosto", 0},
        {"Septiembre", 0},
        {"Octubre", 0},
        {"Noviembre", 0},
        {"Diciembre", 0}
    };

    cJSON *jsonEarnings = cJSON_Parse(readJson("earnings.json"));
    int arraySize = cJSON_GetArraySize(jsonEarnings);

    for (int i = 0; i < arraySize; i++) {
        cJSON *item = cJSON_GetArrayItem(jsonEarnings, i);
        cJSON *idJ = cJSON_GetObjectItemCaseSensitive(item, "fecha");
        cJSON *idJ2 = cJSON_GetObjectItemCaseSensitive(item, "ganancia");

        int dia, mes;
        sscanf(idJ->valuestring, "%d/%d", &dia, &mes);

        months[mes - 1].aparations = months[mes - 1].aparations + idJ2->valueint;
    };

    size_t numItems = sizeof(months) / sizeof(months[0]);
    qsort(months, numItems, sizeof(struct Stats), compareItems);

    for (int j = 0; j < 5; j++) {
        printf("El mes ");
        printf("%s", months[j].name);
        printf(" se recaudo ");
        printf("%d", months[j].aparations);
        printf(" TECDOLLARS\n");
    };

    cJSON_Delete(jsonEarnings);
};