#include "skse/PapyrusNativeFunctions.h"
#include "skse/GameReferences.h"
#include "skse/GameExtraData.h"
#include "skse/PapyrusArgs.h"

namespace SkaarSoulTrap {

	// Decide which soul gem to fill and what soul gem should be removed from and added to the player
	void SkaarSoulTrap(StaticFunctionTag *base, Actor* casterRef, Actor* targetRef, bool isHuman, VMArray<TESForm*> returnValues);

	// Set the Display Name on a soul gem using the name of the soul that was captured in the gem
	void SkaarNameSoulGem(StaticFunctionTag *base, TESObjectREFR *soulGem, TESObjectREFR *capturedSoul);

	// Test if a given soul gem is filled.  Works only for non-Star soul gems (Does NOT work with Azura's Star and Black Star)
	bool SkaarSoulGemIsFilled(StaticFunctionTag *base, TESForm *soulGem);

	// Choose a creature's soul to be attached to a vanilla filled soul gem not created by the player
	TESForm* SkaarGetSoulForVanillaFilledSoulGem(TESForm *soulGem);

	bool RegisterFuncs(VMClassRegistry* registry);
}
