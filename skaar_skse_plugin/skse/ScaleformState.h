#pragma once

#include "skse/ScaleformTypes.h"
#include "skse/GameTypes.h"

class GFxState : public GRefCountBase
{
public:
	enum
	{
		kInterface_None =			0x00,
		kInterface_Translator =		0x03,
		kInterface_Log =			0x04,
		kInterface_ImageLoader =	0x05,
		kInterface_External =		0x09,
		kInterface_FileOpener =		0x0A,
		kInterface_ZlibSupport =	0x1E,
	};

	UInt32	interfaceType;	// 08
};

class GFxLogBase
{
public:
	virtual ~GFxLogBase () { }

	virtual bool    IsVerboseActionErrors() const   { return true; }
};

class SKSEGFxLogger : public GFxState, public GFxLogBase
{
public: 
	SKSEGFxLogger() : GFxState() { interfaceType = kInterface_Log; }
	virtual ~SKSEGFxLogger () { }

	enum
	{
		Log_Channel_General         = 0x10,
		Log_Channel_Script          = 0x20,
		Log_Channel_Parse           = 0x30,
		Log_Channel_Action          = 0x40,
		Log_Channel_Debug           = 0x50,
		Log_Channel_Mask            = 0xF0,
		Log_MessageType_Error       = 0,
		Log_MessageType_Warning     = 1,
		Log_MessageType_Message     = 2,
		Log_Error                   = Log_Channel_General | Log_MessageType_Error,
		Log_Warning                 = Log_Channel_General | Log_MessageType_Warning,
		Log_Message                 = Log_Channel_General | Log_MessageType_Message,
		Log_ScriptError             = Log_Channel_Script | Log_MessageType_Error,
		Log_ScriptWarning           = Log_Channel_Script | Log_MessageType_Warning,
		Log_ScriptMessage           = Log_Channel_Script | Log_MessageType_Message,
		Log_Parse                   = Log_Channel_Parse | 0,
		Log_ParseShape              = Log_Channel_Parse | 1,
		Log_ParseMorphShape         = Log_Channel_Parse | 2,
		Log_ParseAction             = Log_Channel_Parse | 3,
		Log_Action                  = Log_Channel_Action | 0

	};

	virtual void    LogMessageVarg(UInt32 messageType, const char* pfmt, va_list argList);
};

// 08
class TranslationTableItem
{
public:
	wchar_t	* key;
	wchar_t	* translation;

	TranslationTableItem(wchar_t * a_key, wchar_t * a_translation)
		: key(a_key), translation(a_translation) {}

	bool operator==(const TranslationTableItem & rhs) const	{ return key == rhs.key; }
	bool operator==(const wchar_t * a_key) const			{ return key == a_key; }
	operator const wchar_t *() const						{ return key; }

	static inline UInt32 GetHash(const wchar_t ** key)
	{
		UInt32 hash;
		CRC32_Calc4(&hash, (UInt32)*key);
		return hash;
	}

	void Dump(void)
	{
		_MESSAGE("\t\tkey: %S ", key);
		_MESSAGE("\t\ttranslation: %S", translation);
	}
};

typedef tHashSet<TranslationTableItem, const wchar_t *> TranslationTable;

// These are incomplete and only decoded to get the translation table yet.
class BSScaleformTranslator
{
	UInt32		unk_000;
	UInt32		unk_004;
	UInt32		unk_008;
	UInt32		unk_00C;
	UInt32		unk_010;

public:
	TranslationTable	translations;	// 014

	typedef void (__cdecl * _GetCachedString)(wchar_t ** pOut, wchar_t * bufIn);
	static const _GetCachedString GetCachedString;
};

class GFxStateBag
{
protected:
	virtual GFxStateBag *	GetStateBagImpl(void);

public:
	virtual				~GFxStateBag();
	virtual void		SetState(UInt32 type, void * ptr);
	virtual void *		GetStateAddRef(UInt32 type);
	//more

	inline BSScaleformTranslator *		GetTranslator()		{ return (BSScaleformTranslator*)GetStateAddRef(GFxState::kInterface_Translator); }
};
