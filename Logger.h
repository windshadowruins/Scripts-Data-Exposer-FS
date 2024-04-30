#pragma once
#include <string>
#include "ExposerConfig.h"

namespace Logger 
{
    static inline void log(std::string str)
    {
        std::printf(str.append("\n").c_str());
    }

	static inline void logFormatted(const char* format, va_list args)
    {
        vfprintf(stdout, format, args);
    }

    static inline void info(const char* format, ...)
    {
        va_list argptr;
        va_start(argptr, format);
        logFormatted(format, argptr);
    	va_end(argptr);
    }

	static inline void debug(const char* format, ...)
    {
        if (logLevel != LogLevel::DEBUG) return;
        va_list argptr;
        va_start(argptr, format);
        logFormatted(format, argptr);
    	va_end(argptr);
    }

	static inline void error(const char* format, ...)
    {
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stderr, format, argptr);
    	va_end(argptr);
    }
}