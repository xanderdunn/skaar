#include "SkaarSpecialInventoryCrafting.h"
#include "SkaarInventory.h"
#include "SkaarUtilities.h"

#include "skse/PapyrusForm.h"

#include "skse/GameBSExtraData.h"
#include "skse/GameAPI.h"
#include "skse/GameFormComponents.h"
#include "skse/GameForms.h"
#include "skse/GameRTTI.h"
#include "skse/GameObjects.h"
#include "skse/GameExtraData.h"
#include "skse/GameData.h"
#include "skse/PapyrusForm.h"
#include "skse/GameRTTI.h"
#include "skse/PapyrusGame.h"
#include "skse/PapyrusArgs.h"
#include "skse/PapyrusObjectReference.h"

#include <cstdlib>
#include <stdio.h>
#include "skse/GameSettings.h"

static UInt32 const AzurasStarFormID = 0x00063B27;
static UInt32 const BlackStarFormID = 0x00063B29;

static UInt32 const PettySoulGemFilledFormID = 0x0002E4E3;
static UInt32 const LesserSoulGemFilledFormID = 0x0002E4E5;
static UInt32 const CommonSoulGemFilledFormID = 0x0002E4F3;
static UInt32 const GreaterSoulGemFilledFormID = 0x0002E4FB;
static UInt32 const GrandSoulGemFilledFormID = 0x0002E4FF;
static UInt32 const BlackSoulGemFilledFormID = 0x0002E504;

// Creatures to fill vanilla filled gems with:
static UInt32 const mudcrab = 0x0008CACC;
static UInt32 const skeever = 0x00023AB7;
static UInt32 const slaughterfish = 0x00023AB8;
static UInt32 const wolf = 0x00023ABE;
static UInt32 const wolfIce = 0x00023ABF;
static UInt32 const bear = 0x00023A8A;
static UInt32 const bearSnow = 0x00023A8C;
static UInt32 const horker = 0x00023AB1;
static UInt32 const skeletonMelee = 0x000BCCC7;
static UInt32 const skeletonArcher = 0x0002D1FC;
static UInt32 const frostbiteSpider = 0x00109C1C;
static UInt32 const draugrMaleMelee1 = 0x000567C8;
static UInt32 const draugrMaleMelee2 = 0x0004A04E;
static UInt32 const draugrMaleMissle = 0x000A6851;
static UInt32 const troll = 0x00023ABA;
static UInt32 const trollFrost = 0x00023ABB;
static UInt32 const iceWraith = 0x00023AB3;
static UInt32 const mammothTame = 0x000DD641;
static UInt32 const hagraven = 0x00023AB0;
static UInt32 const giant = 0x0003053F;
static UInt32 const falmerMelee = 0x0001E7B2;
static UInt32 const falmerMissle = 0x0001E7B3;
static UInt32 const falmerSpellsword = 0x0006322C;
static UInt32 const mageBreton = 0x001034F3;
static UInt32 const mageArgonian = 0x001062E;
static UInt32 const witch = 0x00074F9E;
static UInt32 const vampire = 0x0002F9E1;
static UInt32 const banditMelee = 0x0001E79C;
static UInt32 const banditMissile = 0x0001E79E;

//TODO: Flesh these out
static UInt32 const pettySouls[] = { mudcrab, skeever, slaughterfish };
static int const pettySoulCount = 3;
static UInt32 const lesserSouls[] = { wolf, wolfIce, bear, bearSnow, horker, skeletonMelee, skeletonArcher };
static int const lesserSoulCount = 7;
static UInt32 const commonSouls[] = { frostbiteSpider, draugrMaleMelee1, draugrMaleMelee2, draugrMaleMissle, troll };
static int const commonSoulCount = 5;
static UInt32 const greaterSouls[] = {hagraven, trollFrost, iceWraith, mammothTame};
static int const greaterSoulCount = 4;
static UInt32 const grandSouls[] = { giant, falmerMelee, falmerMissle, falmerSpellsword };
static int const grandSoulCount = 4;
static UInt32 const blackSouls[] = { mageBreton, mageArgonian, witch, vampire, banditMelee, banditMissile };
static int const blackSoulCount = 4;


