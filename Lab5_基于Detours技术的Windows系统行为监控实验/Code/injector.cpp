// Injector.cpp
#include <windows.h>
#include <stdio.h>
#include <string.h>

// 测试函数声明
void RunTests();

int main()
{
    printf("========================================\n");
    printf("  文件行为监控工具 - Injector\n");
    printf("========================================\n\n");

    // 获取当前目录的完整路径
    char dllPath[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, dllPath);
    strcat_s(dllPath, "\\hook.dll");

    printf("[1] 检查DLL文件...\n");
    if (GetFileAttributesA(dllPath) == INVALID_FILE_ATTRIBUTES)
    {
        printf("[!] 找不到 hook.dll\n");
        printf("[!] 请确保 DLL 文件在当前目录\n");
        printf("按任意键退出...\n");
        getchar();
        return -1;
    }
    printf("[OK] DLL 文件存在: %s\n\n", dllPath);

    // 加载 DLL（这会自动安装钩子）
    printf("[2] 加载 FileMonitor.dll...\n");
    HMODULE hDll = LoadLibraryA(dllPath);
    if (!hDll)
    {
        printf("[!] 加载失败，错误码: %lu\n", GetLastError());
        printf("[!] 请确保 detours.dll 在当前目录或系统路径中\n");
        getchar();
        return -1;
    }
    printf("[OK] DLL 加载成功，钩子已安装\n\n");

    printf("[3] 开始监控文件行为...\n");
    printf("    日志文件: behavior_log.txt\n");
    printf("    所有文件操作都会被记录\n\n");

    // 运行测试
    RunTests();

    // 等待用户输入后卸载
    printf("\n========================================\n");
    printf("按任意键卸载 DLL 并退出...\n");
    getchar();

    printf("\n[4] 卸载 DLL...\n");
    FreeLibrary(hDll);
    printf("[OK] DLL 已卸载，钩子已移除\n");

    printf("\n========================================\n");
    printf("监控完成！请查看 behavior_log.txt\n");
    printf("========================================\n");
    getchar();

    return 0;
}

void RunTests()
{
    printf("\n--- 开始测试 ---\n\n");

    // 测试1: 创建文件并写入内容
    printf("测试1: 创建文件 test.txt 并写入内容\n");
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
        DWORD dwWritten;
        const char* content = "Hello, this is a test file!\n";
        WriteFile(hFile, content, strlen(content), &dwWritten, NULL);
        printf("  已写入内容: %s", content);
        CloseHandle(hFile);
    }
    else
    {
        printf("  创建文件失败\n");
    }

    Sleep(1000);

    // 测试2: 复制文件
    printf("\n测试2: 复制文件 test.txt -> test_copy.txt\n");
    if (CopyFileA("test.txt", "test_copy.txt", FALSE))
        printf("  复制成功\n");
    else
        printf("  复制失败\n");

    Sleep(1000);

    // 测试3: 移动/重命名文件
    printf("\n测试3: 重命名文件 test_copy.txt -> test_renamed.txt\n");
    if (MoveFileA("test_copy.txt", "test_renamed.txt"))
        printf("  重命名成功\n");
    else
        printf("  重命名失败\n");

    Sleep(1000);

    // 测试4: 删除移动后的文件
    printf("\n测试4: 删除文件 test_renamed.txt\n");
    if (DeleteFileA("test_renamed.txt"))
        printf("  删除成功\n");
    else
        printf("  删除失败\n");

    Sleep(1000);

    // 测试5: 用记事本打开文件
    printf("\n测试5: 用记事本打开 test.txt\n");
    printf("  → 正在启动 notepad.exe 打开文件...\n");
    system("notepad.exe test.txt");

    printf("\n请关闭记事本窗口继续...\n");
    system("pause");

    printf("\n--- 测试完成 ---\n");
}