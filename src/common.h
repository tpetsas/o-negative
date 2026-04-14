#pragma once
#include <windows.h>
#include <string>

void LogMessage(const std::string& msg);
void LogHeader();
void LogInjectionInfo();
void LogLastError(const std::string& prefix);

void LoadOriginalDll(const std::string& name, HMODULE& handle);
void LoadModsFromDirectory(const std::string& directory);

std::string GetProcessName();
std::string GetExePath();
std::string GetModulePath(HMODULE module);
std::string GetDirectoryFromPath(const std::string& path);
