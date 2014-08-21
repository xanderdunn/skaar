scriptName  SkaarWorkbenchContainersPlayer extends ReferenceAlias
{Add this script to the Player object to remove items from the workbench container while it's being used}
; Usage: Add to Player, connect GlobalVariable SkaarWorkingAtWorkbench, and set the container as the LinkedRef on the workbench

import SkaarSpecialInventoryCrafting

ObjectReference Property Workbench Auto Hidden			; The workbench that's being used

Event OnItemRemoved(Form akBaseItem, int aiItemCount, ObjectReference akItemReference, ObjectReference akDestContainer)
	ObjectReference SourceContainerRef = Workbench.GetLinkedRef()		; The workbench's source container
	SourceContainerRef.RemoveItem(akBaseItem, aiItemCount, true, None)	; When an item is used during workbench use, also remove it from the workbench's container
EndEvent

Event OnGetUp(ObjectReference akFurniture)
	SkaarSpecialInventoryCrafting.SkaarRemoveItemsInContainerFromContainer(Workbench.GetLinkedRef(), self.GetReference(), 30) ; 30 specifies only ingredients
	self.GetOwningQuest().Reset()
EndEvent

Event OnSit(ObjectReference akFurniture)	
	Workbench = akFurniture
EndEvent
