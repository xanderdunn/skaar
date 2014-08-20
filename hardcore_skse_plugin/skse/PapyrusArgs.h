#pragma once

#include "skse/Utilities.h"
#include "skse/PapyrusVM.h"

class VMState;
class VMValue;
class VMClassRegistry;
struct StaticFunctionTag;
class EffectSetting;

class VMArgList
{
public:
	VMArgList();
	~VMArgList();

	MEMBER_FN_PREFIX(VMArgList);
	DEFINE_MEMBER_FN(GetOffset, UInt32, 0x00C3A620, VMState * state);
	DEFINE_MEMBER_FN(Get, VMValue *, 0x00C3AC40, VMState * state, UInt32 idx, UInt32 offset);
};


template <typename T>
class VMArray
{
public:
	VMValue::ArrayData	* arr;
	UInt32 Length() const				{ return arr ? arr->len : 0; }
	void Get(T * dst, const UInt32 idx)	{ UnpackValue(dst, arr->GetData()+idx); }
	void Set(T * src, const UInt32 idx)
	{
		VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
		PackValue(arr->GetData()+idx, src, registry);
	}
};

template <typename T>
void PackValue(VMValue * dst, T * src, VMClassRegistry * registry);

template <typename T>
void UnpackValue(T * dst, VMValue * src);

template <typename T>
UInt32 GetTypeID(VMClassRegistry * registry);

template <> void PackValue <void>(VMValue * dst, void * src, VMClassRegistry * registry);
template <> void PackValue <UInt32>(VMValue * dst, UInt32 * src, VMClassRegistry * registry);
template <> void PackValue <SInt32>(VMValue * dst, SInt32 * src, VMClassRegistry * registry);
template <> void PackValue <float>(VMValue * dst, float * src, VMClassRegistry * registry);
template <> void PackValue <bool>(VMValue * dst, bool * src, VMClassRegistry * registry);
template <> void PackValue <BSFixedString>(VMValue * dst, BSFixedString * src, VMClassRegistry * registry);

void PackHandle(VMValue * dst, void * src, UInt32 typeID, VMClassRegistry * registry);

template <typename T>
void PackValue(VMValue * dst, T ** src, VMClassRegistry * registry)
{
	typedef remove_pointer <T>::type	BaseType;
	PackHandle(dst, *src, BaseType::kTypeID, registry);
}

template <> void UnpackValue <float>(float * dst, VMValue * src);
template <> void UnpackValue <UInt32>(UInt32 * dst, VMValue * src);
template <> void UnpackValue <SInt32>(SInt32 * dst, VMValue * src);
template <> void UnpackValue <bool>(bool * dst, VMValue * src);
template <> void UnpackValue <BSFixedString>(BSFixedString * dst, VMValue * src);

template <> void UnpackValue <VMArray<float>>(VMArray<float> * dst, VMValue * src);
template <> void UnpackValue <VMArray<UInt32>>(VMArray<UInt32> * dst, VMValue * src);
template <> void UnpackValue <VMArray<SInt32>>(VMArray<SInt32> * dst, VMValue * src);
template <> void UnpackValue <VMArray<bool>>(VMArray<bool> * dst, VMValue * src);
template <> void UnpackValue <VMArray<BSFixedString>>(VMArray<BSFixedString> * dst, VMValue * src);

void * UnpackHandle(VMValue * src, UInt32 typeID);

template <typename T>
void UnpackValue(T ** dst, VMValue * src)
{
	*dst = (T *)UnpackHandle(src, T::kTypeID);
}

template <typename T>
void UnpackArray(VMArray<T> * dst, VMValue * src, const UInt32 type)
{
	VMValue::ArrayData * arrData;

	if (src->type != type || (arrData = src->data.arr, !arrData))
	{
		dst->arr = NULL;
		return;
	}

	dst->arr = arrData;
}

UInt32 GetTypeIDFromFormTypeID(UInt32 formTypeID, VMClassRegistry * registry);

