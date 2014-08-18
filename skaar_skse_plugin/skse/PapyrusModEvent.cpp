#include "PapyrusModEvent.h"

#include <vector>
#include "common/ICriticalSection.h"

#include "PapyrusEvents.h"
#include "PapyrusObjects.h"
#include "PapyrusArgs.h"

class PapyrusModEvent :
	public SKSEObject<kSKSEObjType_ModEvent>,
	public IFunctionArguments
{
private:
	struct Argument
	{
		enum
		{
			kType_None =		0,
			kType_Int =			1,
			kType_Float =		2,
			kType_Bool =		3,
			kType_String =		4,
			kType_Form =		5
		};

		Argument() : type(kType_None) {}

		explicit Argument(SInt32 arg) : type(kType_Int)				{ data.i = arg; }
		explicit Argument(float arg) : type(kType_Float)			{ data.f = arg; }
		explicit Argument(bool arg) : type(kType_Bool)				{ data.b = arg; }
		explicit Argument(BSFixedString arg) : type(kType_String)	{ data.str = arg.data; }
		explicit Argument(TESForm* arg) : type(kType_Form)			{ data.form = arg; }

		UInt32	type;

		union Data
		{
			SInt32		i;
			float		f;
			bool		b;
			const char*	str;
			TESForm*	form;
		}
		data;
	};

public:
	typedef std::vector<Argument>	ArgVectorT;

	PapyrusModEvent()
	{
	}

	void Init(BSFixedString name)
	{
		eventName_ = name;
	}

	virtual void Reset()
	{
		CALL_MEMBER_FN(&eventName_, Release)();
		args_.clear();
	}

	template <typename T>
	void PushArg(T arg)
	{
		args_.push_back(Argument(arg));
	}

	void operator() (const EventRegistration<ModCallbackParameters>& reg)
	{
		VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
		registry->QueueEvent(reg.handle, &reg.params.callbackName, this);
	}

	virtual bool Copy(Output * dst)
	{
		VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();

		UInt32 i = 0;
		//dst->Resize(1 + args_.size());
		dst->Resize(args_.size());

		//dst->Get(i++)->SetString(eventName_.data);

		for (ArgVectorT::iterator it = args_.begin(); it != args_.end(); ++it, ++i)
		{
			switch (it->type)
			{
			case Argument::kType_Int:
				dst->Get(i)->SetInt(it->data.i);
				break;
			case Argument::kType_Float:
				dst->Get(i)->SetFloat(it->data.f);
				break;
			case Argument::kType_Bool:
				dst->Get(i)->SetBool(it->data.b);
				break;
			case Argument::kType_String:
				dst->Get(i)->SetString(it->data.str);
				break;
			case Argument::kType_Form:
				PackValue(dst->Get(i), &it->data.form, registry);
				break;
			};
		}

		return true;
	}

	BSFixedString	EventName() const	{ return eventName_; }

private:
	BSFixedString			eventName_;
	std::vector<Argument>	args_;
};

namespace papyrusModEvent
{
	ICriticalSection					s_objectManagerLock;
	SKSEObjectManager<PapyrusModEvent>	s_objectManager;

	RawHandleT Create(StaticFunctionTag* thisInput, BSFixedString eventName)
	{
		if (!eventName.data)
			return SKSEObjectHandle::NullHandle;

		s_objectManagerLock.Enter();

		SKSEObjectHandle handleOut;
		PapyrusModEvent* obj = s_objectManager.CreateObject(handleOut);
		if (obj)
		{
			obj->Init(eventName);
		}

		s_objectManagerLock.Leave();

		return handleOut;
	}

	bool Send(StaticFunctionTag* thisInput, RawHandleT handle)
	{
		bool result = false;

		s_objectManagerLock.Enter();

		PapyrusModEvent* obj = s_objectManager.GetObject(handle);
		if (obj)
		{
			g_modCallbackRegs.ForEach(obj->EventName(), *obj);

			s_objectManager.FreeObject(handle);
			result = true;
		}

		s_objectManagerLock.Leave();

		return result;
	}

	void Release(StaticFunctionTag* thisInput, RawHandleT handle)
	{
		s_objectManagerLock.Enter();
		s_objectManager.FreeObject(handle);
		s_objectManagerLock.Leave();
	}

	template <typename T>
	void Push(StaticFunctionTag* thisInput, RawHandleT handle, T arg)
	{
		s_objectManagerLock.Enter();

		PapyrusModEvent* obj = s_objectManager.GetObject(handle);
		if (obj)
			obj->PushArg<T>(arg);

		s_objectManagerLock.Leave();
	}

	void RevertGlobalData()
	{
		s_objectManager.FreeAll();
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusModEvent::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, RawHandleT, BSFixedString> ("Create", "ModEvent", papyrusModEvent::Create, registry));
	
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, RawHandleT> ("Send", "ModEvent", papyrusModEvent::Send, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, RawHandleT> ("Release", "ModEvent", papyrusModEvent::Release, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, RawHandleT, bool> ("PushBool", "ModEvent", papyrusModEvent::Push<bool>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, RawHandleT, SInt32> ("PushInt", "ModEvent", papyrusModEvent::Push<SInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, RawHandleT, float> ("PushFloat", "ModEvent", papyrusModEvent::Push<float>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, RawHandleT, BSFixedString> ("PushString", "ModEvent", papyrusModEvent::Push<BSFixedString>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, RawHandleT, TESForm*> ("PushForm", "ModEvent", papyrusModEvent::Push<TESForm*>, registry));

	registry->SetFunctionFlags("ModEvent", "Create", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ModEvent", "Send", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ModEvent", "Release", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ModEvent", "PushBool", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ModEvent", "PushInt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ModEvent", "PushFloat", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ModEvent", "PushString", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ModEvent", "PushForm", VMClassRegistry::kFunctionFlag_NoWait);
}