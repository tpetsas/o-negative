#include <windows.h>
#include <string>

static std::string GetModulePath(HMODULE module) {
    char path[MAX_PATH] = {};
    GetModuleFileNameA(module, path, MAX_PATH);
    return std::string(path);
}

static std::string GetDirectoryFromPath(const std::string& path) {
    size_t lastSlash = path.find_last_of("\\/");
    return (lastSlash != std::string::npos) ? path.substr(0, lastSlash) : ".";
}

static void AppendLine(const std::string& path, const std::string& line) {
    HANDLE h = CreateFileA(
        path.c_str(),
        FILE_APPEND_DATA,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (h != INVALID_HANDLE_VALUE) {
        DWORD written = 0;
        WriteFile(h, line.c_str(), (DWORD)line.size(), &written, nullptr);
        CloseHandle(h);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);

        std::string dllPath = GetModulePath(hModule);
        std::string dllDir  = GetDirectoryFromPath(dllPath);
        std::string logPath = dllDir + "\\test_mod.log";

        AppendLine(logPath, "[test_mod] DLL_PROCESS_ATTACH\r\n");
        AppendLine(logPath, "[test_mod] path: " + dllPath + "\r\n");
    }

    return TRUE;
}
