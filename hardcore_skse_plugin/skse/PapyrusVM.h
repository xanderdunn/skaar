#pragma once

#include "skse/Utilities.h"
#include "skse/GameTypes.h"

class IFunction;
class VMIdentifier;
class VMValue;
class VMClassRegistry;
class IFunctionArguments;

class IObjectHandlePolicy
{
public:
	IObjectHandlePolicy();
	virtual ~IObjectHandlePolicy();

	// this code heavily uses 64-bit values, so many of these arguments may be paired 64-bit (eax/edx)
	virtual bool	IsType(UInt32 typeID, UInt64 handle);
	virtual bool	Unk_02(UInt64 handle);
	virtual UInt64	GetInvalidHandle(void);
	virtual UInt64	Create(UInt32 typeID, void * srcData);
	virtual bool	IsREFR(UInt64 handle);	// return IsType(TESObjectREFR::kTypeID, handle);
	virtual UInt64	Unk_06(UInt32 unk0, UInt32 unk1);
	virtual UInt64	Unk_07(UInt32 unk0, UInt32 unk1);
	virtual void *	Resolve(UInt32 typeID, UInt64 handle);
	virtual void	AddRef(UInt64 handle);
	virtual void	Release(UInt64 handle);
	virtual void	GetName(UInt64 handle, void * outStr);

#ifdef PAPYRUS_CUSTOM_CLASS
	MEMBER_FN_PREFIX(IObjectHandlePolicy);
	DEFINE_MEMBER_FN(Unk_02_Origin, bool, 0x008B3D60, UInt64 handle);
	DEFINE_MEMBER_FN(IsType_Origin, bool, 0x008B3B20, UInt32 typeID, UInt64 handle);
	DEFINE_MEMBER_FN(Create_Origin, UInt64, 0x008B4310, UInt32 typeID, void * srcData);
	DEFINE_MEMBER_FN(Resolve_Origin, void *, 0x008B3E30, UInt32 typeID, UInt64 handle);

	bool Unk_02_Hook(UInt64 handle);
	bool IsType_Hook(UInt32 typeID, UInt64 handle);
	void * Resolve_Hook(UInt32 typeID, UInt64 handle);
	UInt64	Create_Hook(UInt32 typeID, void * srcData);
#endif

//	void	** _vtbl;	// 00
};

extern IObjectHandlePolicy	** g_objectHandlePolicy;

class ObjectBindPolicy
{
public:
	ObjectBindPolicy();
	virtual ~ObjectBindPolicy();

	virtual void	Unk_01(UInt64 unk);
	virtual void	Unk_02(UInt64 unk);
	virtual void	Unk_03(UInt64 unk);
	virtual void	Unk_04(void);
	virtual UInt32	Unk_05(UInt32 unk0, UInt32 unk1);
	virtual void	Unk_06(UInt32 unk0, UInt32 unk1, UInt32 unk2, UInt32 unk3);
	virtual void	Unk_07(UInt32 unk0, UInt32 unk1, UInt32 unk2, UInt32 unk3, UInt32 unk4, UInt32 unk5);
	virtual bool	Unk_08(void);
	virtual void	Unk_09(UInt32 unk0, UInt32 unk1, UInt32 unk2, UInt32 unk3);
	virtual void	Unk_0A(UInt32 unk0, UInt32 unk1, UInt32 unk2, UInt32 unk3);
	virtual void	Unk_0B(UInt32 unk0, UInt32 unk1, UInt32 unk2, UInt32 unk3, UInt32 unk4);
	virtual void	Unk_0C(UInt32 unk0, UInt32 unk1, UInt32 unk2, UInt32 unk3, UInt32 unk4);

//	void	** _vtbl;	// 00

	MEMBER_FN_PREFIX(ObjectBindPolicy);
	DEFINE_MEMBER_FN(BindObject, void, 0x00C2BCD0, VMIdentifier ** identifier, UInt64 handle);
};

// 10
class VMClassInfo
{
public:
	SInt32				refCount;
	StringCache::Ref	name;	// probably StringCache::Ref
	VMClassInfo			* parent;
	void				* unk0C;

	void	AddRef(void);
	void	Release(void);

