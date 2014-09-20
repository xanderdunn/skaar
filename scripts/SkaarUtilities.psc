scriptname SkaarUtilities hidden
{Various helper functions for use in other scripts in the Skaar mod}

Function SkaarSaveJContainers() global
	JDB.writeToFile(JContainers.userDirectory() + "skaar/skaar.json")
EndFunction
