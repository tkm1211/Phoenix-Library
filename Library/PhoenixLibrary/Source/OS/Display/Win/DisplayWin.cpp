#include "pch.h"

#include <stdio.h>
#include <wchar.h>
#include <locale.h>

#include "DisplayWin.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"


namespace Phoenix
{
	namespace OS
	{
		// TODO : move to file
		// mbstowcs
		int MbsToWcs(wchar_t* wcstr, int sizeInWords, const char* mbstr, int count)
		{
			size_t ret;
			mbstowcs_s(&ret, wcstr, sizeInWords, mbstr, count);
			return static_cast<int>(ret);
		}

		std::unique_ptr<IDisplay> IDisplay::Create()
		{
			return std::make_unique<DisplayWin>();
		}

		bool DisplayWin::Initialize(const wchar_t* name, Phoenix::s32 width, Phoenix::s32 height, Phoenix::uintPtr instance)
		{
			//MbsToWcs(this->name, sizeof(this->name), name, static_cast<int>(strlen(name)));
			wcscpy_s(this->name, name);
			hInstaice = reinterpret_cast<HINSTANCE>(instance);

			// サイズ調整
			RECT rect;
			{
				int x = 0;
				int y = 0;
				this->width = width;
				this->height = height;

				SetRect(&rect, x, y, width, height);
				AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
			}

			// ウィンドウ作成
			{
				WNDCLASSEX wcex;
				wcex.cbSize = sizeof(WNDCLASSEX);
				wcex.style = CS_HREDRAW | CS_VREDRAW;
				wcex.lpfnWndProc = HandleMessage;
				wcex.cbClsExtra = 0;
				wcex.cbWndExtra = 0;
				wcex.hInstance = hInstaice;
				wcex.hIcon = 0;
				wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
				wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
				wcex.lpszMenuName = NULL;
				wcex.lpszClassName = this->name;
				wcex.hIconSm = 0;
				RegisterClassEx(&wcex);

				hwnd = CreateWindow(this->name, this->name,
					WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,
					CW_USEDEFAULT, CW_USEDEFAULT,
					rect.right - rect.left, rect.bottom - rect.top,
					NULL, NULL, hInstaice, NULL);

				// 表示
				ShowWindow(hwnd, SW_SHOW);
				UpdateWindow(hwnd);
			}

			// Drag&Dropに対応
			DragAcceptFiles(hwnd, TRUE);

			// 自分を関連付け
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

			// TODO : 作り直し パッド初期化
			PAD.hwnd = hwnd;
			PAD.instance = hInstaice;
			PAD.InitInputDevice();

			return true;
		}

		void DisplayWin::Finalize()
		{
			if (hwnd != nullptr)
			{
				DestroyWindow(hwnd);
			}
			hwnd = nullptr;

			if (name[0])
			{
				UnregisterClass(name, hInstaice);
				name[0] = 0;
			}
		}

		Handle DisplayWin::GetHWND()
		{
			return hwnd;
		}

		int DisplayWin::GetWidth()
		{
			return width;
		}

		int DisplayWin::GetHeight()
		{
			return height;
		}

		WindowHandle DisplayWin::GetHandle()
		{
			return reinterpret_cast<WindowHandle>(hwnd);
		}

		//bool ImGui_ImplWin32_UpdateMouseCursor()
		//{
		//	ImGuiIO& io = ImGui::GetIO();
		//	if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
		//		return false;

		//	ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		//	if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
		//	{
		//		// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
		//		::SetCursor(NULL);
		//	}
		//	else
		//	{
		//		// Show OS mouse cursor
		//		LPTSTR win32_cursor = IDC_ARROW;
		//		switch (imgui_cursor)
		//		{
		//		case ImGuiMouseCursor_Arrow:        win32_cursor = IDC_ARROW; break;
		//		case ImGuiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
		//		case ImGuiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
		//		case ImGuiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
		//		case ImGuiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
		//		case ImGuiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
		//		case ImGuiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
		//		case ImGuiMouseCursor_Hand:         win32_cursor = IDC_HAND; break;
		//		}
		//		::SetCursor(::LoadCursor(NULL, win32_cursor));
		//	}
		//	return true;
		//}

#define DBT_DEVNODES_CHANGED 0x0007

		LRESULT CALLBACK DisplayWin::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			//{
			//	if (ImGui::GetCurrentContext() == NULL)
			//		return 0;

