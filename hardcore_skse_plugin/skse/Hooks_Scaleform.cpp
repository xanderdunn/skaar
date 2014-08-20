#include "Hooks_Scaleform.h"
#include "Hooks_Gameplay.h"
#include "SafeWrite.h"
#include "Utilities.h"
#include "ScaleformCallbacks.h"
#include "ScaleformMovie.h"
#include "ScaleformAPI.h"
#include "ScaleformExtendedData.h"
#include "ScaleformLoader.h"
#include "GameAPI.h"
#include "GameSettings.h"
#include "GameMenus.h"
#include "PluginManager.h"
#include "skse_version.h"
#include "GameForms.h"
#include "GameObjects.h"
#include "GameReferences.h"
#include "GameRTTI.h"
#include "GameData.h"
#include <new>
#include <list>
#include "PapyrusEvents.h"
#include "PapyrusVM.h"
#include "ScaleformState.h"
#include "Translation.h"
#include "GlobalLocks.h"
#include "Hooks_UI.h"
#include "common/IMemPool.h"
#include "HashUtil.h"

//// plugin API

struct ScaleformPluginInfo
{
	const char	* name;
	SKSEScaleformInterface::RegisterCallback	callback;
};

typedef std::list <ScaleformPluginInfo> PluginList;
static PluginList s_plugins;

bool RegisterScaleformPlugin(const char * name, SKSEScaleformInterface::RegisterCallback callback)
{
	// check for a name collision
	for(PluginList::iterator iter = s_plugins.begin(); iter != s_plugins.end(); ++iter)
	{
		if(!strcmp(iter->name, name))
		{
			_WARNING("scaleform plugin name collision: %s", iter->name);
			return false;
		}
	}

	ScaleformPluginInfo	info;

	info.name = name;
	info.callback = callback;

	s_plugins.push_back(info);

	return true;
}

//// commands

class SKSEScaleform_AllowTextInput : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		ASSERT(args->numArgs >= 1);
		
		bool		enable = args->args[0].GetBool();

		InputManager	* inputManager = InputManager::GetSingleton();
		if(inputManager)
		{
			UInt8	allowCount = inputManager->AllowTextInput(enable);
#if _DEBUG
			_MESSAGE("scaleform: AllowTextInput (counter = %d)", allowCount);
#endif
		}
		else
			_WARNING("input manager not initialized");
	}
};

class SKSEScaleform_GetMappedKey : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		ASSERT(args->numArgs >= 3);

		const char	* nameArg = args->args[0].GetString();
		BSFixedString name(nameArg);

		UInt32		deviceType = (UInt32)args->args[1].GetNumber();
		UInt32		contextIdx = (UInt32)args->args[2].GetNumber();
		
		args->result->SetNull();

		InputManager	* inputManager = InputManager::GetSingleton();
		if (!inputManager)
			return;

		UInt32 key = inputManager->GetMappedKey(name, deviceType, contextIdx);

		if (key == 0xFF)
		{
			args->result->SetNumber(-1);
			return;
		}

		if (deviceType == kDeviceType_Mouse)
		{
			args->result->SetNumber(key + InputMap::kMacro_MouseButtonOffset);
		}
		else if (deviceType == kDeviceType_Gamepad)
		{
			UInt32 mapped = InputMap::GamepadMaskToKeycode(key);
			args->result->SetNumber((mapped != InputMap::kMaxMacros ? mapped : -1));
		}
		else
		{
			args->result->SetNumber(key);
		}
	}
};

class SKSEScaleform_StartRemapMode : public GFxFunctionHandler
{
	class RemapHandler : public BSTEventSink<InputEvent>
	{
	public:
		virtual EventResult ReceiveEvent(InputEvent ** evns, InputEventDispatcher * dispatcher)
		{
			ButtonEvent * e = (ButtonEvent*) *evns;

			// Make sure this is really a button event
			if (!e || e->eventType != InputEvent::kEventType_Button)
				return kEvent_Continue;
				
			UInt32 deviceType = e->deviceType;

			if ((dispatcher->IsGamepadEnabled() ^ (deviceType == kDeviceType_Gamepad)) || e->flags == 0 || e->timer != 0.0)
				return kEvent_Continue;
			
			UInt32 keyMask = e->keyMask;
			UInt32 keyCode;

			// Mouse
			if (deviceType == kDeviceType_Mouse)
				keyCode = InputMap::kMacro_MouseButtonOffset + keyMask; 
			// Gamepad
			else if (deviceType == kDeviceType_Gamepad)
				keyCode = InputMap::GamepadMaskToKeycode(keyMask);
			// Keyboard
			else
				keyCode = keyMask;

			// Valid scancode?
			if (keyCode >= InputMap::kMaxMacros)
				keyCode = -1;

			GFxValue arg;
			arg.SetNumber(keyCode);
			scope.Invoke("EndRemapMode", NULL, &arg, 1);

			MenuControls::GetSingleton()->remapMode = false;
			PlayerControls::GetSingleton()->remapMode = false;

			dispatcher->RemoveEventSink(this);
			return kEvent_Continue;
		}

		GFxValue scope;
	};

	RemapHandler	remapHandler;

public:
	virtual void	Invoke(Args * args)
	{
		ASSERT(args->numArgs >= 1);

		remapHandler.scope = args->args[0];

		PlayerControls	* playerControls = PlayerControls::GetSingleton();
		if (!playerControls)
			return;

		MenuControls	* menuControls = MenuControls::GetSingleton();
		if (!menuControls)
			return;

		if (! (*g_inputEventDispatcher))
			return;
		
		(*g_inputEventDispatcher)->AddEventSink(&remapHandler);
		menuControls->remapMode = true;
		playerControls->remapMode = true;
	}
};

