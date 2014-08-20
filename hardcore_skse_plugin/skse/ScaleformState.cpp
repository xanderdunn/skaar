#include "ScaleformState.h"
#include "skse/GameAPI.h"

const BSScaleformTranslator::_GetCachedString BSScaleformTranslator::GetCachedString = (BSScaleformTranslator::_GetCachedString)0x00A51800;

void SKSEGFxLogger::LogMessageVarg(UInt32 messageType, const char* fmt, va_list args)
{
	gLog.Log(IDebugLog::kLevel_Message, fmt, args);
}