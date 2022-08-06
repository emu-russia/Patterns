// Row calculation

#include "pch.h"

extern float WorkspaceLambda;
extern int WorkspaceRowIndex;
extern int WorkspaceRowArrangement;

LIST_ENTRY savedRows = { &savedRows, &savedRows };

static void ClearOldList()
{
	while (!IsListEmpty(&savedRows))
	{
		LIST_ENTRY * entry = savedRows.Flink;
		RemoveEntryList(entry);
		free(entry);
	}
}

static BOOL AddRowEntry(RowEntry * add)
{
	LIST_ENTRY * entry;
	RowEntry * newEntry;
	int entrySize = sizeof(RowEntry);

	entry = (LIST_ENTRY *)malloc(entrySize);
	if (!entry)
	{
		return FALSE;
	}

	memset(entry, 0, entrySize);

	InsertTailList(&savedRows, entry);

	newEntry = (RowEntry *)entry;

	newEntry->index = add->index;
	newEntry->planeX = add->planeX;
	newEntry->planeY = add->planeY;

	return TRUE;
}

struct CoordSize_Pair
{
	long xy;		// X or Y coordinate
	long wh;		// Width or Height (size)
};

LIST_ENTRY * RecalcRows(PatternEntry * patterns, int numPatterns)
{
	CoordSize_Pair* pairs;

	if (!numPatterns)
	{
		return NULL;
	}

	if (!IsListEmpty(&savedRows))
	{
		ClearOldList();
	}

	//
	// Get patterns XY/WH pairs
	//

	pairs = (CoordSize_Pair *)malloc(sizeof(CoordSize_Pair) * numPatterns);
	if (!pairs)
	{
		return NULL;
	}

	for (int i = 0; i < numPatterns; i++)
	{
		pairs[i].xy = WorkspaceRowArrangement == 0 ? patterns[i].PlaneX : patterns[i].PlaneY;
		pairs[i].wh = WorkspaceRowArrangement == 0 ? patterns[i].Width : patterns[i].Height;
	}

	//
	// Sort minmax
	//

	for (int c = 0; c < numPatterns - 1; c++)
	{
		for (int d = 0; d < numPatterns - c - 1; d++)
		{
			if (pairs[d].xy > pairs[d + 1].xy) // For decreasing order use <
			{
				CoordSize_Pair swap = pairs[d];
				pairs[d] = pairs[d + 1];
				pairs[d + 1] = swap;
			}
		}
	}

	//
	// Remove overlapping
	//

	CoordSize_Pair next{};

	next.xy = pairs[0].xy;
	next.wh = pairs[0].wh;

	for (int i = 1; i < numPatterns; i++)
	{
		if ( pairs[i].xy >= next.xy && 
			pairs[i].xy < ( next.xy + next.wh) )
		{
			pairs[i].xy = 0;
		}
		else
		{
			next.xy = pairs[i].xy;
			next.wh = pairs[i].wh;
		}
	}

	//
	// Add rows
	//

	int rowIndex = WorkspaceRowIndex;

	for (int i = 0; i < numPatterns; i++)
	{
		if (pairs[i].xy)
		{
			RowEntry entry{};

			entry.index = rowIndex++;
			entry.planeX = WorkspaceRowArrangement == 0 ? pairs[i].xy : 0;
			entry.planeY = WorkspaceRowArrangement == 0 ? 0 : pairs[i].xy;

			BOOL res = AddRowEntry(&entry);

			if (!res)
			{
				free(pairs);
				return NULL;
			}
		}
	}

	free(pairs);

	return &savedRows;
}
