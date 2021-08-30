#pragma once

#include<tchar.h>	//マルチバイト文字か、ワイド文字か、使用環境より判断
#include<Windows.h>	//OutputDebugStringマクロ
#include<crtdbg.h>	//_ASSERT_EXPRマクロ

//便利なデバッグ機能を、可変長引数を扱えるように拡張。
#ifdef _DEBUG

	//可変長引数を扱えるOutputDebugStringマクロ
	#ifndef OutputDebugStringEx
		#define OutputDebugStringEx(format, ...)\
				{														\
					int size = _sctprintf(format, __VA_ARGS__) + 1;		\
					if(TCHAR* tstr = new TCHAR[size])					\
					{													\
						_stprintf_s(tstr, size, format, __VA_ARGS__);	\
						OutputDebugString(tstr);						\
						delete[] tstr;									\
					}													\
				}
	#endif

	//可変長引数を扱える_ASSERT_EXPRマクロ
	#ifndef _ASSERT_EXPR_EX
		#define _ASSERT_EXPR_EX(expr, format, ...)\
				{														\
					int size = _sctprintf(format, __VA_ARGS__) + 1;		\
					if(TCHAR* tstr = new TCHAR[size])					\
					{													\
						_stprintf_s(tstr, size, format, __VA_ARGS__);	\
						_ASSERT_EXPR(expr, tstr);						\
						delete[] tstr;									\
					}													\
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

//メモリリークを調べるためのデフォルト設定
#ifndef _CRTDBG_DEFAULT_CHECK_MEMORYLEAK_DF
	#define _CRTDBG_DEFAULT_CHECK_MEMORYLEAK_DF _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF
#endif

namespace DebugTools
{
	// プログラム開始から終了までの間で、メモリリークが発生したか調べる。
	// 発生していた場合は、デバッグウィンドウにその旨を表示する。
	// NOTE : 上記の説明はデフォルト設定の場合を指す
	void DebugMemoryLeak(int addFlag = NULL);

	// WindowsAPIの直近のエラーコードをウィンドウ画面で説明し、そのエラーコードでプログラムを終了する。
	// NOTE : デバッグモードでないなら、この関数はなにもしない。
	void ErrorExit(LPTSTR lpszFunction);
}
namespace dbg = DebugTools;

#pragma comment(lib, "DebugToolsLibrary.lib")
