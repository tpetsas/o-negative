#include "common.h"

static HMODULE gRealDInput8 = nullptr;
static const std::string gDInput8Name = "dinput8.dll";


// Optional: Forward DirectInput8Create if needed
extern "C" __declspec(dllexport)
HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion,
                        REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter) {
    LogHeader();
    LogMessage("dinput8.dll proxy loaded");
    LogInjectionInfo();

    // Load al the mods in "mods" directory
    LoadModsFromDirectory("mods");

    LoadOriginalDll(gDInput8Name, gRealDInput8);

    using CreateFunc = decltype(&DirectInput8Create);
    static CreateFunc originalFunc = nullptr;
    if (!originalFunc) {
        originalFunc = (CreateFunc) GetProcAddress (
                gRealDInput8, "DirectInput8Create"
        );
        if (!originalFunc) {
            LogMessage("ERROR: Failed to get address of DirectInput8Create");
            return E_FAIL;
        }
    }

    return originalFunc(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

// Entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        // Optional: LoadOriginalDInput8(); // preload if needed
    }
    return TRUE;
}
