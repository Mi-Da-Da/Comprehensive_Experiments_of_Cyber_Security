// FileMonitorDLL.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <detours.h>
#include <stdio.h>

#pragma comment(lib,"detours.lib")

// 原始函数指针 - CreateFileA
HANDLE(WINAPI* TrueCreateFileA)(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
    ) = CreateFileA;

// 原始函数指针 - CopyFileA
BOOL(WINAPI* TrueCopyFileA)(
    LPCSTR lpExistingFileName,
    LPCSTR lpNewFileName,
    BOOL bFailIfExists
    ) = CopyFileA;

// 原始函数指针 - DeleteFileA
BOOL(WINAPI* TrueDeleteFileA)(
    LPCSTR lpFileName
    ) = DeleteFileA;

// 原始函数指针 - MoveFileA
BOOL(WINAPI* TrueMoveFileA)(
    LPCSTR lpExistingFileName,
    LPCSTR lpNewFileName
    ) = MoveFileA;

// 日志锁
CRITICAL_SECTION g_LogLock;
BOOL g_bLogInit = FALSE;

// 写日志函数
void WriteLog(const char* operation, const char* details)
{
    if (!g_bLogInit) return;

    EnterCriticalSection(&g_LogLock);

    FILE* fp = fopen("behavior_log.txt", "a");
    if (fp)
    {
        fprintf(fp, "[%s] %s\n", operation, details);
        fprintf(fp, "-----------------------------\n");
        fclose(fp);
    }

    LeaveCriticalSection(&g_LogLock);
}

// Hook CreateFileA
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
    char logMsg[512];
    const char* action = "OPEN";

    if (dwCreationDisposition == CREATE_NEW || dwCreationDisposition == CREATE_ALWAYS)
        action = "CREATE";

    sprintf(logMsg, "文件: %s, 操作: %s, 访问类型: %s%s",
        lpFileName, action,
        (dwDesiredAccess & GENERIC_READ) ? "READ " : "",
        (dwDesiredAccess & GENERIC_WRITE) ? "WRITE" : "");

    WriteLog("FileAccess", logMsg);

    return TrueCreateFileA(lpFileName, dwDesiredAccess, dwShareMode,
        lpSecurityAttributes, dwCreationDisposition,
        dwFlagsAndAttributes, hTemplateFile);
}

// Hook CopyFileA
BOOL WINAPI MyCopyFileA(
    LPCSTR lpExistingFileName,
    LPCSTR lpNewFileName,
    BOOL bFailIfExists
)
{
    char logMsg[512];
    sprintf(logMsg, "从: %s 复制到: %s", lpExistingFileName, lpNewFileName);
    WriteLog("CopyFile", logMsg);

    return TrueCopyFileA(lpExistingFileName, lpNewFileName, bFailIfExists);
}

// Hook DeleteFileA
BOOL WINAPI MyDeleteFileA(LPCSTR lpFileName)
{
    char logMsg[512];
    sprintf(logMsg, "删除文件: %s", lpFileName);
    WriteLog("DeleteFile", logMsg);

    return TrueDeleteFileA(lpFileName);
}

// Hook MoveFileA
BOOL WINAPI MyMoveFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName)
{
    char logMsg[512];
    sprintf(logMsg, "从: %s 移动到: %s", lpExistingFileName, lpNewFileName);
    WriteLog("MoveFile", logMsg);

    return TrueMoveFileA(lpExistingFileName, lpNewFileName);
}

// 安装Hook
void InstallHook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourAttach(&(PVOID&)TrueCreateFileA, MyCreateFileA);
    DetourAttach(&(PVOID&)TrueCopyFileA, MyCopyFileA);
    DetourAttach(&(PVOID&)TrueDeleteFileA, MyDeleteFileA);
    DetourAttach(&(PVOID&)TrueMoveFileA, MyMoveFileA);

    DetourTransactionCommit();

    WriteLog("SYSTEM", "DLL loaded - file monitoring hooks installed");
}

// 卸载Hook
void RemoveHook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourDetach(&(PVOID&)TrueCreateFileA, MyCreateFileA);
    DetourDetach(&(PVOID&)TrueCopyFileA, MyCopyFileA);
    DetourDetach(&(PVOID&)TrueDeleteFileA, MyDeleteFileA);
    DetourDetach(&(PVOID&)TrueMoveFileA, MyMoveFileA);

    DetourTransactionCommit();

    WriteLog("SYSTEM", "DLL unloaded - file monitoring hooks removed");
}

// DLL 入口点
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    FILE* fp = NULL;  // 将变量声明移到 switch 外面

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // 初始化日志锁
        InitializeCriticalSection(&g_LogLock);
        g_bLogInit = TRUE;

        // 清空旧日志（仅在第一次加载时清空）
        fp = fopen("behavior_log.txt", "w");
        if (fp)
        {
            fprintf(fp, "=== 文件行为监控日志 ===\n");
            fprintf(fp, "监控启动时间: %s\n", __TIME__);
            fclose(fp);
        }

        // 安装钩子
        InstallHook();
        break;

    case DLL_PROCESS_DETACH:
        // 卸载钩子
        RemoveHook();
        // 删除日志锁
        DeleteCriticalSection(&g_LogLock);
        g_bLogInit = FALSE;
        break;
    }
    return TRUE;
}