			//	ImGuiIO& io = ImGui::GetIO();
			//	switch (msg)
			//	{
			//	case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
			//	case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
			//	case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
			//	case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
			//	{
			//		int button = 0;
			//		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { button = 0; }
			//		if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { button = 1; }
			//		if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { button = 2; }
			//		if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK) { button = (GET_XBUTTON_WPARAM(wparam) == XBUTTON1) ? 3 : 4; }
			//		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == NULL)
			//			::SetCapture(hwnd);
			//		io.MouseDown[button] = true;
			//		return 0;
			//	}
			//	case WM_LBUTTONUP:
			//	case WM_RBUTTONUP:
			//	case WM_MBUTTONUP:
			//	case WM_XBUTTONUP:
			//	{
			//		int button = 0;
			//		if (msg == WM_LBUTTONUP) { button = 0; }
			//		if (msg == WM_RBUTTONUP) { button = 1; }
			//		if (msg == WM_MBUTTONUP) { button = 2; }
			//		if (msg == WM_XBUTTONUP) { button = (GET_XBUTTON_WPARAM(wparam) == XBUTTON1) ? 3 : 4; }
			//		io.MouseDown[button] = false;
			//		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == hwnd)
			//			::ReleaseCapture();
			//		return 0;
			//	}
			//	case WM_MOUSEWHEEL:
			//		io.MouseWheel += (float)GET_WHEEL_DELTA_WPARAM(wparam) / (float)WHEEL_DELTA;
			//		return 0;
			//	case WM_MOUSEHWHEEL:
			//		io.MouseWheelH += (float)GET_WHEEL_DELTA_WPARAM(wparam) / (float)WHEEL_DELTA;
			//		return 0;
			//	case WM_KEYDOWN:
			//	case WM_SYSKEYDOWN:
			//		if (wparam < 256)
			//			io.KeysDown[wparam] = 1;
			//		return 0;
			//	case WM_KEYUP:
			//	case WM_SYSKEYUP:
			//		if (wparam < 256)
			//			io.KeysDown[wparam] = 0;
			//		return 0;
			//	case WM_CHAR:
			//		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
			//		io.AddInputCharacter((unsigned int)wparam);
			//		return 0;
			//	case WM_SETCURSOR:
			//		if (LOWORD(lparam) == HTCLIENT && ImGui_ImplWin32_UpdateMouseCursor())
			//			return 1;
			//		return 0;
			//	/*case WM_DEVICECHANGE:
			//		if ((UINT)wparam == DBT_DEVNODES_CHANGED)
			//			g_WantUpdateHasGamepad = true;
			//		return 0;*/
			//	}
			//	return 0;
			//}

			if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
			{
				return 1;
			}

			switch (msg)
			{
			case WM_PAINT:
				PAINTSTRUCT ps;
				HDC hdc;
				hdc = BeginPaint(hwnd, &ps);
				EndPaint(hwnd, &ps);
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			case WM_CREATE:
				break;
			case WM_KEYDOWN:
				if (wparam == VK_ESCAPE)  PostMessage(hwnd, WM_CLOSE, 0, 0);
				break;
			case WM_ENTERSIZEMOVE:
				//timer.Stop();
				break;
			case WM_EXITSIZEMOVE:
				//timer.Start();
				break;
			case WM_DROPFILES:
				HDROP hdrop;
				WCHAR filename[MAX_PATH];
				int num, i;
				bool multipleLoad;
				multipleLoad = false;


				hdrop = (HDROP)wparam;

				// ドロップされたファイルの個数を得る
				num = DragQueryFile(hdrop, -1, NULL, 0);

				if (1 < num) multipleLoad = true;

				// ファイルを処理
				for (i = 0; i < num; i++)
				{
					DragQueryFile(hdrop, i, filename, sizeof(filename));
					/*
					// filename にファイル名が入っているので、ここで処理を行う。
					//変換文字列格納バッファ
					char	filenameChar[256];

					size_t len = 0;
					errno_t err = 0;

					//ロケール指定
					//setlocale(LC_ALL, "japanese");

					//変換
					err = wcstombs_s(&len, filenameChar, sizeof(filenameChar), filename, _TRUNCATE);

					/*char fileName[256];
					bool isInput = false;
					int cnt = 0;
					for (int i = 0; i < 256; i++)
					{
						if ( filenameChar[i] == 'D' && filenameChar[i+1] == 'a' && filenameChar[i+2] == 't' && filenameChar[i+3] == 'a' )
						{
							isInput = true;
						}
						if ( isInput )
						{
							fileName[cnt++] = filenameChar[i];
						}
					}
					//SceneManager::GetInstance()->PassThePath(filenameChar, multipleLoad);*/
				}
				DragFinish(hdrop);
				break;
			default:
				return DefWindowProc(hwnd, msg, wparam, lparam);
			}

			return 0;
		}
	} // namespace OS
} // namespace Phoenix