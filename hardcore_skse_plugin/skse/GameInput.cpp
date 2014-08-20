#include "GameInput.h"

bool InputEventDispatcher::IsGamepadEnabled(void)
{
	return (gamepad != NULL) && gamepad->IsEnabled();
}

InputEventDispatcher ** g_inputEventDispatcher = (InputEventDispatcher **)0x01B2E724;

InputManager * InputManager::GetSingleton(void)
{
	return *((InputManager **)0x012E7458);
}

UInt8 InputManager::AllowTextInput(bool allow)
{
	if(allow)
	{
		if(allowTextInput == 0xFF)
			_WARNING("InputManager::AllowTextInput: counter overflow");
		else
			allowTextInput++;
	}
	else
	{
		if(allowTextInput == 0)
			_WARNING("InputManager::AllowTextInput: counter underflow");
		else
			allowTextInput--;
	}

	if(IsConsoleMode())
		Console_Print("%s text input, count = %d", allow ? "allowed" : "disallowed", allowTextInput);

	return allowTextInput;
}

UInt32 InputManager::GetMappedKey(BSFixedString name, UInt32 deviceType, UInt32 contextIdx)
{
	ASSERT(contextIdx < kContextCount);

	tArray<InputContext::Mapping> * mappings;
	if (deviceType == kDeviceType_Mouse)
		mappings = &context[contextIdx]->mouseMap;
	else if (deviceType == kDeviceType_Gamepad)
		mappings = &context[contextIdx]->gamepadMap;
	else
		mappings = &context[contextIdx]->keyboardMap;

	for (UInt32 i=0; i < mappings->count; i++)
	{
		InputContext::Mapping m;
		if (!mappings->GetNthItem(i, m))
			break;
		if (m.name == name)
			return m.buttonID;
	}

	// Unbound
	return 0xFF;
}

BSFixedString InputManager::GetMappedControl(UInt32 buttonID, UInt32 deviceType, UInt32 contextIdx)
{
	ASSERT(contextIdx < kContextCount);

	// 0xFF == unbound
	if (buttonID == 0xFF)
		return BSFixedString();

	tArray<InputContext::Mapping> * mappings;
	if (deviceType == kDeviceType_Mouse)
		mappings = &context[contextIdx]->mouseMap;
	else if (deviceType == kDeviceType_Gamepad)
		mappings = &context[contextIdx]->gamepadMap;
	else
		mappings = &context[contextIdx]->keyboardMap;

	for (UInt32 i=0; i < mappings->count; i++)
	{
		InputContext::Mapping m;
		if (!mappings->GetNthItem(i, m))
			break;
		if (m.buttonID == buttonID)
			return m.name;
	}

	return BSFixedString();
}

PlayerControls * PlayerControls::GetSingleton(void)
{
	return *((PlayerControls **)0x012E7454);
}

MenuControls * MenuControls::GetSingleton(void)
{
	return *((MenuControls **)0x01B10D5C);
}
