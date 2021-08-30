#pragma once

//メンバ変数の宣言と同時に、そのアクセッサを定義
#define ___IMPL_HANDLE(type, name)\
		public:																			\
			Microsoft::WRL::ComPtr<type> _p##name;										\
		public:																			\
			type* Get##name() const { return _p##name.Get(); }							\
			void  Set##name(void* p##name) { _p##name = static_cast<type*>(p##name); }	\

//--------------------------------------------------------------------------------------------------------------------------
//
// ◆タスククラスを使ったDx12初期化プログラムの概要
//	 - 'Dx12Componet'クラスで用意されたDx12の初期化に必要なパラメータを、どのように初期化していくか'Dx12TaskBase'クラスで
//	 - 決定し、実装する。ここで、具体的な実装は'Dx12TaskBase'クラスから派生したタスククラスで行う(Dx12Tasks_Impl.hを要参照)。
//	 - 以上の行程で得られるパラメータを、最終的に'Dx12Base'クラスからアクセスできるようにし、'Dx12App'クラスで
//	 - そのパラメータを使ったプログラムを組めるようにしている。
//
//--------------------------------------------------------------------------------------------------------------------------
class Dx12Base;
class Dx12TaskBase;
class Dx12Component;

//--------------------------------------------------------------------------------------------------------------------------
//
// 初期化されたDx12に、アクセスする手段として用意されたクラス
//
//--------------------------------------------------------------------------------------------------------------------------
class Dx12Base
{
public:

	// コンストラクタ、デストラクタ
	Dx12Base(HWND hwnd);
	~Dx12Base();

private:

	// Dx12の初期化に必要なタスクが用意されている
	Dx12TaskBase* _pTask;
};

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

	using _Key		= int;
	using _Task		= Dx12TaskBase*;
	using TaskElem	= std::pair<_Key, _Task>;
	using TaskTable = std::map<_Key, _Task>;

// コンストラクタ、デストラクタ
public:

	Dx12TaskBase() = default;
	Dx12TaskBase(HWND hwnd);
	~Dx12TaskBase();

// タスクの実装時に使用する関数
public:

	// 派生先でタスクを実装してもらう
	virtual	HRESULT Task(_Task pTaskBase) { return E_ABORT; }

	// パラメータにアクセス
	Dx12Component* operator -> () const { return _pParam; }

// タスクの管理
private:

	// タスクの識別番号
	enum TaskID
	{
		CREATE_FACTORY				= 0x00,
		CREATE_FACTORY_6			= 0x06,
		CREATE_DEVICE				= 0x10,
		CREATE_COMMAND_ALLOC		= 0x20,
		CREATE_COMMAND_LIST			= 0x30,
		CREATE_GRAPHIC_COMMAND_LIST	= 0x31,
		CREATE_COMMAND_QUEUE		= 0x40,
		CREATE_SWAPCHAIN			= 0x50,
		CREATE_RTV					= 0x60,
		CREATE_END					= 0xff
	};

	// 'msg'にタスク'pTask'を登録
	void			RegisterTask(_Key msg, _Task pTask);

	// 'msg'に登録されたタスクを実行
	HRESULT			ExcuteTask(_Key msg);

	// 'msg'に登録されたタスクを削除
	void			ReleaseTask(_Key msg);

// メンバ変数
private:

	// タスクを溜め込むところ
	TaskTable		_taskTable;

	// Dx12の初期化に必要なパラメータ
	Dx12Component*	_pParam;
};

//--------------------------------------------------------------------------------------------------------------------------
//
// Dx12の初期化に必要なパラメータを用意するクラス
//
//--------------------------------------------------------------------------------------------------------------------------
class Dx12Component
{
	// 略称定義（タイピングの手間を省くため）
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	Dx12Component(HWND hwnd) : _hwnd(hwnd) {}

	ComPtr<IDXGIFactory6>				_factory;
	ComPtr<ID3D12Device>				_device;
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;
	ComPtr<ID3D12CommandQueue>			_cmdQueue;
	ComPtr<IDXGISwapChain4>				_swapChain;
	HWND								_hwnd;
	ComPtr<ID3D12DescriptorHeap>		_rtvDescHeap;
	std::vector<ComPtr<ID3D12Resource>>	_backBuffs;
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
