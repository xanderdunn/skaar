#include "ScaleformAPI.h"

class ScaleformHeap
{
public:
	virtual void	Fn_00(void);
	virtual void	Fn_01(void);
	virtual void	Fn_02(void);
	virtual void	Fn_03(void);
	virtual void	Fn_04(void);
	virtual void	Fn_05(void);
	virtual void	Fn_06(void);
	virtual void	Fn_07(void);
	virtual void	Fn_08(void);
	virtual void	Fn_09(void);
	virtual void *	Allocate(UInt32 size, UInt32 unk = 0);	// unk is probably align, maybe flags (haven't traced)
	virtual void	Fn_0B(void);
	virtual void	Free(void * ptr);
};

ScaleformHeap ** g_scaleformHeap = (ScaleformHeap **)0x01B40E3C;

void * ScaleformHeap_Allocate(UInt32 size)
{
	return (*g_scaleformHeap)->Allocate(size);
}

void ScaleformHeap_Free(void * ptr)
{
	(*g_scaleformHeap)->Free(ptr);
}

const _InvokeFunction InvokeFunction = (_InvokeFunction)0x00A63C40;
const _GFxAllocateHeap GFxAllocateHeap = (_GFxAllocateHeap)0x00F7AF80;