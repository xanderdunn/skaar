#include "NiNodes.h"
#include "skse/GameObjects.h"

const _GetBoneNames GetBoneNames = (_GetBoneNames)0x00409080; // Builds the skeleton node name list if it doesn't exist
const _GetNodeByName GetNodeByName = (_GetNodeByName)0x00B06140;
const _GetNodeNameByWeaponType GetNodeNameByWeaponType = (_GetNodeNameByWeaponType)0x004AA0C0;
const _GetInternalNode GetInternalNode = (_GetInternalNode)0x004AA0E0;
const _SwapNodeParent SwapNodeParent = (_SwapNodeParent)0x0069F410;

NiBoneNames * NiBoneNames::GetSingleton(void)
{
	return (NiBoneNames *)0x012E2FC0;
}

NiWeaponNodes * NiWeaponNodes::GetSingleton(void)
{
	return (NiWeaponNodes *)0x012E7478;
}


NiNode * NiNode::Create(UInt32 arrBufLen)
{
	void* memory = FormHeap_Allocate(sizeof(NiNode));
	memset(memory, 0, sizeof(NiNode));
	NiNode* xData = (NiNode*)memory;
	CALL_MEMBER_FN(xData, ctor)(arrBufLen);
	return xData;
}

BSFadeNode * BSFadeNode::Create()
{
	void* memory = FormHeap_Allocate(sizeof(BSFadeNode));
	memset(memory, 0, sizeof(BSFadeNode));
	BSFadeNode* xData = (BSFadeNode*)memory;
	CALL_MEMBER_FN(xData, ctor)();
	return xData;
}