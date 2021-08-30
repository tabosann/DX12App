#pragma once

#include"..\Dx12Base.h"

// �^�X�N�N���X��֗��Ɉ����}�N��
#ifndef TASK_BASE_CLASS
#	define TASK_BASE_CLASS public Dx12TaskBase
#endif

//----------------------------------------------------------------------------------------------------------------------------------
// 
// �f�o�C�X�i�`��Ɏg�p����r�f�I�J�[�h�j�̐ݒ�
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateFactory : TASK_BASE_CLASS
{
	virtual HRESULT Task(_Task pTaskBase)
	{
		// ������'pDx12Base'��Dx12FactoryCreator�N���X(�������g)�̌^�ɃL���X�g�����ƁA������'this'�|�C���^�Ɠ����ɂȂ�B
		// ����ƁA���N���X�̃I�u�W�F�N�g�͌p���ɂ��A�������g�̃I�u�W�F�N�g�ł����邽��'ptr'����
		// ���N���X�̃I�u�W�F�N�g�ɃA�N�Z�X�\�ɂȂ�I�i�������Aprivate�����o�ɂ̓A�N�Z�X�s�\�j
		Dx12CreateFactory* p = static_cast<Dx12CreateFactory*>(pTaskBase);
		if (p == nullptr) return E_ABORT;

		int flag = NULL;
#ifdef _DEBUG
		flag |= DXGI_CREATE_FACTORY_DEBUG;
#endif
		return CreateDXGIFactory2(flag, IID_PPV_ARGS((*p)->_factory.ReleaseAndGetAddressOf()));
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// �f�o�C�X�̐���
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateDevice : TASK_BASE_CLASS
{
	virtual HRESULT Task(_Task pTaskBase)
	{
		Dx12CreateDevice* p = static_cast<Dx12CreateDevice*>(pTaskBase);
		if (p == nullptr) return E_ABORT;

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
			hr = D3D12CreateDevice(nullptr, level, IID_PPV_ARGS((*p)->_device.ReleaseAndGetAddressOf()));
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
	virtual HRESULT Task(_Task pTaskBase)
	{
		Dx12CreateCmdAlloc* p = static_cast<Dx12CreateCmdAlloc*>(pTaskBase);
		if (p == nullptr) return E_ABORT;

		return (*p)->_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
													 IID_PPV_ARGS((*p)->_cmdAlloc.ReleaseAndGetAddressOf()));
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// �R�}���h���X�g(���ߕ\)�̐���
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateGraphicCmdList : TASK_BASE_CLASS
{
	virtual HRESULT Task(_Task pTaskBase)
	{
		Dx12CreateGraphicCmdList* p = static_cast<Dx12CreateGraphicCmdList*>(pTaskBase);
		if (p == nullptr) return E_ABORT;

		return (*p)->_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, (*p)->_cmdAlloc.Get(),
												nullptr, IID_PPV_ARGS((*p)->_cmdList.ReleaseAndGetAddressOf()));
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// �R�}���h�Ɋւ���֐���񋟂��Ă����I�u�W�F�N�g�𐶐�
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateCmdQueue : TASK_BASE_CLASS
{
	virtual HRESULT Task(_Task pTaskBase)
	{
		Dx12CreateCmdQueue* p = static_cast<Dx12CreateCmdQueue*>(pTaskBase);
		if (p == nullptr) return E_ABORT;

		// �R�}���h�L���[�̐ݒ�
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		return (*p)->_device->CreateCommandQueue(&desc, IID_PPV_ARGS((*p)->_cmdQueue.ReleaseAndGetAddressOf()));
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// �X���b�v�`�F�[���̍쐬
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateSwapChain : TASK_BASE_CLASS
{
	virtual HRESULT Task(_Task pTaskBase)
	{
		Dx12CreateSwapChain* p = static_cast<Dx12CreateSwapChain*>(pTaskBase);
		if (p == nullptr) return E_ABORT;

		RECT rect;
		GetClientRect((*p)->_hwnd, &rect);
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

		return (*p)->_factory->CreateSwapChainForHwnd((*p)->_cmdQueue.Get(), (*p)->_hwnd, &desc, nullptr, nullptr,
													  (IDXGISwapChain1**)((*p)->_swapChain.ReleaseAndGetAddressOf()));
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// �����_�^�[�Q�b�g�r���[���o�b�N�o�b�t�@�ɐ���
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateRtv : TASK_BASE_CLASS
{
	virtual HRESULT Task(_Task pTaskBase)
	{
		Dx12CreateRtv* p = static_cast<Dx12CreateRtv*>(pTaskBase);
		if (p == nullptr) return E_ABORT;

		// �X���b�v�`�F�[���̃T�C�Y���擾���邽�߂ɁA�X���b�v�`�F�[���̐��������擾
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc1;
		HRESULT hr = (*p)->_swapChain->GetDesc1(&swapChainDesc1);
		if (FAILED(hr)) return hr;

		// �o�b�N�o�b�t�@�̐������������̈���m�ۂ���
		(*p)->_backBuffs.resize(swapChainDesc1.BufferCount);

		// RTV�p�̃f�B�X�N���v�^�q�[�v�𐶐�(�m��)
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		descHeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		descHeapDesc.NumDescriptors	= swapChainDesc1.BufferCount;
		descHeapDesc.NodeMask		= 0;
		hr = (*p)->_device->CreateDescriptorHeap(&descHeapDesc,
												 IID_PPV_ARGS((*p)->_rtvDescHeap.ReleaseAndGetAddressOf()));

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		// �o�b�N�o�b�t�@����RTV���쐬
		auto handle			= (*p)->_rtvDescHeap->GetCPUDescriptorHandleForHeapStart();
		auto incrementSize	= (*p)->_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		for (UINT i = 0; i < swapChainDesc1.BufferCount; ++i)
		{
			// '_backBuffs[i]'��i�Ԗڂ̃o�b�N�o�b�t�@�𐶐�
			hr = (*p)->_swapChain->GetBuffer(i, IID_PPV_ARGS((*p)->_backBuffs[i].ReleaseAndGetAddressOf()));
			if (FAILED(hr)) return hr;

			// RTV��i�Ԗڂ̃o�b�N�o�b�t�@�ɐ���
			rtvDesc.Format = (*p)->_backBuffs[i]->GetDesc().Format;
			(*p)->_device->CreateRenderTargetView((*p)->_backBuffs[i].Get(), &rtvDesc, handle);

			// ���̃o�b�N�o�b�t�@���Q��
			handle.ptr += incrementSize;
		}
		return hr;
	}
};

#ifdef TASK_BASE_CLASS
#	undef TASK_BASE_CLASS
#endif
