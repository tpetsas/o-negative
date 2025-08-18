
#include "common.h"
#include <fstream>
#include <filesystem>
#include <mutex>

static std::mutex gLogMutex;
static std::string gTag = "[O⁻] ";
static bool gLoggedHeader = false;

namespace fs = std::filesystem;

void LogMessage(const std::string& msg) {
    std::ofstream log("o-neg.log", std::ios::app);
    if (log.is_open()) {
    	log << gTag << msg << std::endl;
	}
}

void LoadOriginalDll(const std::string& name, HMODULE& handle) {
    LogMessage("Attempt to load original: " + name);
    if (!handle) {
        char sysPath[MAX_PATH];
        GetSystemDirectoryA(sysPath, MAX_PATH);
        strcat_s(sysPath, "\\");
        strcat_s(sysPath, name.c_str());
        handle = LoadLibraryA(sysPath);
        if (handle) {
            LogMessage("Loaded original " + name);
        } else {
            LogMessage("Failed to load original " + name);
        }
    }
}

void LoadModsFromDirectory(const std::string& directory) {
    LogMessage("Scanning for mods in: " + directory);
    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                std::string path = entry.path().string();
                if (entry.path().extension() == ".dll") {
                    LogMessage("Loading mod: " + path);
                    HMODULE plugin = LoadLibraryA(path.c_str());
                    if (plugin) {
                        LogMessage("Successfully loaded: " + path);
                    } else {
                        LogMessage("Failed to load: " + path);
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        LogMessage(std::string("Error scanning mods: ") + e.what());
    }
}

std::string GetProcessName() {
    char fullPath[MAX_PATH];
    GetModuleFileNameA(NULL, fullPath, MAX_PATH);

    // Extract just the EXE name
    std::string path(fullPath);
    size_t lastSlash = path.find_last_of("\\/");
    return (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;
}

void LogHeader() {
    std::lock_guard<std::mutex> lock(gLogMutex); 
    if (gLoggedHeader)
        return;
    // Log header only once
    LogMessage("O-Negative v1.0 by Thanos Petsas (SkyExplosionist)");
    gLoggedHeader = true;
}

void LogInjectionInfo() {
    std::string exeName = GetProcessName(); 
    LogMessage("Injecting mods into: " + exeName);
    LogMessage("Game detected: Type A+");
    LogMessage("Compatibility confirmed. Proceeding with transfusion.");
}
