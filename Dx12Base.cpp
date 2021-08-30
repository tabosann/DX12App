
#include<Dx12Base.h>
#include<impl\Dx12Tasks_Impl.h>
#include<DebugTools.h>

using namespace std;

Dx12Base::Dx12Base(HWND hwnd) : _pTask(new Dx12TaskBase(hwnd)) {}

Dx12Base::~Dx12Base() { if (_pTask != nullptr) { delete _pTask; _pTask = nullptr; } }

//--------------------------------------------------------------------------------------------------------------------------
//
// これ以降はタスク構造を実装するものなので、あまり気にしなくて良い
//
//--------------------------------------------------------------------------------------------------------------------------

// タスクの登録、実行、削除を同時に実行
#define EXCUTE_ALL_TASK(msg, pTask)\
		{								\
			RegisterTask(msg, pTask);	\
			ExcuteTask(msg);			\
			ReleaseTask(msg);			\
		}

Dx12TaskBase::Dx12TaskBase(HWND hwnd)
	:_pParam(new Dx12Component(hwnd))
{
	EXCUTE_ALL_TASK(TaskID::CREATE_FACTORY_6,				new Dx12CreateFactory);
	EXCUTE_ALL_TASK(TaskID::CREATE_DEVICE,					new Dx12CreateDevice);
	EXCUTE_ALL_TASK(TaskID::CREATE_COMMAND_ALLOC,			new Dx12CreateCmdAlloc);
	EXCUTE_ALL_TASK(TaskID::CREATE_GRAPHIC_COMMAND_LIST,	new Dx12CreateGraphicCmdList);
	EXCUTE_ALL_TASK(TaskID::CREATE_COMMAND_QUEUE,			new Dx12CreateCmdQueue);
	EXCUTE_ALL_TASK(TaskID::CREATE_SWAPCHAIN,				new Dx12CreateSwapChain);
	EXCUTE_ALL_TASK(TaskID::CREATE_RTV,						new Dx12CreateRtv);
}
#undef EXCUTE_ALL_TASK

Dx12TaskBase::~Dx12TaskBase()
{
	// パラメータの解放
	if (_pParam != nullptr) { delete _pParam; _pParam = nullptr; }

	// タスクオブジェクトの解放
	for (TaskTable::iterator it = _taskTable.begin(); it != _taskTable.end(); ++it)
		if (it->second != nullptr) { delete it->second; it->second = nullptr; }

	_taskTable.clear();
}

void Dx12TaskBase::RegisterTask(_Key msg, _Task pTask)
{
	_taskTable.insert(TaskElem(msg, pTask));
}

HRESULT Dx12TaskBase::ExcuteTask(_Key msg)
{
	TaskTable::iterator it = _taskTable.find(msg);
	if (it != _taskTable.end())
		return it->second->Task(this);
	return E_ABORT;
}

void Dx12TaskBase::ReleaseTask(_Key msg)
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
