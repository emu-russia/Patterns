// Standard Cells matching utility.
// Copyright (c) 2015, psxdev.ru

// Code is crappy look away :-)

#include "pch.h"

const char g_szClassName[] = "myWindowClass";

HWND MainWnd;
HWND FlipWnd;
HWND MirrorWnd;

float WorkspaceLambda = 1.0f, WorkspaceLambdaDelta = 1.0f;
int WorkspaceRowIndex = 0;
int WorkspaceRowArrangement = 0;

char CurrentWorkingDir[MAX_PATH];

int SelectedTextSaver;

static char copiedPatternName[0x100];

BOOL ShowPatterns = TRUE;

// nice value of KB, MB or GB, for output
char * FileSmartSize(ULONG size)
{
	static char tempBuf[0x200];
	if (size < 1024)
	{
		sprintf(tempBuf, "%i byte", size);
	}
	else if (size < 1024 * 1024)
	{
		sprintf(tempBuf, "%i KB", size / 1024);
	}
	else if (size < 1024 * 1024 * 1024)
	{
		sprintf(tempBuf, "%i MB", size / 1024 / 1024);
	}
	else sprintf(tempBuf, "%1.1f GB", (float)size / 1024 / 1024 / 1024);
	return tempBuf;
}

void LoadSourceImage(HWND Parent)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = Parent;
	ofn.lpstrFilter = "Jpeg Files (*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "jpg";

	if (GetOpenFileName(&ofn))
	{
		// Do something usefull with the filename stored in szFileName

		JpegLoadImage(szFileName, FALSE);
	}
}

void SaveImage(HWND Parent)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = Parent;
	ofn.lpstrFilter = "Jpeg Files (*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "jpg";

	if (GetOpenFileName(&ofn))
	{
		// Do something usefull with the filename stored in szFileName

		//JpegSaveImage(szFileName);

		MessageBox ( Parent, "Bogus", "Not implemented", MB_OK );
	}
}


void LoadPatternsDB(HWND Parent)
{
	FILE *f;
	int filesize;
	char *buffer;
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = Parent;
	ofn.lpstrFilter = "Txt Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "txt";

	if (GetOpenFileName(&ofn))
	{
		// Do something usefull with the filename stored in szFileName

		f = fopen(szFileName, "rt");

		if (f)
		{
			fseek(f, 0, SEEK_END);
			filesize = ftell(f);
			fseek(f, 0, SEEK_SET);
			buffer = (char *)malloc(filesize + 1);
			fread(buffer, 1, filesize, f);
			buffer[filesize] = 0;
			fclose(f);
			PatternDestroy();
			ParseDatabase(buffer);
			free(buffer);
		}
	}
}


void SavePatternsXml(HWND Parent)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = Parent;
	ofn.lpstrFilter = "XML Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "xml";

	if (GetOpenFileName(&ofn))
	{
		// Do something usefull with the filename stored in szFileName

		XmlSave ( szFileName );
	}
}


void WorkspaceHandler(HWND Parent, void (*Callback)(char *filename))
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = Parent;
	ofn.lpstrFilter = "Workspace Files (*.wrk)\0*.wrk\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "wrk";

	if (GetOpenFileName(&ofn))
	{
		// Do something usefull with the filename stored in szFileName

		Callback(szFileName);
	}
}

