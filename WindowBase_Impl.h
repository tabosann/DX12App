#pragma once

#include"WindowBase.h"

//何らかのメッセージを受け取るとTrueを返す
inline bool CatchMsg()
{
	MSG msg;
	if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		return true;
	}
	return false;
}

inline WNDCLASSEX WindowBase::GetWndClass() const
{
	return _wndClass;
}

inline CREATESTRUCT WindowBase::GetCreateStruct() const
{
	return _cs;
}

inline HWND WindowBase::GetHWND() const
{
	return _hwnd;
}

inline void WindowBase::SetWindowClass(const WNDCLASSEX& wndClass)
{
	WNDPROC temp = _wndClass.lpfnWndProc;
	_wndClass = wndClass;
	_wndClass.lpfnWndProc = temp;
}

inline void WindowBase::SetCreateStruct(const CREATESTRUCT& cs)
{
	_cs = cs;
}

inline bool WindowApp::Begin(LPCTSTR className, int posX, int posY, int width, int height)
{
	return Init(className, posX, posY, width, height);
}

inline void WindowApp::Show() const
{
	ShowWindow(_hwnd, SW_SHOW);
}

inline bool WindowApp::Update() const
{
	return UpdateWindow(_hwnd);
}

inline bool WindowApp::End()
{
	return Release();
}
