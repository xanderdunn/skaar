#include "PapyrusUICallback.h"

#include "common/ICriticalSection.h"

#include "PapyrusArgs.h"
#include "PapyrusUI.h"
#include "PapyrusObjects.h"

class PapyrusUICallback : public SKSEObject<kSKSEObjType_UICallback>
{
public:
	PapyrusUICallback() :
		invokeDelegate_(NULL),
		hasRun_(false)
	{
	}

	virtual void Reset()
	{
		if (invokeDelegate_ && !hasRun_)
			invokeDelegate_->Dispose();

		invokeDelegate_ = NULL;
		hasRun_ = false;
	}

	bool Init(BSFixedString menuName, BSFixedString targetStr)
	{
		using namespace papyrusUI;

		UIInvokeDelegate * cmd = UIInvokeDelegate::Create(menuName.data, targetStr.data);
		if (!cmd)
		{
			_WARNING("Failed to allocate UIInvokeDelegate for PapyrusUICallback.");
			return false;
		}

		cmd->argCount = 0;

		invokeDelegate_ = cmd;

		return true;
	}

	template <typename T>
	void PushArg(T arg)
	{
		using namespace papyrusUI;

		if (invokeDelegate_->argCount >= 128)
		{
			_WARNING("Tried to push more than 128 arguments into PapyrusUICallback.");
			return;
		}

		SetGFxValue<T>(&invokeDelegate_->args[invokeDelegate_->argCount], arg);
		invokeDelegate_->argCount++;
	}

	template <typename T>
	void PushArgs(VMArray<T> args)
	{
		using namespace papyrusUI;

		UInt32 argCount = args.Length();

		UInt32 offset = invokeDelegate_->argCount;
		UInt32 newArgCount = offset + argCount;

		if (newArgCount > 128)
		{
			_WARNING("Tried to push more than 128 arguments into PapyrusUICallback.");
			return;
		}
		
		invokeDelegate_->argCount = newArgCount;
		for (UInt32 i=0; i<argCount; i++, offset++)
		{
			T arg;
			args.Get(&arg, i);
			SetGFxValue<T>(&invokeDelegate_->args[offset], arg);
		}
	}

	bool Send()
	{
		UIManager * uiManager = UIManager::GetSingleton();

		if (!invokeDelegate_ || hasRun_ || !uiManager)
		{
			_WARNING("Failed to run PapyrusUICallback.");
			return false;
		}

		// UI manager is now responsible for disposing
		uiManager->QueueCommand(invokeDelegate_);
		hasRun_ = true;

		return true;
	}

private:
	papyrusUI::UIInvokeDelegate*	invokeDelegate_;
	bool							hasRun_;
};

namespace papyrusUICallback
{
	ICriticalSection						s_objectManagerLock;
	SKSEObjectManager<PapyrusUICallback>	s_objectManager;

	RawHandleT Create(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr)
	{
		if (!menuName.data || !targetStr.data)
			return SKSEObjectHandle::NullHandle;

		s_objectManagerLock.Enter();

		SKSEObjectHandle handleOut;
		PapyrusUICallback* obj = s_objectManager.CreateObject(handleOut);

		if (!obj || !obj->Init(menuName, targetStr))
		{
			s_objectManager.FreeObject(handleOut);
			handleOut = SKSEObjectHandle::NullHandle;
		}

		s_objectManagerLock.Leave();

		return handleOut;
	}

	bool Send(StaticFunctionTag* thisInput, RawHandleT handle)
	{
		bool result = false;

		s_objectManagerLock.Enter();

		PapyrusUICallback* obj = s_objectManager.GetObject(handle);
		if (obj)
		{
			result = obj->Send();
			s_objectManager.FreeObject(handle);
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

		PapyrusUICallback* obj = s_objectManager.GetObject(handle);
		if (obj)
			obj->PushArg<T>(arg);

		s_objectManagerLock.Leave();
	}

	template <typename T>
	void PushArray(StaticFunctionTag* thisInput, RawHandleT handle, VMArray<T> args)
	{
		s_objectManagerLock.Enter();

		PapyrusUICallback* obj = s_objectManager.GetObject(handle);
		if (obj)
			obj->PushArgs<T>(args);

		s_objectManagerLock.Leave();
	}

	void RevertGlobalData()
	{
		s_objectManager.FreeAll();
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusUICallback::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, RawHandleT, BSFixedString, BSFixedString> ("Create", "UICallback", papyrusUICallback::Create, registry));
	
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, RawHandleT> ("Send", "UICallback", papyrusUICallback::Send, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, RawHandleT> ("Release", "UICallback", papyrusUICallback::Release, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, RawHandleT, bool> ("PushBool", "UICallback", papyrusUICallback::Push<bool>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, RawHandleT, SInt32> ("PushInt", "UICallback", papyrusUICallback::Push<SInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, RawHandleT, float> ("PushFloat", "UICallback", papyrusUICallback::Push<float>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, RawHandleT, BSFixedString> ("PushString", "UICallback", papyrusUICallback::Push<BSFixedString>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, RawHandleT, VMArray<bool>> ("PushBoolA", "UICallback", papyrusUICallback::PushArray<bool>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, RawHandleT, VMArray<SInt32>> ("PushIntA", "UICallback", papyrusUICallback::PushArray<SInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, RawHandleT, VMArray<float>> ("PushFloatA", "UICallback", papyrusUICallback::PushArray<float>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, RawHandleT, VMArray<BSFixedString>> ("PushStringA", "UICallback", papyrusUICallback::PushArray<BSFixedString>, registry));

	registry->SetFunctionFlags("UICallback", "Create", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "Send", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "Release", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushBool", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushInt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushFloat", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushString", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushBoolA", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushIntA", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushFloatA", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UICallback", "PushStringA", VMClassRegistry::kFunctionFlag_NoWait);
}