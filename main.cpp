
#include"WindowBase.h"
#include"DebugTools.h"

class GameWindow : public WindowApp
{
public:

	LRESULT	LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

int main()
{
	WindowBase* app[2] = {
		new WindowApp,
		new GameWindow
	};

	for (int i = 0; i < _countof(app); ++i)
	{
		WindowApp* pWnd = app[i];

		if (pWnd->Begin(_T("恐ろしいウィンドウ")))
		{
			pWnd->Show();
			pWnd->Update();
			pWnd->MainLoop();

			if (!pWnd->End())
				ErrorExit((LPTSTR)_T("WindowApp::End"));
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
	return WindowBase::LocalWndProc(hwnd, msg, wparam, lparam);
}
