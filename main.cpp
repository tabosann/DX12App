
#include<Dx12App.h>

int main()
{
	// メモリリークのチェック
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Dx12Appの実体を得る
	Dx12App& app = Dx12App::Instance();

	//ウィンドウの設定をデフォルトから変更
	auto prop = app.GetWndClass();
	prop.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	app.SetWindowClass(prop);

	//ウィンドウに対する操作
	if (app.Begin(_T("Test Window")))
	{
		app.Run();
		app.End();
	}

	// 実体の解放をお忘れなく
	Dx12App::Release();
}
