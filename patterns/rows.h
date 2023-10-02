#pragma once

struct RowEntry
{
	int index;
	int planeX;
	int planeY;
};

struct CoordSize_Pair
{
	long xy;		// X or Y coordinate
	long wh;		// Width or Height (size)
};

void ClearRowsList(std::list<RowEntry*>& savedRows);

void RecalcRows(std::list<RowEntry*>& savedRows, PatternEntry* patterns, int numPatterns);
