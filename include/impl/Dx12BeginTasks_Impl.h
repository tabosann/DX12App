#pragma once

#include"..\Dx12Base.h"

// タスククラスを便利に扱うマクロ
#ifndef TASK_BASE_CLASS

	// ここを変えるだけで、タスクの基底クラスを一斉に変えられる。
#	define TASK_BASE_CLASS public Dx12TaskBase

#endif

// タスク実装の定型
class Dx12Sample : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12Sample& ref = *static_cast<Dx12Sample*>(pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		// ここにタスクを実装

		return E_NOTIMPL;
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// デバイス（描画に使用するビデオカード）の設定
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateFactory : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		// ここで'pDx12Base'がDx12FactoryCreatorクラス(自分自身)の型にキャストされると、扱いは'this'ポインタと同じになる。
		// すると、基底クラスのオブジェクトは継承により、自分自身のオブジェクトでもあるため'ptr'から
		// 基底クラスのオブジェクトにアクセス可能になる！（ただし、privateメンバにはアクセス不可能）
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
// デバイスの生成
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateDevice : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12CreateDevice& ref = static_cast<Dx12CreateDevice&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		// デバッグレイヤーの有効化
		// NOTE : デバイス作成後に有効化すると、デバイスが破棄されてしまうので注意！
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(debugController.ReleaseAndGetAddressOf()));
		if (SUCCEEDED(hr)) debugController->EnableDebugLayer();

		// DirectXのバージョンを識別するためのもの
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

		// 自動で使用するビデオカードを決定する
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
// コマンドを溜め込む場所(ストレージ)の生成
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
// コマンドリスト(命令表)の生成
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
// コマンドに関する関数を提供してくれるオブジェクトを生成
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateCmdQueue : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12CreateCmdQueue& ref = static_cast<Dx12CreateCmdQueue&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		// コマンドキューの設定
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
// スワップチェーンの作成
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
		desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;		//１ピクセルを0.0~1.0の256段階で表現
		desc.BufferUsage        = DXGI_USAGE_BACK_BUFFER;			//
		desc.BufferCount        = 2;								//表裏切り替えられるように２つ用意
		desc.Width              = width;							//画面解像度(横)
		desc.Height             = height;							//画面解像度(縦)
		desc.Stereo             = false;							//スワップエフェクトがシーケンシャルモードならtrue
		desc.SampleDesc.Count   = 1;							    //マルチサンプリング関連
		desc.SampleDesc.Quality = 0;							    //
		desc.Scaling            = DXGI_SCALING_STRETCH;				//裏画面のサイズに合わせてバックバッファをリサイズ
		desc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;	//フリップ前の表画面をフリップ後リセット(フリップ時初期化)
		desc.AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED;		//特に指定しない
		//ウィンドウモード、フルスクリーンモードの切り替え時に
		//そのときのウィンドウサイズに合わせて表示モードも切り替える
		desc.Flags				= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		return ref->_factory->CreateSwapChainForHwnd(ref->_cmdQueue.Get(), ref->_hwnd, &desc, nullptr, nullptr,
													 (IDXGISwapChain1**)(ref->_swapChain.ReleaseAndGetAddressOf()));
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// レンダターゲットビューをバックバッファに生成
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12CreateRtv : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12CreateRtv& ref = static_cast<Dx12CreateRtv&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		// スワップチェーンのサイズを取得するために、スワップチェーンの説明文を取得
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc1;
		HRESULT hr = ref->_swapChain->GetDesc1(&swapChainDesc1);
		if (FAILED(hr)) return hr;

		// バックバッファの数だけメモリ領域を確保する
		ref->_backBuffs.resize(swapChainDesc1.BufferCount);

		// RTV用のディスクリプタヒープを生成(確保)
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		descHeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		descHeapDesc.NumDescriptors	= swapChainDesc1.BufferCount;
		descHeapDesc.NodeMask		= 0;
		hr = ref->_device->CreateDescriptorHeap(&descHeapDesc,
												IID_PPV_ARGS(ref->_rtvDescHeap.ReleaseAndGetAddressOf()));

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		// バックバッファ毎にRTVを作成
		auto handle			= ref->_rtvDescHeap->GetCPUDescriptorHandleForHeapStart();
		auto incrementSize	= ref->_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		for (UINT i = 0; i < swapChainDesc1.BufferCount; ++i)
		{
			// '_backBuffs[i]'にi番目のバックバッファを生成
			hr = ref->_swapChain->GetBuffer(i, IID_PPV_ARGS(ref->_backBuffs[i].ReleaseAndGetAddressOf()));
			if (FAILED(hr)) return hr;

			// RTVをi番目のバックバッファに生成
			rtvDesc.Format = ref->_backBuffs[i]->GetDesc().Format;
			ref->_device->CreateRenderTargetView(ref->_backBuffs[i].Get(), &rtvDesc, handle);

			// 次のバックバッファを参照
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
