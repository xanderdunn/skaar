#include "PapyrusUI.h"
#include "PapyrusVM.h"
#include "ScaleformCallbacks.h"
#include "ScaleformMovie.h"
#include "ScaleformAPI.h"
#include "ScaleformExtendedData.h"
#include "GameMenus.h"
#include "GameEvents.h"
#include "GameForms.h"
#include "GameInput.h"
#include "common/IMemPool.h"
#include "CustomMenu.h"
#include <float.h>

namespace papyrusUI
{
	template <> void SetGFxValue<bool> (GFxValue * val, bool arg)						{ val->SetBool(arg); }
	template <> void SetGFxValue<float> (GFxValue * val, float arg)						{ val->SetNumber(arg); }
	template <> void SetGFxValue<UInt32> (GFxValue * val, UInt32 arg)					{ val->SetNumber(arg); }
	template <> void SetGFxValue<SInt32> (GFxValue * val, SInt32 arg)					{ val->SetNumber(arg); }
	template <> void SetGFxValue<BSFixedString> (GFxValue * val, BSFixedString arg)
	{
		// lifetime of this string will not be managed by the scaleform runtime
		BSFixedString s(arg.data);
		val->SetString(s.data);
	}

	template <> bool GetGFxValue<bool> (GFxValue * val)						{ return (val->GetType() == GFxValue::kType_Bool ? val->GetBool() : false); }
	template <> float GetGFxValue<float> (GFxValue * val)					{ return (val->GetType() == GFxValue::kType_Number ? val->GetNumber() : 0); }
	template <> UInt32 GetGFxValue<UInt32> (GFxValue * val)					{ return (val->GetType() == GFxValue::kType_Number ? (UInt32)val->GetNumber() : 0); }
	template <> SInt32 GetGFxValue<SInt32> (GFxValue * val)					{ return (val->GetType() == GFxValue::kType_Number ? (SInt32)val->GetNumber() : 0); }
	template <> BSFixedString GetGFxValue<BSFixedString> (GFxValue * val)
	{
		return (val->GetType() == GFxValue::kType_String ? BSFixedString(val->GetString()) : BSFixedString());
	}

	// Delegate object pools
	IThreadSafeBasicMemPool<UIInvokeDelegate,64>		s_invokeDelegatePool;
	IThreadSafeBasicMemPool<UIInvokeFormDelegate,64>	s_invokeFormDelegatePool;

	UIInvokeDelegate * UIInvokeDelegate::Create(const char * nameBuf, const char * targetBuf)
	{
		UIInvokeDelegate * cmd = s_invokeDelegatePool.Allocate();
		if (cmd)
		{
			cmd->menuName = nameBuf;
			cmd->target = targetBuf;
			cmd->argCount = 0;
		}
		return cmd;
	}

	void UIInvokeDelegate::Dispose(void)
	{
		s_invokeDelegatePool.Free(this);
	}

	void UIInvokeDelegate::Run(void)
	{
		MenuManager * mm = MenuManager::GetSingleton();
		if (!mm)
			return;

		BSFixedString t(menuName.c_str());
		GFxMovieView * view = mm->GetMovieView(&t);
		if (!view)
			return;

		view->Invoke(target.c_str(), NULL, args, argCount);
	}

	UIInvokeFormDelegate * UIInvokeFormDelegate::Create(const char * nameBuf, const char * targetBuf)
	{
		UIInvokeFormDelegate * cmd = s_invokeFormDelegatePool.Allocate();
		if (cmd)
		{
			cmd->menuName = nameBuf;
			cmd->target = targetBuf;
			cmd->handle = 0;
		}
		return cmd;
	}

	void UIInvokeFormDelegate::Dispose(void)
	{
		s_invokeFormDelegatePool.Free(this);
	}

	void UIInvokeFormDelegate::Run(void)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();
		
		TESForm * form = (TESForm*) policy->Resolve(type, handle);
		if (!form)
			return;

		MenuManager * mm = MenuManager::GetSingleton();
		if (!mm)
			return;

		BSFixedString t(menuName.c_str());
		GFxMovieView * view = mm->GetMovieView(&t);
		if (!view)
			return;

		// Precision problems here while in release mode and this isn't wrapped with controlfp
		unsigned int newValue;
		unsigned int currentState;
		_controlfp_s(&newValue, 0, 0);
		_controlfp_s(&currentState, _PC_53, _MCW_PC);

		GFxValue args;
		view->CreateObject(&args);
		scaleformExtend::FormData(&args, view, form, false, false);

