#pragma once

//--------------------------------------------------------------------------------------------------------------------------
//
// ◆タスククラスを使ったDx12初期化プログラムの概要
//	 - 'Dx12Componet'クラスで用意されたDx12の初期化に必要なパラメータを、どのように初期化していくか'Dx12TaskBase'クラスで
//	 - 決定し、実装する。ここで、具体的な実装は'Dx12TaskBase'クラスから派生したタスククラスで行う(Dx12Tasks_Impl.hを要参照)。
//	 - 以上の行程で得られるパラメータを、最終的に'Dx12Base'クラスからアクセスできるようにし、'Dx12App'クラスで
//	 - そのパラメータを使ったプログラムを組めるようにしている。
//
//--------------------------------------------------------------------------------------------------------------------------
class Dx12TaskBase;
class Dx12Component;
using Dx12Base = Dx12TaskBase;

//--------------------------------------------------------------------------------------------------------------------------
//
// Dx12のパラメータを、どのように初期化するかを決定するクラス
//
//--------------------------------------------------------------------------------------------------------------------------
#include<map>

class Dx12TaskBase
{
// タスク型の定義
protected:

	using KeyVal	= int;
	using TaskVal	= Dx12TaskBase*;
	using TaskElem	= std::pair<KeyVal, TaskVal>;
	using TaskTable = std::map<KeyVal, TaskVal>;

// コンストラクタ、デストラクタ
public:

	Dx12TaskBase() = default;
	Dx12TaskBase(Dx12Component* pCmpt);
	~Dx12TaskBase();

// タスクの実装時に使用する関数
public:

	// 派生先でタスクを実装してもらう
	virtual	HRESULT Task(TaskVal pTaskBase) { return E_ABORT; }

	// コンポーネント(Dx12を構成する材料)にアクセス
	Dx12Component*	operator->() const { return _pCmpt; }

// タスクの管理
public:

	// タスクの識別番号
	struct TaskID
	{
		enum _TaskID
		{
			// 生成に関するタスク
			CREATE_FACTORY,
			CREATE_DEVICE,
			CREATE_COMMAND_ALLOC,
			CREATE_COMMAND_LIST,
			CREATE_COMMAND_QUEUE,
			CREATE_SWAPCHAIN,
			CREATE_RTV,
			CREATE_FENCE,

			// 描画を行うタスク
			RENDER_BEGIN,
			RENDER_UPDATE,
			RENDER_END,

			// GPUに関わるタスク
			GPU_WAIT,
			GPU_COMPUTE,

			// 無効な値（エラー、失敗などを表す際に使う）
			INVALID,
			LAST_TASK_ID = INVALID

		} _task;

		// 相互変換
		TaskID(int msg) : _task(static_cast<_TaskID>(msg)) {}
		operator _TaskID() const { return _task; }

	}; /* struct TaskID */

	// 'msg'にタスク'pTask'を登録
	void	RegisterTask(KeyVal msg, TaskVal pTask);

	// 'msg'に登録されたタスクを実行
	HRESULT	ExcuteTask(KeyVal msg);

	// 'msg'に登録されたタスクを削除
	void	ReleaseTask(KeyVal msg);

// メンバ変数
private:

	// タスクを溜め込むところ
	TaskTable		_taskTable;

	// Dx12の初期化に必要なパラメータ
	Dx12Component*	_pCmpt;
};

//--------------------------------------------------------------------------------------------------------------------------
//
// Dx12の初期化に必要なパラメータを用意するクラス
//
//--------------------------------------------------------------------------------------------------------------------------
class Dx12Component
{
protected:

	// 略称定義（タイピングの手間を省くため）
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	Dx12Component(HWND hwnd) : _hwnd(hwnd) {}

	// 最低限必要なパラメータ
	ComPtr<IDXGIFactory6>					_factory;
	ComPtr<ID3D12Device>					_device;

	// 命令を送るために必要なパラメータ
	ComPtr<ID3D12CommandAllocator>			_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>		_cmdList;
	ComPtr<ID3D12CommandQueue>				_cmdQueue;

	// ウィンドウへの描画に必要なパラメータ
	ComPtr<IDXGISwapChain4>					_swapChain;
	HWND									_hwnd;
	ComPtr<ID3D12DescriptorHeap>			_rtvDescHeap;
	std::vector< ComPtr<ID3D12Resource> >	_backBuffs;

	// 同期処理に必要なパラメータ
	std::pair<ComPtr<ID3D12Fence>, UINT64>	_fence;
};

#if false
template <class T>
class MethodExec
{
public:

	MethodExec(MethodExec<T>(T::* f)()) : func_(f) {};
	MethodExec<T> exec(T* obj) { return (obj->*func_)(); }

private:

	MethodExec<T> (T::* func_)(); // 実行するT型クラスのメソッドポインタ
};
#endif

#undef ___IMPL_HANDLE

#include"impl\Dx12BeginTasks_Impl.h"
