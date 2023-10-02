#pragma once

struct RowEntry
{
	int index;
	int planeX;
	int planeY;
};

void RecalcRows(std::list<RowEntry*>& savedRows, PatternEntry* patterns, int numPatterns);
