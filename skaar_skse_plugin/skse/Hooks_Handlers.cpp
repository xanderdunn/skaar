#include "Hooks_Handlers.h"
#include "SafeWrite.h"

#include "GameRTTI.h"
#include "GameData.h"
#include "GameForms.h"
#include "GameObjects.h"
#include "GameHandlers.h"

#include "PapyrusEvents.h"

template<class T, UInt32 type, UInt32 slot>
UInt8 IHandlerFunctor<Actor, UInt32>::ProcessAction(Actor * actor, UInt32 unk04)
{
	TESForm * object = slot == SKSEActionEvent::kSlot_Voice ? actor->equippedShout : actor->GetEquippedObject(slot == SKSEActionEvent::kSlot_Left);
	SKSEActionEvent evn(type, actor, object, slot);
	g_actionEventDispatcher.SendEvent(&evn);
	return CALL_MEMBER_FN(static_cast<T*>(this), Process_Origin)(actor, unk04);
}

void Hooks_Handlers_Init(void)
{

}

void Hooks_Handlers_Commit(void)
{
	// vtbl overwrites
	SafeWrite32(0x010D5414 + 0x04, GetFnAddr(&IHandlerFunctor<Actor, UInt32>::ProcessAction<WeaponRightSwingHandler, SKSEActionEvent::kType_WeaponSwing, SKSEActionEvent::kSlot_Right>));
	SafeWrite32(0x010D5420 + 0x04, GetFnAddr(&IHandlerFunctor<Actor, UInt32>::ProcessAction<WeaponLeftSwingHandler, SKSEActionEvent::kType_WeaponSwing, SKSEActionEvent::kSlot_Left>));

	SafeWrite32(0x010D5468 + 0x04, GetFnAddr(&IHandlerFunctor<Actor, UInt32>::ProcessAction<RightHandSpellFireHandler, SKSEActionEvent::kType_SpellFire, SKSEActionEvent::kSlot_Right>));
	SafeWrite32(0x010D545C + 0x04, GetFnAddr(&IHandlerFunctor<Actor, UInt32>::ProcessAction<LeftHandSpellFireHandler, SKSEActionEvent::kType_SpellFire, SKSEActionEvent::kSlot_Left>));

	SafeWrite32(0x010D548C + 0x04, GetFnAddr(&IHandlerFunctor<Actor, UInt32>::ProcessAction<RightHandSpellCastHandler, SKSEActionEvent::kType_SpellCast, SKSEActionEvent::kSlot_Right>));
	SafeWrite32(0x010D5480 + 0x04, GetFnAddr(&IHandlerFunctor<Actor, UInt32>::ProcessAction<LeftHandSpellCastHandler, SKSEActionEvent::kType_SpellCast, SKSEActionEvent::kSlot_Left>));

	SafeWrite32(0x010D5474 + 0x04, GetFnAddr(&IHandlerFunctor<Actor, UInt32>::ProcessAction<VoiceSpellFireHandler, SKSEActionEvent::kType_VoiceFire, SKSEActionEvent::kSlot_Voice>));
	SafeWrite32(0x010D5498 + 0x04, GetFnAddr(&IHandlerFunctor<Actor, UInt32>::ProcessAction<VoiceSpellCastHandler, SKSEActionEvent::kType_VoiceCast, SKSEActionEvent::kSlot_Voice>));

	SafeWrite32(0x010D5630 + 0x04, GetFnAddr(&IHandlerFunctor<Actor, UInt32>::ProcessAction<BowDrawnHandler, SKSEActionEvent::kType_BowDraw, SKSEActionEvent::kSlot_Right>));
	SafeWrite32(0x010D563C + 0x04, GetFnAddr(&IHandlerFunctor<Actor, UInt32>::ProcessAction<BowReleaseHandler, SKSEActionEvent::kType_BowRelease, SKSEActionEvent::kSlot_Right>));

	SafeWrite32(0x010D54A4 + 0x04, GetFnAddr(&IHandlerFunctor<Actor, UInt32>::ProcessAction<WeaponBeginDrawRightHandler, SKSEActionEvent::kType_BeginDraw, SKSEActionEvent::kSlot_Right>));
	SafeWrite32(0x010D54B0 + 0x04, GetFnAddr(&IHandlerFunctor<Actor, UInt32>::ProcessAction<WeaponBeginSheatheRightHandler, SKSEActionEvent::kType_BeginSheathe, SKSEActionEvent::kSlot_Right>));

	SafeWrite32(0x010D54D4 + 0x04, GetFnAddr(&IHandlerFunctor<Actor, UInt32>::ProcessAction<RightHandWeaponDrawHandler, SKSEActionEvent::kType_EndDraw, SKSEActionEvent::kSlot_Right>));
	SafeWrite32(0x010D54E0 + 0x04, GetFnAddr(&IHandlerFunctor<Actor, UInt32>::ProcessAction<RightHandWeaponSheatheHandler, SKSEActionEvent::kType_EndSheathe, SKSEActionEvent::kSlot_Right>));
}
