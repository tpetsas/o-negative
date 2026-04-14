#include "common.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;

static HMODULE gRealDInput8 = nullptr;
static const std::string gDInput8Name = "dinput8.dll";


// Optional: Forward DirectInput8Create if needed
extern "C" __declspec(dllexport)
HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion,
                        REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter) {
    LogMessage("DirectInput8Create entered");
    LogHeader();
    LogMessage("dinput8.dll proxy loaded");
    LogMessage("EXE path: " + GetExePath());
    LogMessage("Proxy path: " + GetModulePath((HMODULE)&__ImageBase));
    LogInjectionInfo();

    // Load al the mods in "mods" directory

    std::string modsDir = GetDirectoryFromPath(GetModulePath((HMODULE)&__ImageBase)) + "\\mods";
    LoadModsFromDirectory(modsDir);
    //LoadModsFromDirectory("mods");

    LoadOriginalDll(gDInput8Name, gRealDInput8);

    using CreateFunc = decltype(&DirectInput8Create);
    static CreateFunc originalFunc = nullptr;
    if (!originalFunc) {
        originalFunc = (CreateFunc) GetProcAddress (
                gRealDInput8, "DirectInput8Create"
        );
        if (!originalFunc) {
            LogLastError("ERROR: Failed to get address of DirectInput8Create");
            return E_FAIL;
        }
    }
    LogMessage("Forwarding to original DirectInput8Create");
    return originalFunc(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);

        char path[MAX_PATH] = {};
        GetModuleFileNameA(hModule, path, MAX_PATH);

        OutputDebugStringA("[O-] DLL_PROCESS_ATTACH hit\n");
        OutputDebugStringA(path);
        OutputDebugStringA("\n");

        HANDLE h = CreateFileA(
            "C:\\Temp\\o-neg-dinput.txt",
            FILE_APPEND_DATA,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (h != INVALID_HANDLE_VALUE) {
            DWORD written = 0;
            const char* msg1 = "[O-] DLL_PROCESS_ATTACH hit\r\n";
            WriteFile(h, msg1, (DWORD)strlen(msg1), &written, nullptr);
            WriteFile(h, path, (DWORD)strlen(path), &written, nullptr);
            const char* msg2 = "\r\n";
            WriteFile(h, msg2, 2, &written, nullptr);
            CloseHandle(h);
        }
    }
    return TRUE;
}

