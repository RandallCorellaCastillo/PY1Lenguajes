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
   
    int len = fread(buffer, 1, sizeof(buffer), fp);
    //close the file.
    fclose(fp);
    //open the json object.
    cJSON *json = cJSON_Parse(buffer);
    int arraySize = cJSON_GetArraySize(json);
    //for each item in json.
    for (int i = 0; i < arraySize; i++) {
        /*
        cJSON *item = cJSON_GetArrayItem(json, i);
        cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON *age = cJSON_GetObjectItemCaseSensitive(item, "age");
        if (cJSON_IsString(name)) {
            printf("Nombre: %s, edad: %d\n", name->valuestring, age->valueint);
        }
        */
    }
    //close the object
    cJSON_Delete(json);
    return "";
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
    printf(buffer);
    
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
