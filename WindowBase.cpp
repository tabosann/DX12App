
#include"WindowBase.h"
#include<crtdbg.h>
#include<assert.h>
#include<tchar.h>
#include<iostream>

WindowBase::WindowBase()
{
	ZeroMemory(&_wndClass,	sizeof(WNDCLASSEX));
	ZeroMemory(&_cs,		sizeof(CREATESTRUCT));
	ZeroMemory(&_hwnd,		sizeof(HWND));

	//デフォルト設定
	_wndClass =
	{
		sizeof(WNDCLASSEX),					//この構造体のデータサイズ
		CS_HREDRAW | CS_VREDRAW,			//ウィンドウ操作に対する処理。ここでは「サイズ調節でウィンドウの再描画」を指定
		(WNDPROC)WindowBase::GlobalWndProc,	//クラス内でグローバルなウィンドウプロシージャ
		NULL,								//この構造体データの後ろに追加するメモリ領域のサイズ
		NULL,								//ウィンドウが生成される度に確保されるメモリ領域のサイズ
		GetModuleHandle(NULL),				//アプリケーションのインスタンスハンドル　詳細：https://bit.ly/2VzQ3Ac
		NULL,								//デスクトップなどに表示されるアイコンの設定
		NULL,								//カーソルの指定。ここではデフォルト
		NULL,								//再描画の際に塗る色
		NULL,								//メニューのリソース名。ここではメニューを持たないよう設定
		NULL,								//ウィンドウクラスの名前。適当で大丈夫
		NULL								//タスクバーなどに表示されるアイコンの設定
	};

	//デフォルト設定
	_cs =
	{
		this,					//CreateWindowEx関数でlpParamに渡す
		_wndClass.hInstance,	//アプリケーションのインスタンスハンドル
		NULL,					//ウィンドウメニューのハンドル
		NULL,					//親ウィンドウのハンドル。自分が親ウィンドウならNULL
		CW_USEDEFAULT,			//ウィンドウの縦の長さ（ピクセル数）
		CW_USEDEFAULT,			//ウィンドウの横の長さ（ピクセル数）
		CW_USEDEFAULT,			//ウィンドウ左上頂点のy座標。自分が子ウィンドウなら、親ウィンドウ主体のローカル座標系での話になる。
		CW_USEDEFAULT,			//ウィンドウ左上頂点のx座標。同上
		WS_OVERLAPPEDWINDOW,	//ウィンドウの見た目。ここではデフォルト設定
		NULL,					//ウィンドウの名前
		NULL,					//登録したウィンドウクラスの名前
		NULL					//ウィンドウの見た目（拡張版）。あんま詳細わかってない...
	};
}

WindowBase::~WindowBase()
{
	//_ASSERT_EXPR(UnregisterClass(_cs.lpszClass, _cs.hInstance), _T("登録解除に失敗しました。"));
}

bool WindowBase::Init(LPCTSTR className, int posX, int posY, int width, int height)
{
	bool result = RegisterWindowClass(className);
	if (!result) return false;

	_cs.cx = width;
	_cs.cy = height;
	_cs.x  = posX;
	_cs.y  = posY;
	result = CreateMainWindow(_wndClass.hInstance, _wndClass.lpszClassName);
	if (!result) return false;

	return true;
}

bool WindowBase::RegisterWindowClass(LPCTSTR className)
{
	_wndClass.lpszClassName = className;
	return RegisterClassEx(&_wndClass);
}

bool WindowBase::CreateMainWindow(HINSTANCE hInstance, LPCTSTR className)
{
	_cs.hInstance = hInstance;
	_cs.lpszClass = className;
	_cs.lpszName  = className;
	return _hwnd = CreateWindowEx(
		_cs.dwExStyle, _cs.lpszClass, _cs.lpszName, _cs.style, _cs.x, _cs.y, _cs.cx, _cs.cy, 
		_cs.hwndParent, _cs.hMenu, _cs.hInstance, _cs.lpCreateParams
	);
}

LRESULT WindowBase::GlobalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	WindowBase* ptr = (WindowBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	//取得できた場合(ウィンドウ作成後)
	if (ptr) return ptr->LocalWndProc(hwnd, msg, wparam, lparam);

	//取得できなかった場合(ウィンドウ初期化時)
	if (msg == WM_CREATE)
	{
		if (ptr = (WindowBase*)((LPCREATESTRUCT)lparam)->lpCreateParams)
		{
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)ptr);
			return ptr->LocalWndProc(hwnd, msg, wparam, lparam);
		}
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT WindowBase::LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CLOSE:
		_ASSERT_EXPR(DestroyWindow(hwnd), _T("ウィンドウ破棄に失敗しました。"));
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

bool WindowBase::Release() const
{
	return UnregisterClass(_wndClass.lpszClassName, _wndClass.hInstance);
}

LRESULT WindowApp::LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return WindowBase::LocalWndProc(hwnd, msg, wparam, lparam);
}

void WindowApp::MainLoop()
{
	SampleMainLoop();
}

void WindowApp::SampleMainLoop()
{
	MSG	 msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, _hwnd, NULL, NULL)) != 0)
	{
		//エラー処理
		if (bRet == -1)
		{
			//ウィンドウが破棄されていたらループを抜ける。
			if (IsWindow(msg.hwnd) == 0)
				break;

			//エラー対応

			continue;
		}

		//メッセージ処理
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void WindowApp::SampleGameLoop()
{
	MSG	 msg;
	while (true)
	{
		//メッセージ処理
		if (PeekMessage(&msg, _hwnd, NULL, NULL, PM_REMOVE))
		{
			static int cnt = 0;
			printf("%d\n", cnt++);
			//メッセージ処理
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		//ウィンドウが破棄されていたらループを抜ける。
		if (IsWindow(_hwnd) == 0)
			break;

		//ゲーム処理
	}
}
