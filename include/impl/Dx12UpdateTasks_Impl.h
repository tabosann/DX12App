#pragma once

#include"Dx12BeginTasks_Impl.h"

class Dx12RenderBegin : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12RenderBegin& ref = static_cast<Dx12RenderBegin&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		// 現在処理しているバックバッファの識別番号を取得
		UINT curBackBuffIdx = ref->_swapChain->GetCurrentBackBufferIndex();

		// バックバッファの使用目的を明示化する
		CD3DX12_RESOURCE_BARRIER barrier = {};
		barrier = CD3DX12_RESOURCE_BARRIER::Transition(
					ref->_backBuffs[curBackBuffIdx].Get(), 
					D3D12_RESOURCE_STATE_PRESENT,
					D3D12_RESOURCE_STATE_RENDER_TARGET);
		ref->_cmdList->ResourceBarrier(1, &barrier);

		auto handle = ref->_rtvDescHeap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr = curBackBuffIdx * ref->_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// 背景色の設定
		FLOAT color[4] = { 1.f, 0.f, 0.f, 1.f }; // RGBA
		ref->_cmdList->ClearRenderTargetView(handle, color, 0, nullptr);

		// 描画領域の設定
		CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.f, 0.f, 1.f, 1.f);
		ref->_cmdList->RSSetViewports(1, &viewPort);
		RECT rect = {}; GetClientRect(ref->_hwnd, &rect);
		ref->_cmdList->RSSetScissorRects(1, &rect);
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// GPUの処理とCPUの処理の同期を図るためのタスク
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12GpuWait : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12GpuWait& ref = static_cast<Dx12GpuWait&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		HRESULT hr = ref->_cmdQueue->Signal(ref->_fence.first.Get(), ref->_fence.second);
		if (FAILED(hr)) return hr;

		if (ref->_fence.first->GetCompletedValue() < ref->_fence.second)
		{
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);

			hr = ref->_fence.first->SetEventOnCompletion(ref->_fence.second, event);
			if (FAILED(hr)) return hr;

			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
		return hr;
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// GPGPUを行うタスク
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12GpuCompute : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12GpuCompute& ref = static_cast<Dx12GpuCompute&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		// TODO : GPGPU(GPUに一般的な計算をさせること)の実装

		return E_NOTIMPL;
	}
};

#include"Dx12EndTasks_Impl.h"
