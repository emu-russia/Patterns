#include "pch.h"

extern int WorkspaceRowArrangement;		// 0 - Vert, 1 - Horz

static BOOL ReplaceSpecialChars(char* string)
{
	BOOL Result = FALSE;
	char* ptr = string;

	if (string == NULL)
		return FALSE;

	while (*ptr)
	{
		if (*ptr == '&')
		{
			*ptr = '_';
			Result = TRUE;
		}
		else if (*ptr == '|')
		{
			*ptr = '_';
			Result = TRUE;
		}
		else if (*ptr == '/')
		{
			*ptr = 'N';
			Result = TRUE;
		}
		ptr++;
	}

	return Result;
}

void TxtSave(char* FileName)
{
	int i, NumCells;
	PatternEntry* Pattern;
	PatternItem* Item;
	FILE* f;
	CHAR Name[0x100];

	f = fopen(FileName, "wt");
	if (!f)
		return;

	NumCells = GetPatternEntryNum();

	for (i = 0; i < NumCells; i++)
	{
		Pattern = GetPatternEntry(i);

		Item = PatternGetItem(Pattern->PatternName);

		strcpy(Name, Pattern->PatternName);
		ReplaceSpecialChars(Name);

		fprintf(f, "%s ", Name);
	}

	fprintf(f, "\n");

	fclose(f);
}