	MEMBER_FN_PREFIX(VMClassInfo);
	DEFINE_MEMBER_FN(Destroy, void, 0x00C34ED0);
	DEFINE_MEMBER_FN(GetVariable, SInt32, 0x00C33E30, BSFixedString * name);
	DEFINE_MEMBER_FN(GetFunction, IFunction*, 0x00C36540, const char * fnName);
};

// This type is not fully decoded or correctly sized, just enough to use the functor
class VMScriptInstance
{
public:
	UInt32				unk00;
	VMClassInfo			* classInfo;
	void				* unk08;
	UInt32				unk0C;
	UInt32				formId;
	UInt32				unk14;
	UInt32				unk18;
};

// 04
class IForEachScriptObjectFunctor
{
public:
	IForEachScriptObjectFunctor(){};
	virtual ~IForEachScriptObjectFunctor() {};

	// return true to continue
	virtual bool	Visit(VMScriptInstance * arg, void * arg2) { return false; };

//	void	** _vtbl;	// 00
};

// 48+
class VMUnlinkedClassList
{
	virtual ~VMUnlinkedClassList()	{ }
	virtual VMUnlinkedClassList *	Create(void);
	virtual void					SetUnk0C(void * arg);
	virtual bool					Link(StringCache::Ref * className);

//	void	** _vtbl;	// 00
	VMClassRegistry	* unk04;	// 04
	void	* unk08;	// 08
	void	* unk0C;	// 0C - loader
	UInt8	unk10;		// 10
	UInt8	pad11[3];	// 11
	UInt32	unk14;		// 14
	UInt32	unk18;		// 18
	UInt32	unk1C;		// 1C
	void	* unk20;	// 20
	UInt32	unk24;		// 24
	UInt32	unk28;		// 28
	UInt32	unk2C;		// 2C
	void	* unk30;	// 30
	UInt32	unk34;		// 34
	UInt32	unk38;		// 38
	UInt32	unk3C;		// 3C
	void	* unk40;	// 40
	UInt32	unk44;		// 44
};

// 1C?
class VMIdentifier
{
public:
	enum
	{
		kLockBit = 0x80000000,
		kFastSpinThreshold = 10000
	};

	SInt32			m_refCount;	// 00
	VMClassInfo		* m_type;	// 04
	void			* unk08;	// 08
	void			* unk0C;	// 0C
	volatile UInt64	m_handle;	// 10
	volatile SInt32	m_lock;		// 18

	UInt64	GetHandle(void);

	SInt32	Lock(void);
	void	Unlock(SInt32 oldLock);

	// lock and refcount?
	void	IncrementLock(void);
	SInt32	DecrementLock(void);

	void	Destroy(void);

	MEMBER_FN_PREFIX(VMIdentifier);
	DEFINE_MEMBER_FN(Destroy_Internal, void, 0x00C30E90);
};

// 08
// possibly BSScriptVariable
class VMValue
{
public:
	VMValue()
		:type(kType_None) { data.u = 0; }
	~VMValue()
	{ CALL_MEMBER_FN(this, Destroy)(); }

	enum
	{
		kType_None =		0,
		kType_Identifier =	1,
		kType_String =		2,
		kType_Int =			3,
		kType_Float =		4,
		kType_Bool =		5,

		kType_Unk0B =		0x0B,
		kType_ArraysStart = 11,
		kType_StringArray =	12,
		kType_IntArray =	13,
		kType_FloatArray =	14,
		kType_BoolArray =	15,
		kType_ArraysEnd =   16,

		kNumLiteralArrays = 4
	};

	// 14+
	struct ArrayData
	{
		volatile SInt32	refCount;	// 00
		UInt32			unk04;		// 04
		UInt32			len;		// 08
		UInt32			unk0C;		// 0C
		UInt32			unk10;		// 10
		//		VMValue			data[0];	// 14

		VMValue	*	GetData(void)	{ return (VMValue *)(this + 1); }

		MEMBER_FN_PREFIX(ArrayData);
		DEFINE_MEMBER_FN(Destroy, void, 0x00C3A0A0);
	};

	UInt32	type;	// 00

	union
	{
		SInt32			i;
		UInt32			u;
		float			f;
		bool			b;
		void			* p;
		ArrayData		* arr;
		VMIdentifier	* id;
		const char		* str;	// BSFixedString

