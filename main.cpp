
#include"WindowBase.h"
#include<ExString-main/ExString.h>
#include<DebugTools-main/DebugTools.h>

class GameWindow : public WindowBase
{
public:

	LRESULT	LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WindowBase* pWndList[2] =
	{
		new GameWindow,
		new WindowBase,
	};

	for (int i = 0; i < _countof(pWndList); ++i)
	{
		WindowBase* hwnd = pWndList[i];

		if (hwnd->Begin(ExTString(L"GameWindow No.%d", i)))
		{
			hwnd->Show();
			hwnd->Update();
			hwnd->MainLoop();

			if (!hwnd->End())
				ErrorExit((LPTSTR)_T("End Func"));
		}
	}
}

#include<iostream>
LRESULT GameWindow::LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static int cnt = 0;
	switch (msg)
	{
	case WM_CREATE:
		MessageBox(hwnd, _T("Hello World!"), _T("Hello World!"), NULL);
		break;

	case WM_PAINT:
		std::cout << cnt++ << std::endl;
		break;

	case WM_KEYDOWN:
		if (wparam == VK_SPACE) DestroyWindow(hwnd);
		break;

	default:
		break;
	}
	return WindowFactory::LocalWndProc(hwnd, msg, wparam, lparam);
}
