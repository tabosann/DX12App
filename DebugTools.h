#pragma once

#include<tchar.h>	//シングルか、マルチバイト文字か、自動で判断するマクロがたくさん。
#include<Windows.h>	//OutputDebugStringマクロ
#include<crtdbg.h>	//_ASSERT_EXPRマクロ

//便利なデバッグ機能を、可変長引数を扱えるように拡張。
#ifdef _DEBUG

	//可変長引数を扱えるOutputDebugStringマクロ
	#ifndef OutputDebugStringEx
		#define OutputDebugStringEx(str, ...)\
				{											\
					TCHAR tstr[512];						\
					_stprintf_s(tstr, str, __VA_ARGS__);	\
					OutputDebugString(tstr);				\
				}
	#endif

	//可変長引数を扱える_ASSERT_EXPRマクロ
	#ifndef _ASSERT_EXPR_EX
		#define _ASSERT_EXPR_EX(expr, str, ...)\
				{											\
					TCHAR tstr[256];						\
					_stprintf_s(tstr, str, __VA_ARGS__);	\
					_ASSERT_EXPR(expr, tstr);				\
				}
	#endif

#else

	//デバッグモードでないなら、何もしないようにする(空っぽを定義)。

	#ifndef OutputDebugStringEx
		#define OutputDebugStringEx(str, ...)
	#endif

	#ifndef _ASSERT_EXPR_EX
		#define _ASSERT_EXPR_EX(expr, str, ...)
	#endif

#endif

#ifndef _ASSERT_BASE_EX
	#define _ASSERT_BASE_EX _ASSERT_EXPR_EX
#endif

// WindowsAPIの直近のエラーコードをウィンドウ画面で説明し、そのエラーコードでプログラムを終了する。
// NOTE : デバッグモードでないなら、この関数はなにもしない。
void ErrorExit(LPTSTR lpszFunction);
