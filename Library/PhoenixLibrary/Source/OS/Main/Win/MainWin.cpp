#include "pch.h"
#include <crtdbg.h>
#include <stdio.h>
#include "Phoenix/Types.h"
#include "Phoenix/OS/Main.h"
#include "Phoenix/OS/Path.h"


// ���C�����[�v
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if	defined(PHOENIX_TARGET_DEBUG)

	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �������[���[�N�ꏊ�����p
	//_CrtSetBreakAlloc(196);	// �����͊m�ۂ��ꂽ����

#endif
	MSG msg;
	msg.wParam = 0;

	std::unique_ptr<Phoenix::OS::IMain> main = Phoenix::OS::IMain::Create();

	char name[128];
	GetModuleFileNameA(hInstance, name, sizeof(name));

	//main->SetName(Phoenix::OS::Path::GetFileNameWithoutExtension(Phoenix::OS::Path::GetFileName(name)));

	// ������
	if (main->Initialize(reinterpret_cast<Phoenix::uintPtr>(hInstance)))
	{
		// ���C�����[�v
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
