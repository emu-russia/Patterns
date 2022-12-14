#pragma once

//
// Workspace image state
// Watch for x64 compatibility! (i.e. not implicit int vatiables, only data types with certain size)
//

#pragma pack(push, 1)

struct WorkspaceImage
{
	char Signature[4];          // "WRK\0"

	//
	// Global settings
	//

	float   Lambda;
	float   LambdaDelta;
	long    Flag;               // "flip" / "mirror" checkbox state

	union
	{
		struct
		{
			short RowIndex;
			short RowArrangement;
		};
		long RowSettings;
	};
	long Reserved;

	//
	// Source matching image layer
	//

	long SourceImagePresent;
	long SourceImageOffset;
	long SourceImageLength;
	long ScrollX;
	long ScrollY;

	//
	// Added patterns layer
	//

	long PatternsAdded;
	long PatternsLayerOffset;

};

#pragma pack(pop)

void SaveWorkspace(char *filename);

void LoadWorkspace(char *filename);
