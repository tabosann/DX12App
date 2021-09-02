#pragma once

#include"Dx12BeginTasks_Impl.h"

class Dx12RenderBegin : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12RenderBegin& ref = static_cast<Dx12RenderBegin&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		// ���ݏ������Ă���o�b�N�o�b�t�@�̎��ʔԍ����擾
		UINT curBackBuffIdx = ref->_swapChain->GetCurrentBackBufferIndex();

		// �o�b�N�o�b�t�@�̎g�p�ړI�𖾎�������
		CD3DX12_RESOURCE_BARRIER barrier = {};
		barrier = CD3DX12_RESOURCE_BARRIER::Transition(
					ref->_backBuffs[curBackBuffIdx].Get(), 
					D3D12_RESOURCE_STATE_PRESENT,
					D3D12_RESOURCE_STATE_RENDER_TARGET);
		ref->_cmdList->ResourceBarrier(1, &barrier);

		auto handle = ref->_rtvDescHeap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr = curBackBuffIdx * ref->_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// �w�i�F�̐ݒ�
		FLOAT color[4] = { 1.f, 0.f, 0.f, 1.f }; // RGBA
		ref->_cmdList->ClearRenderTargetView(handle, color, 0, nullptr);

		// �`��̈�̐ݒ�
		CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.f, 0.f, 1.f, 1.f);
		ref->_cmdList->RSSetViewports(1, &viewPort);
		RECT rect = {}; GetClientRect(ref->_hwnd, &rect);
		ref->_cmdList->RSSetScissorRects(1, &rect);
	}
};

//----------------------------------------------------------------------------------------------------------------------------------
// 
// GPU�̏�����CPU�̏����̓�����}�邽�߂̃^�X�N
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
// GPGPU���s���^�X�N
// 
//----------------------------------------------------------------------------------------------------------------------------------
class Dx12GpuCompute : TASK_BASE_CLASS
{
	virtual HRESULT Task(TaskVal pTaskBase)
	{
		Dx12GpuCompute& ref = static_cast<Dx12GpuCompute&>(*pTaskBase);
		if (&ref == nullptr) return E_ABORT;

		// TODO : GPGPU(GPU�Ɉ�ʓI�Ȍv�Z�������邱��)�̎���

		return E_NOTIMPL;
	}
};

#include"Dx12EndTasks_Impl.h"
