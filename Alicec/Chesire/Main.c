#include "Main.h"
#define __ArgumentIs(B) if (strcasecmp(CurrentChesireCommandReader(&CommandReader), B) == 0)
#define __ChesireFile(P, C, E) if (ChesireAcessFile(P, C, E, "w") != 0) return 1;
#define __StandardBufferSize 2048

int ChesireAcessFile(char* Path, const char* Content, const char* Extension, const char* Mode)
{
    char B[256];
    snprintf(B, sizeof(B), "%s.%s", Path, Extension);
    // Texto formatado caminho / extension -> caminho.extension
    FILE* File = fopen(B, Mode);
    if (File == NULL) {
        free(File);
        return 1;
    }
    fputs(Content, File);
    fclose(File);
    return 0;
}

char* ChesireListDirectory(const char* Folder, const char* Filter, int Recursive, char* List, long ListSize)
{
    char Pattern[MAX_PATH];
    WIN32_FIND_DATAA Data;

    snprintf(Pattern, sizeof(Pattern), "%s\\*", Folder);
    HANDLE Handle = FindFirstFileA(Pattern, &Data);
    if (Handle == INVALID_HANDLE_VALUE) return NULL;
    do 
    {
        int IsDirectory = (Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        if ((strcmp(Data.cFileName, ".") == 0) || (strcmp(Data.cFileName, "..") == 0)) continue;
        if (IsDirectory && Recursive) 
        {
            if (Data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) continue;
            char SubFolder[MAX_PATH];
            snprintf(SubFolder, sizeof(SubFolder), "%s\\%s", Folder, Data.cFileName);
            ChesireListDirectory(SubFolder, Filter, Recursive, List, ListSize);
        }

        char* Dot = strrchr(Data.cFileName, '.');
        if (!Dot || _strcmpi(Dot, Filter) != 0) continue;
        
        char K[ListSize];
        snprintf(K, sizeof(K), "%s %s\\%s", List, Folder, Data.cFileName);
        snprintf(List, ListSize, "%s", K);

    } while (FindNextFileA(Handle, &Data));

    FindClose(Handle);
    return List;
}

int ChesireCreateProcess(char* Command)
{
    STARTUPINFOA StartupInfo;
    PROCESS_INFORMATION ProcessInfo;
    ZeroMemory(&StartupInfo, sizeof(StartupInfo));
    ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));
    StartupInfo.cb = sizeof(StartupInfo);
    char CommandLine[__StandardBufferSize];
    if (!CreateProcessA(
        NULL,
        CommandLine,
        NULL, NULL,
        FALSE,
        0, 
        NULL,
        NULL,
        &StartupInfo,
        &ProcessInfo
    ))
    {
        return 1;
    }
    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
    DWORD ExitCode = 0;
    GetExitCodeProcess(ProcessInfo.hProcess, &ExitCode);
    CloseHandle(ProcessInfo.hProcess);
    CloseHandle(ProcessInfo.hThread);
    return (int)ExitCode;
}

int ChesireDisposeProcess()
{

}

void InitializeChesireCommandReader(ChesireCommandReaderRef Reader, long ArgumentCount, char** Arguments)
{
    Reader -> Pointer = 0;
    Reader -> ArgumentCount = ArgumentCount;
    Reader -> Arguments = Arguments;
}

char* CurrentChesireCommandReader(ChesireCommandReaderRef Reader)
{
    return Reader -> Arguments[Reader -> Pointer];
}

char* PeekChesireCommandReader(ChesireCommandReaderRef Reader, long Offset)
{
    long Index = Reader -> Pointer + Offset > 0 ? Reader -> Pointer + Offset : 0;
    if (Index >= Reader -> ArgumentCount) return NULL;
    return Reader -> Arguments[Index];
}

int CreateChesireProjectFolder(char* ProjectName)
{
    __AliceMakeDir(ProjectName)
    
    char A[512]; snprintf(A, sizeof(A), "%s\\%s", ProjectName, "\\src");
    __AliceMakeDir(A)

    char B[512]; snprintf(B, sizeof(B), "%s\\%s", A, "main");
    __ChesireFile(B, "", "al")

    char C[512]; snprintf(C, sizeof(C), "%s\\%s", ProjectName, "config");
    __ChesireFile(C, "", "toml")

    char D[512]; snprintf(D, sizeof(D), "%s\\%s", ProjectName, "\\out");
    __AliceMakeDir(D)
}

int main(int argc, char* argv[]) 
{
    ChesireCommandReader CommandReader;
    InitializeChesireCommandReader(&CommandReader, argc, argv);
    for (
        ;
        CommandReader.Pointer < CommandReader.ArgumentCount;
        CommandReader.Pointer += 1)
    {
        __ArgumentIs("create")
        {
            char* ProjectName = (PeekChesireCommandReader(&CommandReader, 1));
            if (ProjectName == NULL) goto ChesireSetupDone;
            char B[512];
            snprintf(B, sizeof(B), "%s\\", ProjectName);
            CreateChesireProjectFolder(B);
        }
        __ArgumentIs("init")
        {
            CreateChesireProjectFolder("");
        }
        __ArgumentIs("run")
        {
            char Output[] = "out\\AliceExec";
            char AliceCompiler[MAX_PATH];
            DWORD Length = SearchPathA(NULL, "Alice.exe", NULL, sizeof(AliceCompiler), AliceCompiler, NULL);
            if (Length == 0 || Length >= sizeof(AliceCompiler))
            {
                printf("Alice language executable not found!");
                return 1;
            }
            char List[__StandardBufferSize] = "";
            char AliceCompilerCommand[__StandardBufferSize];
            char AliceExecutableCommand[__StandardBufferSize];

            ChesireListDirectory("src", ".al", 0, List, __StandardBufferSize);
            snprintf(AliceCompilerCommand, sizeof(AliceCompilerCommand), "%s --ll --O2 --out=%s %s", AliceCompiler, Output, List);
            snprintf(AliceExecutableCommand, sizeof(AliceExecutableCommand), "\"%s.exe\"", Output);
            int CResult = system(AliceCompilerCommand);
            int DResult = system(AliceExecutableCommand);
        }
    }
    return 0;
    ChesireSetupDone:
        return 1;
}