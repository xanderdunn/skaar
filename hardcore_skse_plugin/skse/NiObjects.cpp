#include "NiObjects.h"
#include "NiExtraData.h"

float * g_worldToCamMatrix = (float*)0x001B3EA10;
NiRect<float> * g_viewPort = (NiRect<float> *)0x001B3EA74;
const _WorldPtToScreenPt3_Internal WorldPtToScreenPt3_Internal = (_WorldPtToScreenPt3_Internal)0x00AB84C0;

void NiRefObject::IncRef(void)
{
	InterlockedIncrement(&m_uiRefCount);
}

bool NiRefObject::Release(void)
{
	return InterlockedDecrement(&m_uiRefCount) == 0;
}

void NiRefObject::DecRef(void)
{
	if(Release())
		DeleteThis();
}

// UNTESTED

int ExtraDataCompare(const void * ls, const void * rs)
{
	return (int)(((NiExtraData*)ls)->m_pcName - ((NiExtraData*)rs)->m_pcName);
}

void NiObjectNET::AddExtraData(NiExtraData * extraData)
{
	extraData->IncRef();

	// No capacity, allocate and grow
	UInt32 newSize = 0;
	if(m_extraDataCapacity == 0) {
		newSize = 1;
		NiExtraData ** extraDataList = (NiExtraData **)FormHeap_Allocate(newSize * sizeof(NiExtraData*));
		extraDataList[0] = extraData;
		m_extraData = extraDataList;
		m_extraDataCapacity = newSize;
		m_extraDataLen = m_extraDataCapacity;
		return;
	}

	// Reached capacity, reallocate and grow
	if(m_extraDataLen == m_extraDataCapacity) {
		newSize = (m_extraDataCapacity * 2) + 1;
		NiExtraData ** extraDataList = (NiExtraData **)FormHeap_Allocate(newSize * sizeof(NiExtraData*));
		// Copy the entries over
		memcpy_s(extraDataList, newSize * sizeof(NiExtraData*), m_extraData, m_extraDataLen * sizeof(NiExtraData*));
		// Zero the allocated entries
		for(UInt16 i = m_extraDataLen; i < newSize; i++)
			extraDataList[i] = NULL;

		FormHeap_Free(m_extraData);
		m_extraData = extraDataList;
		m_extraDataCapacity = newSize;
	}

	m_extraData[++m_extraDataLen] = extraData;
	qsort(m_extraData, m_extraDataLen, sizeof(NiExtraData*), ExtraDataCompare);
}

SInt32 NiObjectNET::GetIndexOf(NiExtraData * extraData)
{
	if(!extraData)
		return -1;
	if(!extraData->m_pcName)
		return -1;

	SInt16 min = 0;
	SInt16 max = (SInt16)m_extraDataLen - 1;
	SInt16 r = -1;
	while (max >= min) { // Iterative binary search
		SInt16 mid = (min + max) >> 1;
		if(m_extraData[mid]->m_pcName == extraData->m_pcName)
			r = mid; // Found entry
		else if (m_extraData[mid]->m_pcName < extraData->m_pcName)
			min = mid + 1;
		else
			max = mid - 1;
	}

	return r;
}

bool NiObjectNET::RemoveExtraData(NiExtraData * extraData)
{
	SInt32 index = GetIndexOf(extraData);
	if(index >= 0) { // Remove the entry
		extraData->DecRef();
		// Shift everything up
		for (UInt16 i = index; i < m_extraDataLen - 1; i++)
			m_extraData[i] = m_extraData[i + 1];
		// Remove the duplicate tail entry
		m_extraData[--m_extraDataLen] = NULL;
		return true;
	}

	return false;
}

// This should be a BSFixedString before passing to this
NiExtraData * NiObjectNET::GetExtraData(const char * name)
{
	SInt16 min = 0;
	SInt16 max = (SInt16)m_extraDataLen - 1;

	while (max >= min) { // Iterative binary search
		SInt16 mid = (min + max) >> 1;
		if(m_extraData[mid]->m_pcName == name)
			return m_extraData[mid]; 
		else if (m_extraData[mid]->m_pcName < name)
			min = mid + 1;
		else
			max = mid - 1;
	}

	return NULL;
}

bool NiCamera::WorldPtToScreenPt3(NiPoint3 * p_in, float * x_out, float * y_out, float * z_out, float zeroTolerance)
{
	return WorldPtToScreenPt3_Internal((float*)&this->m_aafWorldToCam, &this->m_kPort, p_in, x_out, y_out, z_out, zeroTolerance);
}