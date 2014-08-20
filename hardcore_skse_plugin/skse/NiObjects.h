#pragma once

#include "skse/NiRTTI.h"
#include "skse/NiTypes.h"
#include "skse/Utilities.h"

// NiRefObject/NiObject and important children
// generally other children should go in other files
// especially if they can be grouped

class NiCloningProcess;
class NiStream;
class NiObjectGroup;
class NiExtraData;
class NiTimeController;
class NiNode;
class NiGeometry;
class NiRenderedTexture;
class NiSwitchNode;
class NiTriBasedGeom;
class NiTriShape;
class NiTriStrips;
class BSSegmentedTriShape;
class NiRenderTargetGroup;
class NiProperty;
class NiSourceTexture;

class TESObjectCELL;

class TESModelTri;
class BSFaceGenMorphData;

extern float * g_worldToCamMatrix;
extern NiRect<float> * g_viewPort;

typedef bool (* _WorldPtToScreenPt3_Internal)(float * worldToCamMatrix, NiRect<float> * port, NiPoint3 * p_in, float * x_out, float * y_out, float * z_out, float zeroTolerance);
extern const _WorldPtToScreenPt3_Internal WorldPtToScreenPt3_Internal;

// 08
class NiRefObject
{
public:
	NiRefObject();
	virtual ~NiRefObject();

	virtual void	DeleteThis(void);	// calls virtual dtor

	void	IncRef(void);
	void	DecRef(void);
	bool	Release(void);

//	void	** _vtbl;		// 00
	volatile SInt32	m_uiRefCount;	// 04
};

// ### not all of this is verified, I'm just assuming that little has changed from other
// ### games using previous versions of NetImmerse that have released .pdb files for reference

// 08
class NiObject : public NiRefObject
{
public:
	// standard NetImmerse
	virtual NiRTTI *		GetRTTI(void);

	// then a bunch of attempts to avoid dynamic_cast?
	// unverified, do not use
	virtual NiNode			* GetAsNiNode(void);
	virtual NiSwitchNode	* GetAsNiSwitchNode(void);
	virtual UInt32			Unk_05(void);
	virtual UInt32			Unk_06(void);
	virtual NiGeometry		* GetAsNiGeometry(void);
	virtual NiTriBasedGeom	* GetAsNiTriBasedGeom(void);
	virtual NiTriStrips		* GetAsNiTriStrips(void);
	virtual NiTriShape		* GetAsNiTriShape(void);
	virtual BSSegmentedTriShape * GetAsBSSegmentedTriShape(void);
	virtual UInt32			Unk_0C(void);
	virtual UInt32			Unk_0D(void);
	virtual UInt32			Unk_0E(void);
	virtual UInt32			Unk_0F(void);
	virtual UInt32			Unk_10(void);
	virtual UInt32			Unk_11(void);
	virtual UInt32			Unk_12(void);

	// then back to NetImmerse
	virtual NiObject *		CreateClone(NiCloningProcess cloner);

	virtual void			LoadBinary(NiStream * stream);
	virtual void			LinkObject(NiStream * stream);
	virtual bool			RegisterStreamables(NiStream * stream);
	virtual void			SaveBinary(NiStream * stream);
	virtual bool			IsEqual(NiObject * object);

	// viewer appears to be disabled sadly
	// why did you do that? it was really useful for figuring out class data
	// and it's not like it totally bloated up the executa... er never mind

	virtual void			ProcessClone(NiCloningProcess * cloner);
	virtual void			PostLinkObject(NiStream * stream);
	virtual bool			StreamCanSkip(void);
	virtual const NiRTTI *	GetStreamableRTTI(void) const;
	virtual UInt32			GetBlockAllocationSize(void) const;
	virtual NiObjectGroup *	GetGroup(void) const;
	virtual void			SetGroup(NiObjectGroup * group);

	// begin bethesda extensions? possibly just stuff we can't match up
	virtual UInt32			Unk_20(void);

	
	MEMBER_FN_PREFIX(NiObject);
	DEFINE_MEMBER_FN(DeepCopy, NiStream *, 0x00AAFD60, NiObject ** result);
};

