#pragma once
#include <string>
#include "ExposerConfig.h"

namespace Logger 
{
    template<typename ... Args> static inline void log(std::string str, Args ... args)
    {
        std::printf(str.append("\n").c_str(), args ...);
    }


#if _DEBUG
    template<typename ... Args> static inline void debug(std::string str, Args ... args)
    {
        std::printf(str.append("\n").c_str(), args ...);
    }
#else
    template<typename ... Args> static inline void debug(std::string str, Args ... args)
    {
    }
#endif
=======
    static inline std::string asString(LogLevel logLevel)
    {
        if (logLevel == LogLevel::INFO) return "INFO";
        else if (logLevel == LogLevel::DEBUG) return "DEBUG";
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