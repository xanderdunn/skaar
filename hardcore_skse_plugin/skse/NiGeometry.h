#pragma once

#include "skse/NiNodes.h"
#include "skse/NiProperties.h"

// NiGeometry, NiGeometryData and children
MAKE_NI_POINTER(NiGeometryData);
MAKE_NI_POINTER(NiSkinInstance);
MAKE_NI_POINTER(NiProperty);
MAKE_NI_POINTER(NiSkinData);
MAKE_NI_POINTER(NiSkinPartition);

class NiAdditionalGeometryData;
class NiTriShapeData;

// B8+
class NiGeometry : public NiAVObject
{
public:
	virtual void Unk_33(void); // call controller vtbl+0xA0?
	virtual void Unk_34(void); // ret 0
	virtual void Unk_35(void); // same as Unk_33
	virtual void * Unk_36(void); // ret call m_spModelData vtbl+0x9C
	virtual void Unk_37(NiGeometryData * unk1); // set and AddRef geometry data
	virtual void * Unk_38(void); // ret call m_spModelData vtbl+0x94
	virtual UInt16 Unk_39(bool unk1); // ??

	NiPropertyPtr	m_spPropertyState;	// A8 - unconfirmed
	NiPropertyPtr	m_spEffectState;		// AC - unconfirmed
	NiGeometryDataPtr m_spModelData;		// B0
	NiSkinInstancePtr m_spSkinInstance;		// B4

	// ... materials

	//MEMBER_FN_PREFIX(NiGeometry);
	//DEFINE_MEMBER_FN(SetEffectState, void, 0x004614A0, NiProperty * effectState);
	//DEFINE_MEMBER_FN(SetSkinInstance, void, 0x0046AD10, NiSkinInstance * skinInstance);
	void SetEffectState(NiProperty * effectState);
	void SetSkinInstance(NiSkinInstance * skinInstance);
	void SetModelData(NiGeometryData * modelData);
};

class NiTriBasedGeom : public NiGeometry
{
public:
};

class NiTriShape : public NiTriBasedGeom
{
public:
	static NiTriShape * Create(NiTriShapeData * geometry);

	MEMBER_FN_PREFIX(NiTriShape);
	DEFINE_MEMBER_FN(ctor, NiTriShape *, 0x00AB9930, NiTriShapeData * geometry);
};

class BSSegmentedTriShape : public NiTriShape
{
public:
};


class NiTriStrips : public NiTriBasedGeom
{
public:
};


// 48+
class NiGeometryData : public NiObject
{
public:
	enum
	{
		kDataFlag_HasUVs =	1 << 0,
		kDataFlag_HasNBT =	1 << 12,
	};

	UInt16	m_usVertices;				// 08
	UInt16	m_usID;						// 0A
	UInt16	m_usDirtyFlags;				// 0C
	UInt16	m_usDataFlags;				// 0E
	NiBound	m_kBound;					// 10
	NiPoint3	* m_pkVertex;			// 20
	NiPoint3	* m_pkNormal;			// 24 - all normals, then all binormals etc
	NiColorA	* m_pkColor;			// 28 - yes really, floats (b g r a)
	NiPoint2	* m_pkTexture;			// 2C
	UInt32	unk30;						// 30
	UInt32	unk34;						// 34
	UInt32	unkInt2;					// 38
	NiAdditionalGeometryData	* m_spAdditionalGeomData;	// 3C
	UInt32	unk40;						// 40
	UInt8	m_ucKeepFlags;				// 44
	UInt8	m_ucCompressFlags;			// 45
	UInt8	hasGeoData;					// 46

	void	AllocateVerts(UInt32 numVerts);
	void	AllocateNormals(UInt32 numVerts);
	void	AllocateNBT(UInt32 numVerts);
	void	AllocateColors(UInt32 numVerts);
};

// 4C
class NiTriBasedGeomData : public NiGeometryData
{
public:
	UInt16	m_usTriangles;			// 48
	UInt16	m_usActiveTriangles;	// 4A
};

