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
	FILE* f;
	CHAR Name[0x100];

	f = fopen(FileName, "wt");
	if (!f)
		return;

	// Collect patterns

	NumCells = GetPatternEntryNum();
	PatternEntry* patterns = new PatternEntry[NumCells];
	for (i = 0; i < NumCells; i++)
	{
		Pattern = GetPatternEntry(i);
		memcpy(&patterns[i], Pattern, sizeof(PatternEntry));
	}

	// Get the rows

	std::list<RowEntry*> rows;

	RecalcRows(rows, patterns, NumCells);

	// Walk the rows

	std::list< PatternEntry*> row_patterns;

	for (auto it = rows.begin(); it != rows.end(); ++it)
	{
		RowEntry* row = *it;
		row_patterns.clear();

		for (i = 0; i < NumCells; i++)
		{
			Pattern = GetPatternEntry(i);

			bool hit = false;

			if (WorkspaceRowArrangement == 0) {
				if (Pattern->PlaneX >= row->planeX && Pattern->PlaneX < (row->planeX + Pattern->Width)) {
					hit = true;
				}
			}
			else {
				if (Pattern->PlaneY >= row->planeY && Pattern->PlaneY < (row->planeY + Pattern->Height)) {
					hit = true;
				}
			}

			if (hit) {
				row_patterns.push_back(Pattern);
			}
		}

		// Sort
		if (WorkspaceRowArrangement == 0) {
			row_patterns.sort([](const PatternEntry* f, const PatternEntry* s) { return f->PlaneY < s->PlaneY; });
		}
		else {
			row_patterns.sort([](const PatternEntry* f, const PatternEntry* s) { return f->PlaneX < s->PlaneX; });
		}

		fprintf(f, "row %d: ", row->index);
		for (auto row_it = row_patterns.begin(); row_it != row_patterns.end(); ++row_it) {

			Pattern = *row_it;

			strcpy(Name, Pattern->PatternName);
			ReplaceSpecialChars(Name);
			fprintf(f, "%s ", Name);
		}
		fprintf(f, "\n");
	}

	// Cleanup

	ClearRowsList(rows);
	delete[] patterns;

	fprintf(f, "\n");

	fclose(f);
}
