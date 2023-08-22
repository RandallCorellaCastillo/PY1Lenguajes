#include <stdio.h>
#include "cJSON.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int compare(char* val1, char* val2) {
    //compare the char*
    int result = strcmp(val1, val2);
    return result;
}

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

void seekCommun(char* info) {

    char* buffer = readJson("ejemplares.json");
    //open the json object.
    cJSON *json = cJSON_Parse(buffer);
    int arraySize = cJSON_GetArraySize(json);
    //for each item in json.
    for (int i = 0; i < arraySize; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);
        //id
        cJSON *id = cJSON_GetObjectItemCaseSensitive(item, "id");
        //Titulo.
        cJSON *title = cJSON_GetObjectItemCaseSensitive(item, "titulo");
        //author.
        cJSON *author = cJSON_GetObjectItemCaseSensitive(item, "autor");
        //year
        cJSON *year = cJSON_GetObjectItemCaseSensitive(item, "anno");
        //gender
        cJSON *gender = cJSON_GetObjectItemCaseSensitive(item, "genero");
        //sumary
        cJSON *sumary = cJSON_GetObjectItemCaseSensitive(item, "reseña");
        //
        char *resT = strstr(title->valuestring, info);
        char *resA = strstr(author->valuestring, info);
        char *resS = strstr(sumary->valuestring, info);
        if(resA != NULL || resT != NULL || resS != NULL) printf("ID: %s, Titulo: %s, autor: %s, año: %s, genero: %s, reseña: %s\n", 
        id->valuestring, title->valuestring, author->valuestring, year->valuestring, gender->valuestring, sumary->valuestring);
        
      
    }
    //close the object
    cJSON_Delete(json);
}

void advancedSeek(char* info, char typeCE) {
    
}


int numberOnString(char* text) {
    int i = 0;
    //find number in text.
    while (text[i] != '\0') {
        if (!isdigit(text[i])) {
            return 0;
        };
        i++;
    };   
    return 1;
};

int correctString(char* text) {
    int i = 0;
    //find letters in number.
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

void processTokens(char *token) {
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

    if (checkBook(caracteristics[0]) == 0) {
        printf("\nERROR-> ");
        printf("%s", caracteristics[0]);
        printf(" <-ya existe en nuestro catalogo\n");
    } else {
        saveBooks(caracteristics[0], caracteristics[1], caracteristics[2], caracteristics[3], caracteristics[4], caracteristics[5]);
    };
};