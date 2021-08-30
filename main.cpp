
#include<Dx12App.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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

	Dx12App::Release();
}