		BSFixedString *	GetStr(void)	{ return (BSFixedString *)(&str); }
	} data;			// 04

	MEMBER_FN_PREFIX(VMValue);
	DEFINE_MEMBER_FN(Set, void, 0x00C329E0, VMValue * src);
	DEFINE_MEMBER_FN(Destroy, void, 0x00C328E0);
	DEFINE_MEMBER_FN(SetArray, void, 0x00C32CE0, ArrayData * data);

	bool	IsIdentifierArray()
	{
		return (type >= kType_ArraysEnd && type & kType_Identifier);
	}

	bool	IsLiteralArray()
	{
		return type - kType_ArraysStart <= kNumLiteralArrays;
	}

	bool	IsArray()
	{
		return IsLiteralArray() || IsIdentifierArray();
	}

	void	SetNone(void)
	{
		CALL_MEMBER_FN(this, Destroy)();

		type = kType_None;
		data.u = 0;
	}

	void	SetInt(SInt32 i)
	{
		CALL_MEMBER_FN(this, Destroy)();

		type = kType_Int;
		data.i = i;
	}

	void	SetFloat(float f)
	{
		CALL_MEMBER_FN(this, Destroy)();

		type = kType_Float;
		data.f = f;
	}

	void	SetBool(bool b)
	{
		CALL_MEMBER_FN(this, Destroy)();

		type = kType_Bool;
		data.b = b;
	}

	void	SetIdentifier(VMClassInfo * classInfo)
	{
		CALL_MEMBER_FN(this, Destroy)();

		type = (UInt32)classInfo;
		data.id = NULL;
	}

	void	SetIdentifier(VMIdentifier ** identifier)
	{
		if(GetUnmangledType() == kType_Identifier)
		{
			CALL_MEMBER_FN(this, Destroy)();

			if(*identifier)
				(*identifier)->IncrementLock();

			data.id = *identifier;
		}
	}

	void	SetString(const char * str)
	{
		CALL_MEMBER_FN(this, Destroy)();

		type = kType_String;
		CALL_MEMBER_FN(data.GetStr(), Set)(str);
	}

	// 00-0F are untouched
	// 10+ alternate between 0x01 and 0x0B
	UInt32	GetUnmangledType(void);

	bool	IsIdentifier(void)	{ return GetUnmangledType() == kType_Identifier; }
};

STATIC_ASSERT(sizeof(VMValue) == 0x08);

// 4B04
// this does more than hold on to class registrations, but for now that's all we care about
class VMClassRegistry
{
public:
	enum
	{
		kFunctionFlag_NoWait = 0x01
	};

	VMClassRegistry();
	virtual ~VMClassRegistry();

	// ### indices are from 1.5.26

	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	RegisterForm(UInt32 typeID, const char * papyrusClassName);
	virtual void	Unk_09(void);
	virtual bool	GetFormClass(UInt32 formID, VMClassInfo ** outClass);
	virtual void	Unk_0B(void);
	virtual void	Unk_0C(void);
	virtual bool	Unk_0D(StringCache::Ref * className, UInt32 * unk);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(void);
	virtual void	Unk_11(void);
	virtual void	Unk_NewIn16_11(void);	// ### added in 1.6.86, further indices are off by one
	virtual void	Unk_12(void);
	virtual bool	Unk_13(StringCache::Ref * className, VMIdentifier ** identifier);
	virtual bool	CreateArray(VMValue * value, UInt32 size, VMValue::ArrayData ** unk1);
	virtual void	Unk_15(void);
	virtual void	RegisterFunction(IFunction * fn);
	virtual void	SetFunctionFlagsEx(const char * className, UInt32 unk0, const char * fnName, UInt32 flags);
	virtual void	SetFunctionFlags(const char * className, const char * fnName, UInt32 flags);
	virtual void	VisitScripts(UInt64 handle, IForEachScriptObjectFunctor * functor);
	virtual bool	Unk_1A(UInt64 handle, const char * className, VMIdentifier ** identifier);
	virtual void	Unk_1B(void);
	virtual void	Unk_1C(void);
	virtual void	Unk_1D(void);
	virtual void	Unk_1E(void);
	virtual void	Unk_1F(void);
	virtual void	Unk_20(void);
	virtual bool	ExtractValue(UInt64 handle, StringCache::Ref * className, SInt32 variableIndex, VMValue * out);	// unk probably script context object?
	virtual void	QueueEvent(UInt64 handle, const StringCache::Ref * eventName, IFunctionArguments * args);
	virtual void	Unk_23(void);
	virtual void	Unk_24(void);
	virtual void	Unk_25(void);
	virtual void	Unk_26(void);
	virtual void	Unk_27(void);
	virtual void	Unk_28(void);
	virtual void	Unk_29(void);
	virtual void	Unk_2A(void);
	virtual IObjectHandlePolicy *	GetHandlePolicy(void);
	virtual void	Unk_2C(void);
	virtual ObjectBindPolicy *		GetObjectBindPolicy(void);
	virtual void	Unk_2E(void);
	virtual void	Unk_2F(void);
	virtual void	Unk_30(void);
	virtual void	Unk_31(void);
	virtual void	Unk_32(void);
	virtual void	Unk_33(void);

//	void		** _vtbl;		// 0000
	UInt32		unk0004;		// 0004 - refcount?
	void		** vtbl0008;	// 0008
	void		** vtbl000C;	// 000C
	void		** vtbl0010;	// 0010
	UInt32		unk0014[(0x006C - 0x0014) >> 2];	// 0014
	VMUnlinkedClassList	unlinkedClassList;			// 006C
	UInt32		unk00B4[(0x4B04 - 0x00B4) >> 2];	// 00B4
};

