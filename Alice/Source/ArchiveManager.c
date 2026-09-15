#include "Headers/ArchiveManager.h"
#include "Headers/InterpreterTypes.h"
#include "Headers/ArrayUtilities.h"
#include "External/Libraries.h"
#include "Headers/StringUtilities.h"

char* OpenFileNWindows(char* ArchivePath, Usize Capacity) {
    Usize capacity = Capacity;
    int size = 0;

    FILE* file = fopen(ArchivePath, "r");
    if (file == NULL) {
        return NULL;
    }

    char* totalBuffer;

    totalBuffer = (char*) malloc(capacity * sizeof(char));
    if (totalBuffer == NULL) {
        fclose(file);
        return NULL;
    }
    totalBuffer[0] = '\0';
    char buffer[2048];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        while (size + strlen(buffer) + 1 >= capacity) {
            capacity = capacity * 2;
            char *TemporaryBuffer = (char*) realloc(totalBuffer, capacity * sizeof(char));
            if (TemporaryBuffer == NULL) {
                free(totalBuffer);
                fclose(file);
                return NULL;
            }
            totalBuffer = TemporaryBuffer;
        }
        strcpy(&totalBuffer[size], buffer);
        size += strlen(buffer);
    }
    fclose(file);
    return totalBuffer;
}

Alice_CodeObject* OpenMermaidFile(char* ArchivePath) 
{
    Alice_CodeObject* NewCodeObject = malloc(sizeof(Alice_CodeObject));
    char* Code = OpenFileNWindows(ArchivePath, 1024);
    if (Code == NULL) {
        printf("Could not the file!\n");
        return NULL;
    }
    char* Path = ArchivePath;
    char* Name = File(ArchivePath);
    NewCodeObject -> Code = Code;
    NewCodeObject -> Path = Path;
    NewCodeObject -> Name = Name;
    return NewCodeObject;
}
