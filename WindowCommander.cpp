
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
		//�G���[����
		if (bRet == -1)
		{
			//�E�B���h�E���j������Ă����烋�[�v�𔲂���B
			if (IsWindow(msg.hwnd) == 0)
				break;

			//�G���[�Ή�

			continue;
		}

		//���b�Z�[�W����
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void WindowCommander::SampleGameLoop(HWND hwnd)
{
	MSG	 msg;
	while (true)
	{
		//���b�Z�[�W����
		if (PeekMessage(&msg, hwnd, NULL, NULL, PM_REMOVE))
		{
			//���b�Z�[�W����
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		//�E�B���h�E���j������Ă����烋�[�v�𔲂���B
		if (IsWindow(hwnd) == 0)
			break;

		//�Q�[������
	}
}
