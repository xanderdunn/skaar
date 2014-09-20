scriptname SkaarSoulDisplayNPC extends ReferenceAlias

import SkaarUtilities

Faction Property PlayerFaction Auto

Event OnHit(ObjectReference akAggressor, Form akSource, Projectile akProjectile, bool abPowerAttack, bool abSneakAttack, bool abBashAttack, bool abHitBlocked)
	(self.GetReference() as Actor).StopCombat()
	(akAggressor as Actor).StopCombat()
	;SkaarUtilities.MakeFriendly(self.GetReference() as Actor, PlayerFaction)
EndEvent

Event OnCombatStateChanged(Actor akTarget, int aeCombatState)
	(self.GetReference() as Actor).StopCombat()
	If (akTarget)
		akTarget.StopCombat()
		;SkaarUtilities.MakeFriendly(self.GetReference() as Actor, PlayerFaction)
	EndIf
EndEvent
