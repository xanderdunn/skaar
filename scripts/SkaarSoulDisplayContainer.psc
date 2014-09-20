scriptname SkaarSoulDisplayContainer extends ObjectReference
{Activate the Soul Display quest when a filled soul gem is put into the soul display container}

import SkaarSoulDisplayGem
import SkaarUtilities

Int Property kSoulGem = 52 AutoReadOnly	; From FormType.psc
Int Property soulDisplayPosition Auto
{This integer assigned in the Creation Kit dictates which Soul Display Quest Alias will be filled for its created soul}
Faction property PlayerFaction Auto

Function MakeFriendly(Actor actorToBeFriend)
	actorToBeFriend.AllowPCDialogue(false)
	actorToBeFriend.MakePlayerFriend()
	actorToBeFriend.SetRelationshipRank(Game.GetPlayer(), 4)
	actorToBeFriend.RemoveFromAllFactions()
	actorToBeFriend.AddToFaction(PlayerFaction)
	actorToBeFriend.SetAllowFlying(false)
EndFunction

; TODO: Remove this.  It's only for testing
Event OnInit()
	Spell soulTrapSpell = Game.GetForm(0x0004dba4) as Spell
	Game.GetPlayer().AddSpell(soulTrapSpell, true)
EndEvent

Event OnItemAdded(Form akBaseItem, int aiItemCount, ObjectReference akItemReference, ObjectReference akSourceContainer)
	If (akBaseItem.GetType() == kSoulGem)
		Form capturedSoul = JFormDB.getForm(akItemReference, ".skaarsoulgems.capturedSoul")
		If(!capturedSoul) ; For Azura's Star and the  Black Star which are stored by Form rather than ObjectReference
			capturedSoul = JFormDB.getForm(akBaseItem, ".skaarsoulgems.capturedSoul")
		EndIf

		ObjectReference placeMarker = self.GetLinkedRef()
		ObjectReference placedSoul = placeMarker.PlaceAtMe(capturedSoul, 1, true, true)
		JFormDB.setForm(akItemReference, ".skaarsoulgems.placedSoul", placedSoul)
		JFormDB.setInt(akItemReference, ".skaarsoulgems.soulDisplayPosition", soulDisplayPosition)
		SkaarSaveJContainers()

		ReferenceAlias soulDisplayReferenceAlias = Quest.GetQuest("SkaarSoulDisplayQuest").GetNthAlias(soulDisplayPosition) as ReferenceAlias
		soulDisplayReferenceAlias.ForceRefTo(placedSoul)
		MakeFriendly(placedSoul as Actor)
		placedSoul.Enable(true)
	EndIf
EndEvent

Event OnItemRemoved(Form akBaseItem, int aiItemCount, ObjectReference akItemReference, ObjectReference akDestContainer)
	ObjectReference placedSoul = JFormDB.getForm(akItemReference, ".skaarsoulgems.placedSoul") as ObjectReference
	placedSoul.Disable(true)
	placedSoul.Delete()
	placedSoul = None

	JFormDB.setObj(akItemReference, ".skaarsoulgems.placedSoul", 0)
	JFormDB.setObj(akItemReference, ".skaarsoulgems.soulDisplayPosition", 0)
	SkaarSaveJContainers()

	ReferenceAlias soulDisplayReferenceAlias = Quest.GetQuest("SkaarSoulDisplayQuest").GetNthAlias(soulDisplayPosition) as ReferenceAlias
	soulDisplayReferenceAlias.Clear()
EndEvent
