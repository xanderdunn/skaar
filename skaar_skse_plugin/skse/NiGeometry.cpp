#include "NiGeometry.h"
#include "GameAPI.h"

void NiGeometryData::AllocateVerts(UInt32 numVerts)
{
	m_pkVertex = (NiPoint3 *)FormHeap_Allocate(sizeof(NiPoint3) * numVerts);
	m_pkTexture = (NiPoint2 *)FormHeap_Allocate(sizeof(NiPoint2) * numVerts);

	memset(m_pkVertex, 0, sizeof(NiPoint3) * numVerts);
	memset(m_pkTexture, 0, sizeof(NiPoint2) * numVerts);
}

void NiGeometryData::AllocateNormals(UInt32 numVerts)
{
	m_pkNormal = (NiPoint3 *)FormHeap_Allocate(sizeof(NiPoint3) * numVerts);
	memset(m_pkNormal, 0, sizeof(NiPoint3) * numVerts);
}

void NiGeometryData::AllocateNBT(UInt32 numVerts)
{
	m_pkNormal = (NiPoint3 *)FormHeap_Allocate(sizeof(NiPoint3) * 3 * numVerts);
	memset(m_pkNormal, 0, sizeof(NiPoint3) * 3 * numVerts);
}

void NiGeometryData::AllocateColors(UInt32 numVerts)
{
	m_pkColor = (NiColorA *)FormHeap_Allocate(sizeof(NiColorA) * numVerts);
	memset(m_pkColor, 0, sizeof(NiColorA) * numVerts);
}

void NiSkinPartition::Partition::AllocateWeights(UInt32 numVerts)
{
	m_pfWeights = (float *)FormHeap_Allocate(sizeof(float) * 4 * numVerts);
	m_pucBonePalette = (UInt8 *)FormHeap_Allocate(sizeof(UInt8) * 4 * numVerts);

	memset(m_pfWeights, 0, sizeof(float) * 4 * numVerts);
	memset(m_pucBonePalette, 0, sizeof(UInt8) * 4 * numVerts);
}

void NiSkinData::BoneData::AllocateWeights(UInt32 numWeights)
{
	m_pkBoneVertData = (BoneVertData *)FormHeap_Allocate(sizeof(BoneVertData) * numWeights);
	memset(m_pkBoneVertData, 0, sizeof(BoneVertData) * numWeights);
}

void NiGeometry::SetEffectState(NiProperty * effectState)
{
	m_spEffectState = effectState; // handled by NiPointer now
}

void NiGeometry::SetSkinInstance(NiSkinInstance * skinInstance)
{
	m_spSkinInstance = skinInstance; // handled by NiPointer now
}

void NiGeometry::SetModelData(NiGeometryData * modelData)
{
	m_spModelData = modelData; // handled by NiPointer now
}


NiTriShape * NiTriShape::Create(NiTriShapeData * geometry)
{
	void* memory = FormHeap_Allocate(sizeof(NiTriShape));
	memset(memory, 0, sizeof(NiTriShape));
	NiTriShape* xData = (NiTriShape*)memory;
	CALL_MEMBER_FN(xData, ctor)(geometry);
	return xData;
}

NiSkinInstance * NiSkinInstance::Create()
{
	void* memory = FormHeap_Allocate(sizeof(NiSkinInstance));
	memset(memory, 0, sizeof(NiSkinInstance));
	NiSkinInstance* xData = (NiSkinInstance*)memory;
	CALL_MEMBER_FN(xData, ctor)();
	return xData;
}

BSDismemberSkinInstance * BSDismemberSkinInstance::Create()
{
	void* memory = FormHeap_Allocate(sizeof(BSDismemberSkinInstance));
	memset(memory, 0, sizeof(BSDismemberSkinInstance));
	BSDismemberSkinInstance* xData = (BSDismemberSkinInstance*)memory;
	CALL_MEMBER_FN(xData, ctor)();
	return xData;
}