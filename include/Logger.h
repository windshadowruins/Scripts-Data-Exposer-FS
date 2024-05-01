#pragma once
#include <string>
#include "ExposerConfig.h"

namespace Logger 
{
    template<typename ... Args> static inline void log(std::string str, Args ... args)
    {
        std::printf(str.append("\n").c_str(), args ...);
    }

    static inline std::string asString(LogLevel log_level)
    {
        if (log_level == LogLevel::INFO) return "INFO";
        else if (log_level == LogLevel::DEBUG) return "DEBUG";
        else return "UNKNOWN";

    }

	static inline void logFormatted(LogLevel logLevel, const char* format, va_list args)
    {
        std::string formattedLevel = "[" + asString(logLevel) + "] ";
        vfprintf(stdout, formattedLevel.append(format).c_str(), args);
    }

    static inline void info(const char* format, ...)
    {
        va_list argptr;
        va_start(argptr, format);
        logFormatted(LogLevel::INFO, format, argptr);
    	va_end(argptr);
    }

	static inline void debug(const char* format, ...)
    {
        if (logLevel != LogLevel::DEBUG) return;
        va_list argptr;
        va_start(argptr, format);
        logFormatted(LogLevel::DEBUG, format, argptr);
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