const char *	s_lastControlDown;
const char *	s_lastControlUp;
UInt32			s_lastKeycodeDown;
UInt32			s_lastKeycodeUp;

void SetLastControlDown(const char * control, UInt32 keycode)
{
	s_lastControlDown = control;
	s_lastKeycodeDown = keycode;
}

void SetLastControlUp(const char * control, UInt32 keycode)
{
	s_lastControlUp = control;
	s_lastKeycodeUp = keycode;
}

class SKSEScaleform_GetLastControl : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		ASSERT(args->numArgs >= 1);

		bool isKeyDown = args->args[0].GetBool();

		if (isKeyDown)
			args->result->SetString(s_lastControlDown);
		else
			args->result->SetString(s_lastControlUp);
	}
};

class SKSEScaleform_GetLastKeycode : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		ASSERT(args->numArgs >= 1);

		bool isKeyDown = args->args[0].GetBool();

		if (isKeyDown)
			args->result->SetNumber(s_lastKeycodeDown);
		else
			args->result->SetNumber(s_lastKeycodeUp);
	}
};


class SKSEScaleform_Log : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		ASSERT(args->numArgs >= 1);

		const char	* strData = args->args[0].GetString();

		_MESSAGE("%s", strData);
	}
};

class SKSEScaleform_SetINISetting : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		ASSERT(args->numArgs >= 2);

		const char	* settingName = args->args[0].GetString();
		double		settingValue = args->args[1].GetNumber();

#if _DEBUG
		_MESSAGE("scaleform: SetINISetting %s %f", settingName, settingValue);
#endif

		Setting	* setting = GetINISetting(settingName);

		if(setting)
		{
			if(!setting->SetDouble(settingValue))
			{
				_MESSAGE("tried to set a non-floating-point setting (%s)", settingName);
			}
		}
		else
		{
			_MESSAGE("setting not found (%s)", settingName);
		}
	}
};

class SKSEScaleform_GetINISetting : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		ASSERT(args->numArgs >= 1);

		const char	* settingName = args->args[0].GetString();

#if _DEBUG
		_MESSAGE("scaleform: GetINISetting %s", settingName);
#endif

		Setting	* setting = GetINISetting(settingName);

		args->result->SetNull();

		if(setting)
		{
			double	result;

			if(!setting->GetDouble(&result))
			{
				_MESSAGE("tried to get a non-floating-point setting (%s)", settingName);
			}
			else
			{
				args->result->SetNumber(result);
			}
		}
		else
		{
			_MESSAGE("setting not found (%s)", settingName);
		}
	}
};

class SKSEScaleform_OpenMenu : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		ASSERT(args->numArgs >= 1);

		const char	* menuName = args->args[0].GetString();

#if _DEBUG
		_MESSAGE("scaleform: open menu (%s)", menuName);
#endif

		StringCache::Ref	menuNameRef;

		CALL_MEMBER_FN(&menuNameRef, ctor)(menuName);
		CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&menuNameRef, UIMessage::kMessage_Open, NULL);
		CALL_MEMBER_FN(&menuNameRef, Release)();
	}
};

class SKSEScaleform_CloseMenu : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		ASSERT(args->numArgs >= 1);

		const char	* menuName = args->args[0].GetString();

#if _DEBUG
		_MESSAGE("scaleform: close menu (%s)", menuName);
#endif

		StringCache::Ref	menuNameRef;

		CALL_MEMBER_FN(&menuNameRef, ctor)(menuName);
		CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&menuNameRef, UIMessage::kMessage_Close, NULL);
		CALL_MEMBER_FN(&menuNameRef, Release)();
	}
};

static bool s_bExtendData = false;

class SKSEScaleform_ExtendData : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args* args)
	{
		ASSERT(args->numArgs >= 1);

		bool bExtend = args->args[0].GetBool();
		
		s_bExtendData = bExtend;
	}
};

class SKSEScaleform_ForceContainerCategorization : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args* args)
	{
		ASSERT(args->numArgs >= 1);

		bool enable = args->args[0].GetBool();

		Hooks_Gameplay_EnableForceContainerCategorization(enable);
	}
};

class SKSEScaleform_SendModEvent : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args* args)
	{
		if (! g_loadGameLock.TryEnter())
			return;

		ASSERT(args->numArgs >= 1);

		const char	* eventName = args->args[0].GetString();
		const char	* strArg = NULL;
		float		numArg = 0.0;
		UInt32		formidArg = 0;
		TESForm		* formArg = NULL;

		if (args->numArgs >= 2)
			strArg = args->args[1].GetString();

		if (args->numArgs >= 3)
			numArg = args->args[2].GetNumber();

		if (args->numArgs >= 4) {
			formidArg = (UInt32)args->args[3].GetNumber();
			if(formidArg > 0)
				formArg = LookupFormByID(formidArg);
		}

#if _DEBUG
		_MESSAGE("scaleform: send mod event (%s, %s, %d)", eventName, strArg, numArg);
#endif

		BSFixedString aEventName(eventName);
		BSFixedString aStrArg(strArg);
		SKSEModCallbackEvent evn(aEventName, aStrArg, numArg, formArg);
		g_modCallbackEventDispatcher.SendEvent(&evn);

		g_loadGameLock.Leave();
	}
};

