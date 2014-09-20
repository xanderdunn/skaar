ScriptName SkaarMagicSoulTrapFXScript extends ActiveMagicEffect
{Scripted effect for the Soul Trap Visual FX that stores the captured soul}

Function SkaarSoulTrap(Actor casterRef, Actor targetRef, bool isHuman, Form[] returnValues) global native
{Complete SKSE plugin rewrite of the }

Function SkaarNameSoulGem(ObjectReference soulGem, ObjectReference capturedSoul) global native
{Set the new name of the soul gem with its captured soul}

Bool Function SkaarSoulGemIsFilled(Form soulGem) global native
{Test if a regular soul gem is filled (Does not work for Azura's Star or the Black Star)}

Form Function SkaarGetSoulForVanillaFilledSoulGem(Form soulGem) global native
{Get which form to add to a vanilla filled soul gem}

import debug
import SkaarUtilities


;======================================================================================;
;  PROPERTIES  /
;=============/
ImageSpaceModifier property TrapImod auto
{IsMod applied when we trap a soul}
sound property TrapSoundFX auto ; create a sound property we'll point to in the editor
{Sound played when we trap a soul}
VisualEffect property TargetVFX auto
{Visual Effect on Target aiming at Caster}
VisualEffect property CasterVFX auto
{Visual Effect on Caster aming at Target}
EffectShader property CasterFXS auto
{Effect Shader on Caster during Soul trap}
EffectShader property TargetFXS auto
{Effect Shader on Target during Soul trap}
bool property bIsEnchantmentEffect = false auto
{Set this to true if this soul trap is on a weapon enchantment or a spell that can do damage to deal with a fringe case}

Actor Property SkaarStarSoulGemNPC Auto
{A dummy NPC used to fill Azura's Star and the Black Star}

Faction Property CreatureFaction Auto 
Faction Property DaedraFaction Auto

;======================================================================================;
;  VARIABLES   /
;=============/
actor CasterActor
actor Victim
; objectreference playerref
bool DeadAlready = FALSE
bool bUseWait = True
;======================================================================================;
;  EVENTS      /
;=============/

Event OnEffectStart(Actor Target, Actor Caster)
	victim = target
	CasterActor = Caster
	if bIsEnchantmentEffect == False
		DeadAlready = Victim.IsDead()
	endif
	bUseWait = False
EndEvent


Event OnEffectFinish(Actor Target, Actor Caster)
	if victim
		if bUseWait 
			Utility.Wait(0.25)
		endif
		if DeadAlready == False
			bool isHuman = true
			If(Target.IsInFaction(CreatureFaction) || Target.IsInFaction(DaedraFaction))
				isHuman = false
			EndIf

			; SKSE work to determine which soul gem to fill
			Form[] returnValues = new Form[3]
			bool tryAzurasStar = false
			bool tryBlackStar = false
			SkaarSoulTrap(Caster, Target, isHuman, returnValues)
			Form soulGemToRemove = returnValues[0]
			Form soulGemToAdd = returnValues[1]
			Form starForm = returnValues[2]

			bool starTrapWorked = false

			; Azura's Star and Black Star
			If(starForm)
				Int starID = 0
				If(starForm.GetFormID() == 0x00063B27)
					starID = 0x00063B27
				Else
					starID = 0x00063B29
				EndIf

				Caster.RemoveItem(Game.GetFormEx(starID), 1, true, SkaarStarSoulGemNPC)
				If SkaarStarSoulGemNPC.TrapSoul(Target) == true
					Debug.Trace("Skaar: Soul trap star worked")
					starTrapWorked = true
					Form newGem = Game.GetFormEx(starID)
					JFormDB.setForm(newGem, ".skaarsoulgems.capturedSoul", Target.GetActorBase())
					SoulTrapSuccess(Target, Caster, newGem)
				EndIf
				SkaarStarSoulGemNPC.RemoveItem(Game.GetFormEx(starID), 1, true, Caster)	; Put it back in the player's inventory regardless of success
			EndIf

			; Regular soul gems
			If(soulGemToRemove && !starTrapWorked)
				Caster.RemoveItem(soulGemToRemove, 1, true, None)
				ObjectReference newGem = Caster.PlaceAtMe(soulGemToAdd, 1, true, true)
				JFormDB.setForm(newGem, ".skaarsoulgems.capturedSoul", Target.GetActorBase())

				SkaarNameSoulGem(newGem, Target)
				Caster.AddItem(newGem, 1, true)
				newGem.Enable(false)

				SoulTrapSuccess(Target, Caster, newGem)
			endif
		else
			trace(self + "tried to soulTrap, but " + victim + " is already Dead.")
		endif
	endif
endEvent

Function SoulTrapSuccess(Actor Target, Actor Caster, Form newGem)
	SkaarSaveJContainers()

	trace(victim + " is, in fact, dead.  Play soul trap visFX")
	TrapSoundFX.play(Caster)       		; play TrapSoundFX sound from player
	TrapImod.apply()                    ; apply isMod at full strength
	TargetVFX.Play(victim,4.7,Caster)   ; Play TargetVFX and aim them at the player
	CasterVFX.Play(Caster,5.9,victim)
	TargetFXS.Play(victim,2)    		; Play Effect Shaders
	CasterFXS.Play(Caster,3)
EndFunction
