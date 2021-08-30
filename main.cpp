
#include<Dx12App.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Dx12App& app = Dx12App::Instance();

	//�E�B���h�E�̐ݒ���f�t�H���g����ύX
	auto prop = app.GetWndClass();
	prop.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	app.SetWindowClass(prop);

	//�E�B���h�E�ɑ΂��鑀��
	if (app.Begin(_T("Test Window")))
	{
		app.Run();
		app.End();
	}

	Dx12App::Release();
}
