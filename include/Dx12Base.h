#pragma once

//--------------------------------------------------------------------------------------------------------------------------
//
// ���^�X�N�N���X���g����Dx12�������v���O�����̊T�v
//	 - 'Dx12Componet'�N���X�ŗp�ӂ��ꂽDx12�̏������ɕK�v�ȃp�����[�^���A�ǂ̂悤�ɏ��������Ă�����'Dx12TaskBase'�N���X��
//	 - ���肵�A��������B�����ŁA��̓I�Ȏ�����'Dx12TaskBase'�N���X����h�������^�X�N�N���X�ōs��(Dx12Tasks_Impl.h��v�Q��)�B
//	 - �ȏ�̍s���œ�����p�����[�^���A�ŏI�I��'Dx12Base'�N���X����A�N�Z�X�ł���悤�ɂ��A'Dx12App'�N���X��
//	 - ���̃p�����[�^���g�����v���O������g�߂�悤�ɂ��Ă���B
//
//--------------------------------------------------------------------------------------------------------------------------
class Dx12TaskBase;
class Dx12Component;
using Dx12Base = Dx12TaskBase;

//--------------------------------------------------------------------------------------------------------------------------
//
// Dx12�̃p�����[�^���A�ǂ̂悤�ɏ��������邩�����肷��N���X
//
//--------------------------------------------------------------------------------------------------------------------------
#include<map>

class Dx12TaskBase
{
// �^�X�N�^�̒�`
protected:

	using KeyVal	= int;
	using TaskVal	= Dx12TaskBase*;
	using TaskElem	= std::pair<KeyVal, TaskVal>;
	using TaskTable = std::map<KeyVal, TaskVal>;

// �R���X�g���N�^�A�f�X�g���N�^
public:

	Dx12TaskBase() = default;
	Dx12TaskBase(Dx12Component* pCmpt);
	~Dx12TaskBase();

// �^�X�N�̎������Ɏg�p����֐�
public:

	// �h����Ń^�X�N���������Ă��炤
	virtual	HRESULT Task(TaskVal pTaskBase) { return E_ABORT; }

	// �R���|�[�l���g(Dx12���\������ޗ�)�ɃA�N�Z�X
	Dx12Component*	operator->() const { return _pCmpt; }

// �^�X�N�̊Ǘ�
public:

	// �^�X�N�̎��ʔԍ�
	struct TaskID
	{
		enum _TaskID
		{
			// �����Ɋւ���^�X�N
			CREATE_FACTORY,
			CREATE_DEVICE,
			CREATE_COMMAND_ALLOC,
			CREATE_COMMAND_LIST,
			CREATE_COMMAND_QUEUE,
			CREATE_SWAPCHAIN,
			CREATE_RTV,
			CREATE_FENCE,

			// �`����s���^�X�N
			RENDER_BEGIN,
			RENDER_UPDATE,
			RENDER_END,

			// GPU�Ɋւ��^�X�N
			GPU_WAIT,
			GPU_COMPUTE,

			// �����Ȓl�i�G���[�A���s�Ȃǂ�\���ۂɎg���j
			INVALID,
			LAST_TASK_ID = INVALID

		} _task;

		// ���ݕϊ�
		TaskID(int msg) : _task(static_cast<_TaskID>(msg)) {}
		operator _TaskID() const { return _task; }

	}; /* struct TaskID */

	// 'msg'�Ƀ^�X�N'pTask'��o�^
	void	RegisterTask(KeyVal msg, TaskVal pTask);

	// 'msg'�ɓo�^���ꂽ�^�X�N�����s
	HRESULT	ExcuteTask(KeyVal msg);

	// 'msg'�ɓo�^���ꂽ�^�X�N���폜
	void	ReleaseTask(KeyVal msg);

// �����o�ϐ�
private:

	// �^�X�N�𗭂ߍ��ނƂ���
	TaskTable		_taskTable;

	// Dx12�̏������ɕK�v�ȃp�����[�^
	Dx12Component*	_pCmpt;
};

//--------------------------------------------------------------------------------------------------------------------------
//
// Dx12�̏������ɕK�v�ȃp�����[�^��p�ӂ���N���X
//
//--------------------------------------------------------------------------------------------------------------------------
class Dx12Component
{
protected:

	// ���̒�`�i�^�C�s���O�̎�Ԃ��Ȃ����߁j
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	Dx12Component(HWND hwnd) : _hwnd(hwnd) {}

	// �Œ���K�v�ȃp�����[�^
	ComPtr<IDXGIFactory6>					_factory;
	ComPtr<ID3D12Device>					_device;

	// ���߂𑗂邽�߂ɕK�v�ȃp�����[�^
	ComPtr<ID3D12CommandAllocator>			_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>		_cmdList;
	ComPtr<ID3D12CommandQueue>				_cmdQueue;

	// �E�B���h�E�ւ̕`��ɕK�v�ȃp�����[�^
	ComPtr<IDXGISwapChain4>					_swapChain;
	HWND									_hwnd;
	ComPtr<ID3D12DescriptorHeap>			_rtvDescHeap;
	std::vector< ComPtr<ID3D12Resource> >	_backBuffs;

	// ���������ɕK�v�ȃp�����[�^
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

	MethodExec<T> (T::* func_)(); // ���s����T�^�N���X�̃��\�b�h�|�C���^
};
#endif

#undef ___IMPL_HANDLE

#include"impl\Dx12BeginTasks_Impl.h"
