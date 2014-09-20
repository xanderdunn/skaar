scriptName SkaarOreDecoration extends objectReference
{Make an ore mine appear to contain ore without being able to mine it or requring mining furniture marker}

int Property ResourceCount = 1 Auto
{how many resources you get per drop}

int property ResourceCountTotal = 3 auto
{how many resources this has before it is depleted}

int property ResourceCountCurrent = -1 auto Hidden
{Used to track the current remaining resources}

Event OnInit()
	self.Reset()
	self.clearDestruction()
	self.setDestroyed(False)
	resourceCountCurrent = -1
EndEvent
