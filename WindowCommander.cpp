
#include"WindowBase.h"

void WindowCommander::Show(HWND hwnd) const
{
	ShowWindow(hwnd, SW_SHOW);
}

bool WindowCommander::Update(HWND hwnd) const
{
	return UpdateWindow(hwnd);
}

void WindowCommander::SampleMainLoop(HWND hwnd)
{
	MSG	 msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, hwnd, NULL, NULL)) != 0)
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

void WindowCommander::SampleGameLoop(HWND hwnd)
{
	MSG	 msg;
	while (true)
	{
		//メッセージ処理
		if (PeekMessage(&msg, hwnd, NULL, NULL, PM_REMOVE))
		{
			//メッセージ処理
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		//ウィンドウが破棄されていたらループを抜ける。
		if (IsWindow(hwnd) == 0)
			break;

		//ゲーム処理
	}
}