template <> void UnpackValue(VMArray<TESForm*> * dst, VMValue * src) {
	UnpackArray(dst, src, GetTypeIDFromFormTypeID(TESForm::kTypeID, (*g_skyrimVM)->GetClassRegistry()) | VMValue::kType_Identifier);
}

namespace SkaarSoulTrap {

	void SkaarGetGemLevels();

	// Soul gem counts in caster's inventory
	UInt32 emptyPettySoulGemCount = 0;
	UInt32 emptyLesserSoulGemCount = 0;
	UInt32 emptyCommonSoulGemCount = 0;
	UInt32 emptyGreaterSoulGemCount = 0;
	UInt32 emptyGrandSoulGemCount = 0;
	UInt32 emptyBlackSoulGemCount = 0;

	// Game settings for soul gem levels
	double lesserLevel = 0;
	double commonLevel = 0;
	double greaterLevel = 0;
	double grandLevel = 0;

	TESForm* SkaarGetSoulForVanillaFilledSoulGem(StaticFunctionTag *base, TESForm *soulGem) {
		if (!soulGem) {
			return NULL;
		}
		UInt32 soulGemFormID = soulGem->formID;
		switch (soulGemFormID) {
			case PettySoulGemFilledFormID:
			{
				UInt32 randomFormID = SkaarUtilities::randomArrayValue(pettySouls, pettySoulCount);
				return LookupFormByID(randomFormID);
				break;
			}
			case LesserSoulGemFilledFormID:
			{
				UInt32 randomFormID = SkaarUtilities::randomArrayValue(lesserSouls, lesserSoulCount);
				return LookupFormByID(randomFormID);
				break;
			}
			case CommonSoulGemFilledFormID:
			{
				UInt32 randomFormID = SkaarUtilities::randomArrayValue(commonSouls, commonSoulCount);
				return LookupFormByID(randomFormID);
				break;
			}
			case GreaterSoulGemFilledFormID:
			{
				UInt32 randomFormID = SkaarUtilities::randomArrayValue(greaterSouls, greaterSoulCount);
				return LookupFormByID(randomFormID);
				break;
			}
			case GrandSoulGemFilledFormID:
			{
				UInt32 randomFormID = SkaarUtilities::randomArrayValue(grandSouls, grandSoulCount);
				return LookupFormByID(randomFormID);
				break;
			}
			case BlackSoulGemFilledFormID:
			{
				UInt32 randomFormID = SkaarUtilities::randomArrayValue(blackSouls, blackSoulCount);
				return LookupFormByID(randomFormID);
				break;
			}

			default:
				return NULL;
				break;
		}
	}

	void SkaarCapitalizeString(char *string) {
		if (!string) {
			return;
		}
		char previousCharacter = NULL;
		for (size_t i = 0; i < strlen(string); i++) {
			char character = string[i];
			if (i == 0 || previousCharacter == ' ') {
				string[i] = toupper(character);
			}
			else {
				string[i] = tolower(character);
			}
			previousCharacter = character;
		}
	}

	void SkaarNameSoulGem(StaticFunctionTag *base, TESObjectREFR *soulGem, TESObjectREFR *capturedSoul) {
		_MESSAGE("Got soul gem to rename: %i", soulGem->formID);
		if (!soulGem) {
			return;
		}
		BSFixedString targetDisplayName = papyrusObjectReference::GetDisplayName(capturedSoul);

		char displayName[100];

		if (soulGem->baseForm->formID == AzurasStarFormID) {		// Azura's Star
			if (capturedSoul) {
				strcpy_s(displayName, "Azura's Star: ");
				strcat_s(displayName, targetDisplayName.data);
				strcat_s(displayName, " Soul");
			}
			else {
				strcpy_s(displayName, "Azura's Star");
			}
		}
		else if (soulGem->baseForm->formID == BlackStarFormID) {	// Black Star
			if (capturedSoul) {
				strcpy_s(displayName, "The Black Star: ");
				strcat_s(displayName, targetDisplayName.data);
				strcat_s(displayName, "'s Soul");
			}
			else {
				strcpy_s(displayName, "The Black Star");
			}
		}
		else {					// All other soul gems
			strcpy_s(displayName, targetDisplayName.data);
			if (soulGem->baseForm->formID == BlackSoulGemFilledFormID) {
				strcat_s(displayName, "'s Soul");
			}
			else {
				strcat_s(displayName, " Soul");
			}
		}
		SkaarCapitalizeString(displayName);
		_MESSAGE("About to set display name on soul gem %i to %s", soulGem->formID, displayName);
		papyrusObjectReference::SetDisplayName(soulGem, displayName, true);
	}