		_controlfp_s(&currentState, newValue, _MCW_PC);

		view->Invoke(target.c_str(), NULL, &args, 1);
	}

	void InvokeForm(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr, TESForm * form)
	{
		if (!form || !menuName.data || !targetStr.data)
			return;

		UIManager * uiManager = UIManager::GetSingleton();
		if (!uiManager)
			return;

		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();
		
		UInt64 handle = policy->Create(form->formType, form);
		if (handle == policy->GetInvalidHandle())
			return;

		UIInvokeFormDelegate * cmd = UIInvokeFormDelegate::Create(menuName.data, targetStr.data);
		if (!cmd)
		{
			_MESSAGE("Failed to allocate UIInvokeFormDelegate, skipping invoke");
			return;
		}

		cmd->type = form->formType;
		cmd->handle = handle;
		uiManager->QueueCommand(cmd);
	}

	bool IsMenuOpen(StaticFunctionTag* thisInput, BSFixedString menuName)
	{
		if (!menuName.data)
			return 0;

		MenuManager * mm = MenuManager::GetSingleton();
		if (!mm)
			return false;

		return mm->IsMenuOpen(&menuName);
	}

	bool IsTextInputEnabled(StaticFunctionTag * thisInput)
	{
		InputManager	* inputManager = InputManager::GetSingleton();

		if(!inputManager) return false;

		return inputManager->allowTextInput != 0;
	}

	void OpenCustomMenu(StaticFunctionTag* thisInput, BSFixedString swfPath, SInt32 flags)
	{
		CustomMenuCreator::SetSwfPath(swfPath.data);

		BSFixedString s("CustomMenu");
		CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&s, UIMessage::kMessage_Open, NULL);
	}

	void CloseCustomMenu(StaticFunctionTag* thisInput)
	{
		BSFixedString s("CustomMenu");
		CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&s, UIMessage::kMessage_Close, NULL);
	}
};

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusUI::RegisterFuncs(VMClassRegistry* registry)
{
	MenuManager * mm = MenuManager::GetSingleton();
	if (mm)
		mm->Register("CustomMenu", CustomMenuCreator::Create);

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, bool> ("SetBool", "UI", papyrusUI::SetT<bool>, registry));
	
	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, UInt32> ("SetInt", "UI", papyrusUI::SetT<UInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, float> ("SetFloat", "UI", papyrusUI::SetT<float>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, BSFixedString> ("SetString", "UI", papyrusUI::SetT<BSFixedString>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, bool, BSFixedString, BSFixedString> ("GetBool", "UI", papyrusUI::GetT<bool>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, BSFixedString, BSFixedString> ("GetInt", "UI", papyrusUI::GetT<UInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, float, BSFixedString, BSFixedString> ("GetFloat", "UI", papyrusUI::GetT<float>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString> ("GetString", "UI", papyrusUI::GetT<BSFixedString>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, bool> ("InvokeBool", "UI", papyrusUI::InvokeArgT<bool>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, float> ("InvokeFloat", "UI", papyrusUI::InvokeArgT<float>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, UInt32> ("InvokeInt", "UI", papyrusUI::InvokeArgT<UInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, BSFixedString> ("InvokeString", "UI", papyrusUI::InvokeArgT<BSFixedString>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, VMArray<bool>> ("InvokeBoolA", "UI", papyrusUI::InvokeArrayT<bool>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, VMArray<UInt32>> ("InvokeIntA", "UI", papyrusUI::InvokeArrayT<UInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, VMArray<float>> ("InvokeFloatA", "UI", papyrusUI::InvokeArrayT<float>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, VMArray<BSFixedString>> ("InvokeStringA", "UI", papyrusUI::InvokeArrayT<BSFixedString>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, TESForm*> ("InvokeForm", "UI", papyrusUI::InvokeForm, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, BSFixedString> ("IsMenuOpen", "UI", papyrusUI::IsMenuOpen, registry));

	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, bool> ("IsTextInputEnabled", "UI", papyrusUI::IsTextInputEnabled, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, BSFixedString, SInt32> ("OpenCustomMenu", "UI", papyrusUI::OpenCustomMenu, registry));

	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void> ("CloseCustomMenu", "UI", papyrusUI::CloseCustomMenu, registry));

	registry->SetFunctionFlags("UI", "InvokeBool", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeInt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeFloat", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeString", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeBoolA", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeIntA", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeFloatA", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeStringA", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeForm", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "IsTextInputEnabled", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "OpenCustomMenu", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "CloseCustomMenu", VMClassRegistry::kFunctionFlag_NoWait);
}
