#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <math.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include "resource.h"

extern "C"
{
#include "jpeglib.h"
};

//
// Enable pattern texture caching
//

#define TEXCACHE

#include "jpegload.h"
#include "jpegwnd.h"
#include "listutils.h"
#include "mapwnd.h"
#include "patternwnd.h"
#include "profiler.h"
#include "rows.h"
#include "statuswnd.h"
#include "workspace.h"
#include "xmlsaver.h"

#ifdef USEGL
#include <GL/gl.h>
#pragma comment(lib, "opengl32.lib")
#endif
