#pragma once

#include "skse/Utilities.h"

// class NiBinaryStream
// class NiFile : public NiBinaryStream
// class NiMemStream : public NiBinaryStream
// class OutputInfoStream : public NiBinaryStream
// class BSResourceNiBinaryStream : public NiBinaryStream
// class BSResourceStreamParser : public BSResourceNiBinaryStream
// class BSStreamParserData

// Not sure if this is right
class NiBinaryStream
{
public:
	virtual ~NiBinaryStream() { };

	virtual bool	IsValid(void) = 0;
	virtual void	Seek(SInt32 delta) = 0;
	virtual UInt32	GetOffset(void) { return 0; }

	//	void	** _vtbl;	// 00
	UInt32	pad04;		// 04 - align for UInt64
	void	* readFn;	// 08
	void	* writeFn;	// 0C
};

class NiMemStream : public NiBinaryStream
{
public:
	NiMemStream();
	virtual ~NiMemStream();

	virtual bool	IsValid(void) { return true; };
	virtual void	Seek(SInt32 delta);
	virtual UInt32	GetOffset(void);
	virtual void	CopyFrom(void * other);
	virtual void	FunctionSwap(bool swap);

	MEMBER_FN_PREFIX(NiMemStream);
	DEFINE_MEMBER_FN(Construct, NiMemStream *, 0x00B2BBC0);
	DEFINE_MEMBER_FN(Destroy, void, 0x00B2B8B0);
	DEFINE_MEMBER_FN(Seek, void, 0x00B2B910, SInt32);
	DEFINE_MEMBER_FN(GetOffset, UInt32, 0x00B2A390, void);
	DEFINE_MEMBER_FN(CopyFrom, UInt32, 0x00B2B930, void *);
	DEFINE_MEMBER_FN(FunctionSwap, UInt32, 0x00B2BB50, bool);

	void	* data;	// 10
	UInt32	offset;	// 14
	UInt32	unk18;	// 18
	UInt32	unk1C;	// 1C - Inited to 0x400
	UInt8	flag1;	// 20
	UInt8	flag2;	// 21
	UInt16	pad22;	// 22
};

class BSResourceNiBinaryStream : public NiBinaryStream
{
public:
	BSResourceNiBinaryStream();
	BSResourceNiBinaryStream(const char * path);
	virtual ~BSResourceNiBinaryStream();

	// stub implementations
	virtual bool	IsValid(void);
	virtual void	Seek(SInt32 delta);
	virtual UInt32	GetOffset(void);
	virtual void	Unk_04(void) { }
	virtual void	EnableEndianSwap(bool swap);

	void	* unk10;	// 10
	void	* unk14;	// 14
	UInt64	offset;		// 18

	MEMBER_FN_PREFIX(BSResourceNiBinaryStream);
	DEFINE_MEMBER_FN(Construct, BSResourceNiBinaryStream *, 0x00AC6790, const char * filePath, UInt32 unk1, UInt32 unk2);	// unk1 = 0, unk2 = 0
	DEFINE_MEMBER_FN(ReadLine, UInt32, 0x00AC6710, char * dst, UInt32 dstLen, UInt32 terminator);
	DEFINE_MEMBER_FN(Read, UInt32, 0x00AC6410, void * dst, UInt32 len);
	DEFINE_MEMBER_FN(Write, UInt32, 0x00AC6490, void * src, UInt32 len);
	DEFINE_MEMBER_FN(Destroy, void, 0x00AC69B0);

	DEFINE_MEMBER_FN(IsValid, bool, 0x00AC6400, void);
	DEFINE_MEMBER_FN(Seek, void, 0x00AC63B0, SInt32);
	DEFINE_MEMBER_FN(GetOffset, UInt32, 0x009EA560, void);	// return offset;
	DEFINE_MEMBER_FN(EnableEndianSwap, void, 0x00AC66E0, bool);

	UInt32 ReadLine(char * dst, UInt32 dstLen, UInt32 terminator);
	UInt32 ReadLine_w(wchar_t * dst, UInt32 dstLen, UInt32 terminator);	// length in characters
	UInt32 Read(void * dst, UInt32 len);
	UInt32 Write(void * src, UInt32 len);
};

STATIC_ASSERT(offsetof(BSResourceNiBinaryStream, readFn) == 0x08);
STATIC_ASSERT(offsetof(BSResourceNiBinaryStream, offset) == 0x18);
STATIC_ASSERT(sizeof(BSResourceNiBinaryStream) == 0x20);
