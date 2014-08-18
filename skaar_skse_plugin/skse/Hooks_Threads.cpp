#include "Hooks_Threads.h"
#include "SafeWrite.h"
#include "Utilities.h"
#include "GameThreads.h"
#include "common/ICriticalSection.h"
#include <queue>

ICriticalSection		s_taskQueueLock;
std::queue<TaskDelegate*>	s_tasks;

void BSTaskPool::QueueTask(TaskDelegate * cmd)
{
	s_taskQueueLock.Enter();
	s_tasks.push(cmd);
	s_taskQueueLock.Leave();
}

void BSTaskPool::ProcessTasks()
{
	CALL_MEMBER_FN(this, ProcessTaskQueue_HookTarget)();

	s_taskQueueLock.Enter();
	while (!s_tasks.empty())
	{
		TaskDelegate * cmd = s_tasks.front();
		s_tasks.pop();

		cmd->Run();
		cmd->Dispose();
	}
	s_taskQueueLock.Leave();
}

void TaskInterface::AddTask(TaskDelegate * task)
{
	BSTaskPool * taskPool = BSTaskPool::GetSingleton();
	if(taskPool) {
		taskPool->QueueTask(task);
	}
}

void Hooks_Threads_Init(void)
{
	
}

void Hooks_Threads_Commit(void)
{
	WriteRelCall(0x006910F0 + 0x016, GetFnAddr(&BSTaskPool::ProcessTasks));
	WriteRelCall(0x0069CBD0 + 0x578, GetFnAddr(&BSTaskPool::ProcessTasks));
}