	void SkaarGetSoulGemCounts(Actor* casterRef) {
		emptyPettySoulGemCount = SkaarInventory::SkaarContainerCountOfForm(casterRef, LookupFormByID(0x0002E4E2));
		emptyLesserSoulGemCount = SkaarInventory::SkaarContainerCountOfForm(casterRef, LookupFormByID(0x0002E4E4));
		emptyCommonSoulGemCount = SkaarInventory::SkaarContainerCountOfForm(casterRef, LookupFormByID(0x0002E4E6));
		emptyGreaterSoulGemCount = SkaarInventory::SkaarContainerCountOfForm(casterRef, LookupFormByID(0x0002E4F4));
		emptyGrandSoulGemCount = SkaarInventory::SkaarContainerCountOfForm(casterRef, LookupFormByID(0x0002E4FC));
		emptyBlackSoulGemCount = SkaarInventory::SkaarContainerCountOfForm(casterRef, LookupFormByID(0x0002E500));
		_MESSAGE("Found empty gems: %i petty, %i lesser, %i common, %i greater, %i grand, %i black", emptyPettySoulGemCount, emptyLesserSoulGemCount, emptyCommonSoulGemCount, emptyGreaterSoulGemCount, emptyGrandSoulGemCount, emptyBlackSoulGemCount);
	}

	bool SkaarSoulGemIsFilled(StaticFunctionTag *base, TESForm *soulGem) {
		UInt32 id = soulGem->formID;
		if (id == PettySoulGemFilledFormID || id == LesserSoulGemFilledFormID || id == CommonSoulGemFilledFormID || id == GreaterSoulGemFilledFormID || id == GrandSoulGemFilledFormID || id == BlackSoulGemFilledFormID) {
			return true;
		}
		else {
			return false;
		}
	}

