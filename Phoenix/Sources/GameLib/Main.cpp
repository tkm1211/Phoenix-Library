#include <Windows.h>
#include <memory>
#include <assert.h>
#include <tchar.h>
#include <time.h>

#include "FrameWork.h"
#include "ScreenInformation.h"
#include "Utilities.h"


#pragma comment( lib, "winmm.lib" )


LRESULT CALLBACK FnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Phoenix::Lib::FrameWork* f = reinterpret_cast<Phoenix::Lib::FrameWork*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return f ? f->HandleMessage(hwnd, msg, wparam, lparam) : DefWindowProc(hwnd, msg, wparam, lparam);
}


INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, INT cmdShow)
{
#ifdef USE_DEBUG

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(87481);

#endif

	// �}���`�X���b�h�Ή�
	HRESULT hr = CoInitializeEx(0, COINITBASE_MULTITHREADED);
	_ASSERT_EXPR(SUCCEEDED(hr), hrTrace(hr));

	// �E�B���h�E�쐬
	HWND hwnd;
	{
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = FnWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = instance;
		wcex.hIcon = 0;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = _T("Phoenix");
		wcex.hIconSm = 0;
		RegisterClassEx(&wcex);

		RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		hwnd = CreateWindow(_T("Phoenix"), _T("Phoenix"), WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, instance, NULL);
		ShowWindow(hwnd, cmdShow);
	}

	// �����ݒ�
	srand((unsigned int)time(NULL));

	// Drag&Drop�ɑΉ�
	DragAcceptFiles(hwnd, TRUE);

	// FrameWork�쐬
	Phoenix::Lib::FrameWork f(hwnd, instance);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&f));

	return f.Run();

}