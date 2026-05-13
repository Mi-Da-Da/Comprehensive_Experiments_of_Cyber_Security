#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <detours.h>
#include <stdio.h>

#pragma comment(lib,"detours.lib")

// 原函数指针
HANDLE(WINAPI* TrueCreateFileA)(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
    ) = CreateFileA;


// Hook函数
HANDLE WINAPI MyCreateFileA(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
)
{
    FILE* fp = fopen("log.txt", "a");

    if (fp)
    {
        fprintf(fp, "CreateFile called: %s\n", lpFileName);
        fclose(fp);
    }

    return TrueCreateFileA(
        lpFileName,
        dwDesiredAccess,
        dwShareMode,
        lpSecurityAttributes,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        hTemplateFile
    );
}


void Hook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourAttach(&(PVOID&)TrueCreateFileA, MyCreateFileA);

    DetourTransactionCommit();
}

void UnHook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourDetach(&(PVOID&)TrueCreateFileA, MyCreateFileA);

    DetourTransactionCommit();
}

int main()
{
    Hook();

    // 测试
    HANDLE hFile = CreateFileA(
        "test.txt",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hFile);
    }

    UnHook();

    return 0;
}