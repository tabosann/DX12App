#pragma once

#include"..\Dx12Base.h"

// �^�X�N�N���X��֗��Ɉ����}�N��
#ifndef TASK_BASE_CLASS

	// ������ς��邾���ŁA�^�X�N�̊��N���X����Ăɕς�����B
#	define TASK_BASE_CLASS public Dx12TaskBase

#endif

// �^�X�N�����̒�^
class Dx12Sample : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12Sample& ref = *static_cast<Dx12Sample*>(pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		// �����Ƀ^�X�N������

		return E_NOTIMPL;
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// �f�o�C�X�i�`��Ɏg�p����r�f�I�J�[�h�j�̐ݒ�
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateFactory : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		// ������'pDx12Base'��Dx12FactoryCreator�N���X(�������g)�̌^�ɃL���X�g�����ƁA������'this'�|�C���^�Ɠ����ɂȂ�B
		// ����ƁA���N���X�̃I�u�W�F�N�g�͌p���ɂ��A�������g�̃I�u�W�F�N�g�ł����邽��'ptr'����
		// ���N���X�̃I�u�W�F�N�g�ɃA�N�Z�X�\�ɂȂ�I�i�������Aprivate�����o�ɂ̓A�N�Z�X�s�\�j
		Dx12CreateFactory& ref = static_cast<Dx12CreateFactory&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		int flag = NULL;
#ifdef _DEBUG
		flag |= DXGI_CREATE_FACTORY_DEBUG;
#endif
		return CreateDXGIFactory2(flag, IID_PPV_ARGS(ref->_factory.ReleaseAndGetAddressOf()));
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// �f�o�C�X�̐���
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateDevice : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12CreateDevice& ref = static_cast<Dx12CreateDevice&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		// �f�o�b�O���C���[�̗L����
		// NOTE : �f�o�C�X�쐬��ɗL��������ƁA�f�o�C�X���j������Ă��܂��̂Œ��ӁI
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(debugController.ReleaseAndGetAddressOf()));
		if (SUCCEEDED(hr)) debugController->EnableDebugLayer();

		// DirectX�̃o�[�W���������ʂ��邽�߂̂���
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
			D3D_FEATURE_LEVEL_1_0_CORE
		};

		// �����Ŏg�p����r�f�I�J�[�h�����肷��
		for (auto level : featureLevels)
		{
			hr = D3D12CreateDevice(nullptr, level, IID_PPV_ARGS(ref->_device.ReleaseAndGetAddressOf()));
			if (SUCCEEDED(hr)) return hr;
		}
		return hr;
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// �R�}���h�𗭂ߍ��ޏꏊ(�X�g���[�W)�̐���
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateCmdAlloc : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12CreateCmdAlloc& ref = static_cast<Dx12CreateCmdAlloc&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		return ref->_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
													IID_PPV_ARGS(ref->_cmdAlloc.ReleaseAndGetAddressOf()));
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// �R�}���h���X�g(���ߕ\)�̐���
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateGraphicCmdList : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12CreateGraphicCmdList& ref = static_cast<Dx12CreateGraphicCmdList&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		return ref->_device->CreateCommandList(	0, D3D12_COMMAND_LIST_TYPE_DIRECT, ref->_cmdAlloc.Get(),
												nullptr, IID_PPV_ARGS(ref->_cmdList.ReleaseAndGetAddressOf()));
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// �R�}���h�Ɋւ���֐���񋟂��Ă����I�u�W�F�N�g�𐶐�
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateCmdQueue : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12CreateCmdQueue& ref = static_cast<Dx12CreateCmdQueue&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		// �R�}���h�L���[�̐ݒ�
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		return ref->_device->CreateCommandQueue(&desc, IID_PPV_ARGS(ref->_cmdQueue.ReleaseAndGetAddressOf()));
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// �X���b�v�`�F�[���̍쐬
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateSwapChain : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12CreateSwapChain& ref = static_cast<Dx12CreateSwapChain&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		RECT rect;
		GetClientRect(ref->_hwnd, &rect);
		UINT width	= rect.right - rect.left;
		UINT height = rect.bottom - rect.top;

		DXGI_SWAP_CHAIN_DESC1 desc = {};
		desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;		//�P�s�N�Z����0.0~1.0��256�i�K�ŕ\��
		desc.BufferUsage        = DXGI_USAGE_BACK_BUFFER;			//
		desc.BufferCount        = 2;								//�\���؂�ւ�����悤�ɂQ�p��
		desc.Width              = width;							//��ʉ𑜓x(��)
		desc.Height             = height;							//��ʉ𑜓x(�c)
		desc.Stereo             = false;							//�X���b�v�G�t�F�N�g���V�[�P���V�������[�h�Ȃ�true
		desc.SampleDesc.Count   = 1;							    //�}���`�T���v�����O�֘A
		desc.SampleDesc.Quality = 0;							    //
		desc.Scaling            = DXGI_SCALING_STRETCH;				//����ʂ̃T�C�Y�ɍ��킹�ăo�b�N�o�b�t�@�����T�C�Y
		desc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;	//�t���b�v�O�̕\��ʂ��t���b�v�ナ�Z�b�g(�t���b�v��������)
		desc.AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED;		//���Ɏw�肵�Ȃ�
		//�E�B���h�E���[�h�A�t���X�N���[�����[�h�̐؂�ւ�����
		//���̂Ƃ��̃E�B���h�E�T�C�Y�ɍ��킹�ĕ\�����[�h���؂�ւ���
		desc.Flags				= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		return ref->_factory->CreateSwapChainForHwnd(ref->_cmdQueue.Get(), ref->_hwnd, &desc, nullptr, nullptr,
													 (IDXGISwapChain1**)(ref->_swapChain.ReleaseAndGetAddressOf()));
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// �����_�^�[�Q�b�g�r���[���o�b�N�o�b�t�@�ɐ���
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateRtv : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12CreateRtv& ref = static_cast<Dx12CreateRtv&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		// �X���b�v�`�F�[���̃T�C�Y���擾���邽�߂ɁA�X���b�v�`�F�[���̐��������擾
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc1;
		HRESULT hr = ref->_swapChain->GetDesc1(&swapChainDesc1);
		if (FAILED(hr)) return hr;

		// �o�b�N�o�b�t�@�̐������������̈���m�ۂ���
		ref->_backBuffs.resize(swapChainDesc1.BufferCount);

		// RTV�p�̃f�B�X�N���v�^�q�[�v�𐶐�(�m��)
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		descHeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		descHeapDesc.NumDescriptors	= swapChainDesc1.BufferCount;
		descHeapDesc.NodeMask		= 0;
		hr = ref->_device->CreateDescriptorHeap(&descHeapDesc,
												IID_PPV_ARGS(ref->_rtvDescHeap.ReleaseAndGetAddressOf()));

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		// �o�b�N�o�b�t�@����RTV���쐬
		auto handle			= ref->_rtvDescHeap->GetCPUDescriptorHandleForHeapStart();
		auto incrementSize	= ref->_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		for (UINT i = 0; i < swapChainDesc1.BufferCount; ++i)
		{
			// '_backBuffs[i]'��i�Ԗڂ̃o�b�N�o�b�t�@�𐶐�
			hr = ref->_swapChain->GetBuffer(i, IID_PPV_ARGS(ref->_backBuffs[i].ReleaseAndGetAddressOf()));
			if (FAILED(hr)) return hr;

			// RTV��i�Ԗڂ̃o�b�N�o�b�t�@�ɐ���
			rtvDesc.Format = ref->_backBuffs[i]->GetDesc().Format;
			ref->_device->CreateRenderTargetView(ref->_backBuffs[i].Get(), &rtvDesc, handle);

			// ���̃o�b�N�o�b�t�@���Q��
			handle.ptr += incrementSize;
		}
		return hr;
	}
};

class Dx12CreateFence : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12CreateFence& ref = static_cast<Dx12CreateFence&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		return ref->_device->CreateFence(ref->_fence.second, D3D12_FENCE_FLAG_NONE,
										 IID_PPV_ARGS(ref->_fence.first.ReleaseAndGetAddressOf()));
	}
};

#include"Dx12UpdateTasks_Impl.h"