class SKSEScaleform_RequestActivePlayerEffects : public GFxFunctionHandler
{
	class ActiveEffectVisitor
	{
		GFxMovieView	* movieView;
		GFxValue		* activeEffects;

	public:
		ActiveEffectVisitor(GFxMovieView * a_movieView, GFxValue * a_activeEffects)
			: movieView(a_movieView), activeEffects(a_activeEffects) {}

		bool Accept(ActiveEffect * e)
		{
			if (!e)
				return false;

			if (e->duration <= 0)
				return true;

			if (e->effect == NULL || e->effect->mgef == NULL)
				return true;

			EffectSetting * mgef = e->effect->mgef;

			// Skip effect if condition wasn't evaluated to true or HideInUI flag is set
			if (e->flags & ActiveEffect::kFlag_Inactive || mgef->properties.flags & EffectSetting::Properties::kEffectType_HideInUI)
				return true;
			
			GFxValue obj;
			movieView->CreateObject(&obj);

			RegisterNumber(&obj, "duration", e->duration);
			RegisterNumber(&obj, "elapsed", e->elapsed);
			RegisterNumber(&obj, "magnitude", e->magnitude);
			RegisterNumber(&obj, "effectFlags", mgef->properties.flags);
			RegisterNumber(&obj, "subType", mgef->school());
			RegisterNumber(&obj, "archetype", mgef->properties.archetype);
			RegisterNumber(&obj, "actorValue", mgef->properties.primaryValue);
			RegisterNumber(&obj, "magicType", mgef->properties.resistance);

			// Use pointer value to track effects
			RegisterNumber(&obj, "id", (UInt32)e);
			
			activeEffects->PushBack(&obj);

			return true;
		}
	};

public:
	virtual void	Invoke(Args* args)
	{
		if (! g_loadGameLock.TryEnter())
			return;

		ASSERT(args->numArgs >= 1);
		ASSERT(args->args[0].GetType() == GFxValue::kType_Array);

		tList<ActiveEffect> * effects = (*g_thePlayer)->magicTarget.GetActiveEffects();

		if(effects)
		{
			ActiveEffectVisitor v(args->movie, &args->args[0]);
			effects->Visit(v);
		}

		g_loadGameLock.Leave();
	}
};

class SKSEScaleform_ExtendForm : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args* args)
	{
		if (! g_loadGameLock.TryEnter())
			return;

		ASSERT(args->numArgs >= 1);
		ASSERT(args->args[0].GetType() == GFxValue::kType_Number);
		ASSERT(args->args[1].GetType() == GFxValue::kType_Object);

		UInt32		formidArg = 0;
		TESForm		* formArg = NULL;
		GFxValue	* object = NULL;
		bool		bExtra = false;
					// False - Pull only basic data
					// True - Pull all form data
		bool		bRecursive = false;
					// False - Pull only first level data
					// True - Pull all Form children

		if (args->numArgs >= 1) {
			formidArg = (UInt32)args->args[0].GetNumber();
			if(formidArg > 0)
				formArg = LookupFormByID(formidArg);
		}

		if (args->numArgs >= 2)
			object = &args->args[1];

		if (args->numArgs >= 3)
			bExtra = args->args[2].GetBool();

		if (args->numArgs >= 4)
			bRecursive = args->args[3].GetBool();

		if(formArg && object)
			scaleformExtend::FormData(object, args->movie, formArg, bExtra, bRecursive);

		g_loadGameLock.Leave();
	}
};

class SKSEScaleform_RequestActorValues : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		if (! g_loadGameLock.TryEnter())
			return;

		ASSERT(args->numArgs >= 1);
		ASSERT(args->args[0].GetType() == GFxValue::kType_Number);
		ASSERT(args->args[1].GetType() == GFxValue::kType_Array);
		ASSERT(args->args[2].GetType() == GFxValue::kType_Array);

		UInt32		formidArg = 0;
		UInt32		actorValue = 0;
		TESForm		* formArg;
		GFxValue	* inArray = NULL;
		GFxValue	* outArray = NULL;

		if (args->numArgs >= 1) {
			formidArg = (UInt32)args->args[0].GetNumber();
			if(formidArg > 0)
				formArg = LookupFormByID(formidArg);
		}

		if(!formArg) {
			_MESSAGE("SKSEScaleform_RequestActorValues - Invalid form (%X)", formidArg);
			g_loadGameLock.Leave();
			return;
		}

		Actor * actor = DYNAMIC_CAST(formArg, TESForm, Actor);
		if(!actor) {
			_MESSAGE("SKSEScaleform_RequestActorValues - Invalid form type (%X)", formidArg);
			g_loadGameLock.Leave();
			return;
		}

		if (args->numArgs >= 2) {
			inArray = &args->args[1];
		}
		if (args->numArgs >= 3) {
			outArray = &args->args[2];
		}

		GFxValue val;
		UInt32 size = inArray->GetArraySize();
		for(UInt32 i = 0; i < size; i++)
		{
			inArray->GetElement(i, &val);
			if(val.GetType() != GFxValue::kType_Number)
				continue;

			GFxValue avOut;
			args->movie->CreateObject(&avOut);

			UInt32 actorValue = val.GetNumber();
			RegisterNumber(&avOut, "id", actorValue);
			RegisterNumber(&avOut, "base", actor->actorValueOwner.GetBase(actorValue));
			RegisterNumber(&avOut, "current", actor->actorValueOwner.GetCurrent(actorValue));
			RegisterNumber(&avOut, "maximum", actor->actorValueOwner.GetMaximum(actorValue));

			outArray->PushBack(&avOut);
		}

		g_loadGameLock.Leave();
	}
};

