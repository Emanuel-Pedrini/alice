#include "Headers/ArchiveManager.h"

char* OpenFileNWindows(char* ArchivePath, Usize Capacity) {
    Usize capacity = Capacity;
    int size = 0;

    FILE* file = fopen(ArchivePath, "r");
    if (file == NULL) 
    {
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
        printf(">> ERROR : Could not find \'%s\'!\n", ArchivePath);
        return NULL;
    }
    char* Path = ArchivePath;
    char* Name = File(ArchivePath);
    NewCodeObject -> Code = Code;
    NewCodeObject -> Path = Path;
    NewCodeObject -> Name = Name;
    return NewCodeObject;
}

int AliceFileWrite(char* Path, CString Content, CString Mode) 
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

int AlicePathExists(CString Path)
{
    struct stat StatInformation;
    return stat(Path, &StatInformation) == 0;
}

int PathRename(CString OriginalPath, CString NewPath)
{
    return rename(OriginalPath, NewPath);
}

#define WIN32_LEAN_AND_MEAN
int ListDirectory(CString Folder, CString Filter, __AliceVector* Buffer)
{
    char Pattern[MAX_PATH];
    WIN32_FIND_DATAA Data;

    snprintf(Pattern, sizeof(Pattern), "%s\\*", Folder);
    HANDLE Handle = FindFirstFile(Pattern, &Data);
    if (Handle == INVALID_HANDLE_VALUE) return 1;
    do 
    {
        int IsDirectory = (Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        if ((strcmp(Data.cFileName, ".") == 0) || (strcmp(Data.cFileName, "..") == 0)) continue;
        if (IsDirectory) 
        {
            if (Data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) continue;
            char SubFolder[MAX_PATH];
            snprintf(SubFolder, sizeof(SubFolder), "%s\\%s", Folder, Data.cFileName);
            ListDirectory(SubFolder, Filter, Buffer);
        }

        CString Dot = strchr(Data.cFileName, '.');
        if (!Dot || _strcmpi(Dot, Filter) != 0) continue; 

        __AliceString* Path = __AlStringFrom(Folder);
        __AppendAlString(Path, "\\");
        __AppendAlString(Path, Data.cFileName);
        __PushAlVector(Buffer, Path);
    } while (FindNextFileA(Handle, &Data));
    FindClose(Handle);
    return 0;
}
