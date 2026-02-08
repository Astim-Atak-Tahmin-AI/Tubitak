#pragma once
#include <Arduino.h>

bool logger_init(bool formatIfFail=true);
bool logger_appendLine(const char* path, const String& line);
size_t logger_fileSize(const char* path);
bool logger_rotateIfTooBig(const char* path, size_t maxBytes, const char* rotatedPath);