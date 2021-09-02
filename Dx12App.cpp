
#include<Dx12App.h>

// 唯一の実体を定義
Dx12App* Dx12App::_pInstance = nullptr;

//------------------------------------------------------------------------------------------------------------
// 
// 公開メンバの定義
//
//------------------------------------------------------------------------------------------------------------

Dx12App& Dx12App::Instance()
{
	if (_pInstance == nullptr)
		return *(_pInstance = new Dx12App);
	return *_pInstance;
}

void Dx12App::Run()
{
	// タスククラスの登録
	_pDx12Base = new Dx12Base(new Dx12UserComponent(_hwnd));

	// TODO : あるならユーザ定義のタスクを登録する
	_pDx12Base->RegisterTask(UserTaskID::IMGUI_DX12_INIT, nullptr);

	// 
	this->ShowWindow();
	this->UpdateWindow();
	this->MainLoop();
}

void Dx12App::MainLoop()
{
	MSG msg;
	while (true)
	{
		// ウィンドウが破棄されたらループ脱出
		if (!IsWindow(_hwnd)) break;

		// ウィンドウプロシージャへ移動
		if (PeekMessage(&msg, _hwnd, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		// 待機処理
		_pDx12Base->ExcuteTask(UserTaskID::IMGUI_DX12_INIT);
	}
}

void Dx12App::Release()
{
	if (_pInstance != nullptr) { delete _pInstance; _pInstance = nullptr; }
}

//------------------------------------------------------------------------------------------------------------
// 
// 非公開メンバの定義
//
//------------------------------------------------------------------------------------------------------------

LRESULT Dx12App::LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		MessageBox(hwnd, _T("Test Window"), _T("Notification"), 0);
		break;
	}
	case WM_PAINT:
	{
		static LONGLONG cnt = 0;
		std::cout << ++cnt << std::endl;
		break;
	}
	case WM_KEYDOWN:
	{
		if (wparam == VK_SPACE)
			DestroyWindow(hwnd);
		break;
	}
	default:
		break;
	}
	return wbl::WindowBase::LocalWndProc(hwnd, msg, wparam, lparam);
}
