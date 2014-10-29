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
#include "Commctrl.h"
#include "Resource.h"


// TODO: reference additional headers your program requires here
#include "Array.h"
#include "File.h"
#include "Text_Arrays.h"
#include "Liste_Fichiers.h"

#define LOADING_TREE { \
	TEXT("Data"), \
	TEXT("Data/Text"), \
	TEXT("Data/Text/de"), \
	TEXT("Data/Text/en"), \
	TEXT("Data/Text/es"), \
	TEXT("Data/Text/fr"), \
	TEXT("Data/Text/it"), \
	TEXT("Data/Text/kana"), \
	TEXT("Data/Text/ko") \
	}
#define LOADING_TREE_SIZE 9

#define INI_FILE TEXT(".//TextEditor.ini")
#define INI_CAT_OPT TEXT("Options")
#define INI_VAR_OPT_PRO TEXT("Prompt")
#define INI_VAR_OPT_AUS TEXT("AutoSave")
#define APP_NAME TEXT("TextEditor")
#define SIZEOF_LONG sizeof(long)

HINSTANCE global_get_hInst();
BOOL CALLBACK Editor_WNDPROC(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK About_WNDPROC(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Option_WNDPROC(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Max_WNDPROC(HWND , UINT , WPARAM , LPARAM);
