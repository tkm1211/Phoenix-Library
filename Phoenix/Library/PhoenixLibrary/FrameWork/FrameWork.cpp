#include <DX11GraphicsDevice.h>

#include "FrameWork.h"


namespace Phoenix
{
	namespace Lib
	{
		int FrameWork::Run(Scene* firstScene)
		{
			if (!Init(firstScene)) return 0;
			ImGuiInit();

			MSG msg = {};
			while (WM_QUIT != msg.message)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_KEYDOWN && (msg.wParam == VK_ESCAPE)) PostQuitMessage(0);

					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					timer.Tick();
					CalculateFrameStats();
					elapsedTime = timer.TimeInterval();

					Update(elapsedTime);
					Render(elapsedTime);
				}
			}

			ImGuiUnInit();
			UnInit();

			return static_cast<int>(msg.wParam);
		}

		LRESULT CALLBACK FrameWork::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) return 1;

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
				timer.Stop();
				break;
			case WM_EXITSIZEMOVE:
				timer.Start();
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
					}*/

					//SceneManager::GetInstance()->PassThePath(filenameChar, multipleLoad);
				}
				DragFinish(hdrop);
				break;
			default:
				return DefWindowProc(hwnd, msg, wparam, lparam);
			}

			return 0;
		}

		bool FrameWork::Init(Scene* firstScene, bool enableMsaa, int subSamples)
		{
			graphicsDevice = std::make_shared<DX11GraphicsDevice>();
			graphicsDevice->Init(hwnd, enableMsaa, subSamples);

			sceneManager = std::make_shared<SceneManager>();
			sceneManager->Init(firstScene, graphicsDevice.get());

			return true;
		}

		void FrameWork::UnInit()
		{
			graphicsDevice->UnInit();
			sceneManager->UnInit();
		}

		void FrameWork::Update(float elapsedTime)
		{
			sceneManager->Update();
			ImGuiUpdate();
		}

		void FrameWork::Render(float elapsedTime)
		{
			graphicsDevice->ClearRenderTargets();
			graphicsDevice->ClearShaderResources();

			graphicsDevice->SetSamplers();
			graphicsDevice->SetRasterizerState();
			graphicsDevice->SetBlendState();

			graphicsDevice->Clear();
			graphicsDevice->SetRenderTargetAndDepthStesilState();

			sceneManager->Render();

			graphicsDevice->Present(1);
		}

		void FrameWork::ImGuiInit()
		{
			ImGui::SetCurrentContext(ImGui::CreateContext());
			ImGui_ImplWin32_Init(hwnd);
			ImGui_ImplDX11_Init(graphicsDevice->GetDevice().Get(), graphicsDevice->GetImmediateContext().Get());

			ImGui::StyleColorsLight();
			{
				ImGui::GetStyle().WindowRounding = 1.0f;
				ImGui::GetStyle().FrameRounding = 1.0f;
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
				ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
				ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
				ImGui::GetStyle().Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
				ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
				ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
				ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(0.57f, 0.57f, 0.57f, 1.00f);
				ImGui::GetStyle().Colors[ImGuiCol_Tab] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
				ImGui::GetStyle().Colors[ImGuiCol_TabActive] = ImVec4(0.57f, 0.57f, 0.57f, 1.00f);
			}

			ImGuiIO& io = ImGui::GetIO();
			io.IniFilename = NULL;
			io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 20.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		}

		void FrameWork::ImGuiUnInit()
		{
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		}

		void FrameWork::ImGuiUpdate()
		{
			sceneManager->ImGui();
		}

		void FrameWork::CalculateFrameStats()
		{
			static int frames = 0;
			static float timeTlapsed = 0.0f;

			frames++;

			if ((timer.timeStamp() - timeTlapsed) >= 1.0f)
			{
				float fps = static_cast<float>(frames);
				float mspf = 1000.0f / fps;
				std::ostringstream outs;
				outs.precision(6);
				outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
				SetWindowTextA(hwnd, outs.str().c_str()); // TODO : Draw fps

				frames = 0;
				timeTlapsed += 1.0f;
			}
		}
	}
}