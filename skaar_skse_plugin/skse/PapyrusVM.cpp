#include "PapyrusVM.h"
#include "GameAPI.h"

IObjectHandlePolicy	** g_objectHandlePolicy = (IObjectHandlePolicy **)0x01BA7008;
SkyrimVM			** g_skyrimVM = (SkyrimVM **)0x012E568C;

void VMClassInfo::AddRef(void)
{
	InterlockedIncrement(&refCount);
}

void VMClassInfo::Release(void)
{
	if(!InterlockedDecrement(&refCount))
	{
		CALL_MEMBER_FN(this, Destroy)();
		FormHeap_Free(this);
	}
}

UInt64 VMIdentifier::GetHandle(void)
{
	UInt64	result = (*g_objectHandlePolicy)->GetInvalidHandle();

	SInt32	oldLock = Lock();
	result = m_handle;
	Unlock(oldLock);

	return result;
}

SInt32 VMIdentifier::Lock(void)
{
	UInt32	spinCount = 0;
	SInt32	lockValue;

	while(true)
	{
		lockValue = m_lock;

		if(lockValue & kLockBit)
		{
			// someone else is holding the lock, sleep
			if(spinCount <= kFastSpinThreshold)
			{
				Sleep(0);
				spinCount++;
			}
			else
			{
				Sleep(1);
			}
		}
		else
		{
			// try to take the lock
			if(InterlockedCompareExchange(&m_lock, lockValue | kLockBit, lockValue) == lockValue)
				break;
		}
	}

	return lockValue;
}

void VMIdentifier::Unlock(SInt32 oldLock)
{
	m_lock = oldLock;
}

// try to increment the lock
// on 1 -> 2, call IObjectHandlePolicy::Unk_09(m_handle) with the lock held
void VMIdentifier::IncrementLock(void)
{
	UInt32	spinCount = 0;
	
	while(true)
	{
		SInt32	lockValue = m_lock;

		if(lockValue & kLockBit)
		{
			if(spinCount <= kFastSpinThreshold)
			{
				Sleep(0);
				spinCount++;
			}
			else
			{
				Sleep(1);
			}
		}
		else
		{
			if(lockValue == 1)
			{
				if(InterlockedCompareExchange(&m_lock, lockValue | kLockBit, lockValue) == lockValue)
				{
					(*g_objectHandlePolicy)->AddRef(m_handle);
					m_lock = 2;
					break;
				}
			}
			else
			{
				if(InterlockedCompareExchange(&m_lock, lockValue + 1, lockValue) == lockValue)
					break;
			}
		}
	}
}

// try to decrement the lock
// on 2 -> 1, call IObjectHandlePolicy::Unk_0A(m_handle) with the lock held
SInt32 VMIdentifier::DecrementLock(void)
{
	UInt32	spinCount = 0;

	while(true)
	{
		SInt32	lockValue = m_lock;

		if(lockValue & kLockBit)
		{
			if(spinCount <= kFastSpinThreshold)
			{
				Sleep(0);
				spinCount++;
			}
			else
			{
				Sleep(1);
			}
		}
		else
		{
			if(lockValue == 2)
			{
				if(InterlockedCompareExchange(&m_lock, lockValue | kLockBit, lockValue) == lockValue)
				{
					(*g_objectHandlePolicy)->Release(m_handle);
					m_lock = 1;
					return 1;
				}
			}
			else
			{
				if(InterlockedCompareExchange(&m_lock, lockValue - 1, lockValue) == lockValue)
					return lockValue - 1;
			}
		}
	}
}

void VMIdentifier::Destroy(void)
{
	CALL_MEMBER_FN(this, Destroy_Internal)();
	FormHeap_Free(this);
}

UInt32 VMValue::GetUnmangledType(void)
{
	if(type <= 0x0F)
		return type;

	return (type & 1) ? kType_Unk0B : kType_Identifier;
}

UInt32 SkyrimVM::ClearInvalidRegistrations(void)
{
	IObjectHandlePolicy * policy = m_classRegistry->GetHandlePolicy();
	UInt64 invalidHandle = policy->GetInvalidHandle();

	m_updateLock.Lock();

	UInt32 count = 0;
	while(m_updateRegHolder.Remove(invalidHandle) == true)
		count++;

	while(m_updateGameTimeRegHolder.Remove(invalidHandle) == true)
		count++;

	m_updateLock.Release();

	return count;
}

#ifdef PAPYRUS_CUSTOM_CLASS
#include "GameFormComponents.h"
#include "GameReferences.h"

// Parses the handle again for some unknown reason
bool IObjectHandlePolicy::Unk_02_Hook(UInt64 handle)
{
	PlayerCharacter* pPC = (*g_thePlayer);
	if(pPC) {
		return ((handle & 0x0000AA0000000000) == 0x0000AA0000000000) && ((handle & 0xFFFFFFFF) == pPC->formID);
	}

	return CALL_MEMBER_FN(this, Unk_02_Origin)(handle);
}

// Parses the handle to ensure that this type is correct for this handle
bool IObjectHandlePolicy::IsType_Hook(UInt32 typeID, UInt64 handle)
{
	if(typeID == kFormType_TintMask) {
		PlayerCharacter* pPC = (*g_thePlayer);
		if(pPC) {
			return ((handle & 0x0000AA0000000000) == 0x0000AA0000000000) && ((handle & 0xFFFFFFFF) == pPC->formID);
		}
	}

	return CALL_MEMBER_FN(this, IsType_Origin)(typeID, handle);
}

// Creates a HandleID for the particular object
UInt64 IObjectHandlePolicy::Create_Hook(UInt32 typeID, void * srcData)
{
	if(typeID == kFormType_TintMask) {
		TintMask * tintMask = (TintMask*)srcData;
		PlayerCharacter* pPC = (*g_thePlayer);
		SInt32 tintIndex = -1;
		if(pPC) {
			tintIndex = pPC->tintMasks.GetItemIndex(tintMask);
		}

		if(tintIndex == -1) {
			return GetInvalidHandle();
		}

		return (UInt64)(0x0000AA0000000000 | ((UInt64)tintIndex << 32) | pPC->formID);
	}

	return CALL_MEMBER_FN(this, Create_Origin)(typeID, srcData);
}

// Resolves the Object via handle
void * IObjectHandlePolicy::Resolve_Hook(UInt32 typeID, UInt64 handle)
{
	if(typeID == kFormType_TintMask) {
		UInt32 tintIndex = (handle & 0x000000FF00000000) >> 32;
		TintMask * tintMask = NULL;
		PlayerCharacter* pPC = (*g_thePlayer);
		if(!pPC) {
			return NULL;
		}
		if(pPC->tintMasks.GetNthItem(tintIndex, tintMask)) {
			return tintMask;
		}
	}

	return CALL_MEMBER_FN(this, Resolve_Origin)(typeID, handle);
}
#endif

/* // 00C644E0
bool VMClassRegistry::CreateArrayVariable(VMValue * value, UInt32 size)
{
	bool ret = false;
	if(CALL_MEMBER_FN(value, IsArray)()) {
		VMValue::ArrayData * arrayData = NULL;
		if(CreateArray(value, size, arrayData) && arrayData)
		{
			ret = true;
			VMValue tempValue = *value;
			CALL_MEMBER_FN(value, Set)(&tempValue);
			CALL_MEMBER_FN(&tempValue, Destroy)();
			CALL_MEMBER_FN(value, DestroyArray)(arrayData);
		}

		arrayData->Destroy();
		CALL_MEMBER_FN(value, Destroy)();
	}

	return ret;
}*/