	void SkaarSoulTrap(StaticFunctionTag *base, Actor* casterRef, Actor* targetRef, bool isHuman, VMArray<TESForm*> returnValues) {
		_MESSAGE("SkaarSoulTrap called");
		SkaarGetSoulGemCounts(casterRef);

		TESForm *azurasStarForm = LookupFormByID(AzurasStarFormID);
		TESForm *blackStarForm = LookupFormByID(BlackStarFormID);
		UInt32 azurasStarCount = SkaarInventory::SkaarContainerCountOfForm(casterRef, azurasStarForm);
		UInt32 blackStarCount = SkaarInventory::SkaarContainerCountOfForm(casterRef, blackStarForm);
		_MESSAGE("Found %i Azura's Star and %i Black Star", azurasStarCount, blackStarCount);

		// TODO: Check that AzurasStar or BlackStar are empty
		UInt32 totalSoulGemCount = emptyPettySoulGemCount + emptyLesserSoulGemCount + emptyCommonSoulGemCount + emptyGreaterSoulGemCount + emptyGrandSoulGemCount;
		if (casterRef && targetRef && targetRef->IsDead(1) && ((!isHuman && (totalSoulGemCount + azurasStarCount)) || ((emptyBlackSoulGemCount + blackStarCount) && isHuman))) {
			UInt32 targetLevel = CALL_MEMBER_FN(targetRef, GetLevel)();
			_MESSAGE("Got target level %i", targetLevel);

			TESForm *formToRemove = NULL;
			TESForm *formToAdd = NULL;

			if (isHuman) {				// black soul gems only
				if (blackStarCount) {					// Try Black Star
					_MESSAGE("Attempt fill Black Star");
					returnValues.Set(&blackStarForm, 2);
				}
				if (emptyBlackSoulGemCount) {	// Use a black soul gem
					_MESSAGE("Fill black soul gem");
					formToRemove = LookupFormByID(0x0002E500);
					formToAdd = LookupFormByID(BlackSoulGemFilledFormID);
				}
				else {								// No black soul gem to fill it
					_MESSAGE("No black soul gem found");
				}
			}
			else {						// it is not human, use regular gems
				_MESSAGE("Skaar: Try to fill a non-human soul gem");
				SkaarGetGemLevels();

				if (emptyPettySoulGemCount && targetLevel < lesserLevel) {
					_MESSAGE("Skaar: Fill petty gem");
					formToRemove = LookupFormByID(0x0002E4E2);
					formToAdd = LookupFormByID(PettySoulGemFilledFormID);
				}
				else if (emptyLesserSoulGemCount && targetLevel < commonLevel && targetLevel >= lesserLevel) {
					_MESSAGE("Skaar: Fill lesser gem");
					formToRemove = LookupFormByID(0x0002E4E4);
					formToAdd = LookupFormByID(LesserSoulGemFilledFormID);
				}
				else if (emptyCommonSoulGemCount && targetLevel < greaterLevel && targetLevel >= commonLevel) {
					_MESSAGE("Skaar: Fill common gem");
					formToRemove = LookupFormByID(0x0002E4E6);
					formToAdd = LookupFormByID(CommonSoulGemFilledFormID);
				}
				else if (emptyGreaterSoulGemCount && targetLevel < grandLevel && targetLevel >= greaterLevel) {
					_MESSAGE("Skaar: Fill greater gem");
					formToRemove = LookupFormByID(0x0002E4F4);
					formToAdd = LookupFormByID(GreaterSoulGemFilledFormID);
				}
				else if (azurasStarCount) {
					_MESSAGE("Attempt fill Azura's Star");
					returnValues.Set(&azurasStarForm, 2);
				}
				if (targetLevel >= grandLevel) {	// Grand Soul Gem
					_MESSAGE("Skaar: Fill grand gem");
					formToRemove = LookupFormByID(0x0002E4FC);
					formToAdd = LookupFormByID(GrandSoulGemFilledFormID);
				}
			}
			returnValues.Set(&formToRemove, 0);
			returnValues.Set(&formToAdd, 1);
		}
	}

	void SkaarGetGemLevels() {	// Get the levels for each soul type that ought to be captured
		SettingCollectionMap *settings = *g_gameSettingCollection;
		if (settings) {
			Setting *lesserSetting = settings->Get("iLesserSoulActorLevel");
			if (lesserSetting) {
				lesserSetting->GetDouble(&lesserLevel);
			}

			Setting *commonSetting = settings->Get("iCommonSoulActorLevel");
			if (commonSetting) {
				commonSetting->GetDouble(&commonLevel);
			}

			Setting *greaterSetting = settings->Get("iGreaterSoulActorLevel");
			if (greaterSetting) {
				greaterSetting->GetDouble(&greaterLevel);
			}

			Setting *grandSetting = settings->Get("iGrandSoulActorLevel");
			if (grandSetting) {
				grandSetting->GetDouble(&grandLevel);
			}
		}
	}

	bool RegisterFuncs(VMClassRegistry* registry) {

		registry->RegisterFunction(
			new NativeFunction4 <StaticFunctionTag, void, Actor*, Actor*, bool, VMArray<TESForm*>>("SkaarSoulTrap", "SkaarMagicSoulTrapFXScript", SkaarSoulTrap::SkaarSoulTrap, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, TESObjectREFR*, TESObjectREFR*>("SkaarNameSoulGem", "SkaarMagicSoulTrapFXScript", SkaarSoulTrap::SkaarNameSoulGem, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, bool, TESForm*>("SkaarSoulGemIsFilled", "SkaarMagicSoulTrapFXScript", SkaarSoulTrap::SkaarSoulGemIsFilled, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, TESForm*, TESForm*>("SkaarGetSoulForVanillaFilledSoulGem", "SkaarMagicSoulTrapFXScript", SkaarSoulTrap::SkaarGetSoulForVanillaFilledSoulGem, registry));

		return true;
	}
}
