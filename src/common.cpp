
#include "common.h"
#include <fstream>
#include <filesystem>
#include <mutex>

static std::mutex gLogMutex;
static std::string gTag = "[O⁻] ";
static bool gMainLogStarted = false;

namespace fs = std::filesystem;

std::string GetModulePath(HMODULE module) {
    char path[MAX_PATH] = {};
    GetModuleFileNameA(module, path, MAX_PATH);
    return std::string(path);
}

std::string GetExePath() {
    return GetModulePath(nullptr);
}

std::string GetDirectoryFromPath(const std::string& path) {
    size_t lastSlash = path.find_last_of("\\/");
    return (lastSlash != std::string::npos) ? path.substr(0, lastSlash) : ".";
}

static std::string GetLogPath() {
    return GetDirectoryFromPath(GetExePath()) + "\\o-neg.log";
}

static std::string GetLastErrorString(DWORD err) {
    if (err == 0) return "0";

    LPSTR buffer = nullptr;
    DWORD size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&buffer,
        0,
        nullptr
    );

    std::string message = (size && buffer) ? std::string(buffer, size) : "Unknown error";
    if (buffer) LocalFree(buffer);
    return message;
}

void LogMessage(const std::string& msg) {
    std::lock_guard<std::mutex> lock(gLogMutex);

    std::ios::openmode mode = std::ios::out;
    mode |= gMainLogStarted ? std::ios::app : std::ios::trunc;

    std::ofstream log(GetLogPath(), mode);
    if (!log.is_open()) {
        return;
    }

    if (!gMainLogStarted) {
        log << gTag << "===== NEW RUN =====" << std::endl;
        gMainLogStarted = true;
    }

    log << gTag << msg << std::endl;
}

void LogLastError(const std::string& prefix) {
    DWORD err = GetLastError();
    LogMessage(prefix + " | GetLastError=" + std::to_string(err) + " | " + GetLastErrorString(err));
}

void LoadOriginalDll(const std::string& name, HMODULE& handle) {
    LogMessage("Attempt to load original: " + name);

    if (!handle) {
        char sysPath[MAX_PATH] = {};
        GetSystemDirectoryA(sysPath, MAX_PATH);
        strcat_s(sysPath, "\\");
        strcat_s(sysPath, name.c_str());

        LogMessage(std::string("Resolved original path: ") + sysPath);

        handle = LoadLibraryA(sysPath);
        if (handle) {
            LogMessage("Loaded original " + name);
        } else {
            LogLastError("Failed to load original " + name);
        }
    }
}

void LoadModsFromDirectory(const std::string& directory) {
    LogMessage("Scanning for mods in: " + directory);

    if (!fs::exists(directory)) {
        LogMessage("Mods directory does not exist");
        return;
    }

    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".dll") {
                std::string path = entry.path().string();
                LogMessage("Loading mod: " + path);

                HMODULE plugin = LoadLibraryA(path.c_str());
                if (plugin) {
                    LogMessage("Successfully loaded: " + path);
                } else {
                    LogLastError("Failed to load: " + path);
                }
            }
        }
    } catch (const std::exception& e) {
        LogMessage(std::string("Error scanning mods: ") + e.what());
    }
}

std::string GetProcessName() {
    std::string path = GetExePath();
    size_t lastSlash = path.find_last_of("\\/");
    return (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;
}

void LogInjectionInfo() {
    LogMessage("Injecting mods into: " + GetProcessName());
    LogMessage("Game detected: Type A+");
    LogMessage("Compatibility confirmed. Proceeding with transfusion.");
}
