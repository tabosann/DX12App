
#include"WindowBase.h"
#include<crtdbg.h>

WindowFactory::WindowFactory()
{
	ZeroMemory(&_wndClass,	sizeof(WNDCLASSEX));
	ZeroMemory(&_cs,		sizeof(CREATESTRUCT));
	ZeroMemory(&_hwnd,		sizeof(HWND));

	//デフォルト設定
	_wndClass =
	{
		sizeof(WNDCLASSEX),						//この構造体のデータサイズ
		CS_HREDRAW | CS_VREDRAW,				//ウィンドウ操作に対する処理。ここでは「サイズ調節でウィンドウの再描画」を指定
		(WNDPROC)WindowFactory::GlobalWndProc,	//クラス内でグローバルなウィンドウプロシージャ
		NULL,									//この構造体データの後ろに追加するメモリ領域のサイズ
		NULL,									//ウィンドウが生成される度に確保されるメモリ領域のサイズ
		GetModuleHandle(NULL),					//アプリケーションのインスタンスハンドル　詳細：https://bit.ly/2VzQ3Ac
		NULL,									//デスクトップなどに表示されるアイコンの設定
		NULL,									//カーソルの指定。ここではデフォルト
		NULL,									//再描画の際に塗る色
		NULL,									//メニューのリソース名。ここではメニューを持たないよう設定
		NULL,									//ウィンドウクラスの名前。適当で大丈夫
		NULL									//タスクバーなどに表示されるアイコンの設定
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

WindowFactory::~WindowFactory()
{
	
}

bool WindowFactory::Init(LPCTSTR className, int posX, int posY, int width, int height)
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

bool WindowFactory::RegisterWindowClass(LPCTSTR className)
{
	size_t size = _tcslen(className) + 1;
	if (TCHAR* src = new TCHAR[size])
	{
		_tcscpy_s(src, size, className);
		_wndClass.lpszClassName = src;
	}
	return RegisterClassEx(&_wndClass);
}

bool WindowFactory::CreateMainWindow(HINSTANCE hInstance, LPCTSTR className)
{
	_cs.hInstance = hInstance;
	_cs.lpszClass = className;
	_cs.lpszName  = className;
	return _hwnd = CreateWindowEx(
		_cs.dwExStyle, _cs.lpszClass, _cs.lpszName, _cs.style, _cs.x, _cs.y, _cs.cx, _cs.cy, 
		_cs.hwndParent, _cs.hMenu, _cs.hInstance, _cs.lpCreateParams
	);
}

LRESULT WindowFactory::GlobalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	WindowFactory* ptr = (WindowFactory*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	//取得できた場合(ウィンドウ作成後)
	if (ptr) return ptr->LocalWndProc(hwnd, msg, wparam, lparam);

	//取得できなかった場合(ウィンドウ初期化時)
	if (msg == WM_CREATE)
	{
		if (ptr = (WindowFactory*)((LPCREATESTRUCT)lparam)->lpCreateParams)
		{
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)ptr);
			return ptr->LocalWndProc(hwnd, msg, wparam, lparam);
		}
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT WindowFactory::LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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

bool WindowFactory::Release() const
{
	return UnregisterClass(_wndClass.lpszClassName, _wndClass.hInstance);
}

WNDCLASSEX WindowFactory::GetWndClass() const
{
	return _wndClass;
}

CREATESTRUCT WindowFactory::GetCreateStruct() const
{
	return _cs;
}

HWND WindowFactory::GetHWND() const
{
	return _hwnd;
}

void WindowFactory::SetWindowClass(const WNDCLASSEX& wndClass)
{
	WNDPROC temp = _wndClass.lpfnWndProc;
	_wndClass = wndClass;
	_wndClass.lpfnWndProc = temp;
}

void WindowFactory::SetCreateStruct(const CREATESTRUCT& cs)
{
	_cs = cs;
}