STATIC_ASSERT(offsetof(VMClassRegistry, unlinkedClassList) == 0x006C);
STATIC_ASSERT(sizeof(VMClassRegistry) == 0x4B04);

// 45D0
class SkyrimVM
{
public:
	SkyrimVM();
	virtual ~SkyrimVM();

	virtual void	Unk_01(void);

//	void						** _vtbl;				// 0000
	UInt32						eventSinks[63];		// 0004
	VMClassRegistry				* m_classRegistry;		// 0100
	UInt8						pad104[0x46C - 0x104];	// 0104
	SimpleLock					m_updateLock;			// 046C
	UpdateRegistrationHolder	m_updateRegHolder;		// 0474
	UpdateRegistrationHolder	m_updateGameTimeRegHolder;	// 0480

	VMClassRegistry	*	GetClassRegistry(void)	{ return m_classRegistry; }

	UInt32				ClearInvalidRegistrations(void);

	MEMBER_FN_PREFIX(SkyrimVM);

	// Used by Hooks_Papyrus
	DEFINE_MEMBER_FN(UnregisterFromSleep_Internal, void, 0x008D4C40, UInt64 handle);
	DEFINE_MEMBER_FN(RevertGlobalData_Internal, bool, 0x008D5DF0);
	DEFINE_MEMBER_FN(SaveRegSleepEventHandles_Internal, bool, 0x008CD8F0, void * handleReaderWriter, void * saveStorageWrapper);
	DEFINE_MEMBER_FN(LoadRegSleepEventHandles_Internal, bool, 0x008D3DB0, void * handleReaderWriter, void * loadStorageWrapper);

	void OnFormDelete_Hook(UInt64 handle);
	void RevertGlobalData_Hook(void);
	bool SaveGlobalData_Hook(void * handleReaderWriter, void * saveStorageWrapper);
	bool LoadGlobalData_Hook(void * handleReaderWriter, void * loadStorageWrapper);
};
STATIC_ASSERT(offsetof(SkyrimVM, m_classRegistry) == 0x100);

extern SkyrimVM	** g_skyrimVM;

class IFunctionArguments
{
public:
	virtual ~IFunctionArguments()	{ }

	struct Output
	{
		UInt32	unk00;		// 00
		VMValue	* m_data;	// 04 - tArray <VMValue>?
		UInt32	unk08;		// 08
		UInt32	m_size;		// 0C

		void	Resize(UInt32 len)	{ CALL_MEMBER_FN(this, Resize)(len); }
		VMValue	* Get(UInt32 idx)	{ return (idx < m_size) ? &m_data[idx] : NULL; }

		MEMBER_FN_PREFIX(Output);
		DEFINE_MEMBER_FN(Resize, bool, 0x008C2A70, UInt32 len);
	};

	virtual bool	Copy(Output * dst) = 0;
};
