#pragma once

#include"WindowBase.h"
#include"Dx12Base.h"
#include<iostream>

class Dx12App : public wbl::WindowBase
{
// 外部で使用する関数
public:

	// 唯一の実体を生成し、それを返す。
	static Dx12App& Instance();

	// プログラムの起動（Begin関数とEnd関数の間にやること）
	void			Run();

	// 唯一の実体を解放する
	// NOTE : 一度解放すると、二度と同じものを使えなくなるので注意
	static void		Release();

// 外部で使用しない関数
private:

	// 生成したウィンドウの動作をここで決める
	virtual LRESULT	LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// ここに自分がやりたいことをプログラムする
	// e.g. 描画、GPUによる計算など
	virtual void	MainLoop();

	// コンストラクタ
	// NOTE : シングルトン設計のため、Instance関数でのみ使用すること
	Dx12App() : wbl::WindowBase() {}

	// デストラクタ
	// NOTE : Release関数でのみ使用する（delete演算子で呼び出す）こと
	~Dx12App() { delete _pDx12Base; }

	// 唯一の実体へのポインタ
	static Dx12App* _pInstance;

	// Dx12の初期化を担う（こいつに作成済のウィンドウハンドラを渡すこと）
	Dx12Base*		_pDx12Base;
};