class SKSEScaleform_GetClipboardData : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		if (! g_loadGameLock.TryEnter())
			return;

		if(OpenClipboard(NULL))
		{
			BOOL unicode = IsClipboardFormatAvailable(CF_UNICODETEXT);
			BOOL utf8 = IsClipboardFormatAvailable(CF_TEXT);
			if(unicode || utf8)
			{
				HANDLE	handle = GetClipboardData(unicode ? CF_UNICODETEXT : CF_TEXT);
				if(handle)
				{
					LPTSTR	textData = (LPTSTR)GlobalLock(handle);
					if(textData)
					{
						if(unicode)
							args->movie->CreateWideString(args->result, (const wchar_t*)textData);
						else
							args->movie->CreateString(args->result, (const char*)textData);

						GlobalUnlock(handle);
					}
				}
			}

			CloseClipboard();
		}
		
		g_loadGameLock.Leave();
	}
};

class SKSEScaleform_SetClipboardData : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		if(!g_loadGameLock.TryEnter())
			return;

		if(OpenClipboard(NULL))
		{
			const wchar_t	* textUnicode = args->args[0].GetWideString();
			const char		* textUtf8 = args->args[0].GetString();
			void			* textIn = NULL;

			UInt32			size = 0;
			if(textUnicode) {
				size = (wcslen(textUnicode) + 1) * sizeof(wchar_t);
				textIn = (void*)textUnicode;
			}
			else if(textUtf8) {
				size = strlen(textUtf8) + 1;
				textIn = (void*)textUtf8;
			}
			
			if(textIn && size > 0)
			{
				HANDLE  handle = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, size);
				if(handle)
				{
					LPTSTR  textData = (LPTSTR)GlobalLock(handle);
					if(textData)
					{
						memcpy(textData, textIn, size);
						GlobalUnlock(handle);

						EmptyClipboard();
						if(SetClipboardData(textUtf8 ? CF_TEXT : CF_UNICODETEXT, handle))
						{
							handle = NULL;  // ownership passed to the OS
						}
					}

					// clean up the allocation if something failed
					if(handle)
					{
						GlobalFree(handle);
					}
				}
			}

			CloseClipboard();
		}

		g_loadGameLock.Leave();
	}
};

class SKSEScaleform_GetPlayerSex : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		if(!g_loadGameLock.TryEnter())
			return;

		args->result->SetNull();

		TESNPC * actorBase = DYNAMIC_CAST((*g_thePlayer)->baseForm, TESForm, TESNPC);
		if(actorBase) {
			args->result->SetNumber(CALL_MEMBER_FN(actorBase, GetSex)());
		}

		g_loadGameLock.Leave();
	}
};

class SKSEScaleform_EnableMapMenuMouseWheel : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		if(!g_loadGameLock.TryEnter())
			return;

		ASSERT(args->numArgs >= 1);

		bool enable = args->args[0].GetBool();

		Hooks_Gameplay_EnableMapMenuMouseWheel(enable);

		g_loadGameLock.Leave();
	}
};

class SKSEScaleform_ShowOnMap : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		if(!g_loadGameLock.TryEnter())
			return;

		ASSERT(args->numArgs >= 1);

		UInt32			index = args->args[0].GetNumber();
		UIStringHolder	* stringHolder = UIStringHolder::GetSingleton();
		MenuManager		* mm = MenuManager::GetSingleton();

		if (index >= 0 && mm->IsMenuOpen(&stringHolder->mapMenu))
		{
			MapMenu * mapMenu = static_cast<MapMenu*>(mm->GetMenu(&stringHolder->mapMenu));

			if (mapMenu && index < mapMenu->markers.count)
			{
				MapMenu::MarkerData & marker = mapMenu->markers[index];
				UInt32 refHandle = marker.refHandle;

				if (refHandle)
				{
					RefHandleUIData	* msgData = (RefHandleUIData*) CreateUIMessageData(&stringHolder->refHandleUIData);

					if (msgData)
					{
						msgData->refHandle = refHandle;
						CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&stringHolder->mapMenu, UIMessage::kMessage_Refresh, msgData);
					}
				}
			}
		}

		g_loadGameLock.Leave();
	}
};

typedef std::list<SInt32>							IndexStorageList;
typedef std::map<std::string,IndexStorageList>		IndexStorageTable;

IndexStorageTable	s_indexTable;

class SKSEScaleform_StoreIndices : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		if(!g_loadGameLock.TryEnter())
			return;

		ASSERT(args->numArgs >= 2);
		ASSERT(args->args[0].GetType() == GFxValue::kType_String);
		ASSERT(args->args[1].GetType() == GFxValue::kType_Array);

		std::string key(args->args[0].GetString());
		GFxValue * indices = &args->args[1];

		s_indexTable[key].clear();

		for (UInt32 i=0; i<indices->GetArraySize(); i++)
		{
			GFxValue t;
			if (indices->GetElement(i, &t))
				if (t.GetType() == GFxValue::kType_Number)
					s_indexTable[key].push_back(t.GetNumber());
		}

		g_loadGameLock.Leave();
	}
};

