#include <string.h>
#include <stdio.h>
#include <cJSON.h>


int compare(char* val1, char* val2) {
    int result = strcmp(val1, val2);
    return result;
}

char* readJson(char* URL){
    // open the file
    FILE *fp = fopen(URL, "r");
    char buffer[1024];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);

    int arraySize = cJSON_GetArraySize(cJSON_GetArrayItem(json, 0)) + 1;



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
    cJSON_Delete(json);
    return "";
}