template <> UInt32 GetTypeID <void>(VMClassRegistry * registry);
template <> UInt32 GetTypeID <UInt32>(VMClassRegistry * registry);
template <> UInt32 GetTypeID <SInt32>(VMClassRegistry * registry);
template <> UInt32 GetTypeID <int>(VMClassRegistry * registry);
template <> UInt32 GetTypeID <float>(VMClassRegistry * registry);
template <> UInt32 GetTypeID <bool>(VMClassRegistry * registry);
template <> UInt32 GetTypeID <BSFixedString>(VMClassRegistry * registry);

template <> UInt32 GetTypeID <VMArray<UInt32>>(VMClassRegistry * registry);
template <> UInt32 GetTypeID <VMArray<SInt32>>(VMClassRegistry * registry);
template <> UInt32 GetTypeID <VMArray<int>>(VMClassRegistry * registry);
template <> UInt32 GetTypeID <VMArray<float>>(VMClassRegistry * registry);
template <> UInt32 GetTypeID <VMArray<bool>>(VMClassRegistry * registry);
template <> UInt32 GetTypeID <VMArray<BSFixedString>>(VMClassRegistry * registry);

template<typename T>
struct IsArrayType
{
	enum { value = 0 };
	typedef T TypedArg;
};

template<typename T>
struct IsArrayType<VMArray<T*>>
{
	enum { value = 1 };
	typedef T TypedArg;
};

template <typename T>
UInt32 GetTypeID <T *>(VMClassRegistry * registry)
{
	UInt32		result;

	typedef remove_pointer <IsArrayType<T>::TypedArg>::type	BaseType;
	if(!IsArrayType<T>::value) {
		result = GetTypeIDFromFormTypeID(BaseType::kTypeID, registry);
	} else { // Arrays are ClassInfo ptr + 1
		result = GetTypeIDFromFormTypeID(BaseType::kTypeID, registry) | VMValue::kType_Identifier;
	}

	return result;
}

template <class T>
struct IsStaticType
{
	enum { value = 0 };
};

template <>
struct IsStaticType <StaticFunctionTag>
{
	enum { value = 1 };
};


#ifdef _NATIVEDUMP
template <typename T>
const char * GetArgumentTypeName(VMClassRegistry * registry);

template <> const char * GetArgumentTypeName <void>(VMClassRegistry * registry);
template <> const char * GetArgumentTypeName <UInt32>(VMClassRegistry * registry);
template <> const char * GetArgumentTypeName <SInt32>(VMClassRegistry * registry);
template <> const char * GetArgumentTypeName <int>(VMClassRegistry * registry);
template <> const char * GetArgumentTypeName <float>(VMClassRegistry * registry);
template <> const char * GetArgumentTypeName <bool>(VMClassRegistry * registry);
template <> const char * GetArgumentTypeName <BSFixedString>(VMClassRegistry * registry);
template <> const char * GetArgumentTypeName <VMArray<UInt32>>(VMClassRegistry * registry);
template <> const char * GetArgumentTypeName <VMArray<SInt32>>(VMClassRegistry * registry);
template <> const char * GetArgumentTypeName <VMArray<int>>(VMClassRegistry * registry);
template <> const char * GetArgumentTypeName <VMArray<float>>(VMClassRegistry * registry);
template <> const char * GetArgumentTypeName <VMArray<bool>>(VMClassRegistry * registry);
template <> const char * GetArgumentTypeName <VMArray<BSFixedString>>(VMClassRegistry * registry);

const char * GetTypeNameFromFormTypeID(UInt32 formTypeID, VMClassRegistry * registry);

template <typename T>
const char * GetArgumentTypeName <T *>(VMClassRegistry * registry)
{
	typedef remove_pointer <T>::type	BaseType;
	
	return GetTypeNameFromFormTypeID(BaseType::kTypeID, registry);
}
#endif