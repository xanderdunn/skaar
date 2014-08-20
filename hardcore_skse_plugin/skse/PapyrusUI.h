#pragma once

#include "GameMenus.h"
#include "ScaleformCallbacks.h"
#include "ScaleformMovie.h"
#include "Hooks_UI.h"

struct StaticFunctionTag;
class VMClassRegistry;
template <typename T> class VMArray;


namespace papyrusUI
{
	template <typename T> void SetGFxValue(GFxValue * val, T arg);
	template <> void SetGFxValue<bool> (GFxValue * val, bool arg);
	template <> void SetGFxValue<float> (GFxValue * val, float arg);
	template <> void SetGFxValue<UInt32> (GFxValue * val, UInt32 arg);
	template <> void SetGFxValue<SInt32> (GFxValue * val, SInt32 arg);
	template <> void SetGFxValue<BSFixedString> (GFxValue * val, BSFixedString arg);

	template <typename T> T GetGFxValue(GFxValue * val);
	template <> bool GetGFxValue<bool> (GFxValue * val);
	template <> float GetGFxValue<float> (GFxValue * val);
	template <> UInt32 GetGFxValue<UInt32> (GFxValue * val);
	template <> SInt32 GetGFxValue<SInt32> (GFxValue * val);
	template <> BSFixedString GetGFxValue<BSFixedString> (GFxValue * val);

	void RegisterFuncs(VMClassRegistry* registry);

	class UIInvokeDelegate : public UIDelegate
	{
	private:
		std::string		menuName;
		std::string		target;

	public:
		static UIInvokeDelegate * Create(const char * nameBuf, const char * targetBuf);

		UIInvokeDelegate()	: argCount(0) {};

		virtual void	Run(void);
		virtual void	Dispose(void);

		UInt32			argCount;
		GFxValue		args[128];
	};

	class UIInvokeFormDelegate : public UIDelegate
	{
	private:
		std::string		menuName;
		std::string		target;

	public:
		static UIInvokeFormDelegate * Create(const char * nameBuf, const char * targetBuf);

		UIInvokeFormDelegate()	: type(0), handle(0) {};

		virtual void	Run(void);
		virtual void	Dispose(void);

		UInt32			type;
		UInt64			handle;
	};
	
	template <typename T>
	void SetT(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr, T value)
	{
		if (!menuName.data || !targetStr.data)
			return;

		MenuManager * mm = MenuManager::GetSingleton();
		if (!mm)
			return;

		GFxMovieView * view = mm->GetMovieView(&menuName);
		if (!view)
			return;

		GFxValue fxValue;
		SetGFxValue<T>(&fxValue, value);

		view->SetVariable(targetStr.data, &fxValue, 1);
	}

	template <typename T>
	T GetT(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString sourceStr)
	{
		if (!menuName.data || !sourceStr.data)
			return 0;

		MenuManager * mm = MenuManager::GetSingleton();
		if (!mm)
			return 0;

		GFxMovieView * view = mm->GetMovieView(&menuName);
		if (!view)
			return 0;

		GFxValue fxResult;
		if (! view->GetVariable(&fxResult, sourceStr.data))
			return 0;

		return GetGFxValue<T>(&fxResult);
	}

	template <typename T>
	void InvokeArgT(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr, T arg)
	{
		if (!menuName.data || !targetStr.data)
			return;

		UIManager * uiManager = UIManager::GetSingleton();
		if (!uiManager)
			return;

		UIInvokeDelegate * cmd = UIInvokeDelegate::Create(menuName.data, targetStr.data);
		if (!cmd)
		{
			_MESSAGE("Failed to allocate UIInvokeDelegate, skipping invoke");
			return;
		}

		cmd->argCount = 1;
		SetGFxValue<T>(&cmd->args[0], arg);

		uiManager->QueueCommand(cmd);
	}

	template <typename T>
	void InvokeArrayT(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr, VMArray<T> args)
	{
		if (!menuName.data || !targetStr.data)
			return;

		UIManager * uiManager = UIManager::GetSingleton();
		if (!uiManager)
			return;

		UInt32 argCount = args.Length();

		UIInvokeDelegate * cmd = UIInvokeDelegate::Create(menuName.data, targetStr.data);
		if (!cmd)
		{
			_MESSAGE("Failed to allocate UIInvokeDelegate, skipping invoke");
			return;
		}
		
		cmd->argCount = argCount;
		for (UInt32 i=0; i<argCount; i++)
		{
			T arg;
			args.Get(&arg, i);
			SetGFxValue<T>(&cmd->args[i], arg);
		}

		uiManager->QueueCommand(cmd);
	}

	bool IsMenuOpen(StaticFunctionTag* thisInput, BSFixedString menuName);

	void InvokeForm(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr, TESForm * form);

	void OpenCustomMenu(StaticFunctionTag* thisInput, BSFixedString swfPath, SInt32 flags);
	void CloseCustomMenu(StaticFunctionTag* thisInput);
}