class SKSEScaleform_LoadIndices : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		if(!g_loadGameLock.TryEnter())
			return;

		ASSERT(args->numArgs >= 2);
		ASSERT(args->args[0].GetType() == GFxValue::kType_String);
		ASSERT(args->args[1].GetType() == GFxValue::kType_Array);

		std::string key(args->args[0].GetString());
		GFxValue * indicesOut = &args->args[1];

		IndexStorageTable::iterator indices = s_indexTable.find(key);

		if (indices != s_indexTable.end())
		{
			for (IndexStorageList::iterator iter = indices->second.begin(); iter != indices->second.end(); ++iter)
			{
				GFxValue t;
				t.SetNumber(*iter);
				indicesOut->PushBack(&t);
			}
		}

		g_loadGameLock.Leave();
	}
};

class SKSEScaleform_GetModList : public GFxFunctionHandler
{
public:
	virtual void	Invoke(Args * args)
	{
		if(!g_loadGameLock.TryEnter())
			return;

		DataHandler* pDataHandler = DataHandler::GetSingleton();

		args->movie->CreateArray(args->result);

		for(UInt32 i = 0; i < pDataHandler->modList.modInfoList.Count(); i++)
		{
			ModInfo* modInfo = pDataHandler->modList.modInfoList.GetNthItem(i);
			if(modInfo)
			{
				GFxValue info;
				args->movie->CreateObject(&info);

				RegisterNumber(&info, "index", i);
				RegisterString(&info, args->movie, "name", modInfo->name);
				RegisterString(&info, args->movie, "author", modInfo->author.Get());
				RegisterString(&info, args->movie, "description", modInfo->description.Get());

				args->result->PushBack(&info);
			}
		}

		g_loadGameLock.Leave();
	}
};

/*
class GetScriptVariableFunctor : public IForEachScriptObjectFunctor
{
public:
	GetScriptVariableFunctor(VMClassRegistry * registry, UInt64 handle, BSFixedString var)
	{
		m_registry = registry;
		m_handle = handle;
		m_variable = var;
	}
	virtual ~GetScriptVariableFunctor(){}

	virtual bool Visit(VMScriptInstance * script, void * arg2)
	{
		UInt32 variableId = CALL_MEMBER_FN(script->classInfo, GetVariable)(&m_variable);
		if(variableId == -1) {
			return true;
		}
		if(m_registry->ExtractValue(m_handle, &script->classInfo->name, variableId, &m_result))
		{
			return false;
		}

		return true;
	}

	VMValue * getResult() { return m_result.type == VMValue::kType_None ? NULL : &m_result; }

private:
	VMClassRegistry	* m_registry;
	BSFixedString	m_variable;
	UInt64			m_handle;
	VMValue			m_result;
};

#include "PapyrusArgs.h"

class SKSEScaleform_GetVMVariable : public GFxFunctionHandler
{
	virtual void	Invoke(Args * args)
	{
		if(!g_loadGameLock.TryEnter())
			return;

		ASSERT(args->numArgs >= 1);
		ASSERT(args->args[0].GetType() == GFxValue::kType_Number);
		ASSERT(args->args[1].GetType() == GFxValue::kType_String);

		const char * varName = args->args[1].GetString();
		BSFixedString variableName(varName);

		UInt32		formId;
		TESForm		* form = NULL;


		if (args->numArgs >= 1) {
			formId = (UInt32)args->args[0].GetNumber();
			if(formId > 0)
				form = LookupFormByID(formId);
		}

		args->result->SetNull();

		if(form) {
			VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
			IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

			UInt64 handle = policy->Create(form->formType, form);
			if (handle != policy->GetInvalidHandle())
			{
				GetScriptVariableFunctor scriptVariable(registry, handle, variableName);
				registry->VisitScripts(handle, &scriptVariable);
				VMValue * retValue;
				if((retValue = scriptVariable.getResult()) != NULL)
				{
					if(retValue->IsIdentifier())
					{
						TESForm * retForm;
						UnpackValue<TESForm>(&retForm, retValue);
						if(retForm) {
							args->movie->CreateObject(args->result);
							scaleformExtend::FormData(args->result, args->movie, retForm, false, false);
						}
					}
					else
					{
						switch(retValue->type)
						{
						case VMValue::kType_Bool:
							{
								bool b;
								UnpackValue<bool>(&b, retValue);
								args->result->SetNumber(b);
							}
							break;
						case VMValue::kType_Int:
							{
								SInt32 i;
								UnpackValue<SInt32>(&i, retValue);
								args->result->SetNumber(i);
							}
							break;
						case VMValue::kType_Float:
							{
								float f;
								UnpackValue<float>(&f, retValue);
								args->result->SetNumber(f);
							}
							break;
						case VMValue::kType_String:
							{
								BSFixedString str;
								UnpackValue<BSFixedString>(&str, retValue);
								args->movie->CreateString(args->result, str.data);
							}
							break;
						case VMValue::kType_BoolArray:
							{
								VMArray<bool> bArray;
								UnpackValue<VMArray<bool>>(&bArray, retValue);
								args->movie->CreateArray(args->result);

								bool tmp;
								for(int i = 0; i < bArray.Length(); i++) {
									GFxValue gfxValue;
									bArray.Get(&tmp, i);
									gfxValue.SetNumber(tmp);
									args->result->PushBack(&gfxValue);
								}
							}
							break;
						case VMValue::kType_IntArray:
							{
								VMArray<SInt32> iArray;
								UnpackValue<VMArray<SInt32>>(&iArray, retValue);
								args->movie->CreateArray(args->result);

								SInt32 tmp;
								for(int i = 0; i < iArray.Length(); i++) {
									GFxValue gfxValue;
									iArray.Get(&tmp, i);
									gfxValue.SetNumber(tmp);
									args->result->PushBack(&gfxValue);
								}
							}
							break;
						case VMValue::kType_FloatArray:
							{
								VMArray<float> fArray;
								UnpackValue<VMArray<float>>(&fArray, retValue);
								args->movie->CreateArray(args->result);

								float tmp;
								for(int i = 0; i < fArray.Length(); i++) {
									GFxValue gfxValue;
									fArray.Get(&tmp, i);
									gfxValue.SetNumber(tmp);
									args->result->PushBack(&gfxValue);
								}
							}
							break;
						case VMValue::kType_StringArray:
							{
								VMArray<BSFixedString> sArray;
								UnpackValue<VMArray<BSFixedString>>(&sArray, retValue);
								args->movie->CreateArray(args->result);

								BSFixedString tmp;
								for(int i = 0; i < sArray.Length(); i++) {
									GFxValue gfxValue;
									sArray.Get(&tmp, i);
									args->movie->CreateString(&gfxValue, tmp.data);
									args->result->PushBack(&gfxValue);
								}
							}
							break;
						}
					}
				}
			}
		}

		g_loadGameLock.Leave();
	}
};*/