INT_PTR CALLBACK SettingsDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char Text[1024];

	switch (Message)
	{
		case WM_INITDIALOG:

			//
			// Set Lambda
			//

			sprintf(Text, "%.1f", WorkspaceLambda);
			SetDlgItemText(hwnd, ID_LAMBDA, Text);
			sprintf(Text, "%.1f", WorkspaceLambdaDelta);
			SetDlgItemText(hwnd, ID_LAMBDA_DELTA, Text);

			//
			// Row index
			//

			sprintf(Text, "%i", WorkspaceRowIndex);
			SetDlgItemText(hwnd, ID_ROW_INDEX, Text);

			//
			// Row arrangement
			//

			sprintf(Text, "%i", WorkspaceRowArrangement);
			SetDlgItemText(hwnd, ID_ROW_ARRANGEMENT, Text);

			return TRUE;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case IDOK:

				//
				// Get Lambda, row index, arrangement
				//

				GetDlgItemText(hwnd, ID_LAMBDA, Text, sizeof(Text));
				WorkspaceLambda = (float) atof(Text);
				GetDlgItemText(hwnd, ID_LAMBDA_DELTA, Text, sizeof(Text));
				WorkspaceLambdaDelta = (float) atof(Text);
				GetDlgItemText(hwnd, ID_ROW_INDEX, Text, sizeof(Text));
				WorkspaceRowIndex = atoi(Text);
				GetDlgItemText(hwnd, ID_ROW_ARRANGEMENT, Text, sizeof(Text));
				WorkspaceRowArrangement = atoi(Text);

				sprintf(Text, "Lambda / Delta / Row : %.1f / %.1f / %i",
					WorkspaceLambda, WorkspaceLambdaDelta, WorkspaceRowIndex);
				SetStatusText(STATUS_LAMBDA_DELTA, Text);

				JpegRedraw();

				EndDialog(hwnd, IDOK);
				break;
			case IDCANCEL:
				EndDialog(hwnd, IDCANCEL);
				break;
			}
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK ContainsStringDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char Text[1024];

	switch (Message)
	{
		case WM_INITDIALOG:
			return TRUE;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case IDOK:
				GetDlgItemText(hwnd, ID_PART_OF_NAME, Text, sizeof(Text));

				if (strlen(Text) >= 3)
				{
					JpegNextContainsString(Text);
				}

				EndDialog(hwnd, IDOK);
				break;
			case IDCANCEL:
				EndDialog(hwnd, IDCANCEL);
				break;
			}
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT Rect;
	PatternEntry * selected;

	switch (msg)
	{
		case WM_CREATE:
			CreateStatusBar(hwnd);
			JpegInit(hwnd);
			PatternInit(hwnd, "patterns_db.txt");
			MapInit(hwnd);

			GetClientRect(hwnd, &Rect);
			JpegResize(Rect.right, Rect.bottom);
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_SIZE:
			JpegResize(LOWORD(lParam), HIWORD(lParam) - GetStatusBarHeight());
			PatternResize(LOWORD(lParam), HIWORD(lParam) - GetStatusBarHeight());
			MapResize(LOWORD(lParam), HIWORD(lParam) - GetStatusBarHeight());
			ResizeStatusBar(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case ID_FILE_EXIT:
				exit(0);
				break;
			case ID_FILE_LOAD:
				LoadSourceImage(hwnd);
				break;
			case ID_FILE_LOAD_PATTERNS_DB:
				LoadPatternsDB(hwnd);
				break;
			case ID_SAVE_PATXML:
				SavePatternsXml(hwnd);
				break;
			case ID_SETTINGS:
				DialogBox(GetModuleHandle(NULL),
					MAKEINTRESOURCE(IDD_SETTINGS), hwnd, SettingsDlgProc);
				break;
			case ID_WORKSPACE_LOAD:
				WorkspaceHandler(hwnd, LoadWorkspace);
				break;
			case ID_WORKSPACE_SAVE:
				WorkspaceHandler(hwnd, SaveWorkspace);
				break;
			case ID_ENSURE_VISIBLE:
				JpegEnsureVisible(JpegGetSelectedPattern());
				break;
			case ID_REMOVE_ALL_PATTERNS:
				JpegRemoveAllPatterns();
				break;
			case ID_DEBUG_REMOVE:
				//JpegRemoveAllLessX(6400);
				//JpegRemoveAllGreaterX(5800);
				break;
			case ID_NAVIGATE_ORIGIN:
				JpegGotoOrigin();
				break;
			case ID_NAVIGATE_UNKNOWN:
				JpegNextUnknown();
				break;
			case ID_NAVIGATE_GARBAGE:
				JpegNextGarbage();
				break;
			case ID_NAVIGATE_CONTAINS:
				DialogBox(GetModuleHandle(NULL),
					MAKEINTRESOURCE(IDD_CONTAINS), hwnd, ContainsStringDlgProc);
				break;
			case ID_REMOVE_SELECTION:
				JpegSelectPattern(NULL);
				JpegRedraw();
				break;
			case ID_PATTERN_MOVE_LEFT:
				selected = JpegGetSelectedPattern();
				if (selected)
				{
					selected->PlaneX--;
					selected->PosX--;
					JpegRedraw();
				}
				break;
			case ID_PATTERN_MOVE_RIGHT:
				selected = JpegGetSelectedPattern();
				if (selected)
				{
					selected->PlaneX++;
					selected->PosX++;
					JpegRedraw();
				}
				break;
			case ID_PATTERN_MOVE_UP:
				selected = JpegGetSelectedPattern();
				if (selected)
				{
					selected->PlaneY--;
					selected->PosY--;
					JpegRedraw();
				}
				break;
			case ID_PATTERN_MOVE_DOWN:
				selected = JpegGetSelectedPattern();
				if (selected)
				{
					selected->PlaneY++;
					selected->PosY++;
					JpegRedraw();
				}
				break;
			case ID_HELP_ABOUT:
				MessageBox(NULL, "Patterns, v.1.1\n(c) 2022, Emu-Russia", "About Patterns",
					MB_ICONINFORMATION | MB_OK);
				break;
			case ID_HELP_HOTKEYS:
				MessageBox(NULL, 
					"Escape: remove selection\n"
					"Home: goto origin (0,0)\n"
					"Arrows: fine movement of selected pattern\n" 
					"Ctrl+C: copy selected pattern\n"
					"Ctrl+V: paste pattern at selection box coords\n"
					"Ctrl+U: cycle next unknown pattern (if any)\n"
					"Ctrl+G: cycle next garbage pattern (if any)\n"
					"Space: show/hide patterns layer\n"
					,
					"Hotkey bindings",
					MB_ICONINFORMATION | MB_OK);
				break;
			case ID_EDIT_COPY:
				selected = JpegGetSelectedPattern();
				if (selected)
				{
					strcpy(copiedPatternName, selected->PatternName);
				}
				else
				{
					copiedPatternName[0] = 0;
				}
				break;
			case ID_EDIT_PASTE:
				if (copiedPatternName[0] != 0)
				{
					AddPatternEntry(copiedPatternName);
					JpegRedraw();
				}
				break;
			case ID_SHOW_PROFILER:
				if (PerfRunning())
				{
					PerfShutdown();
					CheckMenuItem(GetMenu(hwnd), ID_SHOW_PROFILER, MF_BYCOMMAND | MF_UNCHECKED);
				}
				else
				{
					PerfInit();
					CheckMenuItem(GetMenu(hwnd), ID_SHOW_PROFILER, MF_BYCOMMAND | MF_CHECKED);
				}
				break;
			case ID_SHOW_PATTERNS:
				if (ShowPatterns)
				{
					ShowPatterns = FALSE;
					CheckMenuItem(GetMenu(hwnd), ID_SHOW_PATTERNS, MF_BYCOMMAND | MF_UNCHECKED);
					JpegRedraw();
				}
				else
				{
					ShowPatterns = TRUE;
					CheckMenuItem(GetMenu(hwnd), ID_SHOW_PATTERNS, MF_BYCOMMAND | MF_CHECKED);
					JpegRedraw();
				}
				break;
			}
			if (HIWORD(wParam) == BN_CLICKED && (HWND)lParam == FlipWnd)
			{
				if (Button_GetCheck(FlipWnd) == BST_CHECKED)
				{
					Button_SetCheck(FlipWnd, BST_UNCHECKED);
				}
				else
				{
					Button_SetCheck(FlipWnd, BST_CHECKED);
				}
				RearrangePatternTiles();
				PatternRedraw();
			}
			if (HIWORD(wParam) == BN_CLICKED && (HWND)lParam == MirrorWnd)
			{
				if (Button_GetCheck(MirrorWnd) == BST_CHECKED)
				{
					Button_SetCheck(MirrorWnd, BST_UNCHECKED);
				}
				else
				{
					Button_SetCheck(MirrorWnd, BST_CHECKED);
				}
				RearrangePatternTiles();
				PatternRedraw();
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void AddProfilerProcs(void)
{
	//
	// Register only time-consuming procedures
	//

	//
	// JpegWnd
	//

	PerfRegisterEntity("JpegLoadImage");
	PerfRegisterEntity("AddPatternEntry");
	PerfRegisterEntity("UpdatePatternEntry");
	PerfRegisterEntity("JpegRedraw");
	PerfRegisterEntity("JpegSelectPattern");
	PerfRegisterEntity("UpdateEntryPositions");
	PerfRegisterEntity("Entry WM_PAINT");
	PerfRegisterEntity("JpegWnd WM_PAINT");
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	MSG Msg;
	HACCEL haccel;

#if defined(_DEBUG)
	AllocConsole();

	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

	printf("Patterns, v.1.1\n");
#endif

	AddProfilerProcs();

	GetCurrentDirectory(sizeof(CurrentWorkingDir), CurrentWorkingDir);

	InitCommonControls();

	//Step 1: Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 5);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	MainWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Patterns",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	if (MainWnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(MainWnd, nCmdShow);
	UpdateWindow(MainWnd);

	//
	// Show patterns by default
	//

	ShowPatterns = TRUE;
	CheckMenuItem(GetMenu(MainWnd), ID_SHOW_PATTERNS, MF_BYCOMMAND | MF_CHECKED);

	haccel = LoadAccelerators(hInstance, "PatternsAccel");

	// Step 3: The Message Loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		if (!TranslateAccelerator(
			MainWnd,  // handle to receiving window
			haccel,    // handle to active accelerator table
			&Msg))         // message data
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
	return (int) Msg.wParam;
}
