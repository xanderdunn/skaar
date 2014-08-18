#include "PapyrusArgs.h"
#include "PapyrusNativeFunctions.h"
#include "PapyrusVM.h"
#include "GameForms.h"

//// type -> VMValue

template <> void PackValue <void>(VMValue * dst, void * src, VMClassRegistry * registry)
{
	dst->SetNone();
}

template <> void PackValue <UInt32>(VMValue * dst, UInt32 * src, VMClassRegistry * registry)
{
	dst->SetInt(*src);
}

template <> void PackValue <SInt32>(VMValue * dst, SInt32 * src, VMClassRegistry * registry)
{
	dst->SetInt(*src);
}

template <> void PackValue <float>(VMValue * dst, float * src, VMClassRegistry * registry)
{
	dst->SetFloat(*src);
}

template <> void PackValue <bool>(VMValue * dst, bool * src, VMClassRegistry * registry)
{
	dst->SetBool(*src);
}

template <> void PackValue <BSFixedString>(VMValue * dst, BSFixedString * src, VMClassRegistry * registry)
{
	dst->SetString(src->data);
}

void BindID(VMIdentifier ** identifier, void * srcData, VMClassRegistry * registry, IObjectHandlePolicy * handlePolicy, UInt32 typeID)
{
	UInt32	unk = 0;

	VMClassInfo	* classInfo = (*identifier)->m_type;
	if(classInfo)
		classInfo->AddRef();

	if(registry->Unk_0D(&classInfo->name, &unk))
	{
		UInt64	handle = handlePolicy->Create(typeID, srcData);

		if(	handlePolicy->IsType(unk, handle) ||
			(handle == handlePolicy->GetInvalidHandle()))
		{
			CALL_MEMBER_FN(registry->GetObjectBindPolicy(), BindObject)(identifier, handle);
		}
	}

	if(classInfo)
		classInfo->Release();
}

void PackHandle(VMValue * dst, void * src, UInt32 typeID, VMClassRegistry * registry)
{
	dst->SetNone();

	if(!src) return;

	VMClassInfo	* classInfo = NULL;

	// get class info
	if(registry->GetFormClass(typeID, &classInfo))
		if(classInfo)
			classInfo->Release();

	if(!classInfo) return;

	IObjectHandlePolicy	* handlePolicy = registry->GetHandlePolicy();

	UInt64			handle = handlePolicy->Create(typeID, src);
	VMIdentifier	* identifier = NULL;

	// find existing identifier
	if(!registry->Unk_1A(handle, classInfo->name.data, &identifier))
	{
		if(registry->Unk_13(&classInfo->name, &identifier))
		{
			if(identifier)
			{
				BindID(&identifier, src, registry, handlePolicy, typeID);
			}
		}
	}

	// copy the identifier out
	if(identifier)
	{
		VMValue	tempValue;

		tempValue.SetIdentifier(classInfo);

		CALL_MEMBER_FN(dst, Set)(&tempValue);
		dst->SetIdentifier(&identifier);
	}

	// release our reference
	if(identifier)
	{
		if(!identifier->DecrementLock())
		{
			identifier->Destroy();
		}
	}
}

//// VMValue -> type

template <> void UnpackValue <float>(float * dst, VMValue * src)
{
	switch(src->type)
	{
	case VMValue::kType_Int:
		*dst = src->data.i;
		break;

	case VMValue::kType_Float:
		*dst = src->data.f;
		break;

	case VMValue::kType_Bool:
		*dst = src->data.b;
		break;

	default:
		*dst = 0;
		break;
	}
}

template <> void UnpackValue <UInt32>(UInt32 * dst, VMValue * src)
{
	switch(src->type)
	{
	case VMValue::kType_Int:
		*dst = src->data.u;
		break;

	case VMValue::kType_Float:
		*dst = src->data.f;
		break;

	case VMValue::kType_Bool:
		*dst = src->data.b;
		break;

	default:
		*dst = 0;
		break;
	}
}

template <> void UnpackValue <SInt32>(SInt32 * dst, VMValue * src)
{
	switch(src->type)
	{
	case VMValue::kType_Int:
		*dst = src->data.u;
		break;

	case VMValue::kType_Float:
		*dst = src->data.f;
		break;

	case VMValue::kType_Bool:
		*dst = src->data.b;
		break;

	default:
		*dst = 0;
		break;
	}
}

template <> void UnpackValue <bool>(bool * dst, VMValue * src)
{
	switch(src->type)
	{
	case VMValue::kType_Int:
		*dst = src->data.u != 0;
		break;

	case VMValue::kType_Float:
		*dst = src->data.f != 0;
		break;

	case VMValue::kType_Bool:
		*dst = src->data.b;
		break;

	default:
		*dst = 0;
		break;
	}
}

template <> void UnpackValue <BSFixedString>(BSFixedString * dst, VMValue * src)
{
	const char	* data = NULL;

	if(src->type == VMValue::kType_String)
		data = src->data.str;

	CALL_MEMBER_FN(dst, Set)(data);
}

template <> void UnpackValue <VMArray<UInt32>>(VMArray<UInt32> * dst, VMValue * src)
{
	UnpackArray(dst, src, VMValue::kType_IntArray);
}

template <> void UnpackValue <VMArray<SInt32>>(VMArray<SInt32> * dst, VMValue * src)
{
	UnpackArray(dst, src, VMValue::kType_IntArray);
}

