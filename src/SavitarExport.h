#ifndef SAVITAR_EXPORT_H
#define SAVITAR_EXPORT_H

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
