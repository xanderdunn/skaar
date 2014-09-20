scriptName  SkaarMoveAcrossEnchantingWorkbench extends ObjectReference
{Move an object in a cycle across the image on the enchanter's workbench}

int property translationStep = 1 auto hidden
{Which step of the total translation the object is currently undergoing, where 1 is the first step}

float property initialX auto hidden
float property initialY auto hidden
float property initialZ auto hidden

Event OnLoad()
	Debug.Trace("Starting to move object")
	self.initialX = self.GetPositionX()
	self.initialY = self.GetPositionY()
	self.initialZ = self.GetPositionZ()
	self.NextTranslationStep()
EndEvent

Event OnTranslationComplete()
	self.NextTranslationStep()
EndEvent

Function NextTranslationStep()
	If (translationStep == 1)
		; -1531.0112, 1412.0574
		self.TranslateTo(self.GetPositionX() + 21.6967, self.GetPositionY() + 23.2563, self.initialZ, self.GetAngleX(), self.GetAngleY(), self.GetAngleZ(), 15, 0)
	ElseIf (translationStep == 2)
		; -1497.5134, 1395.8850
		self.TranslateTo(self.GetPositionX() + 33.4978, self.GetPositionY() - 16.1724, self.initialZ, self.GetAngleX(), self.GetAngleY(), self.GetAngleZ(), 15, 0)
	ElseIf (translationStep == 3)
		; -1510.2629, 1365.4802
		self.TranslateTo(self.GetPositionX() - 12.7495, self.GetPositionY() - 30.4048, self.initialZ, self.GetAngleX(), self.GetAngleY(), self.GetAngleZ(), 15, 0)
	ElseIf (translationStep == 4)
		; -1538.9158, 1361.1577
		self.TranslateTo(self.GetPositionX() - 28.6529, self.GetPositionY() - 4.3225, self.initialZ, self.GetAngleX(), self.GetAngleY(), self.GetAngleZ(), 15, 0)
	ElseIf (translationStep == 5)
		self.TranslateTo(self.initialX, self.initialY, self.initialZ, self.GetAngleX(), self.GetAngleY(), self.GetAngleZ(), 15, 0)
		self.translationStep = 1
		return
	EndIf
	self.translationStep += 1
EndFunction