#pragma once

#include"WindowBase.h"

inline bool WindowBase::Begin(LPCTSTR className, int posX, int posY, int width, int height)
{
	return Init(className, posX, posY, width, height);
}

inline void WindowBase::Show() const
{
	WindowCommander::Show(_hwnd);
}

inline bool WindowBase::Update() const
{
	return WindowCommander::Update(_hwnd);
}

inline void WindowBase::MainLoop()
{
	SampleMainLoop(_hwnd);
}

inline bool WindowBase::End()
{
	return Release();
}
