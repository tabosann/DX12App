#pragma once

#include"..\WindowBase.h"

inline bool wbl::WindowBase::Begin(LPCTSTR className, int posX, int posY, int width, int height)
{
	return Init(className, posX, posY, width, height);
}

inline void wbl::WindowBase::ShowWindow() const
{
	WindowCommander::ShowWindow(_hwnd);
}

inline bool wbl::WindowBase::UpdateWindow() const
{
	return WindowCommander::UpdateWindow(_hwnd);
}

inline LRESULT wbl::WindowBase::LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return WindowFactory::LocalWndProc(hwnd, msg, wparam, lparam);
}

inline void wbl::WindowBase::MainLoop()
{
	WindowCommander::SampleMainLoop(_hwnd);
}

inline bool wbl::WindowBase::End()
{
	return Release();
}
