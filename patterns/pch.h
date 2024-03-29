#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <math.h>
#include <list>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include "resource.h"

#define VERSION_STR L"v.1.3"
#define VERSION_YEAR_STR L"2023"

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
#include "mapwnd.h"
#include "patternwnd.h"
#include "profiler.h"
#include "rows.h"
#include "statuswnd.h"
#include "workspace.h"
#include "xmlsaver.h"
#include "txtsaver.h"

#include <GL/gl.h>