template <> void UnpackValue <VMArray<float>>(VMArray<float> * dst, VMValue * src)
{
	UnpackArray(dst, src, VMValue::kType_FloatArray);
}

template <> void UnpackValue <VMArray<bool>>(VMArray<bool> * dst, VMValue * src)
{
	UnpackArray(dst, src, VMValue::kType_BoolArray);
}

template <> void UnpackValue <VMArray<BSFixedString>>(VMArray<BSFixedString> * dst, VMValue * src)
{
	UnpackArray(dst, src, VMValue::kType_StringArray);
}

// This should be templated
template <> void UnpackValue(VMArray<EffectSetting*> * dst, VMValue * src)
{
	UnpackArray(dst, src, GetTypeIDFromFormTypeID(EffectSetting::kTypeID, (*g_skyrimVM)->GetClassRegistry()) | VMValue::kType_Identifier);
}

void * UnpackHandle(VMValue * src, UInt32 typeID)
{
	if(!src->IsIdentifier()) return NULL;
	if(!src->data.id) return NULL;

	UInt64	handle = src->data.id->GetHandle();

	if(!(*g_objectHandlePolicy)->IsType(typeID, handle)) return NULL;
	if(!(*g_objectHandlePolicy)->Unk_02(handle)) return NULL;

	return (*g_objectHandlePolicy)->Resolve(typeID, handle);
}

//// type -> type ID

template <> UInt32 GetTypeID <void>(VMClassRegistry * registry)						{ return VMValue::kType_None; }
template <> UInt32 GetTypeID <UInt32>(VMClassRegistry * registry)					{ return VMValue::kType_Int; }
template <> UInt32 GetTypeID <SInt32>(VMClassRegistry * registry)					{ return VMValue::kType_Int; }
template <> UInt32 GetTypeID <int>(VMClassRegistry * registry)						{ return VMValue::kType_Int; }
template <> UInt32 GetTypeID <float>(VMClassRegistry * registry)					{ return VMValue::kType_Float; }
template <> UInt32 GetTypeID <bool>(VMClassRegistry * registry)						{ return VMValue::kType_Bool; }
template <> UInt32 GetTypeID <BSFixedString>(VMClassRegistry * registry)			{ return VMValue::kType_String; }

template <> UInt32 GetTypeID <VMArray<UInt32>>(VMClassRegistry * registry)			{ return VMValue::kType_IntArray; }
template <> UInt32 GetTypeID <VMArray<SInt32>>(VMClassRegistry * registry)			{ return VMValue::kType_IntArray; }
template <> UInt32 GetTypeID <VMArray<int>>(VMClassRegistry * registry)				{ return VMValue::kType_IntArray; }
template <> UInt32 GetTypeID <VMArray<float>>(VMClassRegistry * registry)			{ return VMValue::kType_FloatArray; }
template <> UInt32 GetTypeID <VMArray<bool>>(VMClassRegistry * registry)			{ return VMValue::kType_BoolArray; }
template <> UInt32 GetTypeID <VMArray<BSFixedString>>(VMClassRegistry * registry)	{ return VMValue::kType_StringArray; }

UInt32 GetTypeIDFromFormTypeID(UInt32 formTypeID, VMClassRegistry * registry)
{
	UInt32		result = 0;
	VMClassInfo	* info = NULL;

	if(registry->GetFormClass(formTypeID, &info))
	{
		if(info)
		{
			result = (UInt32)info;

			info->Release();	// yes, really
		}
	}

	return result;
}

#ifdef _NATIVEDUMP
const char * GetTypeNameFromFormTypeID(UInt32 formTypeID, VMClassRegistry * registry)
{
	VMClassInfo * classInfo = NULL;
	if(registry->GetFormClass(formTypeID, &classInfo))
	{
		return classInfo->name.data;
	}

	return "";
}

template <> const char * GetArgumentTypeName <void>(VMClassRegistry * registry){return "void";}
template <> const char * GetArgumentTypeName <UInt32>(VMClassRegistry * registry){return "int";}
template <> const char * GetArgumentTypeName <SInt32>(VMClassRegistry * registry){return "int";}
template <> const char * GetArgumentTypeName <int>(VMClassRegistry * registry){return "int";}
template <> const char * GetArgumentTypeName <float>(VMClassRegistry * registry){return "float";}
template <> const char * GetArgumentTypeName <bool>(VMClassRegistry * registry){return "bool";}
template <> const char * GetArgumentTypeName <BSFixedString>(VMClassRegistry * registry){return "string";}
template <> const char * GetArgumentTypeName <VMArray<UInt32>>(VMClassRegistry * registry){return "int[]";}
template <> const char * GetArgumentTypeName <VMArray<SInt32>>(VMClassRegistry * registry){return "int[]";}
template <> const char * GetArgumentTypeName <VMArray<int>>(VMClassRegistry * registry){return "int[]";}
template <> const char * GetArgumentTypeName <VMArray<float>>(VMClassRegistry * registry){return "float[]";}
template <> const char * GetArgumentTypeName <VMArray<bool>>(VMClassRegistry * registry){return "bool[]";}
template <> const char * GetArgumentTypeName <VMArray<BSFixedString>>(VMClassRegistry * registry){return "string[]";}
#endif