//// item card extensions

// 20
class StandardItemData
{
public:
	virtual ~StandardItemData();

	virtual const char *	GetName(void);
	virtual UInt32			GetCount(void);
	virtual UInt32			GetEquipState(void);
	virtual UInt32			GetFilterFlag(void);
	virtual UInt32			GetFavorite(void);
	virtual bool			GetEnabled(void);

//	void						** _vtbl;	// 00
	PlayerCharacter::ObjDesc	* objDesc;	// 04
	void						* unk08;	// 08
	UInt32						unk0C;		// 0C
	GFxValue					fxValue;	// 10

	MEMBER_FN_PREFIX(StandardItemData);
	DEFINE_MEMBER_FN(ctor_data, StandardItemData *, 0x00842140, GFxMovieView ** movieView, PlayerCharacter::ObjDesc * objDesc, int unk);

	enum { kCtorHookAddress = 0x008433B0 + 0x0049 };

	StandardItemData * ctor_Hook(GFxMovieView ** movieView, PlayerCharacter::ObjDesc * objDesc, int unk);
};

STATIC_ASSERT(sizeof(StandardItemData) == 0x20);

StandardItemData * StandardItemData::ctor_Hook(GFxMovieView ** movieView, PlayerCharacter::ObjDesc * objDesc, int unk)
{
	StandardItemData	* result = CALL_MEMBER_FN(this, ctor_data)(movieView, objDesc, unk);

//	_MESSAGE("StandardItemData hook");

	if(s_bExtendData)
	{
		scaleformExtend::CommonItemData(&result->fxValue, objDesc->form);
		scaleformExtend::StandardItemData(&result->fxValue, objDesc->form);
		scaleformExtend::InventoryData(&result->fxValue, *movieView, objDesc);
		// Calling this to set scrolls, potions, ingredients
		// as this function is called for inventory, barter, container
		scaleformExtend::MagicItemData(&result->fxValue, *movieView, objDesc->form, true, false);
	}

	return result;
}

// 20
class MagicItemData
{
public:
	virtual ~MagicItemData();

//	void			** _vtbl;	// 00
	UInt32			unk04;		// 04
	UInt16			unk08;		// 08
	UInt16			unk0A;		// 0A
	TESForm			* form;		// 0C
	GFxValue		fxValue;	// 10

	MEMBER_FN_PREFIX(MagicItemData);
	DEFINE_MEMBER_FN(ctor_data, MagicItemData *, 0x00873D70, GFxMovieView ** movieView, TESForm * pForm, int unk);

	enum { kCtorHookAddress = 0x008742E0 + 0x0049 };

	MagicItemData * ctor_Hook(GFxMovieView ** movieView, TESForm * pForm, int unk);
};

STATIC_ASSERT(sizeof(MagicItemData) == 0x20);

MagicItemData * MagicItemData::ctor_Hook(GFxMovieView ** movieView, TESForm * pForm, int unk)
{
	MagicItemData	* result = CALL_MEMBER_FN(this, ctor_data)(movieView, pForm, unk);

//	_MESSAGE("MagicItemData hook");

	if(s_bExtendData)
	{
		scaleformExtend::CommonItemData(&result->fxValue, pForm);
		scaleformExtend::MagicItemData(&result->fxValue, *movieView, pForm, true, false);
	}

	return result;
}

//// fav menu data

namespace favMenuDataHook
{
	// One hook for items (1) and magic (2), and a special case of magic if the player is a vampire lord (3).

	typedef const char * (__cdecl * _GetName)(TESForm * form);
	static const _GetName GetName = (_GetName)0x00452300;

	// Not used, for documenation only
	struct FavData
	{
		GFxMovieView	** movieView;	// 00
		UInt32			unk04;			// 04
		GFxValue		* entryList;	// 08
	};

