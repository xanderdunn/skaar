#include "GameSettings.h"
#include "GameAPI.h"

extern SettingCollectionList	** g_iniSettingCollection = (SettingCollectionList **)0x01B97F14;
extern SettingCollectionList	** g_iniPrefSettingCollection = (SettingCollectionList **)0x01B8FAF8;
extern SettingCollectionMap		** g_gameSettingCollection = (SettingCollectionMap **)0x012E7164;

const float	* g_mapLocalHeight = *(float **)0x0124166C;
const float	* g_localMapMinFrustumWidth = *(float **)0x012B5B80;

// sLanguage:General
// ### if you were using g_gameLanguage, just look it up in g_iniSettingCollection

UInt32 Setting::GetType(void) const
{
	if(!name || !name[0]) return kType_Unknown;

	switch(name[0])
	{
		case 'b':	return kType_Bool;
		case 'c':	return kType_Unknown;
		case 'h':	return kType_Unknown;
		case 'i':	return kType_Integer;
		case 'u':	return kType_Unknown;
		case 'f':	return kType_Float;
		case 'S':	return kType_String;	// dynamically allocated string
		case 's':	return kType_String;	// statically allocated string
		case 'r':	return kType_ID6;
		case 'a':	return kType_ID;
	}

	return kType_Unknown;
}

bool Setting::GetDouble(double * out) const
{
	switch(GetType())
	{
		case kType_Integer:	*out = data.s32; break;
		case kType_Float:	*out = data.f32; break;
		case kType_String:	return false;
		case kType_Bool:	*out = data.u8 ? 1 : 0; break;
		case kType_ID6:		*out = data.u32 >> 8; break;
		case kType_ID:		*out = data.u32; break;
		default:			return false;
		case kType_Unknown:	return false;
	}

	return true;
}

bool Setting::SetDouble(double value)
{
	switch(GetType())
	{
		case kType_Integer:	data.s32 = value; break;
		case kType_Float:	data.f32 = value; break;
		case kType_String:	return false;
		case kType_Bool:	data.u8 = value ? 1 : 0; break;
		case kType_ID6:		data.u32 = ((UInt32)value) << 8; break;
		case kType_ID:		data.u32 = value; break;
		default:			return false;
		case kType_Unknown:	return false;
	}

	return true;
}

char * FormHeap_Strdup(const char * src)
{
	UInt32	len = strlen(src) + 1;
	char	* result = (char *)FormHeap_Allocate(len);
	memcpy(result, src, len);

	return result;
}

bool Setting::SetString(const char * value)
{
	if(GetType() == kType_String)
	{
		if(name[0] == 'S')
			FormHeap_Free(data.s);

		data.s = FormHeap_Strdup(value);

		// mark string as dynamically allocated
		if(name[0] != 'S')
		{
			name = FormHeap_Strdup(name);
			name[0] = 'S';
		}

		return true;
	}
	else
	{
		return false;
	}
}

Setting	* SettingCollectionList::Get(const char * name)
{
	Setting	* result = NULL;

	if(!CALL_MEMBER_FN(this, Get_Internal)(name, &result))
		result = NULL;

	return result;
}

Setting * SettingCollectionMap::Get(const char * name)
{
	return items.Get(name);
}

Setting * GetINISetting(const char * name)
{
	Setting	* setting = (*g_iniSettingCollection)->Get(name);
	if(!setting)
		setting = (*g_iniPrefSettingCollection)->Get(name);

	return setting;
}