// 54
class NiTriShapeData : public NiTriBasedGeomData
{
public:
	UInt32	m_uiTriListLength;		// 4C
	UInt16	* m_pusTriList;			// 50
};

// 58
class NiTriShapeDynamicData : public NiTriShapeData
{
public:
	struct SharedNormalArray
	{
		UInt16	m_usNumSharedNormals;
		UInt16	* m_pusSharedNormalIndexArray;
	};
	SharedNormalArray * m_pkSharedNormals;	// 54
	UInt16				m_usSharedNormalsArraySize;	// 56
};

// 10
class NiSkinPartition : public NiObject
{
public:
	// 28
	struct Partition
	{
		UInt16		* m_pusBones;			// 00
		float		* m_pfWeights;			// 04
		UInt16		* m_pusVertexMap;		// 08
		UInt8		* m_pucBonePalette;		// 0C
		UInt16		* m_pusTriList;			// 10
		UInt16		* m_pusStripLengths;	// 14
		UInt16		m_usVertices;			// 18
		UInt16		m_usTriangles;			// 1A
		UInt16		m_usBones;				// 1C
		UInt16		m_usStrips;				// 1E
		UInt16		m_usBonesPerVertex;		// 20
		UInt8		pad22[2];				// 22
		UInt32		unk24;					// 24

		void	AllocateWeights(UInt32 numVerts);
	};

	UInt32		m_uiPartitions;		// 08
	Partition	* m_pkPartitions;	// 0C
};

// 48
class NiSkinData : public NiObject
{
public:
	// 08
	struct BoneVertData
	{
		UInt16	m_usVert;	// 00
		UInt8	pad02[2];	// 02
		float	m_fWeight;	// 04?
	};

	// 4C
	struct BoneData
	{
		NiTransform		m_kSkinToBone;		// 00
		NiBound			m_kBound;			// 34
		BoneVertData	* m_pkBoneVertData;	// 44
		UInt16			m_usVerts;			// 48
		UInt8			pad4A[2];			// 4A

		void	AllocateWeights(UInt32 numVerts);
	};

	NiSkinPartition	* m_spSkinPartition;	// 08
	NiTransform		m_kRootParentToSkin;	// 0C
	BoneData		* m_pkBoneData;			// 40
	UInt32			m_uiBones;				// 44

	// ctor - AD4780
};

STATIC_ASSERT(sizeof(NiSkinData::BoneVertData) == 0x08);

// 38
class NiSkinInstance : public NiObject
{
public:
	NiSkinDataPtr		m_spSkinData;		// 08
	NiSkinPartitionPtr	m_spSkinPartition;	// 0C
	NiAVObject			* m_pkRootParent;	// 10
	NiAVObject			** m_ppkBones;		// 14
	
	UInt32	unk18;							// 18
	SInt32	unk1C;							// 1C
	UInt32	unk20;							// 20
	UInt32	numFlags;						// 24
	UInt32	unk28;							// 28
	UInt32 	* flags;						// 2C
	UInt32	unk30;							// 30
	UInt32	unk34;							// 34

	static NiSkinInstance * Create();

	MEMBER_FN_PREFIX(NiSkinInstance);
	DEFINE_MEMBER_FN(ctor, NiSkinInstance *, 0x00ABDB90);
};
STATIC_ASSERT(sizeof(NiSkinInstance) == 0x38);

// 44
class BSDismemberSkinInstance : public NiSkinInstance
{
public:
	UInt32	numPartitions;					// 38
	UInt32	* partitionFlags;				// 3C
	UInt8	unk40;							// 40
	UInt8	pad41[3];						// 41

	static BSDismemberSkinInstance * Create();

	MEMBER_FN_PREFIX(BSDismemberSkinInstance);
	DEFINE_MEMBER_FN(ctor, BSDismemberSkinInstance *, 0x00ABDBD0);
};
STATIC_ASSERT(sizeof(BSDismemberSkinInstance) == 0x44);