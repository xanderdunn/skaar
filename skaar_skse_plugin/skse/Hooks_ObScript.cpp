#include "Hooks_ObScript.h"
#include "CommandTable.h"
#include "GameAPI.h"
#include "skse_version.h"
#include "PapyrusVM.h"

CommandTable	g_commandTable;

const CommandInfo	* g_blockTypeStart =		(CommandInfo *)0x0124B1A8;
const CommandInfo	* g_blockTypeEnd =			(CommandInfo *)0x0124B720;
const CommandInfo	* g_consoleCommandsStart =	(CommandInfo *)0x0124B748;
const CommandInfo	* g_consoleCommandsEnd =	(CommandInfo *)0x0124E858;
const CommandInfo	* g_scriptCommandsStart =	(CommandInfo *)0x0124E880;
const CommandInfo	* g_scriptCommandsEnd =		(CommandInfo *)0x01255B30;

static bool IsEmptyStr(const char * data)
{
	return !data || !data[0];
}

void DumpCommands(const CommandInfo * start, const CommandInfo * end)
{
	for(const CommandInfo * iter = start; iter < end; ++iter)
	{
		std::string	line;

		if(!iter->eval || IsEmptyStr(iter->longName)) continue;

		line = iter->longName;

		if(!IsEmptyStr(iter->shortName))
		{
			line += ", ";
			line += iter->shortName;
		}

		if(iter->numParams)
		{
			ParamInfo	* params = iter->params;

			line += ": ";

			for(UInt32 i = 0; i < iter->numParams; i++)
			{
				ParamInfo	* param = &params[i];

				if(i) line += ", ";

				if(param->isOptional) line += "(";

				line += param->typeStr;

				if(param->isOptional) line += ")";
			}
		}

		if(iter->flags)
			line += " [cond]";

		_MESSAGE("%04X %s", iter->opcode, line.c_str());

#if 0
		if(!IsEmptyStr(iter->helpText))
		{
			gLog.Indent();
			_MESSAGE("%s", iter->helpText);
			gLog.Outdent();
		}
#endif
	}
}

void ObScript_DumpCommands(void)
{
	DumpCommands(g_commandTable.Begin(), g_commandTable.End());
}

static const CommandTable::PatchLocation kPatch_ScriptCommands_Start[] =
{
	{	0x00516B0B, 0x00 },
	{	0x00516CD4, 0x04 },
	{	0x00516D6D, 0x08 },
	{	0x00517D87, 0x00 },
	{	0x0051B33F, 0x00 },
	{	0x00542459, 0x0C },
	{	0x00542471, 0x00 },
	{	0x00542489, 0x04 },
	{	0x005424A5, 0x0C },
	{	0x005424BF, 0x04 },
	{	0x005424DD, 0x00 },
	{	0x005424FF, 0x00 },
	{	0x0054251D, 0x04 },
	{	0x00542543, 0x00 },
	{	0x0054255A, 0x00 },
	{	0x005EA479, 0x20 },
	{	0x005EA485, 0x10 },
	{	0x005EA54E, 0x12 },
	{	0x005EA591, 0x12 },
	{	0x005EA59D, 0x14 },
	{	0x005EA5D8, 0x12 },
	{	0x005EA5E5, 0x14 },
	{	0x005EA646, 0x12 },
	{	0x005EA652, 0x14 },
	{	0x005EA680, 0x12 },
	{	0x005EA6F1, 0x12 },
	{	0x005EA723, 0x12 },
	{	0x005EA72F, 0x14 },
	{	0x005EA785, 0x12 },
	{	0x005EA7C3, 0x12 },
	{	0x005EA7CF, 0x14 },
	{	0x005EA878, 0x12 },
	{	0x005EA8A7, 0x14 },
	{	0x005EA984, 0x12 },
	{	0x005EA9A6, 0x14 },
	{	0x005EAABE, 0x12 },
	{	0x005EAB08, 0x12 },
	{	0x005EAB18, 0x14 },
	{	0x005EABB1, 0x14 },
	{	0x006A2D0C, 0x12 },
	{	0x006A2D39, 0x14 },
	{	0 },
};

static const CommandTable::PatchLocation kPatch_ScriptCommands_End[] =
{
	{	0x00516C12, 0x08 },
	{	0x00516CFF, 0x04 },
	{	0 },
};

static const CommandTable::PatchLocation kPatch_ScriptCommands_MaxIdx[] =
{
	{	0x00502C10 + 0x0087 + 1,	0 },
	{	0x00516AD0 + 0x0029 + 2,	(UInt32)(-0x1001) },
	{	0x00516C10 + 0x0000 + 6,	0 },
	{	0x00517C20 + 0x0156 + 1,	(UInt32)(-0x1001) },
	{	0x0051ABE0 + 0x0751 + 2,	(UInt32)(-0x1001) },
	{	0x005E98C0 + 0x000D + 2,	(UInt32)(-0x1001) },
	{	0x005E98C0 + 0x001D + 1,	(UInt32)(-0x1001) },
	{	0x005E98C0 + 0x0029 + 2,	(UInt32)(-0x1001) },

	{	0 },
};

static const CommandTable::PatchSet kPatchSet =
{
	kPatch_ScriptCommands_Start,
	kPatch_ScriptCommands_End,
	kPatch_ScriptCommands_MaxIdx
};

