#pragma once

#include "skse/Utilities.h"
#include "skse/NiTypes.h"

class NiBinaryStream;
class NiObject;

// 5B4 or less
class NiStream
{
public:
	NiStream();
	virtual ~NiStream();

	virtual bool	LoadStream(NiBinaryStream * stream);
	virtual bool	LoadBuf(char * buf, UInt32 len);
	virtual bool	LoadPath(const char * path);
	virtual bool	SaveStream(NiBinaryStream * stream);
	virtual bool	SaveBuf(char ** buf, UInt32 * len);
	virtual bool	SavePath(const char * path);

	MEMBER_FN_PREFIX(NiStream);
	DEFINE_MEMBER_FN(ctor, NiStream *, 0x00AB2C50);
	DEFINE_MEMBER_FN(dtor, void, 0x00AB1ED0);
	DEFINE_MEMBER_FN(AddObject, void, 0x00AB2E90, NiObject * object);

//	void	** _vtbl;								// 000
	UInt32	pad004[(0x1F4 - 0x004) >> 2];			// 004
	NiTLargeArray <NiObject *>	m_objects;			// 1F4
	NiTLargeArray <UInt32>		m_objectSizes;		// 20C
	NiTLargeArray <NiObject *>	m_rootObjects;		// 224
	NiTLargeArray <char *>		m_stringTable;		// 23C
	UInt32	unk254;									// 254
	UInt32	unk258;									// 258
	UInt32	unk25C;									// 25C
	UInt32	unk260;									// 260
	NiTMap<NiObject*, UInt32>	m_pointerMap;		// 264
};

class DeepCopyStream : public NiStream
{
public:
	DeepCopyStream();
	virtual ~DeepCopyStream();

	// vtbl 001117394

	MEMBER_FN_PREFIX(DeepCopyStream);
	DEFINE_MEMBER_FN(SaveStream, bool, 0x00AB08C0, char ** buffer, UInt32 * length);
	DEFINE_MEMBER_FN(LoadStream, bool, 0x00AB0410, char * buffer, UInt32 length);
};

STATIC_ASSERT(offsetof(NiStream, m_objects) == 0x1F4);
STATIC_ASSERT(offsetof(NiStream, m_objectSizes) == 0x20C);
STATIC_ASSERT(offsetof(NiStream, m_rootObjects) == 0x224);
STATIC_ASSERT(offsetof(NiStream, m_stringTable) == 0x23C);
