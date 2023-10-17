#pragma once
/* #include <iostream> */

enum LOG_LEVEL { WARNING = 0, ERROR = 1, DEBUG = 2, INFO = 3 };

#ifdef LOG
#define LOG_ERROR(M)                                                           \
    do {                                                                       \
        if (Log::IsLevelActive(Log::Error))                                    \
            (Log::GetStream() << "[ERROR]: " << M << "\n");                    \
    } while (false)
#define LOG_INFO(M)                                                            \
    do {                                                                       \
        if (Log::IsLevelActive(Log::Info))                                     \
            (Log::GetStream() << "[INFO]: " << M << "\n");                     \
    } while (false)
#define LOG_WARNING(M)                                                         \
    do {                                                                       \
        if (Log::IsLevelActive(Log::Warning))                                  \
            (Log::GetStream() << "[WARNING]: " << M << "\n");                  \
    } while (false)
#else
#define LOG_ERROR(M)
#define LOG_INFO(M)
#define LOG_WARNING(M)
#endif
