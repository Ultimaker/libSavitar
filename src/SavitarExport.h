#ifndef SAVITAR_EXPORT_H
#define SAVITAR_EXPORT_H

// This is included here as a workaround for Python's library adding declarations that
// conflict with C++'s standard library.
#include <python.h>

#if _WIN32
    #ifdef MAKE_SAVITAR_LIB
        #define SAVITAR_EXPORT __declspec(dllexport)
    #else
        #define SAVITAR_EXPORT
    #endif
#else
    #define SAVITAR_EXPORT
#endif

#endif
