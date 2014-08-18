scriptName  SkaarSpecialInventoryWorkbench extends ObjectReference
{Make a workbench reference access items from a designated container in addition to the player's inventory}
import Debug
import SkaarSpecialInventoryCrafting

ObjectReference Property SourceContainerRef Auto	; Container with the player's crafting materials
GlobalVariable Property SkaarWorkingAtWorkbench Auto

Event OnActivate ( objectReference triggerRef )
	Debug.Trace("I am " + self)
	SkaarWorkingAtWorkbench.SetValue(1)
	SkaarSpecialInventoryCrafting.SkaarAddItemsFromContainerToContainer(SourceContainerRef, triggerRef, 30)
endEvent

Function Copypasta(ObjectReference akSourceContainer = None, ObjectReference akDestContainer = None, Int aiIndex = 0, Form akForm = None)
	aiIndex = akSourceContainer.GetNumItems()
	While (aiIndex > 0)
		aiIndex -= 1
		akForm = akSourceContainer.GetNthForm(aiIndex)
		akDestContainer.AddItem(akForm, akSourceContainer.GetItemCount(akForm))
	EndWhile
EndFunction
