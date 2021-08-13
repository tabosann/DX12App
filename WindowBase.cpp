
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

	//�f�t�H���g�ݒ�
	_wndClass =
	{
		sizeof(WNDCLASSEX),					//���̍\���̂̃f�[�^�T�C�Y
		CS_HREDRAW | CS_VREDRAW,			//�E�B���h�E����ɑ΂��鏈���B�����ł́u�T�C�Y���߂ŃE�B���h�E�̍ĕ`��v���w��
		(WNDPROC)WindowBase::GlobalWndProc,	//�N���X���ŃO���[�o���ȃE�B���h�E�v���V�[�W��
		NULL,								//���̍\���̃f�[�^�̌��ɒǉ����郁�����̈�̃T�C�Y
		NULL,								//�E�B���h�E�����������x�Ɋm�ۂ���郁�����̈�̃T�C�Y
		GetModuleHandle(NULL),				//�A�v���P�[�V�����̃C���X�^���X�n���h���@�ڍׁFhttps://bit.ly/2VzQ3Ac
		NULL,								//�f�X�N�g�b�v�Ȃǂɕ\�������A�C�R���̐ݒ�
		NULL,								//�J�[�\���̎w��B�����ł̓f�t�H���g
		NULL,								//�ĕ`��̍ۂɓh��F
		NULL,								//���j���[�̃��\�[�X���B�����ł̓��j���[�������Ȃ��悤�ݒ�
		NULL,								//�E�B���h�E�N���X�̖��O�B�K���ő��v
		NULL								//�^�X�N�o�[�Ȃǂɕ\�������A�C�R���̐ݒ�
	};

	//�f�t�H���g�ݒ�
	_cs =
	{
		this,					//CreateWindowEx�֐���lpParam�ɓn��
		_wndClass.hInstance,	//�A�v���P�[�V�����̃C���X�^���X�n���h��
		NULL,					//�E�B���h�E���j���[�̃n���h��
		NULL,					//�e�E�B���h�E�̃n���h���B�������e�E�B���h�E�Ȃ�NULL
		CW_USEDEFAULT,			//�E�B���h�E�̏c�̒����i�s�N�Z�����j
		CW_USEDEFAULT,			//�E�B���h�E�̉��̒����i�s�N�Z�����j
		CW_USEDEFAULT,			//�E�B���h�E���㒸�_��y���W�B�������q�E�B���h�E�Ȃ�A�e�E�B���h�E��̂̃��[�J�����W�n�ł̘b�ɂȂ�B
		CW_USEDEFAULT,			//�E�B���h�E���㒸�_��x���W�B����
		WS_OVERLAPPEDWINDOW,	//�E�B���h�E�̌����ځB�����ł̓f�t�H���g�ݒ�
		NULL,					//�E�B���h�E�̖��O
		NULL,					//�o�^�����E�B���h�E�N���X�̖��O
		NULL					//�E�B���h�E�̌����ځi�g���Łj�B����܏ڍׂ킩���ĂȂ�...
	};
}

WindowBase::~WindowBase()
{
	//_ASSERT_EXPR(UnregisterClass(_cs.lpszClass, _cs.hInstance), _T("�o�^�����Ɏ��s���܂����B"));
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

	//�擾�ł����ꍇ(�E�B���h�E�쐬��)
	if (ptr) return ptr->LocalWndProc(hwnd, msg, wparam, lparam);

	//�擾�ł��Ȃ������ꍇ(�E�B���h�E��������)
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
		_ASSERT_EXPR(DestroyWindow(hwnd), _T("�E�B���h�E�j���Ɏ��s���܂����B"));
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

void WindowApp::SampleGameLoop()
{
	MSG	 msg;
	while (true)
	{
		//���b�Z�[�W����
		if (PeekMessage(&msg, _hwnd, NULL, NULL, PM_REMOVE))
		{
			static int cnt = 0;
			printf("%d\n", cnt++);
			//���b�Z�[�W����
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		//�E�B���h�E���j������Ă����烋�[�v�𔲂���B
		if (IsWindow(_hwnd) == 0)
			break;

		//�Q�[������
	}
}
