#pragma once

//�����o�ϐ��̐錾�Ɠ����ɁA���̃A�N�Z�b�T���`
#define ___IMPL_HANDLE(type, name)\
		public:																			\
			Microsoft::WRL::ComPtr<type> _p##name;										\
		public:																			\
			type* Get##name() const { return _p##name.Get(); }							\
			void  Set##name(void* p##name) { _p##name = static_cast<type*>(p##name); }	\

//--------------------------------------------------------------------------------------------------------------------------
//
// ���^�X�N�N���X���g����Dx12�������v���O�����̊T�v
//	 - 'Dx12Componet'�N���X�ŗp�ӂ��ꂽDx12�̏������ɕK�v�ȃp�����[�^���A�ǂ̂悤�ɏ��������Ă�����'Dx12TaskBase'�N���X��
//	 - ���肵�A��������B�����ŁA��̓I�Ȏ�����'Dx12TaskBase'�N���X����h�������^�X�N�N���X�ōs��(Dx12Tasks_Impl.h��v�Q��)�B
//	 - �ȏ�̍s���œ�����p�����[�^���A�ŏI�I��'Dx12Base'�N���X����A�N�Z�X�ł���悤�ɂ��A'Dx12App'�N���X��
//	 - ���̃p�����[�^���g�����v���O������g�߂�悤�ɂ��Ă���B
//
//--------------------------------------------------------------------------------------------------------------------------
class Dx12Base;
class Dx12TaskBase;
class Dx12Component;

//--------------------------------------------------------------------------------------------------------------------------
//
// ���������ꂽDx12�ɁA�A�N�Z�X�����i�Ƃ��ėp�ӂ��ꂽ�N���X
//
//--------------------------------------------------------------------------------------------------------------------------
class Dx12Base
{
public:

	// �R���X�g���N�^�A�f�X�g���N�^
	Dx12Base(HWND hwnd);
	~Dx12Base();

private:

	// Dx12�̏������ɕK�v�ȃ^�X�N���p�ӂ���Ă���
	Dx12TaskBase* _pTask;
};

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

	using _Key		= int;
	using _Task		= Dx12TaskBase*;
	using TaskElem	= std::pair<_Key, _Task>;
	using TaskTable = std::map<_Key, _Task>;

// �R���X�g���N�^�A�f�X�g���N�^
public:

	Dx12TaskBase() = default;
	Dx12TaskBase(HWND hwnd);
	~Dx12TaskBase();

// �^�X�N�̎������Ɏg�p����֐�
public:

	// �h����Ń^�X�N���������Ă��炤
	virtual	HRESULT Task(_Task pTaskBase) { return E_ABORT; }

	// �p�����[�^�ɃA�N�Z�X
	Dx12Component* operator -> () const { return _pParam; }

// �^�X�N�̊Ǘ�
private:

	// �^�X�N�̎��ʔԍ�
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

	// 'msg'�Ƀ^�X�N'pTask'��o�^
	void			RegisterTask(_Key msg, _Task pTask);

	// 'msg'�ɓo�^���ꂽ�^�X�N�����s
	HRESULT			ExcuteTask(_Key msg);

	// 'msg'�ɓo�^���ꂽ�^�X�N���폜
	void			ReleaseTask(_Key msg);

// �����o�ϐ�
private:

	// �^�X�N�𗭂ߍ��ނƂ���
	TaskTable		_taskTable;

	// Dx12�̏������ɕK�v�ȃp�����[�^
	Dx12Component*	_pParam;
};

//--------------------------------------------------------------------------------------------------------------------------
//
// Dx12�̏������ɕK�v�ȃp�����[�^��p�ӂ���N���X
//
//--------------------------------------------------------------------------------------------------------------------------
class Dx12Component
{
	// ���̒�`�i�^�C�s���O�̎�Ԃ��Ȃ����߁j
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

	MethodExec<T> (T::* func_)(); // ���s����T�^�N���X�̃��\�b�h�|�C���^
};
#endif

#undef ___IMPL_HANDLE
