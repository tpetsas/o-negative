
#pragma once

#include <windows.h>
#include <string>

void LogMessage(const std::string& msg);
void LoadOriginalDll(const std::string& name, HMODULE& handle);
void LoadModsFromDirectory(const std::string& directory);
std::string GetProcessName();
void LogHeader();
void LogInjectionInfo();
