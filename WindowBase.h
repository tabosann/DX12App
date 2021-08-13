#pragma once

#include<Windows.h>
#include<tchar.h>

class WindowBase 
{
//メンバ変数
protected:

	WNDCLASSEX		_wndClass;	//ウィンドウの基本設定を保持する構造体
	CREATESTRUCT	_cs;		//ウィンドウ作成に必要なパラメータを保持する構造体
	HWND			_hwnd;		//ウィンドウハンドラ(＝ウィンドウオブジェクト)

//コンストラクタ、デストラクタ
protected:

	WindowBase();
	virtual ~WindowBase();

	WindowBase(const WindowBase&) = delete;
	WindowBase& operator=(const WindowBase&) = delete;

//初期化関連
protected:

	//ウィンドウの設定から作成までを行い、いつ表示しても大丈夫な状態にする。
	bool Init(LPCTSTR className, int posX = CW_USEDEFAULT, int posY = CW_USEDEFAULT, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);
	
	//設定したウィンドウの見た目やプロシージャを登録する。
	bool RegisterWindowClass(LPCTSTR className);

	//登録したウィンドウの設定でウィンドウを作成。
	bool CreateMainWindow(HINSTANCE hInstance, LPCTSTR className);

//ウィンドウプロシージャ
protected:
	virtual LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	static	LRESULT GlobalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

//後片付け
protected:

	//デストラクタが呼ばれる前にやっておきたい解放処理など
	virtual bool Release() const;

//アクセッサ
public:

	WNDCLASSEX		GetWndClass() const;
	CREATESTRUCT	GetCreateStruct() const;

	// NOTE : ウィンドウハンドラはポインタであることに注意。
	HWND			GetHWND() const;

	void			SetWindowClass(const WNDCLASSEX& wndClass);
	void			SetCreateStruct(const CREATESTRUCT& cs);
};

class WindowApp : public WindowBase
{
//ウィンドウのお仕事内容
public:

	//ウィンドウプロシージャをオーバーライドし、自由に処理を追加する。
	LRESULT	LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

//ウィンドウのお仕事準備
public:

	//初期化
	bool Begin(LPCTSTR className, int posX = CW_USEDEFAULT, int posY = CW_USEDEFAULT, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);

//ウィンドウのお仕事実践
public:

	//ウィンドウの表示
	void		 Show() const;
	//WM_PAINTメッセージの送信
	bool		 Update() const;

	//オーバーライドして中身を各自実装してください。
	virtual void MainLoop();
	//ループ処理のサンプル
	void		 SampleMainLoop();
	//ゲームのメインループ実装例
	void		 SampleGameLoop();

//ウィンドウのお仕事終了
public:

	//解放処理
	bool End();
};

#include"WindowBase_Impl.h"
