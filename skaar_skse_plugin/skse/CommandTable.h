#pragma once

#include <vector>

struct ParamInfo;
class TESObjectREFR;
class Script;
class ScriptLocals;
class ScriptLineBuffer;
class ScriptBuffer;

#define COMMAND_ARGS		ParamInfo * paramInfo, void * scriptData, TESObjectREFR * thisObj, TESObjectREFR * containingObj, Script * scriptObj, ScriptLocals * locals, double * result, UInt32 * opcodeOffsetPtr
#define COMMAND_ARGS_EVAL	TESObjectREFR * thisObj, void * arg1, void * arg2, double * result

typedef bool (* Cmd_Execute)(COMMAND_ARGS);
bool Cmd_Default_Execute(COMMAND_ARGS);

typedef bool (* Cmd_Parse)(UInt32 numParams, ParamInfo * paramInfo, ScriptLineBuffer * lineBuf, ScriptBuffer * scriptBuf);
bool Cmd_Default_Parse(UInt32 numParams, ParamInfo * paramInfo, ScriptLineBuffer * lineBuf, ScriptBuffer * scriptBuf);

typedef bool (* Cmd_Eval)(COMMAND_ARGS_EVAL);
bool Cmd_Default_Eval(COMMAND_ARGS_EVAL);

typedef bool (* _ExtractArgs)(ParamInfo * paramInfo, void * scriptData, UInt32 * arg2, TESObjectREFR * arg3, TESObjectREFR * arg4, Script * script, ScriptLocals * eventList, ...);
extern const _ExtractArgs ExtractArgs;

#ifdef RUNTIME
#define HANDLER(x)	x
#define HANDLER_EVAL(x)	x
#else
#define HANDLER(x)	Cmd_Default_Execute
#define HANDLER_EVAL(x)	Cmd_Default_Eval
#endif

#define DEFINE_CMD_FULL(name, altName, description, refRequired, numParams, paramInfo, parser) \
	extern bool Cmd_ ## name ## _Execute(COMMAND_ARGS); \
	static CommandInfo (kCommandInfo_ ## name) = { \
	#name, \
	#altName, \
	0, \
	#description, \
	refRequired, \
	numParams, \
	paramInfo, \
	HANDLER(Cmd_ ## name ## _Execute), \
	parser, \
	NULL, \
	0 \
}

#define DEFINE_CMD_ALT(name, altName, description, refRequired, numParams, paramInfo) \
	DEFINE_CMD_FULL(name, altName, description, refRequired, numParams, paramInfo, Cmd_Default_Parse)	

#define DEFINE_COMMAND(name, description, refRequired, numParams, paramInfo) \
	DEFINE_CMD_FULL(name, , description, refRequired, numParams, paramInfo, Cmd_Default_Parse)	

#define DEFINE_COMMAND_PLUGIN(name, description, refRequired, numParams, paramInfo) \
	DEFINE_CMD_FULL(name, , description, refRequired, numParams, paramInfo, NULL)

// for commands which can be used as conditionals
#define DEFINE_CMD_COND(name, description, refRequired, paramInfo) \
	extern bool Cmd_ ## name ## _Execute(COMMAND_ARGS); \
	extern bool Cmd_ ## name ## _Eval(COMMAND_ARGS_EVAL); \
	static CommandInfo (kCommandInfo_ ## name) = { \
	#name,	\
	"",		\
	0,		\
	#description,	\
	refRequired,	\
	(sizeof(paramInfo) / sizeof(ParamInfo)),	\
	paramInfo,	\
	HANDLER(Cmd_ ## name ## _Execute),	\
	Cmd_Default_Parse,	\
	HANDLER_EVAL(Cmd_ ## name ## _Eval),	\
	1	\
}

// internal name SCRIPT_PARAMETER
struct ParamInfo
{
	const char	* typeStr;
	UInt32		typeID;		// ParamType
	UInt32		isOptional;	// do other bits do things?
};

struct CommandInfo
{
	const char	* longName;		// 00
	const char	* shortName;	// 04
	UInt32		opcode;			// 08
	const char	* helpText;		// 0C
	UInt16		needsParent;	// 10
	UInt16		numParams;		// 12
	ParamInfo	* params;		// 14

	// handlers
	Cmd_Execute	execute;		// 18
	Cmd_Parse	parse;			// 1C
	Cmd_Eval	eval;			// 20

	UInt32		flags;			// 24
};

class CommandTable
{
public:
	CommandTable();
	~CommandTable();

	struct PatchLocation
	{
		UInt32	ptr;
		UInt32	offset;
		UInt32	type;
	};

	struct PatchSet
	{
		const PatchLocation * start;
		const PatchLocation * end;
		const PatchLocation * maxIdx;
	};

	void	Init(UInt32 baseID, UInt32 sizeHint = 0);
	void	Read(const CommandInfo * start, const CommandInfo * end);

	void	Add(const CommandInfo * cmd = NULL);	// add at cur ID
	UInt32	GetID(void)	{ return m_curID; }

	void	PatchEXE(const PatchSet * patches);

	CommandInfo * Begin() { return &m_commands[0]; }
	CommandInfo * End() { return &m_commands[0] + m_commands.size(); }

private:
	typedef std::vector <CommandInfo>	CommandList;
	CommandList	m_commands;

	UInt32	m_baseID;	// should be the same as m_commands[0].opcode when the table is filled
	UInt32	m_curID;
	bool	m_patched;

	void	ApplyPatch(const PatchLocation * patch, UInt32 newData);
};
