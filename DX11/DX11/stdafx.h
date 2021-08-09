// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
//#include <iostream>
//#include "DebugStream.h"


//#define	TRACE(_format_, ...)					\
//		{						\
//			char temp[MAX_PATH];			\
//			sprintf(temp, _format_, __VA_ARGS__);	\
//			OutputDebugString(temp);		\
//		}

#   define MyOutputDebugString( str, ... ) \
      { \
        TCHAR c[256]; \
        _stprintf_s( c, str, __VA_ARGS__ ); \
        OutputDebugString( c ); \
      }

// TODO: reference additional headers your program requires here
