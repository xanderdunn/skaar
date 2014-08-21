scriptName  SkaarSpecialInventoryWorkbench extends ObjectReference
{Make a workbench reference access items from a designated container in addition to the player's inventory}
; Connect to the workbench, connect GlobalVariable SkaarWorkingAtWorkbench, and set the container as the LinkedRef on it

import SkaarSpecialInventoryCrafting

GlobalVariable Property SkaarWorkingAtWorkbench Auto

Event OnActivate ( objectReference triggerRef )
	SkaarWorkingAtWorkbench.SetValue(1)
	SkaarSpecialInventoryCrafting.SkaarAddItemsFromContainerToContainer(self.GetLinkedRef(NONE), triggerRef, 30)
endEvent
