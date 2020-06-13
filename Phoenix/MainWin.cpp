#include <Windows.h>
#include <tchar.h>
#include <assert.h>
#include <time.h>
#include "Phoenix/OS/Main.h"


int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
#ifdef USE_DEBUG

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(87481);

#endif

	// マルチスレッド対応
	//HRESULT hr = CoInitializeEx(0, COINITBASE_MULTITHREADED);
	//assert(!hr);

	// 乱数設定
	srand((unsigned int)time(NULL));

	//// サイズ調整
	//RECT rect;
	//{
	//	int x = 0;
	//	int y = 0;

	//	SetRect(&rect, x, y, 1920, 1080);
	//	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	//}

	// ウィンドウ作成
	//HWND hwnd;
	//{
	//	WNDCLASSEX wcex;
	//	wcex.cbSize = sizeof(WNDCLASSEX);
	//	wcex.style = CS_HREDRAW | CS_VREDRAW;
	//	wcex.lpfnWndProc = Phoenix::OS::DisplayWin;
	//	wcex.cbClsExtra = 0;
	//	wcex.cbWndExtra = 0;
	//	wcex.hInstance = instance;
	//	wcex.hIcon = 0;
	//	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	//	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//	wcex.lpszMenuName = NULL;
	//	wcex.lpszClassName = _T("Phoenix");
	//	wcex.hIconSm = 0;
	//	RegisterClassEx(&wcex);

	//	hwnd = CreateWindow(_T("Phoenix"), _T("Phoenix"),
	//		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,
	//		CW_USEDEFAULT, CW_USEDEFAULT,
	//		rect.right - rect.left, rect.bottom - rect.top,
	//		NULL, NULL, instance, NULL);

	//	// 表示
	//	ShowWindow(hwnd, SW_SHOW);
	//	UpdateWindow(hwnd);
	//}

	// Drag&Dropに対応
	//DragAcceptFiles(hwnd, TRUE);

	// メイン作成
	//std::unique_ptr<Phoenix::OS::IMain> main = Phoenix::OS::IMain::Create();

	MSG msg = {};
	//if (main->Initialize(_T("Phoenix"), 1920, 1080, reinterpret_cast<uintptr_t>(instance)))
	//{
	//	// ゲームループ
	//	while (WM_QUIT != msg.message)
	//	{
	//		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	//		{
	//			if (msg.message == WM_KEYDOWN && (msg.wParam == VK_ESCAPE)) PostQuitMessage(0);

	//			TranslateMessage(&msg);
	//			DispatchMessage(&msg);
	//		}
	//		else
	//		{
	//			/*timer.Tick();
	//			CalculateFrameStats();
	//			elapsedTime = timer.TimeInterval();

	//			Update(elapsedTime);
	//			Render(elapsedTime);*/

	//			main->Run();
	//		}
	//	}
	//}
	//main->Finalize();

	return (int)msg.wParam;
}