#pragma once

struct RowEntry
{
	LIST_ENTRY entry;
	int index;
	int planeX;
	int planeY;
};

LIST_ENTRY * RecalcRows(PatternEntry * patterns, int numPatterns);
