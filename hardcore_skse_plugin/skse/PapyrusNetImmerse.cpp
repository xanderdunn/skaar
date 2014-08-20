#include "PapyrusNetImmerse.h"

#include "GameAPI.h"
#include "GameForms.h"
#include "GameRTTI.h"
#include "GameReferences.h"
#include "GameObjects.h"
#include "GameThreads.h"

#include "NiNodes.h"
#include "NiGeometry.h"


namespace papyrusNetImmerse
{
	NiAVObject * ResolveNode(TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		if(!obj) return NULL;

		NiAVObject	* result = obj->GetNiNode();

		// special-case for the player, switch between first/third-person
		PlayerCharacter * player = DYNAMIC_CAST(obj, TESObjectREFR, PlayerCharacter);
		if(player && player->loadedState)
			result = firstPerson ? player->firstPersonSkeleton : player->loadedState->node;

		// name lookup
		if(obj && nodeName.data[0] && result)
			result = result->GetObjectByName(&nodeName.data);

		return result;
	}

	bool HasNode(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		return object != NULL;
	}

	float GetNodePositionX(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		return object ? object->m_worldTransform.pos.x : 0;
	}

	float GetNodePositionY(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		return object ? object->m_worldTransform.pos.y : 0;
	}

	float GetNodePositionZ(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		return object ? object->m_worldTransform.pos.z : 0;
	}

	float GetRelativeNodePositionX(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeNameA, BSFixedString nodeNameB, bool firstPerson)
	{
		NiAVObject	* objectA = ResolveNode(obj, nodeNameA, firstPerson);
		NiAVObject	* objectB = ResolveNode(obj, nodeNameB, firstPerson);

		return (objectA && objectB) ? objectB->m_worldTransform.pos.x - objectA->m_worldTransform.pos.x : 0;
	}

	float GetRelativeNodePositionY(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeNameA, BSFixedString nodeNameB, bool firstPerson)
	{
		NiAVObject	* objectA = ResolveNode(obj, nodeNameA, firstPerson);
		NiAVObject	* objectB = ResolveNode(obj, nodeNameB, firstPerson);

		return (objectA && objectB) ? objectB->m_worldTransform.pos.y - objectA->m_worldTransform.pos.y : 0;
	}

	float GetRelativeNodePositionZ(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeNameA, BSFixedString nodeNameB, bool firstPerson)
	{
		NiAVObject	* objectA = ResolveNode(obj, nodeNameA, firstPerson);
		NiAVObject	* objectB = ResolveNode(obj, nodeNameB, firstPerson);

		return (objectA && objectB) ? objectB->m_worldTransform.pos.z - objectA->m_worldTransform.pos.z : 0;
	}

	float GetNodeScale(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		return object ? object->m_localTransform.scale : 0;
	}

	void SetNodePositionX(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, float value, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		if(object)
		{
			object->m_localTransform.pos.x = value;
			BSTaskPool * taskPool = BSTaskPool::GetSingleton();
			if(taskPool)
				taskPool->UpdateWorldData(object);
		}
	}

	void SetNodePositionY(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, float value, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		if(object)
		{
			object->m_localTransform.pos.y = value;
			BSTaskPool * taskPool = BSTaskPool::GetSingleton();
			if(taskPool)
				taskPool->UpdateWorldData(object);
		}
	}

	void SetNodePositionZ(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, float value, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		if(object)
		{
			object->m_localTransform.pos.z = value;
			BSTaskPool * taskPool = BSTaskPool::GetSingleton();
			if(taskPool)
				taskPool->UpdateWorldData(object);
		}
	}

	void SetNodeScale(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, float value, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		if(object)
		{
			object->m_localTransform.scale = value;
			BSTaskPool * taskPool = BSTaskPool::GetSingleton();
			if(taskPool)
				taskPool->UpdateWorldData(object);
		}
	}

	void SetNodeTextureSet(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, BGSTextureSet * textureSet, bool firstPerson)
	{
		if(!textureSet) return;

		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		if(object)
		{
			NiGeometry * geometry = object->GetAsNiGeometry();
			if(geometry)
			{
				BSTaskPool * taskPool = BSTaskPool::GetSingleton();
				if(taskPool)
				{
					CALL_MEMBER_FN(taskPool, SetNiGeometryTexture)(geometry, textureSet);
				}
			}
		}
	}
};

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusNetImmerse::RegisterFuncs(VMClassRegistry* registry)
{
	// NiNode Manipulation
	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, bool>("HasNode", "NetImmerse", papyrusNetImmerse::HasNode, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, bool>("GetNodePositionX", "NetImmerse", papyrusNetImmerse::GetNodePositionX, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, bool>("GetNodePositionY", "NetImmerse", papyrusNetImmerse::GetNodePositionY, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, bool>("GetNodePositionZ", "NetImmerse", papyrusNetImmerse::GetNodePositionZ, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, bool>("GetNodeScale", "NetImmerse", papyrusNetImmerse::GetNodeScale, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, TESObjectREFR*, BSFixedString, float, bool>("SetNodeScale", "NetImmerse", papyrusNetImmerse::SetNodeScale, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, TESObjectREFR*, BSFixedString, float, bool>("SetNodePositionX", "NetImmerse", papyrusNetImmerse::SetNodePositionX, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, TESObjectREFR*, BSFixedString, float, bool>("SetNodePositionY", "NetImmerse", papyrusNetImmerse::SetNodePositionY, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, TESObjectREFR*, BSFixedString, float, bool>("SetNodePositionZ", "NetImmerse", papyrusNetImmerse::SetNodePositionZ, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, TESObjectREFR*, BSFixedString, BGSTextureSet*, bool>("SetNodeTextureSet", "NetImmerse", papyrusNetImmerse::SetNodeTextureSet, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, BSFixedString, bool>("GetRelativeNodePositionX", "NetImmerse", papyrusNetImmerse::GetRelativeNodePositionX, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, BSFixedString, bool>("GetRelativeNodePositionY", "NetImmerse", papyrusNetImmerse::GetRelativeNodePositionY, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, BSFixedString, bool>("GetRelativeNodePositionZ", "NetImmerse", papyrusNetImmerse::GetRelativeNodePositionZ, registry));
	/*registry->SetFunctionFlags("NetImmerse", "HasNode", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("NetImmerse", "GetNodePositionX", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("NetImmerse", "GetNodePositionY", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("NetImmerse", "GetNodePositionZ", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("NetImmerse", "GetNodeScale", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("NetImmerse", "SetNodeScale", VMClassRegistry::kFunctionFlag_NoWait);*/
}