	void __stdcall SetItemData(IMenu * menu, GFxValue * dataContainer, PlayerCharacter::ObjDesc * objDesc)
	{
		if (s_bExtendData)
		{
			GFxMovieView * movieView = menu->view;

			scaleformExtend::CommonItemData(dataContainer, objDesc->form);
			scaleformExtend::StandardItemData(dataContainer, objDesc->form);
			scaleformExtend::InventoryData(dataContainer, movieView, objDesc);
			scaleformExtend::MagicItemData(dataContainer, movieView, objDesc->form, true, false);

			// itemId to uniquely identify items
			const char* name = CALL_MEMBER_FN(objDesc, GenerateName)();
			SInt32 itemId = (SInt32)HashUtil::CRC32(name, objDesc->form->formID & 0x00FFFFFF);
			RegisterNumber(dataContainer, "itemId", itemId);
		}
	};

	void __stdcall SetMagicData(GFxMovieView * movieView, GFxValue * dataContainer, TESForm * form)
	{
		if (s_bExtendData)
		{
			scaleformExtend::CommonItemData(dataContainer, form);
			scaleformExtend::MagicItemData(dataContainer, movieView, form, true, false);

			// itemId to uniquely identify items
			TESFullName* pFullName = DYNAMIC_CAST(form, TESForm, TESFullName);
			const char * name;
			if (pFullName)
				name = pFullName->name.data;
			else
				name = NULL;
			SInt32 itemId = (SInt32)HashUtil::CRC32(name, form->formID & 0x00FFFFFF);
			RegisterNumber(dataContainer, "itemId", itemId);
		}
	};

	// 1 - Item
	static const UInt32 kSetItemData_Base = 0x0085C580;
	static const UInt32 kSetItemData_retn = kSetItemData_Base + 0x9F;

	enum
	{
		kSetItemData_EntryStackOffset	= 0x460,
		kSetItemData_VarContainerObj	= -0x430
	};

	__declspec(naked) void SetItemData_Entry(void)
	{
		__asm
		{
			// overwritten code
			call	eax				// movieView->CreateObject(containerObj,0,0,0)

			lea		eax, [esp + (kSetItemData_EntryStackOffset-0x10) + kSetItemData_VarContainerObj]

			// insert call to our data extend function
			pushad
			push	esi				// esi contains ObjDesc*
			push	eax
			push	ebp				// ebp contains IMenu* (this)
			call	SetItemData		// stdcall so we don't need to do work
			popad

			// inline the overwritten call of 0x7BFCC0 (return objDesc->count)
			mov		eax, [esi+8]

			jmp		[kSetItemData_retn]
		}
	};

	// 2 - Magic
	static const UInt32 kSetMagicData_Base = 0x0085C030;
	static const UInt32 kSetMagicData_retn = kSetMagicData_Base + 0x49;

	enum
	{
		kSetMagicData_EntryStackOffset	= 0x48,
		kSetMagicData_VarContainerObj	= -0x10
	};

	__declspec(naked) void SetMagicData_Entry(void)
	{
		__asm
		{
			// overwritten code
			call    edx				// movieView->CreateObject(containerObj,0,0,0)

			lea		eax, [esp + (kSetMagicData_EntryStackOffset-0x10) + kSetMagicData_VarContainerObj]

			// insert call to our data extend function
			pushad
			push	ebp				// ebp contains TESForm*
			push	eax
			mov		eax, [edi]		// edi contains FavData* (this)
			mov		eax, [eax]
			push	eax
			call	SetMagicData	// stdcall so we don't need to do work
			popad

			// overwritten code
			push	ebp
			call	GetName

			jmp		[kSetMagicData_retn]
		}
	};

	// 3 - VampireLord
	static const UInt32 kSetVampireData_Base = 0x0085C2E0;
	static const UInt32 kSetVampireData_retn = kSetVampireData_Base + 0x6C;

	enum
	{
		kSetVampireData_EntryStackOffset	= 0x48,
		kSetVampireData_VarContainerObj		= -0x10
	};

	__declspec(naked) void SetVampireData_Entry(void)
	{
		__asm
		{
			// overwritten code
			call    edx				// movieView->CreateObject(containerObj,0,0,0)

			lea		eax, [esp + (kSetVampireData_EntryStackOffset-0x10) + kSetVampireData_VarContainerObj]

			// insert call to our data extend function
			pushad
			push	esi				// esi contains TESForm*
			push	eax
			mov		eax, [edi]		// edi contains FavData* (this)
			mov		eax, [eax]
			push	eax
			call	SetMagicData	// stdcall so we don't need to do work
			popad

			// overwritten code
			push	esi
			call	GetName

			jmp		[kSetVampireData_retn]
		}
	};
}

