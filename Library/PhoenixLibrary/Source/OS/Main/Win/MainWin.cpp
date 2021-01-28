#include "pch.h"
#include <crtdbg.h>
#include <stdio.h>
#include "Phoenix/Types.h"
#include "Phoenix/OS/Main.h"
#include "Phoenix/OS/Path.h"


// メインループ
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if	defined(PHOENIX_TARGET_DEBUG)

	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// メモリーリーク場所発見用
	//_CrtSetBreakAlloc(196);	// 引数は確保された順番

#endif
	MSG msg;
	msg.wParam = 0;

	std::unique_ptr<Phoenix::OS::IMain> main = Phoenix::OS::IMain::Create();

	char name[128];
	GetModuleFileNameA(hInstance, name, sizeof(name));

	//main->SetName(Phoenix::OS::Path::GetFileNameWithoutExtension(Phoenix::OS::Path::GetFileName(name)));

	// 初期化
	if (main->Initialize(reinterpret_cast<Phoenix::uintPtr>(hInstance)))
	{
		// メインループ
		for (;;)
		{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if (!::GetMessage(&msg, NULL, 0, 0)) break;
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				//if (main->IsExit())
				//	::PostQuitMessage(0);
				//else
					main->Run();
			}
		}
	}
	main->Finalize();

	return (int)msg.wParam;
}
