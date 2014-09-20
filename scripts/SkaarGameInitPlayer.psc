scriptname SkaarGameInitPlayer extends ReferenceAlias
{Initilization and scripts that always run on the player for the Skaar mod}

import SkaarMagicSoulTrapFXScript
import SkaarUtilities

Actor Property SkaarStarSoulGemNPC Auto
{A dummy NPC used to fill Azura's Star and the Black Star}

int Property kSoulGem = 52 AutoReadOnly
Actor Property Player Auto
{Set this to the Player}

; Event OnPlayerLoadGame()
	; int allSoulGemsArray = JFormMap.allKeys(JDB.solveObj(".skaarsoulgems"))

	; Int soulGemsCount = JArray.count(allSoulGemsArray)
	; While(soulGemsCount > 0)
	; 	ObjectReference soulGemForm = JArray.getForm(allSoulGemsArray, soulGemsCount - 1) as ObjectReference
	; 	If(!soulGemForm)
	; 		Debug.Trace("This soul gem has been consumed")
	; 	Else
	; 		Debug.Trace("Found soul gem: " + soulGemForm)
	; 	EndIf
	; 	soulGemsCount -= 1
	; EndWhile
	; JDB.writeToFile(JContainers.userDirectory() + "skaar/skaar.json")
; EndEvent

; Set the display name of Azura's Star and the Black Star for the soul display portion of the mod
Event OnItemAdded(Form akBaseItem, int aiItemCount, ObjectReference akItemReference, ObjectReference akSourceContainer)
	If(akBaseItem.GetType() == kSoulGem)
		Int formID = akBaseItem.GetFormID()
		If(formID == 0x00063B27 || formID == 0x00063B29)	; Just added Azura's Star or Black Star to inventory.  Make sure it's named correctly
			Form capturedSoul = JFormDB.getForm(akBaseItem, ".skaarsoulgems.capturedSoul")
			If(capturedSoul)
				NameSoulGem(akItemReference, capturedSoul)
			Else
				SkaarNameSoulGem(akItemReference, None)	; Reset name to original
			EndIf
		ElseIf(SkaarSoulGemIsFilled(akBaseItem) && !JFormDB.getForm(akItemReference, ".skaarsoulgems.capturedSoul") && !(formID == 0x00063B27 || formID == 0x00063B29))	; 
			Debug.Trace("Skaar: Picked up soul gem that is filled and not in the database: " + akBaseItem)
				Form capturedSoul = SkaarGetSoulForVanillaFilledSoulGem(akBaseItem)
				If(capturedSoul)
					Player.RemoveItem(akItemReference, 1, true, SkaarStarSoulGemNPC)
					ObjectReference newGem = Player.PlaceAtMe(akBaseItem, 1, true, true)
					JFormDB.setForm(newGem, ".skaarsoulgems.capturedSoul", capturedSoul)
					NameSoulGem(newGem, capturedSoul)
					Player.AddItem(newGem, 1, true)
					SkaarSaveJContainers()
				EndIf

		EndIf
	EndIf
EndEvent

Function NameSoulGem(ObjectReference mySoulGem, Form capturedSoul)
	Debug.Trace("Try to name " + mySoulGem + ", with captured soul " + capturedSoul)
	ObjectReference capturedSoulInstance = Player.PlaceAtMe(capturedSoul, 1, false, true)
	Debug.Trace("Got capturedSoulInstance = " + capturedSoulInstance)
	SkaarNameSoulGem(mySoulGem, capturedSoulInstance)
	capturedSoulInstance.Delete()
	capturedSoulInstance = None
EndFunction
