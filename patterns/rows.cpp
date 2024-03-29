// Row calculation

#include "pch.h"

extern int WorkspaceRowIndex;
extern int WorkspaceRowArrangement;

void ClearRowsList(std::list<RowEntry*>& savedRows)
{
	while (!savedRows.empty())
	{
		RowEntry* entry = savedRows.back();
		savedRows.pop_back();
		delete entry;
	}
}

static void AddRowEntry(std::list<RowEntry*>& savedRows, RowEntry * add)
{
	RowEntry* newEntry = new RowEntry;

	memset(newEntry, 0, sizeof(RowEntry));

	newEntry->index = add->index;
	newEntry->planeX = add->planeX;
	newEntry->planeY = add->planeY;

	savedRows.push_back(newEntry);
}

void RecalcRows(std::list<RowEntry*>& savedRows, PatternEntry * patterns, int numPatterns)
{
	if (!numPatterns)
	{
		return;
	}

	//
	// Get patterns XY/WH pairs
	//

	CoordSize_Pair* pairs = new CoordSize_Pair[numPatterns];

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

			AddRowEntry(savedRows, &entry);
		}
	}

	delete [] pairs;
}
