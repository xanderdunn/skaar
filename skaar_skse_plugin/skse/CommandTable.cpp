#include "CommandTable.h"
#include "SafeWrite.h"

const Cmd_Parse		g_defaultParseCommand =		(Cmd_Parse)0x00517F40;
const _ExtractArgs	ExtractArgs =				(_ExtractArgs)0x00514830;

static const CommandInfo kPaddingCommand =
{
	"", "",
	0,
	"command used for padding",
	0,
	0,
	NULL,

	Cmd_Default_Execute,
	NULL,
	NULL,
	NULL
};

bool Cmd_Default_Execute(COMMAND_ARGS)
{
	return true;
}

bool Cmd_Default_Eval(COMMAND_ARGS_EVAL)
{
	return true;
}

bool Cmd_Default_Parse(UInt32 numParams, ParamInfo * paramInfo, ScriptLineBuffer * lineBuf, ScriptBuffer * scriptBuf)
{
	return g_defaultParseCommand(numParams, paramInfo, lineBuf, scriptBuf);
}

CommandTable::CommandTable()
:m_baseID(0), m_curID(0), m_patched(false)
{
	//
}

CommandTable::~CommandTable()
{

}

void CommandTable::Init(UInt32 baseID, UInt32 sizeHint)
{
	ASSERT(m_commands.empty());

	m_baseID = baseID;
	m_curID = baseID;

	if(sizeHint)
		m_commands.reserve(sizeHint);
}

void CommandTable::Read(const CommandInfo * start, const CommandInfo * end)
{
	for(const CommandInfo * iter = start ; iter != end; ++iter)
	{
		ASSERT(iter->opcode == m_curID);

		m_commands.push_back(*iter);
		m_curID++;
	}
}

void CommandTable::Add(const CommandInfo * cmdSrc)
{
	if(!cmdSrc) cmdSrc = &kPaddingCommand;

	CommandInfo	cmd = *cmdSrc;

	cmd.opcode = m_curID;
	m_curID++;

	if(!cmd.eval) cmd.eval = Cmd_Default_Eval;
	if(!cmd.parse) cmd.parse = Cmd_Default_Parse;

	m_commands.push_back(cmd);
}

void CommandTable::PatchEXE(const PatchSet * patches)
{
	ApplyPatch(patches->start, (UInt32)&m_commands.front());
	ApplyPatch(patches->end, (UInt32)&m_commands.back());
	ApplyPatch(patches->maxIdx, m_commands.size() + m_baseID);
}

void CommandTable::ApplyPatch(const PatchLocation * patch, UInt32 newData)
{
	for(; patch->ptr; ++patch)
	{
		switch(patch->type)
		{
		case 0:
			SafeWrite32(patch->ptr, newData + patch->offset);
			break;

		case 1:
			SafeWrite16(patch->ptr, newData + patch->offset);
			break;
		}
	}
}
