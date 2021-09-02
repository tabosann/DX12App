#pragma once

#include"WindowBase.h"
#include"Dx12Base.h"
#include<iostream>

class Dx12App : public wbl::WindowBase
{
// ユーザ定義関数
private:

	// 生成したウィンドウの動作をここで決める
	virtual LRESULT	LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// ここに自分がやりたいことをプログラムする
	// e.g. 描画、GPUによる計算など
	virtual void	MainLoop();

public:

	// 唯一の実体を生成し、それを返す。
	static Dx12App& Instance();

	// プログラムの起動（Begin関数とEnd関数の間にやること）
	void			Run();

	// 唯一の実体を解放する
	// NOTE : 一度解放すると、二度と同じものを使えなくなるので注意
	static void		Release();

private:

	// コンストラクタ
	// NOTE : シングルトン設計のため、Instance関数でのみ使用すること
	Dx12App() : wbl::WindowBase(), _pDx12Base(nullptr) {}

	// デストラクタ
	// NOTE : Release関数でのみ使用する（delete演算子で呼び出す）こと
	~Dx12App() { delete _pDx12Base; }

	// ユーザ定義のタスク識別番号
	struct UserTaskID : Dx12Base::TaskID
	{
		using TaskID::TaskID;
		enum _TaskID
		{
			IMGUI_DX12_INIT = LAST_TASK_ID + 1,
			IMGUI_DX12_BEGIN,
			IMGUI_DX12_UPDATE,
			IMGUI_DX12_END,
			LAST_USER_TASK_ID
		};
	};

private:

	// 唯一の実体へのポインタ
	static Dx12App* _pInstance;

	// Dx12の初期化を担う（こいつに作成済のウィンドウハンドラを渡すこと）
	Dx12Base*		_pDx12Base;
};

// タスククラスに渡したいパラメータをユーザがここで実装する
class Dx12UserComponent : public Dx12Component
{
public:

	using Dx12Component::Dx12Component;

	FLOAT	_backGroundColor[4];	// RGBA

	// TODO : 立方体の頂点バッファ、インデックスバッファ
};
