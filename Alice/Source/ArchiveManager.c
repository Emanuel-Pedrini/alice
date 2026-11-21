#include "Headers/ArchiveManager.h"
#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/CStringUtils.h"
#include "Headers/ForwardInclude.h"
#include "External/Libraries.h"

char* OpenFileNWindows(char* ArchivePath, Usize Capacity) {
    Usize capacity = Capacity;
    int size = 0;

    FILE* file = fopen(ArchivePath, "r");
    if (file == NULL) {
        return NULL;
    }

    char* totalBuffer;

    totalBuffer = (char*) __AlAllocate(capacity * sizeof(char));
    if (totalBuffer == NULL) {
        fclose(file);
        return NULL;
    }
    totalBuffer[0] = '\0';
    char buffer[2048];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        while (size + strlen(buffer) + 1 >= capacity) {
            Usize old_capacity = capacity;
            capacity = capacity * 2;
            __QMark
            char *TemporaryBuffer = (char*) __AlReallocate(totalBuffer, old_capacity * sizeof(char), capacity * sizeof(char));
            if (TemporaryBuffer == NULL) {
                __QUnmark
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

AliceCodeObject* OpenAliceFile(char* ArchivePath) 
{
    AliceCodeObject* NewCodeObject = __AlAllocate(sizeof(AliceCodeObject));
    char* Code = OpenFileNWindows(ArchivePath, 1024);
    if (Code == NULL) {
        printf(">> ERROR : Could not find '%s'!\n", ArchivePath);
        return NULL;
    }
    char* Path = ArchivePath;
    char* Name = File(ArchivePath);
    NewCodeObject -> Code = Code;
    NewCodeObject -> Path = Path;
    NewCodeObject -> Name = Name;
    return NewCodeObject;
}

int FileWrite(char* Path, const char* Content, const char* Mode) 
{
    FILE* File = fopen(Path, Mode);
    if (File == NULL) {
        free(File);
        return 1;
    }
    fputs(Content, File);
    fclose(File);
    return 0;
}

int BinaryWrite(char* Path, unsigned int Content)
{
    FILE* File = fopen(Path, "w");
    if (File == NULL) {
        free(File);
        return 1;
    }
    fwrite(&Content, sizeof(unsigned int), 1, File);
    fclose(File);
    return 0;   
}