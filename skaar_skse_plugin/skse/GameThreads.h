#pragma once

#include "Utilities.h"
#include "GameAPI.h"

class Actor;
class BGSHeadPart;
class NiAVObject;
class BGSTextureSet;
class NiAVObject;

class TaskDelegate
{
public:
	virtual void Run() = 0;
	virtual void Dispose() = 0;
};

class SKSETaskUpdateTintMasks : public TaskDelegate
{
public:
	virtual void Run();
	virtual void Dispose();
};

class SKSETaskUpdateHairColor : public TaskDelegate
{
public:
	virtual void Run();
	virtual void Dispose();
};

class SKSETaskRegenHead : public TaskDelegate
{
public:
	static SKSETaskRegenHead * Create(Actor * actor);
	virtual void Run();
	virtual void Dispose();

private:
	Actor* m_actor;
};

class SKSETaskChangeHeadPart : public TaskDelegate
{
public:
	static SKSETaskChangeHeadPart * Create(Actor * actor, BGSHeadPart* oldPart, BGSHeadPart* newPart);
	virtual void Run();
	virtual void Dispose();

private:
	Actor* m_actor;
	BGSHeadPart* m_oldPart;
	BGSHeadPart* m_newPart;
};

class SKSETaskUpdateWeight : public TaskDelegate
{
public:
	static SKSETaskUpdateWeight * Create(Actor * actor, float delta, UInt32 updateFlags, bool redrawWeapon);
	virtual void Run();
	virtual void Dispose();

private:
	Actor	* m_actor;
	float	m_delta;
	UInt32	m_updateFlags;
	bool	m_redraw;
};

class SKSETaskUpdateWorldData : public TaskDelegate
{
public:
	static SKSETaskUpdateWorldData * Create(NiAVObject * object);
	virtual void Run();
	virtual void Dispose();

private:
	NiAVObject * m_object;
};

class SKSETaskUpdateExpression : public TaskDelegate
{
public:
	static SKSETaskUpdateExpression * Create(Actor * actor, UInt8 type, UInt16 index, float value);
	virtual void Run();
	virtual void Dispose();

private:
	UInt8	m_type;
	UInt16	m_index;
	Actor	* m_actor;
	float	m_value;
};


class BSTaskPool
{
public:
	MEMBER_FN_PREFIX(BSTaskPool);
	DEFINE_MEMBER_FN(SetNiGeometryTexture, UInt32, 0x006A4590, NiAVObject * geometry, BGSTextureSet * textureSet);

	void ProcessTasks(void);
	void QueueTask(TaskDelegate * cmd);

	DEFINE_MEMBER_FN(ProcessTaskQueue_HookTarget, void, 0x006A0B30);

	void UpdateTintMasks();
	void UpdateHairColor();

	void RegenerateHead(Actor * actor);
	void ChangeHeadPart(Actor * actor, BGSHeadPart * oldPart, BGSHeadPart * newPart);
	void UpdateWeight(Actor * actor, float delta, UInt32 updateFlags, bool redrawWeapon);
	void UpdateWorldData(NiAVObject * object);
	void UpdateExpression(Actor * actor, UInt8 type, UInt16 index, float value);

	static BSTaskPool *	GetSingleton(void)
	{
		return *((BSTaskPool **)0x01B38308);
	}
};
