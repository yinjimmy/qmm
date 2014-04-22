#ifndef __CCPLATFORMDEFINE_H__
#define __CCPLATFORMDEFINE_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_QT)
    #include <QtGlobal>
    #undef CC_DLL
    #if defined(_USRDLL)
        #ifdef Q_OS_WIN
                #define CC_DLL     __declspec(dllexport)
        #else
                #define CC_DLL
        #endif
    #else 		/* use a DLL library */
        #define CC_DLL
    #endif
#endif

#include <assert.h>

#if CC_DISABLE_ASSERT > 0
#define CC_ASSERT(cond)
#else
#define CC_ASSERT(cond)    assert(cond)
#endif
#define CC_UNUSED_PARAM(unusedparam) (void)unusedparam

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif



#endif /* __CCPLATFORMDEFINE_H__*/