STATIC_ASSERT(sizeof(NiObject) == 0x08);

// 18
class NiObjectNET : public NiObject
{
public:
	const char	* m_name;

	NiTimeController	* m_controller;	// 0C next pointer at +0x30

	NiExtraData	** m_extraData;			// 10 extra data
	UInt16		m_extraDataLen;			// 14 max valid entry
	UInt16		m_extraDataCapacity;	// 16 array len

	// UNTESTED
	void AddExtraData(NiExtraData * extraData);
	bool RemoveExtraData(NiExtraData * extraData);
	SInt32 GetIndexOf(NiExtraData * extraData);
	NiExtraData * GetExtraData(const char * name);
};

STATIC_ASSERT(sizeof(NiObjectNET) == 0x18);

// A8
class NiAVObject : public NiObjectNET
{
public:
	enum
	{
		kFlag_SelectiveUpdate =				0x00000002,
		kFlag_UpdatePropertyControllers =	0x00000004,

		kFlag_SelectiveUpdateRigid =		0x00000010,

		kFlag_OverrideSelectiveTransforms =	0x00000080,
	};

	struct ControllerUpdateContext
	{
		enum
		{
			kDirty =	1 << 0,
		};

		float	delta;
		UInt8	flags;
	};

	virtual void	UpdateControllers(ControllerUpdateContext * ctx);	// calls controller vtbl+0x8C
	virtual void	UpdateNodeBound(ControllerUpdateContext * ctx);
	virtual void	ApplyTransform(NiMatrix33 * mtx, NiPoint3 * translate, bool postTransform);
	virtual void	SetPropertyState(NiProperty * prop);
	virtual void	Unk_25(UInt32 arg0);
	virtual void	Unk_26(UInt32 arg0);
	virtual NiAVObject *	GetObjectByName(const char ** name);	// BSFixedString? alternatively BSFixedString is a typedef of a netimmerse type
	virtual void	SetSelectiveUpdateFlags(bool * selectiveUpdate, bool selectiveUpdateTransforms, bool * rigid);
	virtual void	UpdateDownwardPass(ControllerUpdateContext * ctx, UInt32 unk1);
	virtual void	UpdateSelectedDownwardPass(ControllerUpdateContext * ctx, UInt32 unk1);
	virtual void	UpdateRigidDownwardPass(ControllerUpdateContext * ctx, UInt32 unk1);
	virtual void	UpdateWorldBound(void);
	virtual void	UpdateWorldData(ControllerUpdateContext * ctx);
	virtual void	UpdateNoControllers(ControllerUpdateContext * ctx);
	virtual void	UpdateDownwardPassTempParent(NiNode * parent, ControllerUpdateContext * ctx);
	virtual void	Unk_30(void);	// calls virtual function on parent
	virtual void	Unk_31(UInt32 arg0);
	virtual void	Unk_32(UInt32 arg0);

	NiNode		* m_parent;			// 18
	NiAVObject	* unk1C;			// 1C
	NiTransform	m_localTransform;	// 20
	NiTransform	m_worldTransform;	// 54
	float		unk88;				// 88
	float		unk8C;				// 8C
	float		unk90;				// 90
	float		unk94;				// 94
	UInt32		m_flags;			// 98 - bitfield
	float		unk9C;				// 9C
	UInt32		unkA0;				// A0
	UInt8		unkA4;				// A4
	UInt8		unkA5;				// A5 - bitfield

	MEMBER_FN_PREFIX(NiAVObject);
	DEFINE_MEMBER_FN(UpdateNode, void, 0x00AAF320, ControllerUpdateContext * ctx);
};
STATIC_ASSERT(sizeof(NiAVObject) == 0xA8);

MAKE_NI_POINTER(NiAVObject);

// Bethesda class, unknown name
class BSRenderTargetGroup : public NiObject
{
public:
	virtual ~BSRenderTargetGroup();

	NiRenderTargetGroup * unk08[6];			// 08
	UInt32	unk20;							// 20
	UInt32	unk24;							// 24
	UInt32	unk28;							// 28
	UInt32	unk2C;							// 2C inited to FFFFFFFF
	NiRenderedTexture * renderedTexture[4];	// 30

