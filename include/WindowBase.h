#pragma once

#include<Windows.h>
#include<tchar.h>

namespace WindowBaseLibrary
{
	//--------------------------------------------------------------------------------------------------------------
	// 
	// ウィンドウの登録、作成のみを行うクラス
	// 
	//--------------------------------------------------------------------------------------------------------------
	class WindowFactory
	{
	//メンバ変数
	protected:

		WNDCLASSEX		_wndClass;	//ウィンドウの基本設定を保持する構造体
		CREATESTRUCT	_cs;		//ウィンドウ作成に必要なパラメータを保持する構造体
		HWND			_hwnd;		//ウィンドウハンドラ(＝ウィンドウオブジェクトのポインタ)

	//コンストラクタ
	protected:

		WindowFactory();
		WindowFactory(const WindowFactory&) = delete;
		WindowFactory& operator=(const WindowFactory&) = delete;

	//初期化関連
	protected:

		//ウィンドウの設定から作成までを行い、いつ表示しても大丈夫な状態にする。
		bool			Init(
			LPCTSTR className,
			int		posX   = CW_USEDEFAULT,
			int		posY   = CW_USEDEFAULT,
			int		width  = CW_USEDEFAULT,
			int		height = CW_USEDEFAULT
		);

		//設定したウィンドウの見た目やプロシージャを登録する。
		bool			RegisterWindowClass(LPCTSTR className);

		//登録したウィンドウの設定でウィンドウを作成。
		bool			CreateMainWindow(HINSTANCE hInstance, LPCTSTR className);

	//ウィンドウプロシージャ
	protected:
		virtual LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	private:
		static	LRESULT GlobalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//後片付け
	protected:

		//解放処理
		bool			Release();

	//アクセッサ
	public:

		WNDCLASSEX		GetWndClass() const;
		CREATESTRUCT	GetCreateStruct() const;
		void			SetWindowClass(const WNDCLASSEX& wndClass);
		void			SetCreateStruct(const CREATESTRUCT& cs);
	};

	//--------------------------------------------------------------------------------------------------------------
	// 
	// 作成されたウィンドウを、ユーザーが扱いやすくテコ入れされたクラス
	// 
	//--------------------------------------------------------------------------------------------------------------
	class WindowBase : public WindowFactory
	{
	//独自実装が可能なメンバ関数
	public:

		//各自オーバーライドして独自の実装ができます。
		virtual LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		virtual void	MainLoop();

	//実装済みメンバ関数
	public:

		bool			Begin(
			LPCTSTR className,
			int		posX   = CW_USEDEFAULT,
			int		posY   = CW_USEDEFAULT,
			int		width  = CW_USEDEFAULT,
			int		height = CW_USEDEFAULT
		);
		void			ShowWindow() const;
		bool			UpdateWindow() const;
		bool			End();

	//ウィンドウに対する操作を行うクラス
	private:

		class WindowCommander;
	};

	//--------------------------------------------------------------------------------------------------------------
	// 
	// ウィンドウに対する操作のみを行うクラス(WindowBaseクラスのみアクセス可能なクラス)
	// 
	//--------------------------------------------------------------------------------------------------------------
	class WindowBase::WindowCommander
	{
	public:

		//ウィンドウの表示
		static void		ShowWindow(HWND hwnd);
		//WM_PAINTメッセージの送信
		static bool		UpdateWindow(HWND hwnd);

		//ループ処理のサンプル
		static void		SampleMainLoop(HWND hwnd);
		//ゲームのメインループ実装例
		static void		SampleGameLoop(HWND hwnd);
	};
}
namespace wbl = WindowBaseLibrary;

#include"impl\WindowBase_Impl.h"

#pragma comment(lib, "WindowBaseLibrary.lib")
