#include "Hooks_UI.h"
#include "Hooks_Threads.h"
#include "SafeWrite.h"
#include "Utilities.h"
#include "GameMenus.h"
#include "common/ICriticalSection.h"
#include <queue>

ICriticalSection		s_queueLock;
std::queue<UIDelegate*>	s_commands;

void UIManager::QueueCommand(UIDelegate * cmd)
{
	s_queueLock.Enter();
	s_commands.push(cmd);
	s_queueLock.Leave();
}

void UIManager::ProcessCommands(void)
{
	CALL_MEMBER_FN(this, ProcessEventQueue_HookTarget)();
	
	s_queueLock.Enter();
	while (!s_commands.empty())
	{
		UIDelegate * cmd = s_commands.front();
		s_commands.pop();

		cmd->Run();
		cmd->Dispose();
	}
	s_queueLock.Leave();
}

void TaskInterface::AddUITask(UIDelegate * task)
{
	UIManager * uiManager = UIManager::GetSingleton();
	if(uiManager) {
		uiManager->QueueCommand(task);
	}
}

void Hooks_UI_Commit(void)
{
	WriteRelCall(0x00A5D370 + 0x0808, GetFnAddr(&UIManager::ProcessCommands));
}