// core commands
static void PrintVersion(void)
{
	if(IsConsoleMode())
	{
		Console_Print("SKSE version: %d.%d.%d, release idx %d, runtime %08X",
			SKSE_VERSION_INTEGER, SKSE_VERSION_INTEGER_MINOR, SKSE_VERSION_INTEGER_BETA,
			SKSE_VERSION_RELEASEIDX, RUNTIME_VERSION);
	}
}

bool Cmd_GetSKSEVersion_Eval(COMMAND_ARGS_EVAL)
{
	*result = SKSE_VERSION_INTEGER;

	PrintVersion();

	return true;
}

bool Cmd_GetSKSEVersion_Execute(COMMAND_ARGS)
{
	return Cmd_GetSKSEVersion_Eval(thisObj, 0, 0, result);
}

bool Cmd_GetSKSEVersionMinor_Eval(COMMAND_ARGS_EVAL)
{
	*result = SKSE_VERSION_INTEGER_MINOR;

	PrintVersion();

	return true;
}

bool Cmd_GetSKSEVersionMinor_Execute(COMMAND_ARGS)
{
	return Cmd_GetSKSEVersionMinor_Eval(thisObj, 0, 0, result);
}

bool Cmd_GetSKSEVersionBeta_Eval(COMMAND_ARGS_EVAL)
{
	*result = SKSE_VERSION_INTEGER;

	PrintVersion();

	return true;
}

bool Cmd_GetSKSEVersionBeta_Execute(COMMAND_ARGS)
{
	return Cmd_GetSKSEVersionBeta_Eval(thisObj, 0, 0, result);
}

bool Cmd_GetSKSERelease_Eval(COMMAND_ARGS_EVAL)
{
	*result = SKSE_VERSION_RELEASEIDX;

	PrintVersion();

	return true;
}

bool Cmd_GetSKSERelease_Execute(COMMAND_ARGS)
{
	return Cmd_GetSKSERelease_Eval(thisObj, 0, 0, result);
}

bool Cmd_ClearInvalidRegistrations_Eval(COMMAND_ARGS_EVAL)
{
	UInt32 count = (*g_skyrimVM)->ClearInvalidRegistrations();

	if(IsConsoleMode())
	{
		Console_Print("Removed %d invalid OnUpdate registration(s)", count);
	}

	*result = count;

	return true;
}

bool Cmd_ClearInvalidRegistrations_Execute(COMMAND_ARGS)
{
	return Cmd_ClearInvalidRegistrations_Eval(thisObj, 0, 0, result);
}

#include "GameData.h"
#include "GameObjects.h"
#include "GameAPI.h"
#include "GameReferences.h"
#include "GameForms.h"
#include "GameRTTI.h"
#include "GameExtraData.h"

#include "PapyrusKeyword.h"
#include "PapyrusForm.h"
#include "PapyrusObjectReference.h"
#include "PapyrusStringUtil.h"

#include "NiSerialization.h"
#include "NiNodes.h"

bool Cmd_SKSETestFunc_Eval(COMMAND_ARGS_EVAL)
{
	NiNode	* node = (*g_thePlayer)->GetNiNode();

	UInt8		niStreamMemory[0x5B4];
	NiStream	* niStream = (NiStream *)niStreamMemory;
	CALL_MEMBER_FN(niStream, ctor)();

	node->IncRef();
	niStream->m_rootObjects.Append(node);

	niStream->SavePath("head.nif");

	CALL_MEMBER_FN(niStream, dtor)();

	return true;	
}

bool Cmd_SKSETestFunc_Execute(COMMAND_ARGS)
{
	return Cmd_SKSETestFunc_Eval(thisObj, 0, 0, result);
}

DEFINE_CMD_COND(GetSKSEVersion, "returns the major SKSE version number", false, NULL);
DEFINE_CMD_COND(GetSKSEVersionMinor, "returns the minor SKSE version number", false, NULL);
DEFINE_CMD_COND(GetSKSEVersionBeta, "returns the beta SKSE version number", false, NULL);
DEFINE_CMD_COND(GetSKSERelease, "returns the SKSE release number", false, NULL);
DEFINE_CMD_COND(ClearInvalidRegistrations, "clears invalid event registrations", false, NULL);
DEFINE_CMD_COND(SKSETestFunc, "", false, NULL);

void Hooks_ObScript_Init(void)
{
	// read vanilla commands
	g_commandTable.Init(0x1000, 0x1480);
	g_commandTable.Read(g_scriptCommandsStart, g_scriptCommandsEnd);

	// pad to 0x1400 to give bethesda room
	// pretty sure obscript is dead, but eh be safe

	while(g_commandTable.GetID() < 0x1400)
		g_commandTable.Add();

	// add our commands
#define CMD(name)	g_commandTable.Add(&kCommandInfo_##name)

	CMD(GetSKSEVersion);
	CMD(GetSKSEVersionMinor);
	CMD(GetSKSEVersionBeta);
	CMD(GetSKSERelease);
	CMD(ClearInvalidRegistrations);

#ifdef _DEBUG
	CMD(SKSETestFunc);
#endif

#undef CMD

	g_commandTable.Add();	// padding command
}

void Hooks_ObScript_Commit(void)
{
	// patch the exe
	g_commandTable.PatchEXE(&kPatchSet);
}
