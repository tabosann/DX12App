
#include<Dx12Base.h>
#include<impl\Dx12EndTasks_Impl.h>
#include<DebugTools.h>

using namespace std;

//--------------------------------------------------------------------------------------------------------------------------
//
// これ以降はタスク構造を実装するものなので、あまり気にしなくて良い
//
//--------------------------------------------------------------------------------------------------------------------------

// タスクの登録、実行、削除を同時に実行
#define EXCUTE_TASK_SEQUENCE(msg, pTask)\
		{								\
			RegisterTask(msg, pTask);	\
			ExcuteTask(msg);			\
			ReleaseTask(msg);			\
		}

Dx12TaskBase::Dx12TaskBase(Dx12Component* pCmpt)
	:_pCmpt(pCmpt)
{
	EXCUTE_TASK_SEQUENCE(TaskID::CREATE_FACTORY,		new Dx12CreateFactory);
	EXCUTE_TASK_SEQUENCE(TaskID::CREATE_DEVICE,			new Dx12CreateDevice);
	EXCUTE_TASK_SEQUENCE(TaskID::CREATE_COMMAND_ALLOC,	new Dx12CreateCmdAlloc);
	EXCUTE_TASK_SEQUENCE(TaskID::CREATE_COMMAND_LIST,	new Dx12CreateGraphicCmdList);
	EXCUTE_TASK_SEQUENCE(TaskID::CREATE_COMMAND_QUEUE,	new Dx12CreateCmdQueue);
	EXCUTE_TASK_SEQUENCE(TaskID::CREATE_SWAPCHAIN,		new Dx12CreateSwapChain);
	EXCUTE_TASK_SEQUENCE(TaskID::CREATE_RTV,			new Dx12CreateRtv);
	EXCUTE_TASK_SEQUENCE(TaskID::CREATE_FENCE,			new Dx12CreateFence);

	RegisterTask(TaskID::RENDER_BEGIN, new Dx12RenderBegin);
	RegisterTask(TaskID::GPU_WAIT, new Dx12GpuWait);
}
#undef EXCUTE_TASK_SEQUENCE

Dx12TaskBase::~Dx12TaskBase()
{
	// パラメータの解放
	if (_pCmpt != nullptr) { delete _pCmpt; _pCmpt = nullptr; }

	// タスクオブジェクトの解放
	for (TaskTable::iterator it = _taskTable.begin(); it != _taskTable.end(); ++it)
		if (it->second != nullptr) { delete it->second; it->second = nullptr; }

	_taskTable.clear();
}

void Dx12TaskBase::RegisterTask(KeyVal msg, TaskVal pTask)
{
	_taskTable.insert(TaskElem(msg, pTask));
}

HRESULT Dx12TaskBase::ExcuteTask(KeyVal msg)
{
	TaskTable::iterator it = _taskTable.find(msg);
	if (it != _taskTable.end())
		return it->second->Task(this);
	return E_ABORT;
}

void Dx12TaskBase::ReleaseTask(KeyVal msg)
{
	TaskTable::iterator it = _taskTable.find(msg);
	if (it != _taskTable.end())
	{
		if (it->second != nullptr)
		{
			delete it->second;
			it->second = nullptr;
		}
		_taskTable.erase(it);
	}
}
