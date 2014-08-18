scriptName SkaarSpecialInventoryCrafting Hidden

Function SkaarAddItemsFromContainerToContainer(ObjectReference pSourceContainerRef, ObjectReference pDestContainerRef, Int typeID) global native
{Add all items of type typeID to in pSourceContainerRef to pDestcontainerRef}

Function SkaarRemoveItemsInContainerFromContainer(ObjectReference pInContainerRef, ObjectReference pFromContainerRef, Int typeID) global native
{Remove all items in pInContainerRef of type typeID from pFromContainerRef}
