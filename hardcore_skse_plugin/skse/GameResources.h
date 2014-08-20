#pragma once

#include "GameTypes.h"
#include "NiObjects.h"

class BSResource
{
public:
	virtual ~BSResource();

	template<typename D>
	class EntryDBBase
	{
	public:
		virtual ~EntryDBBase();
		virtual void Unk_01(void);	// Pure
		virtual void Unk_02(void);
		virtual void Unk_03(void);
		virtual void Unk_04(void);
		virtual void Unk_05(void);
	};

	template<typename D>
	class EntryDB : public EntryDBBase<D>
	{
	public:
		virtual ~EntryDB();
		virtual void Unk_01(void);
		virtual void Unk_02(void);
		virtual void Unk_03(void);
		virtual void Unk_04(void);
		virtual void Unk_05(void);

		D data;	// 04
	};
};

struct BSFaceGenDB
{
	struct TRI
	{
		struct DBTraits
		{
			struct MorphVertex
			{
				SInt16 x;
				SInt16 y;
				SInt16 z;
			};
			struct MorphData
			{
				const char	* morphName;	// 00
				UInt32 unk04;				// 04
				MorphVertex	* diffData;		// 08
				UInt32	unk0C;				// 0C
				UInt32	unk10;				// 10
				float	baseDiff;			// 14
				UInt32	diffVertexNum;		// 18
				UInt32	baseVertexNum;		// 1C
				UInt32	diffVertexPosNum;	// 20
			};

			struct MorphSet
			{
				UInt32	unk00;					// 00
				const char	* fileName;			// 04
				tArray<MorphData>	morphData;	// 08
				UInt32	unk14;					// 14
				UInt32	unk18;					// 18
				UInt32	unk1C;					// 1C
				UInt32	unk20;					// 20
				UInt32	unk24;					// 24
				UInt32	unk28;					// 28
				BSFaceGenMorphData	* faceMorphData;	// 2C - BSFaceGenMorphDataHead
			};

			struct Data
			{
				UInt32	unk00;			// 00
				UInt32	unk04;			// 04 - tri\0
				UInt32	unk08;			// 08
				UInt32	unk0C;			// 0C
				UInt32	unk10;			// 10
				Data	* next;			// 14
				void	* unk18;		// 18
				MorphSet	* morphSet;	// 1C
			};

			UInt32 unk04[(0x6C - 0x04) >> 2];	// 04
			tArray<Data*> triEntries;			// 6C
		};
	};

	struct FG
	{
		struct DBTraits
		{

		};
	};
};

// 4 less due to vtbl of BSResource::EntryDB not being part of the equation
STATIC_ASSERT(offsetof(BSFaceGenDB::TRI::DBTraits, triEntries) == 0x68);

class FaceGenTriDatabase : public BSResource::EntryDB<BSFaceGenDB::TRI::DBTraits>
{
public:
	static FaceGenTriDatabase * GetSingleton(void);
	static BSFaceGenDB::TRI::DBTraits::Data * GetInvalidEntry(void);
};

// DumpCode
/*
FaceGenTriDatabase * triDatabase = FaceGenTriDatabase::GetSingleton();
_MESSAGE("TRI Database...");
//DumpClass(triDatabase, sizeof(FaceGenTriDatabase)/4);
gLog.Indent();
BSFaceGenDB::TRI::DBTraits::Data * triEntry;
_MESSAGE("Database Capacity: %d", triDatabase->data.triEntries.arr.capacity);
for(UInt32 i = 0; i < triDatabase->data.triEntries.arr.capacity; i++) {
	triEntry = triDatabase->data.triEntries[i];
	if(triEntry && triEntry != FaceGenTriDatabase::GetInvalidEntry()) {
		_MESSAGE("Tri Entry %d", i);
		//DumpClass(triEntry, sizeof(BSFaceGenDB::TRI::DBTraits::Data)/4);
		if(triEntry->morphSet) {
			//_MESSAGE("Set %d - %s", i, triEntry->morphSet->fileName);
			//DumpClass(triEntry->morphSet, sizeof(BSFaceGenDB::TRI::DBTraits::MorphSet)/4);
			_MESSAGE("Set - %s MorphCount: %d", triEntry->morphSet->fileName, triEntry->morphSet->morphData.count);
			for(UInt32 n = 0; n < triEntry->morphSet->morphData.count; n++) {
				BSFaceGenDB::TRI::DBTraits::MorphData morphEntry;
				if(triEntry->morphSet->morphData.GetNthItem(n, morphEntry)) {
					//_MESSAGE("Morph %d %d - %s", i, n, morphEntry.morphName);
					//DumpClass(&morphEntry, sizeof(BSFaceGenDB::TRI::DBTraits::MorphData)/4);
					_MESSAGE("Morph %d %d - %s baseDiff: %f DiffVertNum: %d BaseVertNum: %d DiffVertPosNum: %d", i, n, morphEntry.morphName, morphEntry.baseDiff, morphEntry.diffVertexNum, morphEntry.baseVertexNum, morphEntry.diffVertexPosNum);
				}
			}

			gLog.Indent();
			UInt32 k = 0;
			BSFaceGenDB::TRI::DBTraits::Data * children = triEntry->next;
			while(children) {
				_MESSAGE("Child Set - %s MorphCount: %d", children->morphSet->fileName, children->morphSet->morphData.count);
				gLog.Indent();
				//_MESSAGE("Child Set %d - %s", i, children->morphSet->fileName);
				//DumpClass(children->morphSet, sizeof(BSFaceGenDB::TRI::DBTraits::MorphSet)/4);
				for(UInt32 n = 0; n < children->morphSet->morphData.count; n++) {
					BSFaceGenDB::TRI::DBTraits::MorphData morphEntry;
					if(children->morphSet->morphData.GetNthItem(n, morphEntry)) {
						//_MESSAGE("Child Morph %d %d - %s", k, n, morphEntry.morphName);
						//DumpClass(&morphEntry, sizeof(BSFaceGenDB::TRI::DBTraits::MorphData)/4);
						_MESSAGE("Child Morph %d %d - %s baseDiff: %f DiffVertNum: %d BaseVertNum: %d DiffVertPosNum: %d", i, n, morphEntry.morphName, morphEntry.baseDiff, morphEntry.diffVertexNum, morphEntry.baseVertexNum, morphEntry.diffVertexPosNum);
					}
				}
				gLog.Outdent();
				k++;
				children = children->next;
			}
			gLog.Outdent();
		}
	} else {
		_MESSAGE("Invalid entry %d", i);
	}
}

gLog.Outdent();
*/