//// core hook
void __stdcall InstallHooks(GFxMovieView * view)
{
	// called from a task, must be threadsafe

//	_MESSAGE("got new scaleform menu (view = %08X)", view);

	GFxValue	globals;

	bool	result = view->GetVariable(&globals, "_global");
	if(!result)
	{
		_ERROR("couldn't get _global");
		return;
	}

	GFxValue	skse;
	view->CreateObject(&skse);

	// root functions
	RegisterFunction <SKSEScaleform_AllowTextInput>(&skse, view, "AllowTextInput");
	RegisterFunction <SKSEScaleform_GetMappedKey>(&skse, view, "GetMappedKey");
	RegisterFunction <SKSEScaleform_StartRemapMode>(&skse, view, "StartRemapMode");
	RegisterFunction <SKSEScaleform_GetLastControl>(&skse, view, "GetLastControl");
	RegisterFunction <SKSEScaleform_GetLastKeycode>(&skse, view, "GetLastKeycode");
	RegisterFunction <SKSEScaleform_Log>(&skse, view, "Log");
	RegisterFunction <SKSEScaleform_SetINISetting>(&skse, view, "SetINISetting");
	RegisterFunction <SKSEScaleform_GetINISetting>(&skse, view, "GetINISetting");
	RegisterFunction <SKSEScaleform_OpenMenu>(&skse, view, "OpenMenu");
	RegisterFunction <SKSEScaleform_CloseMenu>(&skse, view, "CloseMenu");
	RegisterFunction <SKSEScaleform_ExtendData>(&skse, view, "ExtendData");
	RegisterFunction <SKSEScaleform_ForceContainerCategorization>(&skse, view, "ForceContainerCategorization");
	RegisterFunction <SKSEScaleform_SendModEvent>(&skse, view, "SendModEvent");
	RegisterFunction <SKSEScaleform_RequestActivePlayerEffects>(&skse, view, "RequestActivePlayerEffects");
	RegisterFunction <SKSEScaleform_ExtendForm>(&skse, view, "ExtendForm");
	RegisterFunction <SKSEScaleform_RequestActorValues>(&skse, view, "RequestActorValues");
	RegisterFunction <SKSEScaleform_GetClipboardData>(&skse, view, "GetClipboardData");
	RegisterFunction <SKSEScaleform_SetClipboardData>(&skse, view, "SetClipboardData");
	RegisterFunction <SKSEScaleform_GetPlayerSex>(&skse, view, "GetPlayerSex");
	//RegisterFunction <SKSEScaleform_GetVMVariable>(&skse, view, "GetVMVariable");
	RegisterFunction <SKSEScaleform_EnableMapMenuMouseWheel>(&skse, view, "EnableMapMenuMouseWheel");
	RegisterFunction <SKSEScaleform_ShowOnMap>(&skse, view, "ShowOnMap");
	RegisterFunction <SKSEScaleform_StoreIndices>(&skse, view, "StoreIndices");
	RegisterFunction <SKSEScaleform_LoadIndices>(&skse, view, "LoadIndices");
	RegisterFunction <SKSEScaleform_GetModList>(&skse, view, "GetModList");

	// version
	GFxValue	version;
	view->CreateObject(&version);

	RegisterNumber(&version, "major", SKSE_VERSION_INTEGER);
	RegisterNumber(&version, "minor", SKSE_VERSION_INTEGER_MINOR);
	RegisterNumber(&version, "beta", SKSE_VERSION_INTEGER_BETA);
	RegisterNumber(&version, "releaseIdx", SKSE_VERSION_RELEASEIDX);

	skse.SetMember("version", &version);

	// plugins
	GFxValue	plugins;
	view->CreateObject(&plugins);

	for(PluginList::iterator iter = s_plugins.begin(); iter != s_plugins.end(); ++iter)
	{
		GFxValue	plugin;
		view->CreateObject(&plugin);

		iter->callback(view, &plugin);

		plugins.SetMember(iter->name, &plugin);
	}

	skse.SetMember("plugins", &plugins);

	globals.SetMember("skse", &skse);
}

static const UInt32 kInstallHooks_Base = 0x00A60C80;
static const UInt32 kInstallHooks_Entry_retn = kInstallHooks_Base + 0xBE;

__declspec(naked) void InstallHooks_Entry(void)
{
	__asm
	{
		pushad
		push	esi				// esi contains GFxMovieView
		call	InstallHooks	// stdcall so we don't need to do work
		popad

		// overwritten code
		mov		edx, [edx + 0x6C]
		push	eax
		mov		ecx, esi

		jmp		[kInstallHooks_Entry_retn]
	}
}
/*
void * GFxAllocateHeap_Hook(HeapDesc * heap, ScaleformAllocator * allocator)
{
	heap->flags |= HeapDesc::kHeap_FastTinyBlocks | HeapDesc::kHeap_Root;
	heap->threshold = ~0;
	return GFxAllocateHeap(heap, allocator);
}
*/
void Hooks_Scaleform_Commit(void)
{

	UInt32	logScaleform = 0;
	if(GetConfigOption_UInt32("Interface", "bEnableGFXLog", &logScaleform))
	{
		if(logScaleform)
		{
			g_logScaleform = true;
		}
	}

	// movie creation hook
	WriteRelJump(kInstallHooks_Base + 0xB8, (UInt32)InstallHooks_Entry);

	// item card data creation hook
	WriteRelCall(StandardItemData::kCtorHookAddress, GetFnAddr(&StandardItemData::ctor_Hook));
	WriteRelCall(MagicItemData::kCtorHookAddress, GetFnAddr(&MagicItemData::ctor_Hook));
	
	// fav menu data hooks
	WriteRelJump(favMenuDataHook::kSetItemData_Base + 0x96, (UInt32)favMenuDataHook::SetItemData_Entry);
	WriteRelJump(favMenuDataHook::kSetMagicData_Base + 0x41, (UInt32)favMenuDataHook::SetMagicData_Entry);
	WriteRelJump(favMenuDataHook::kSetVampireData_Base + 0x64, (UInt32)favMenuDataHook::SetVampireData_Entry);

	// gfxloader creation hook
	WriteRelCall(GFxLoader::kCtorHookAddress, GetFnAddr(&GFxLoader::ctor_Hook));

	// GFx Heap Allocation hook
	//WriteRelCall(0x00A60FE0 + 0xB0, (UInt32)GFxAllocateHeap_Hook);
}
