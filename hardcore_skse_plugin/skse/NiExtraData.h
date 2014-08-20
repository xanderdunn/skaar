#pragma once

#include "skse/NiTypes.h"
#include "skse/NiObjects.h"
#include "skse/GameTypes.h"

class BSFaceGenKeyframe
{
public:
	virtual ~BSFaceGenKeyframe();

	virtual void Unk_01(void); // pure
	virtual void Unk_02(void); // pure
	virtual void Unk_03(void); // pure
	virtual void Unk_04(UInt32 unk1); // pure
	virtual void Unk_05(void); // pure
	virtual void Unk_06(void); // pure
	virtual void Unk_07(void); // pure
	virtual void Unk_08(void); // pure
	virtual void Unk_09(void); // pure
	virtual void Unk_0A(void); // pure
	virtual void Unk_0B(void); // pure
	virtual void Unk_0C(void);
	virtual void Unk_0D(void);
};

class BSFaceGenKeyframeMultiple : public BSFaceGenKeyframe
{
public:
	UInt32	type;	// 04
	UInt32	unk08;	// 08

	float	* values;	// 0C
	UInt32	count;		// 10
	UInt8	isUpdated;	// 14 - Set to 0 for update request
	UInt8	pad15[3];
};
STATIC_ASSERT(sizeof(BSFaceGenKeyframeMultiple) == 0x18);

class NiExtraData : public NiObject
{
public:
	NiExtraData();
	~NiExtraData();

	char *	m_pcName;	// 08
};

// 10
class NiStringExtraData : public NiExtraData
{
public:
	NiStringExtraData();
	~NiStringExtraData();

	char	* m_pString;	// 0C
};

// 10
class NiIntegerExtraData : public NiExtraData
{
public:
	NiIntegerExtraData();
	~NiIntegerExtraData();

	SInt32 m_data;	// 0C
};

// 14
class NiBinaryExtraData : public NiExtraData
{
public:
	NiBinaryExtraData();
	~NiBinaryExtraData();

	char		* m_data;	// 0C
	UInt32		m_size;		// 10
};

// 14
class NiFloatsExtraData : public NiExtraData
{
public:
	NiFloatsExtraData();
	~NiFloatsExtraData();

	UInt32	m_size;		// 0C
	float	* m_data;	// 10
};

// 14
class NiIntegersExtraData : public NiExtraData
{
public:
	NiIntegersExtraData();
	~NiIntegersExtraData();

	UInt32	m_size;		// 0C
	SInt32	* m_data;	// 10
};

// 14
class NiStringsExtraData : public NiExtraData
{
public:
	NiStringsExtraData();
	~NiStringsExtraData();

	UInt32	m_size;		// 0C
	char	** m_data;	// 10
};

// 1C
class NiVectorExtraData : public NiExtraData
{
public:
	NiVectorExtraData();
	~NiVectorExtraData();

	float m_vector[4];
};

// 1B4
class BSFaceGenAnimationData : public NiExtraData
{
public:
	enum {
		kKeyframeType_Expression = 0,
		kKeyframeType_Unk1,
		kKeyframeType_Modifier,
		kKeyframeType_Phoneme,
		kKeyframeType_Reset = 255,
		kNumKeyframes = 12
	};
	UInt32	unk0C;							// 0C
	BSFaceGenKeyframeMultiple	keyFrames[kNumKeyframes];	// 10
	float	unk130;							// 130
	float	unk134;							// 134
	float	unk138;							// 138
	UInt32	unk13C;							// 13C
	UInt32	unk140;							// 140
	UInt32	unk144;							// 144
	UInt32	unk148;							// 148
	float	unk14C;							// 14C
	float	unk150;							// 150
	UInt32	unk154;							// 154
	UInt8	unk158;							// 158
	UInt8	unk159;							// 159
	UInt8	unk15A;							// 15A
	UInt8	unk15B;							// 15B
	UInt32	unk15C;							// 15C
	UInt32	unk160;							// 160
	UInt32	unk164;							// 164
	UInt32	unk168;							// 168
	UInt32	unk16C;							// 16C
	UInt32	unk170;							// 170
	UInt32	unk174;							// 174
	UInt32	unk178;							// 178
	UInt32	unk17C;							// 17C
	UInt32	unk180;							// 180
	UInt32	unk184;							// 184
	UInt32	unk188;							// 188
	float	unk18C;							// 18C
	float	unk190;							// 190
	float	unk194;							// 194
	float	unk198;							// 198
	UInt8	unk19C;							// 19C
	UInt8	unk19D;							// 19D
	UInt8	unk19E;							// 19E
	UInt8	unk19F;							// 19F
	UInt8	unk1A0;							// 1A0
	UInt8	unk1A1;							// 1A1
	UInt8	unk1A2;							// 1A2
	UInt8	unk1A3;							// 1A3
	UInt8	unk1A4;							// 1A4
	UInt8	unk1A5;							// 1A5
	UInt8	overrideFlag;					// 1A6
	UInt8	unk1A7;							// 1A7
	UInt32	unk1A8[(0x1B4 - 0x1A8) >> 2];	// 1A8

	MEMBER_FN_PREFIX(BSFaceGenAnimationData);
	DEFINE_MEMBER_FN(SetExpression, void, 0x0059DB90, UInt32 type, float value);
	DEFINE_MEMBER_FN(SetPhonome, void, 0x005352D0, UInt32 type, float value);
	DEFINE_MEMBER_FN(SetModifier, void, 0x005352A0, UInt32 type, float value);
	DEFINE_MEMBER_FN(SetCustom, void, 0x00535300, UInt32 type, float value);
	DEFINE_MEMBER_FN(Reset, void, 0x0059E320, float value, UInt8 unk1, UInt8 unk2, UInt8 unk3, UInt8 unk4);
	
};

STATIC_ASSERT(offsetof(BSFaceGenAnimationData, keyFrames) == 0x10);
STATIC_ASSERT(offsetof(BSFaceGenAnimationData, overrideFlag) == 0x1A6);
STATIC_ASSERT(sizeof(BSFaceGenAnimationData) == 0x1B4);


// 30
class BSFaceGenModelExtraData : public NiExtraData
{
public:
	BSFaceGenModel* m_model;
	BSFixedString bones[8];
	
};
STATIC_ASSERT(sizeof(BSFaceGenModelExtraData) == 0x30);