#include "GameTypes.h"

void SimpleLock::Lock(void)
{
	SInt32 myThreadID = GetCurrentThreadId();
	if (threadID == myThreadID) {
		lockCount++;
		return;
	}

	UInt32 spinCount = 0;
	while (InterlockedCompareExchange(&threadID, myThreadID, 0))
		Sleep(++spinCount > kFastSpinThreshold);

    lockCount = 1;
}

void SimpleLock::Release(void)
{
	if (--lockCount == 0)
		InterlockedCompareExchange(&threadID, 0, threadID);
}

StringCache * StringCache::GetSingleton(void)
{
	typedef StringCache * (* _GetSingleton)(void);

	return ((_GetSingleton)0x00A50B00)();
}

StringCache::Lock * StringCache::GetLock(UInt32 crc16)
{
	return &locks[crc16 & 0x1F];
}

StringCache::Ref::Ref(const char * buf)
{
	CALL_MEMBER_FN(this, ctor)(buf);
}

BSString::~BSString()
{
	if(m_data)
	{
		FormHeap_Free(m_data);
		m_data = NULL;
	}
}

const char * BSString::Get(void)
{
	return m_data ? m_data : "";
}

const _CRC32_Calc4 CRC32_Calc4 = (_CRC32_Calc4)0x00A4A570;
const _CRC32_Calc8 CRC32_Calc8 = (_CRC32_Calc8)0x00A4A5D0;

void UpdateRegistrationHolder::Order(UInt32 index)
{
	UInt32 count = m_regs.count;
	UInt32 pivotIndex = count >> 1;

	if (index >= pivotIndex)
		return;

	Registration * pOldReg = m_regs[index];
	UInt32 startIndex = index;
	UInt32 cmpIndex;
	do
	{
		cmpIndex = 2*index + 1;

		if (cmpIndex < (count-1))
		{
			Registration * pCur = m_regs[cmpIndex];
			Registration * pSucc = m_regs[cmpIndex+1];
			if (!pCur || !pSucc || pCur->schedTime > pSucc->schedTime)
				cmpIndex++;
		}

		Registration * pCmpReg = m_regs[cmpIndex];
		if (!pCmpReg || !pOldReg || pCmpReg->schedTime > pOldReg->schedTime)
			break;

		m_regs[index] = pCmpReg;
		index = cmpIndex;

	}
	while (cmpIndex < pivotIndex);

	if (index != startIndex)
		m_regs[index] = pOldReg;

}

bool UpdateRegistrationHolder::Remove(UInt64 & handle)
{
	if (m_regs.count == 0)
		return false;

	for (UInt32 i=0; i<m_regs.count; i++)
	{
		Registration * pReg = m_regs[i];

		if (pReg && pReg->handle == handle)
		{
			UInt32 lastIndex = m_regs.count - 1;

			// Remove last entry => no reorganizing necessary
			if (i == lastIndex)
			{
				pReg->Release();
				m_regs.count--;
			}
			else
			{
				m_regs[i] = m_regs[lastIndex];
				m_regs[lastIndex] = pReg;	// seems pointless, but the original code does it

				pReg->Release();
				m_regs.count--;

				Order(i);
			}
			return true;
		}
	}
	return false;
}
