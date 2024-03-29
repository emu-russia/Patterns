// Integrated profiler.

// How does it work.
// 1. Register profiler entities in list
// 2. Encapsulate PerfStart / PerfStop in corresponding procedures to collect stats
// 3. Update procedure stats periodically on screen as overlay strings

#include "pch.h"

static BOOLEAN PerfActive;

static HFONT PerfFont;

#define FONT_HEIGHT 13

std::list<PROFILER_ENTRY *> PerfHead;

static BOOLEAN PerfWorkerExitFlag;

BOOLEAN PerfRunning(void)
{
	return PerfActive;
}

LARGE_INTEGER PerfTotalExecutionTime(void)
{
	LARGE_INTEGER Z;
	LARGE_INTEGER One;

	Z.QuadPart = 0;
	One.QuadPart = 1;

	for (auto it = PerfHead.begin(); it != PerfHead.end(); ++it) {
		PROFILER_ENTRY* Entry = *it;
		Z.QuadPart += Entry->ExecutionTime.QuadPart;
	}

	return Z.QuadPart ? Z : One;
}

DWORD WINAPI PerfWorkerThread(LPVOID lpParameter)
{
	PROFILER_ENTRY * Entry;
	LARGE_INTEGER TotalTime;

	//
	// Reset execution times
	//

	while (PerfWorkerExitFlag == FALSE)
	{
		TotalTime = PerfTotalExecutionTime();

		for (auto it = PerfHead.begin(); it != PerfHead.end(); ++it) {
			Entry = *it;
			Entry->ExecutionTime.QuadPart = 0;
		}

		if (TotalTime.QuadPart != 1) JpegRedraw();

		Sleep(1000);
	}

	return 0;
}

void PerfInit(void)
{
	LOGFONT LogFont;

	if (PerfActive) return;

	memset(&LogFont, 0, sizeof(LogFont));

	strcpy(LogFont.lfFaceName, "Calibri");
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfEscapement = 0;
	LogFont.lfHeight = FONT_HEIGHT;

	PerfFont = CreateFontIndirect(&LogFont);

	PerfWorkerExitFlag = FALSE;

	CreateThread(NULL, 0, PerfWorkerThread, NULL, 0, NULL);

	PerfActive = TRUE;
	JpegRedraw();
}

void PerfShutdown(void)
{
	if (!PerfActive) return;
	PerfActive = FALSE;

	if (PerfFont)
		DeleteObject(PerfFont);

	PerfWorkerExitFlag = TRUE;
	
	JpegRedraw();
}

void PerfRegisterEntity(char *ProcName)
{
	PROFILER_ENTRY * Entry;

	Entry = new PROFILER_ENTRY;

	memset(Entry, 0, sizeof(PROFILER_ENTRY));

	strncpy(Entry->ProcName, ProcName, sizeof(Entry->ProcName) - 1);

	PerfHead.push_back(Entry);
}

PROFILER_ENTRY *PerfGetEntry(char *ProcName)
{
	PROFILER_ENTRY *Entry;

	for (auto it = PerfHead.begin(); it != PerfHead.end(); ++it) {
		Entry = *it;
		if (!strcmp(Entry->ProcName, ProcName)) return Entry;
	}

	return NULL;
}

void PerfStart(char *ProcName)
{
	PROFILER_ENTRY *Entry;

	Entry = PerfGetEntry(ProcName);

	if (Entry && PerfActive)
	{
		QueryPerformanceCounter(&Entry->StartTime);
	}
}

void PerfStop(char *ProcName)
{
	PROFILER_ENTRY *Entry;

	Entry = PerfGetEntry(ProcName);

	if (Entry && PerfActive)
	{
		QueryPerformanceCounter(&Entry->StopTime);

		Entry->ExecutionTime.QuadPart += Entry->StopTime.QuadPart - Entry->StartTime.QuadPart;
	}
}

void PerfUpdateStats(HDC hdc)
{
	char Text[1024];
	int TextLength;
	PROFILER_ENTRY *Entry;
	int y;
	LARGE_INTEGER TotalTime;
	RECT Rect;

	if (!PerfActive) return;

	Rect.top = 0;
	Rect.left = 0;
	Rect.right = 200;
	Rect.bottom = 150;

	FillRect(hdc, &Rect, (HBRUSH)(COLOR_WINDOW + 2));

	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);

	if (PerfFont) SelectObject(hdc, PerfFont);

	//
	// Calc total execution time
	//

	TotalTime = PerfTotalExecutionTime();

	//
	// Enumerate all profiler entities
	//

	y = 0;

	for (auto it = PerfHead.begin(); it != PerfHead.end(); ++it) {
		Entry = *it;

		TextLength = sprintf(Text, "%s: %.2f%%",
			Entry->ProcName,
			((float)Entry->ExecutionTime.QuadPart * 100.0f) / (float)TotalTime.QuadPart);

		TextOut(hdc, 0, y * FONT_HEIGHT, Text, TextLength);

		y++;
	}
}
