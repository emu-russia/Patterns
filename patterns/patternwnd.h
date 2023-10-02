#pragma once

enum
{
	CellNot = 1,
	CellBuffer,
	CellMux,
	CellLogic,
	CellAdder,
	CellBusSupp,
	CellFlipFlop,
	CellLatch,
	CellOther,
};

struct PatternItem
{
	char    Name[128];
	float   Lambda;
	unsigned char * PatternRawImage;
	long    PatternBufferSize;
	long    PatternWidth;
	long    PatternHeight;
	HBITMAP PatternBitmap;
	long    Hidden;
	long    pcount;
	long    ncount;
	long    Type;
};

enum
{
	ViasInput = 1,
	ViasOutput,
	ViasInout,
};

struct ViasEntry
{
	char ViasName[128];

	float OffsetX;

	float OffsetY;

	long Type;

};

struct ViasCollectionEntry
{
	char PatternName[128];

	std::list<ViasEntry*> ViasHead;
};

void PatternInit(HWND Parent, char * dbfile);

void PatternResize(int Width, int Height);

void RearrangePatternTiles(void);

void PatternRedraw(void);

PatternItem * PatternGetItem(char * PatternName);

void DrawPattern ( PatternItem *Item,
				   HDC hdc,
				   LPRECT Rect,
				   BOOL Flipped,
				   BOOL Mirrored,
				   BOOL Box,
				   BOOL Label,
				   BOOL SelectHint,
				   BOOL ViasEnable );

void PatternDestroy(void);

void ParseDatabase(char *text);

ViasCollectionEntry * GetViasCollection ( char * PatternName );
