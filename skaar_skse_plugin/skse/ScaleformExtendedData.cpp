#include "ScaleformCallbacks.h"
#include "ScaleformMovie.h"
#include "ScaleformExtendedData.h"
#include "GameData.h"
#include "GameForms.h"
#include "GameObjects.h"
#include "GameReferences.h"
#include "GameRTTI.h"

// Helpers
double round(double r)
{
	return (r >= 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

void RegisterString(GFxValue * dst,  GFxMovieView * view, const char * name, const char * str)
{
	GFxValue	fxValue;

	view->CreateString(&fxValue, str);

	dst->SetMember(name, &fxValue);
}

void RegisterWideString(GFxValue * dst,  GFxMovieView * view, const char * name, const wchar_t * str)
{
	GFxValue	fxValue;

	view->CreateWideString(&fxValue, str);

	dst->SetMember(name, &fxValue);
}

void RegisterNumber(GFxValue * dst, const char * name, double value)
{
	GFxValue	fxValue;

	fxValue.SetNumber(value);

	dst->SetMember(name, &fxValue);
}

void RegisterBool(GFxValue * dst, const char * name, bool value)
{
	GFxValue fxValue;
	fxValue.SetBool(value);
	dst->SetMember(name, &fxValue);
}

void RegisterKeywords(GFxValue * pFxVal, GFxMovieView * view, BGSKeywordForm * keywordForm)
{
	GFxValue	keywordRoot;
	view->CreateObject(&keywordRoot);

	// Add all keywords as boolean properties with value true

	UInt32 count = keywordForm->numKeywords;
	BGSKeyword ** keywords = keywordForm->keywords;
	if(keywords)
	{
		for(int i = 0; i < count; i++)
		{
			BGSKeyword * pKey = keywords[i];
			if(pKey)
			{
				const char * keyString = pKey->keyword.Get();
				if(keyString)
				{
					RegisterBool(&keywordRoot, keyString, true);
				}
			}
		}
	}

	pFxVal->SetMember("keywords", &keywordRoot);
}

class FormListExtender : public BGSListForm::Visitor
{
	GFxValue		* m_array;
	GFxMovieView	* m_movie;
	bool	m_bExtra;
	bool	m_bRecursive;
	
public:
	FormListExtender::FormListExtender(GFxValue	* formArray, GFxMovieView * movie, bool bExtra, bool bRecursive) : m_array(formArray), m_movie(movie), m_bExtra(bExtra), m_bRecursive(bRecursive) { }
	virtual bool Accept(TESForm * form)
	{
		GFxValue arrArg;
		m_movie->CreateObject(&arrArg);
		scaleformExtend::FormData(&arrArg, m_movie, form, m_bRecursive ? m_bExtra : false, m_bRecursive);
		m_array->PushBack(&arrArg);
		return false;
	};
};

// Data extensions
namespace scaleformExtend
{
	void CommonItemData(GFxValue* pFxVal, TESForm * pForm)
	{
		if(!pFxVal || !pForm || !pFxVal->IsObject())
			return;

		RegisterNumber(pFxVal, "formType", (double)pForm->GetFormType());
		RegisterNumber(pFxVal, "formId", (double)pForm->formID);
	}

	void StandardItemData(GFxValue * pFxVal, TESForm * pForm)
	{
		if(!pForm || !pFxVal || !pFxVal->IsObject())
			return;

		switch(pForm->GetFormType())
		{
		case kFormType_Armor:
			{
				TESObjectARMO * pArmor = DYNAMIC_CAST(pForm, TESForm, TESObjectARMO);
				if(pArmor)
				{
					RegisterNumber(pFxVal, "partMask", pArmor->bipedObject.data.parts);
					RegisterNumber(pFxVal, "weightClass", pArmor->bipedObject.data.weightClass);
				}
			}
			break;

		case kFormType_Ammo:
			{
				TESAmmo * pAmmo = DYNAMIC_CAST(pForm, TESForm, TESAmmo);
				if(pAmmo)
				{
					RegisterNumber(pFxVal, "flags", pAmmo->settings.flags);
				}
			}
			break;

		case kFormType_Weapon:
			{
				TESObjectWEAP * pWeapon = DYNAMIC_CAST(pForm, TESForm, TESObjectWEAP);
				if(pWeapon)
				{
					RegisterNumber(pFxVal, "subType", pWeapon->type()); // DEPRECATED
					RegisterNumber(pFxVal, "weaponType", pWeapon->type());
					RegisterNumber(pFxVal, "speed", pWeapon->speed());
					RegisterNumber(pFxVal, "reach", pWeapon->reach());
					RegisterNumber(pFxVal, "stagger", pWeapon->stagger());
					RegisterNumber(pFxVal, "critDamage", pWeapon->critDamage());
					RegisterNumber(pFxVal, "minRange", pWeapon->minRange());
					RegisterNumber(pFxVal, "maxRange", pWeapon->maxRange());
					RegisterNumber(pFxVal, "baseDamage", pWeapon->damage.GetAttackDamage());

					BGSEquipSlot * equipSlot = pWeapon->equipType.GetEquipSlot();
					if (equipSlot)
						RegisterNumber(pFxVal, "equipSlot", equipSlot->formID);
				}
			}
			break;

		case kFormType_SoulGem:
			{
				TESSoulGem	* soulGem = DYNAMIC_CAST(pForm, TESForm, TESSoulGem);
				if(soulGem)
				{
					RegisterNumber(pFxVal, "soulSize", soulGem->soulSize);
					RegisterNumber(pFxVal, "gemSize", soulGem->gemSize);
				}
			}
			break;

		case kFormType_Potion:
			{
				AlchemyItem * pAlchemy = DYNAMIC_CAST(pForm, TESForm, AlchemyItem);
				if(pAlchemy)
				{
					RegisterNumber(pFxVal, "flags", pAlchemy->itemData.flags);
				}
			}
			break;

		case kFormType_Book:
			{
				TESObjectBOOK * pBook = DYNAMIC_CAST(pForm, TESForm, TESObjectBOOK);
				if(pBook)
				{
					RegisterNumber(pFxVal, "flags", pBook->data.flags);
					RegisterNumber(pFxVal, "bookType", pBook->data.type);
					switch(pBook->data.GetSanitizedType())
					{
					case TESObjectBOOK::Data::kType_Skill:
						RegisterNumber(pFxVal, "teachesSkill", pBook->data.teaches.skill);
						break;

					case TESObjectBOOK::Data::kType_Spell:
						{
							double formID = -1;

							if(pBook->data.teaches.spell)
								formID = pBook->data.teaches.spell->formID;

							RegisterNumber(pFxVal, "teachesSpell", formID);
						}
						break;
					}
				}
			}
			break;

		default:
			break;
		}
	}


	void MagicItemData(GFxValue * pFxVal, GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive)
	{
		if(!pFxVal || !pForm || !pFxVal->IsObject())
			return;

		switch(pForm->GetFormType())
		{
		case kFormType_Spell:
		case kFormType_ScrollItem:
		case kFormType_Ingredient:
		case kFormType_Potion:
		case kFormType_Enchantment:
			{
				MagicItem * pMagicItem = DYNAMIC_CAST(pForm, TESForm, MagicItem);
				if(pMagicItem)
				{
					if(pMagicItem->fullName.name.data)
						RegisterString(pFxVal, movieView, "spellName", pMagicItem->fullName.name.data);

					MagicItem::EffectItem * pEffect = CALL_MEMBER_FN(pMagicItem, GetCostliestEffectItem)(5, false);
					if(pEffect && pEffect->mgef)
					{
						RegisterNumber(pFxVal, "magnitude", pEffect->magnitude);
						RegisterNumber(pFxVal, "duration", pEffect->duration);
						RegisterNumber(pFxVal, "area", pEffect->area);

						// PLB: Normally I'd have this as a separate object but SkyUI is already using this
						scaleformExtend::MagicItemData(pFxVal, movieView, pEffect->mgef, bRecursive ? bExtra : false, bRecursive);
					}
				}

				SpellItem * pSpellItem = DYNAMIC_CAST(pMagicItem, MagicItem, SpellItem);
				if(pSpellItem)
				{
					RegisterNumber(pFxVal, "spellType", pSpellItem->data.type);
					RegisterNumber(pFxVal, "trueCost", pSpellItem->GetMagickaCost());

					BGSEquipSlot * equipSlot = pSpellItem->equipType.GetEquipSlot();
					if (equipSlot)
						RegisterNumber(pFxVal, "equipSlot", equipSlot->formID);
				}

				AlchemyItem * pAlchemyItem = DYNAMIC_CAST(pMagicItem, MagicItem, AlchemyItem);
				if(pAlchemyItem)
				{
					if(pAlchemyItem->itemData.useSound) {
						GFxValue useSound;
						movieView->CreateObject(&useSound);
						scaleformExtend::FormData(&useSound, movieView, pAlchemyItem->itemData.useSound, bRecursive ? bExtra : false, bRecursive);
						pFxVal->SetMember("useSound", &useSound);
					}
				}

				EnchantmentItem * pEnchantItem = DYNAMIC_CAST(pMagicItem, MagicItem, EnchantmentItem);
				if(pEnchantItem)
				{
					RegisterNumber(pFxVal, "flags", (double)pMagicItem->flags);

					GFxValue baseEnchant;
					movieView->CreateObject(&baseEnchant);
					scaleformExtend::FormData(&baseEnchant, movieView, pEnchantItem->data.baseEnchantment, bRecursive ? bExtra : false, bRecursive);
					pFxVal->SetMember("baseEnchant", &baseEnchant);

					GFxValue restrictions;
					movieView->CreateObject(&restrictions);
					scaleformExtend::FormData(&restrictions, movieView, pEnchantItem->data.restrictions, bRecursive ? bExtra : false, bRecursive);
					pFxVal->SetMember("restrictions", &restrictions);
				}
			}
			break;

		case kFormType_EffectSetting:
			{
				EffectSetting * pEffectSetting = DYNAMIC_CAST(pForm, TESForm, EffectSetting);
				if(pEffectSetting)
				{
					if(pEffectSetting->fullName.name.data)
						RegisterString(pFxVal, movieView, "effectName", pEffectSetting->fullName.name.data);

					RegisterNumber(pFxVal, "subType", pEffectSetting->school()); // DEPRECATED
					RegisterNumber(pFxVal, "effectFlags", pEffectSetting->properties.flags);
					RegisterNumber(pFxVal, "school", pEffectSetting->school());
					RegisterNumber(pFxVal, "skillLevel", pEffectSetting->level());
					RegisterNumber(pFxVal, "archetype", pEffectSetting->properties.archetype);
					RegisterNumber(pFxVal, "deliveryType", pEffectSetting->properties.deliveryType);
					RegisterNumber(pFxVal, "castTime", pEffectSetting->properties.castingTime);
					RegisterNumber(pFxVal, "delayTime", pEffectSetting->properties.delayTime);
					RegisterNumber(pFxVal, "actorValue", pEffectSetting->properties.primaryValue);
					RegisterNumber(pFxVal, "castType", pEffectSetting->properties.castType);
					RegisterNumber(pFxVal, "magicType", pEffectSetting->properties.resistance);
				}
			}
			break;
		case kFormType_Shout:
			{
				TESShout * pShout = DYNAMIC_CAST(pForm, TESForm, TESShout);
				if(pShout)
				{
					if(pShout->fullName.name.data)
						RegisterString(pFxVal, movieView, "fullName", pShout->fullName.name.data);

					// Words
					GFxValue words;
					movieView->CreateArray(&words);
					for(int i = 0; i < pShout->Words::kNumWords; i++)
					{
						GFxValue word;
						movieView->CreateObject(&word);

						if(pShout->words[i].word && pShout->words[i].word->fullName.name.data)
							RegisterString(&word, movieView, "word", pShout->words[i].word->fullName.name.data);

						if(pShout->words[i].word && pShout->words[i].word->word.data)
							RegisterString(&word, movieView, "fullName", pShout->words[i].word->word.data);

						RegisterNumber(&word, "recoveryTime", pShout->words[i].recoverytime);

						// Spell Object
						scaleformExtend::FormData(&word, movieView, pShout->words[i].spell, bRecursive ? bExtra : false, bRecursive);
						words.PushBack(&word);
					}
					pFxVal->SetMember("words", &words);
				}
			}
			break;

		default:
			break;
		}
	}

	void ActorData(GFxValue * pFxVal,  GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive)
	{
		if(!pForm || !pFxVal || !pFxVal->IsObject())
			return;

		switch(pForm->GetFormType())
		{
		case kFormType_Race:
			{
				TESRace * pRace = DYNAMIC_CAST(pForm, TESForm, TESRace);
				if(pRace)
				{
					if(pRace->fullName.name.data)
						RegisterString(pFxVal, movieView, "fullName", pRace->fullName.name.data);

					if(pRace->editorId.data)
						RegisterString(pFxVal, movieView, "editorId", pRace->editorId.data);

					// Spells
					GFxValue spells;
					movieView->CreateArray(&spells);
					for(int i = 0; i < pRace->spellList.GetSpellCount(); i++)
					{
						GFxValue spell;
						movieView->CreateObject(&spell);
						scaleformExtend::FormData(&spell, movieView, pRace->spellList.GetNthSpell(i), bRecursive ? bExtra : false, bRecursive);
						spells.PushBack(&spell);
					}
					pFxVal->SetMember("spells", &spells);

					// Shouts
					GFxValue shouts;
					movieView->CreateArray(&shouts);
					for(int i = 0; i < pRace->spellList.GetShoutCount(); i++)
					{
						GFxValue shout;
						movieView->CreateObject(&shout);
						scaleformExtend::FormData(&shout, movieView, pRace->spellList.GetNthShout(i), bRecursive ? bExtra : false, bRecursive);
						shouts.PushBack(&shout);
					}
					pFxVal->SetMember("shouts", &shouts);

					GFxValue bonuses;
					movieView->CreateArray(&bonuses);
					for(int i = 0; i < TESRace::kRace_NumSkillBonuses; i++)
					{
						GFxValue skillBonus;
						movieView->CreateObject(&skillBonus);
						RegisterNumber(&skillBonus, "skill", pRace->data.skillBonus[i].skill);
						RegisterNumber(&skillBonus, "bonus", pRace->data.skillBonus[i].bonus);
						bonuses.PushBack(&skillBonus);
					}
					pFxVal->SetMember("skillBonuses", &bonuses);
				}
			}
			break;
		case kFormType_NPC:
			{
				TESNPC * pNPC = DYNAMIC_CAST(pForm, TESForm, TESNPC);
				if(pNPC)
				{
					if(pNPC->fullName.name.data)
						RegisterString(pFxVal, movieView, "fullName", pNPC->fullName.name.data);
					if(pNPC->shortName.data)
						RegisterString(pFxVal, movieView, "shortName", pNPC->shortName.data);

					RegisterNumber(pFxVal, "weight", pNPC->weight);

					bool isLevelMult = (pNPC->actorData.flags & TESActorBaseData::kFlag_PCLevelMult) == TESActorBaseData::kFlag_PCLevelMult;
					if(isLevelMult)
						RegisterNumber(pFxVal, "levelMult", (double)pNPC->actorData.level / 1000.0);
					else
						RegisterNumber(pFxVal, "level", (double)pNPC->actorData.level);

					RegisterNumber(pFxVal, "minLevel", (double)pNPC->actorData.minLevel);
					RegisterNumber(pFxVal, "maxLevel", (double)pNPC->actorData.maxLevel);

					GFxValue race;
					movieView->CreateObject(&race);
					scaleformExtend::FormData(&race, movieView, pNPC->race.race, bRecursive ? bExtra : false, bRecursive);
					pFxVal->SetMember("race", &race);

					// Spells
					GFxValue spells;
					movieView->CreateArray(&spells);
					for(int i = 0; i < pNPC->spellList.GetSpellCount(); i++)
					{
						GFxValue spell;
						movieView->CreateObject(&spell);
						scaleformExtend::FormData(&spell, movieView, pNPC->spellList.GetNthSpell(i), bRecursive ? bExtra : false, bRecursive);
						spells.PushBack(&spell);
					}
					pFxVal->SetMember("spells", &spells);

					// Shouts
					GFxValue shouts;
					movieView->CreateArray(&shouts);
					for(int i = 0; i < pNPC->spellList.GetShoutCount(); i++)
					{
						GFxValue shout;
						movieView->CreateObject(&shout);
						scaleformExtend::FormData(&shout, movieView, pNPC->spellList.GetNthShout(i), bRecursive ? bExtra : false, bRecursive);
						shouts.PushBack(&shout);
					}
					pFxVal->SetMember("shouts", &shouts);
				}
			}
			break;

		case kFormType_Character:
			{
				Actor * pActor = DYNAMIC_CAST(pForm, TESForm, Actor);
				if(pActor)
				{
					// ActorBase as Object
					GFxValue actorBase;
					movieView->CreateObject(&actorBase);

					scaleformExtend::FormData(&actorBase, movieView, pActor->baseForm, bRecursive ? bExtra : false, bRecursive);
					pFxVal->SetMember("actorBase", &actorBase);

					RegisterString(pFxVal, movieView, "fullName", CALL_MEMBER_FN(pActor, GetReferenceName)());

					// Spells as Array
					GFxValue addedSpells;
					movieView->CreateArray(&addedSpells);

					for(int i = 0; i < pActor->addedSpells.Length(); i++)
					{
						GFxValue spell;
						movieView->CreateObject(&spell);
						scaleformExtend::FormData(&spell, movieView, pActor->addedSpells.Get(i), bRecursive ? bExtra : false, bRecursive);
						addedSpells.PushBack(&spell);
					}

					pFxVal->SetMember("spells", &addedSpells);

					// ActiveEffects as Array
					GFxValue activeEffects;
					movieView->CreateArray(&activeEffects);

					tList<ActiveEffect> * effects = pActor->magicTarget.GetActiveEffects();
					if(effects)
					{
						for(int i = 0; i < effects->Count(); i++)
						{
							GFxValue effect;
							movieView->CreateObject(&effect);

							ActiveEffect * pEffect = effects->GetNthItem(i);

							if(pEffect->item)
								scaleformExtend::MagicItemData(&effect, movieView, pEffect->item, bRecursive ? bExtra : false, bRecursive);

							RegisterNumber(&effect, "elapsed", pEffect->elapsed);
							RegisterNumber(&effect, "duration", pEffect->duration);
							RegisterNumber(&effect, "magnitude", pEffect->magnitude);
							RegisterBool(&effect, "inactive", (pEffect->flags & ActiveEffect::kFlag_Inactive) == ActiveEffect::kFlag_Inactive);
							
							// ActiveEffect
							if(pEffect->effect && pEffect->effect->mgef)
								scaleformExtend::MagicItemData(&effect, movieView, pEffect->effect->mgef, bRecursive ? bExtra : false, bRecursive);
							
							activeEffects.PushBack(&effect);
						}
					}
					pFxVal->SetMember("activeEffects", &activeEffects);

					GFxValue actorValues;
					movieView->CreateArray(&actorValues);

					for(int i = 0; i < ActorValueList::kNumActorValues; i++)
					{
						GFxValue actorValue;
						movieView->CreateObject(&actorValue);
						RegisterNumber(&actorValue, "current", pActor->actorValueOwner.GetCurrent(i));
						RegisterNumber(&actorValue, "maximum", pActor->actorValueOwner.GetMaximum(i));
						RegisterNumber(&actorValue, "base", pActor->actorValueOwner.GetBase(i));
						actorValues.PushBack(&actorValue);
					}

					pFxVal->SetMember("actorValues", &actorValues);
				}
				PlayerCharacter* pPC = DYNAMIC_CAST(pForm, TESForm, PlayerCharacter);
				if(pPC)
				{
					RegisterNumber(pFxVal, "perkPoints", (double)pPC->numPerkPoints);
				}
			}
			break;

		default:
			break;
		}
	}

	void FormListData(GFxValue * pFxVal, GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive)
	{
		if(!pForm || !pFxVal || !pFxVal->IsObject())
			return;

		switch(pForm->GetFormType())
		{
		case kFormType_List:
			{
				BGSListForm * formList = DYNAMIC_CAST(pForm, TESForm, BGSListForm);
				if(formList)
				{
					GFxValue formArray;
					movieView->CreateArray(&formArray);

					FormListExtender extender(&formArray, movieView, bExtra, bRecursive);
					formList->Visit(extender);

					pFxVal->SetMember("forms", &formArray);
				}
			}
			break;
		default:
			break;
		}
	}

	void MiscData(GFxValue * pFxVal, GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive)
	{
		if(!pForm || !pFxVal || !pFxVal->IsObject())
			return;

		switch(pForm->GetFormType())
		{
		case kFormType_Message:
			{
				BGSMessage * message = DYNAMIC_CAST(pForm, TESForm, BGSMessage);
				if(message)
				{
					GFxValue btnArray;
					movieView->CreateArray(&btnArray);
					BSFixedString * btnText;
					for(int i = 0; i < message->buttons.Count(); i++)
					{
						GFxValue fxValue;
						btnText = message->buttons.GetNthItem(i);
						if(btnText) {
							movieView->CreateString(&fxValue, btnText->data);
							btnArray.PushBack(&fxValue);
						}
					}

					pFxVal->SetMember("buttons", &btnArray);
					RegisterString(pFxVal, movieView, "message", message->GetDescription().c_str());
				}
			}
			break;
		case kFormType_Quest:
			{
				TESQuest * quest = DYNAMIC_CAST(pForm, TESForm, TESQuest);
				if(quest)
				{
					RegisterString(pFxVal, movieView, "fullName", quest->fullName.name.data);
					RegisterNumber(pFxVal, "flags", quest->unk07C.flags);
					RegisterNumber(pFxVal, "priority", quest->unk07C.priority);
					RegisterString(pFxVal, movieView, "editorId", quest->questID.Get());

					GFxValue aliasArray;

					movieView->CreateArray(&aliasArray);
					
					for(int i = 0; i < quest->aliases.count; i++)
					{
						BGSBaseAlias* alias;
						if(quest->aliases.GetNthItem(i, alias)) {
							GFxValue arrArg;
							movieView->CreateObject(&arrArg);
							RegisterString(&arrArg, movieView, "name", alias->name.data);
							RegisterNumber(&arrArg, "id", alias->aliasId);
							RegisterNumber(&arrArg, "flags", alias->flags);
							aliasArray.PushBack(&arrArg);
						}
					}

					pFxVal->SetMember("aliases", &aliasArray);
				}
			}
			break;
		case kFormType_HeadPart:
			{
				BGSHeadPart * headPart = DYNAMIC_CAST(pForm, TESForm, BGSHeadPart);
				if(headPart)
				{
					RegisterString(pFxVal, movieView, "fullName", headPart->fullName.name.data);
					RegisterString(pFxVal, movieView, "partName", headPart->partName.data);
					RegisterNumber(pFxVal, "partFlags", headPart->partFlags);
					RegisterNumber(pFxVal, "partType", headPart->type);

					RegisterString(pFxVal, movieView, "modelPath", headPart->model.GetModelName());
					RegisterString(pFxVal, movieView, "chargenMorphPath", headPart->chargenMorph.GetModelName());
					RegisterString(pFxVal, movieView, "raceMorphPath", headPart->raceMorph.GetModelName());

					GFxValue extraParts;
					movieView->CreateArray(&extraParts);
					for(int i = 0; i < headPart->extraParts.count; i++)
					{
						BGSHeadPart* extraPart;
						if(headPart->extraParts.GetNthItem(i, extraPart)) {
							GFxValue arrArg;
							movieView->CreateObject(&arrArg);
							scaleformExtend::FormData(&arrArg, movieView, extraPart, bRecursive ? bExtra : false, bRecursive);
							extraParts.PushBack(&arrArg);
						}
					}

					pFxVal->SetMember("extraParts", &extraParts);

					GFxValue validRaces;
					movieView->CreateObject(&validRaces);
					scaleformExtend::FormData(&validRaces, movieView, headPart->validRaces, bRecursive ? bExtra : false, bRecursive);
					pFxVal->SetMember("validRaces", &validRaces);

					GFxValue textureSet;
					movieView->CreateObject(&textureSet);
					scaleformExtend::FormData(&textureSet, movieView, headPart->textureSet, bRecursive ? bExtra : false, bRecursive);
					pFxVal->SetMember("textureSet", &textureSet);
				}
			}
			break;
		case kFormType_TextureSet:
			{
				BGSTextureSet * textureSet = DYNAMIC_CAST(pForm, TESForm, BGSTextureSet);
				if(textureSet)
				{
					GFxValue textureArray;
					movieView->CreateArray(&textureArray);
					for(int i = 0; i < BGSTextureSet::kNumTextures; i++)
					{
						GFxValue strArg;
						movieView->CreateString(&strArg, textureSet->texturePaths[i].str.data);
						textureArray.PushBack(&strArg);
					}

					pFxVal->SetMember("textures", &textureArray);
				}
			}
			break;
		default:
			break;
		}
	}

	// Convenience function, maybe combine all other functions into this one anyway?
	// bExtra - Adds all extra data other than FormId and FormType
	// bRecursive - Processes child forms
	void FormData(GFxValue * pFxVal, GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive)
	{
		if(!pForm || !pFxVal || !pFxVal->IsObject())
			return;

		scaleformExtend::CommonItemData(pFxVal, pForm);
		if(bExtra) {
			scaleformExtend::StandardItemData(pFxVal, pForm);
			scaleformExtend::MagicItemData(pFxVal, movieView, pForm, bExtra, bRecursive);
			scaleformExtend::ActorData(pFxVal, movieView, pForm, bExtra, bRecursive);
			scaleformExtend::MiscData(pFxVal, movieView, pForm, bExtra, bRecursive);
		}
		scaleformExtend::FormListData(pFxVal, movieView, pForm, bExtra, bRecursive);
	}

	void InventoryData(GFxValue * pFxVal, GFxMovieView * movieView, PlayerCharacter::ObjDesc * objDesc)
	{
		TESForm	* pForm = objDesc->form;

		if(!pForm || !pFxVal || !pFxVal->IsObject())
			return;

		PlayerCharacter	* pPC = *g_thePlayer;

		BGSKeywordForm	* keywords = DYNAMIC_CAST(pForm, TESForm, BGSKeywordForm);
		if(keywords)
			RegisterKeywords(pFxVal, movieView, keywords);

		switch(pForm->GetFormType())
		{
		case kFormType_Armor:
			{
				TESObjectARMO * pArmor = DYNAMIC_CAST(pForm, TESForm, TESObjectARMO);
				if(pArmor)
				{
					double armorValue = CALL_MEMBER_FN(pPC, GetArmorValue)(objDesc);
					armorValue = round(armorValue);
					RegisterNumber(pFxVal, "armor", armorValue);
				}
			}
			break;

		case kFormType_Weapon:
			{
				TESObjectWEAP * pWeapon = DYNAMIC_CAST(pForm, TESForm, TESObjectWEAP);
				if(pWeapon)
				{
					double damage = CALL_MEMBER_FN(pPC, GetDamage)(objDesc);
					damage = round(damage);
					RegisterNumber(pFxVal, "damage", damage);
				}
			}
			break;

		case kFormType_Ammo:
			{
				TESAmmo * pAmmo = DYNAMIC_CAST(pForm, TESForm, TESAmmo);
				if(pAmmo)
				{
					double damage = CALL_MEMBER_FN(pPC, GetDamage)(objDesc);
					damage = round(damage);
					RegisterNumber(pFxVal, "damage", damage);
				}
			}
			break;

		default:
			break;
		}
	}
}
