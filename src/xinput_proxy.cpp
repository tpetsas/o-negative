#include <windows.h>
#include <cstring>
#include <string>
#include <mutex>
#include "common.h"

static HMODULE gRealXInput = nullptr;
static const std::string gXInputName = "xinput1_4.dll";
static std::once_flag gInitOnce;

// Minimal XInput type definitions
typedef struct _XINPUT_GAMEPAD {
    WORD  wButtons;
    BYTE  bLeftTrigger;
    BYTE  bRightTrigger;
    SHORT sThumbLX;
    SHORT sThumbLY;
    SHORT sThumbRX;
    SHORT sThumbRY;
} XINPUT_GAMEPAD;

typedef struct _XINPUT_STATE {
    DWORD          dwPacketNumber;
    XINPUT_GAMEPAD Gamepad;
} XINPUT_STATE;

typedef struct _XINPUT_VIBRATION {
    WORD wLeftMotorSpeed;
    WORD wRightMotorSpeed;
} XINPUT_VIBRATION;

typedef struct _XINPUT_CAPABILITIES {
    BYTE             Type;
    BYTE             SubType;
    WORD             Flags;
    XINPUT_GAMEPAD   Gamepad;
    XINPUT_VIBRATION Vibration;
} XINPUT_CAPABILITIES;

typedef struct _XINPUT_BATTERY_INFORMATION {
    BYTE BatteryType;
    BYTE BatteryLevel;
} XINPUT_BATTERY_INFORMATION;

typedef struct _XINPUT_KEYSTROKE {
    WORD  VirtualKey;
    WCHAR Unicode;
    WORD  Flags;
    BYTE  UserIndex;
    BYTE  HidCode;
} XINPUT_KEYSTROKE;

static void EnsureOriginalLoaded() {
    if (!gRealXInput) {
        LoadOriginalDll(gXInputName, gRealXInput);
    }
}

template <typename T>
static T Resolve(const char* name) {
    EnsureOriginalLoaded();
    if (!gRealXInput) {
        return nullptr;
    }

    FARPROC proc = GetProcAddress(gRealXInput, name);
    if (!proc) {
        LogLastError(std::string("GetProcAddress failed for ") + name);
        return nullptr;
    }

    return reinterpret_cast<T>(proc);
}

static void InitializeProxy() {
    std::call_once(gInitOnce, []() {
        LogMessage("Initializing through xinput1_4 proxy");
        LogInjectionInfo();
        LogMessage("EXE path: " + GetExePath());
        std::string exeDir = GetDirectoryFromPath(GetExePath());
        std::string modsDir = exeDir + "\\mods";
        LogMessage("Resolved mods directory: " + modsDir);
        LoadModsFromDirectory(modsDir);
    });
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        char path[MAX_PATH] = {};
        GetModuleFileNameA(hModule, path, MAX_PATH);
        OutputDebugStringA("[O-] xinput1_4 attach\n");
        OutputDebugStringA(path);
        OutputDebugStringA("\n");
    }
    return TRUE;
}

extern "C" __declspec(dllexport)
DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState) {
    InitializeProxy();
    using Fn = DWORD (WINAPI*)(DWORD, XINPUT_STATE*);
    static Fn original = nullptr;

    if (!original) {
        original = Resolve<Fn>("XInputGetState");
        if (!original) {
            return ERROR_PROC_NOT_FOUND;
        }
    }

    DWORD result = original(dwUserIndex, pState);
    return result;
}

extern "C" __declspec(dllexport)
DWORD WINAPI XInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration) {

    using Fn = DWORD (WINAPI*)(DWORD, XINPUT_VIBRATION*);
    static Fn original = nullptr;

    if (!original) {
        original = Resolve<Fn>("XInputSetState");
        if (!original) {
            return ERROR_PROC_NOT_FOUND;
        }
    }

    DWORD result = original(dwUserIndex, pVibration);
    return result;
}

extern "C" __declspec(dllexport)
DWORD WINAPI XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities) {

    using Fn = DWORD (WINAPI*)(DWORD, DWORD, XINPUT_CAPABILITIES*);
    static Fn original = nullptr;

    if (!original) {
        original = Resolve<Fn>("XInputGetCapabilities");
        if (!original) {
            return ERROR_PROC_NOT_FOUND;
        }
    }

    DWORD result = original(dwUserIndex, dwFlags, pCapabilities);
    return result;
}

extern "C" __declspec(dllexport)
void WINAPI XInputEnable(BOOL enable) {

    using Fn = void (WINAPI*)(BOOL);
    static Fn original = nullptr;

    if (!original) {
        original = Resolve<Fn>("XInputEnable");
        if (!original) {
            return;
        }
    }

    original(enable);
}

extern "C" __declspec(dllexport)
DWORD WINAPI XInputGetBatteryInformation(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation) {

    using Fn = DWORD (WINAPI*)(DWORD, BYTE, XINPUT_BATTERY_INFORMATION*);
    static Fn original = nullptr;

    if (!original) {
        original = Resolve<Fn>("XInputGetBatteryInformation");
        if (!original) {
            return ERROR_PROC_NOT_FOUND;
        }
    }

    DWORD result = original(dwUserIndex, devType, pBatteryInformation);
    return result;
}

extern "C" __declspec(dllexport)
DWORD WINAPI XInputGetKeystroke(DWORD dwUserIndex, DWORD dwReserved, XINPUT_KEYSTROKE* pKeystroke) {

    using Fn = DWORD (WINAPI*)(DWORD, DWORD, XINPUT_KEYSTROKE*);
    static Fn original = nullptr;

    if (!original) {
        original = Resolve<Fn>("XInputGetKeystroke");
        if (!original) {
            return ERROR_PROC_NOT_FOUND;
        }
    }

    DWORD result = original(dwUserIndex, dwReserved, pKeystroke);
    return result;
}

extern "C" __declspec(dllexport)
DWORD WINAPI XInputGetAudioDeviceIds(
    DWORD dwUserIndex,
    LPWSTR pRenderDeviceId,
    UINT* pRenderCount,
    LPWSTR pCaptureDeviceId,
    UINT* pCaptureCount)
{

    using Fn = DWORD (WINAPI*)(DWORD, LPWSTR, UINT*, LPWSTR, UINT*);
    static Fn original = nullptr;

    if (!original) {
        original = Resolve<Fn>("XInputGetAudioDeviceIds");
        if (!original) {
            return ERROR_PROC_NOT_FOUND;
        }
    }

    DWORD result = original(dwUserIndex, pRenderDeviceId, pRenderCount, pCaptureDeviceId, pCaptureCount);
    return result;
}