	static BSRenderTargetGroup *	GetPlayerFaceMask(void)
	{
		return *((BSRenderTargetGroup **)0x01B3FD54);
	}
};

// 10
class BSFaceGenModel : public NiRefObject
{
public:
	struct Data08
	{
		UInt32				unk00;		// 00
		NiAVObject			* unk04;	// 04
		NiAVObject			* unk08;	// 08
		UInt32				unk0C;		// 0C
		BSFaceGenMorphData	* unk10;	// 10
	};

	Data08	* unk08;	// 08
	UInt32	unk0C;		// 0C

	MEMBER_FN_PREFIX(BSFaceGenModel);
	DEFINE_MEMBER_FN(ctor, void, 0x005A5D40);
	DEFINE_MEMBER_FN(CopyFrom, void, 0x005A5D80, BSFaceGenModel * other);
	DEFINE_MEMBER_FN(SetModelData, bool, 0x005A60D0, const char * meshPath, void * unk1, UInt8 unk2);
	DEFINE_MEMBER_FN(ApplyMorph, UInt8, 0x005A5B70, const char ** morphName, TESModelTri * triModel, NiAVObject ** headNode, float relative, UInt8 unk1);
};

// ??
class BSFaceGenMorphData : public NiRefObject
{
public:
	void	* unk08;	// 08
	UInt32	unk0C;		// 0C

	//MEMBER_FN_PREFIX(BSFaceGenMorphData);
	//DEFINE_MEMBER_FN(ApplyMorph, UInt8, 0x005A75F0, const char ** morphName, NiAVObject * faceTrishape, float relative, UInt8 unk2);
};

// 10
class BSFaceGenModelMap : public NiRefObject
{
public:
	struct Entry
	{
		BSFaceGenModel	* unk00;	// 00
		UInt32			unk04;		// 04
	};

	Entry	unk08;	// 08
};

// 6C
class LoadedAreaBound : public NiRefObject
{
public:
	virtual ~LoadedAreaBound();

	UInt32 unk08;
	UInt32 unk0C;
	UInt32 unk10;
	UInt32 unk14;
	UInt32 unk18;
	UInt32 unk1C;
	UInt32 unk20;
	TESObjectCELL * cell; // 24
	UInt32 unk28;
	UInt32 unk2C;
	UInt32 unk30;
	UInt32 unk34;
	UInt32 unk38; // inited 0xDEADBEEF
	UInt32 unk3C;
	UInt32 unk40;
	NiPoint3 boundsMax; // 44
	NiPoint3 boundsMin;	// 50
	UInt32 unk5C;
	UInt32 unk60;
	UInt32 unk64;
	UInt32 unk68;
};
STATIC_ASSERT(sizeof(LoadedAreaBound) == 0x6C);
STATIC_ASSERT(offsetof(LoadedAreaBound, boundsMax) == 0x44);

// 4C
class BSFaceGenMorphDataHead : public BSFaceGenMorphData
{
public:
	UInt32	unk10[(0x48 - 0x10) >> 2];
	UInt32	unk48;
};

STATIC_ASSERT(offsetof(BSFaceGenMorphDataHead, unk48) == 0x48);
STATIC_ASSERT(sizeof(BSFaceGenMorphDataHead) == 0x4C);


// 10
class BSFaceGenMorphDataHair : public BSFaceGenMorphData
{
public:
};
STATIC_ASSERT(sizeof(BSFaceGenMorphDataHair) == 0x10);

class NiCamera : public NiAVObject
{
public:
	float			m_aafWorldToCam[4][4];	// A8
	NiFrustum		m_frustum;				// E8
	float			m_fMinNearPlaneDist;	// 104
	float			m_fMaxFarNearRatio;		// 108
	NiRect<float>	m_kPort;				// 10C
	float			m_fLODAdjust;			// 11C

	bool WorldPtToScreenPt3(NiPoint3 * p_in, float * x_out, float * y_out, float * z_out, float zeroTolerance = 1e-5);
};
STATIC_ASSERT(offsetof(NiCamera, m_frustum) == 0xE8);
STATIC_ASSERT(offsetof(NiCamera, m_fLODAdjust) == 0x11C);