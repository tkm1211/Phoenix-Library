#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <sstream>
#include <memory>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>

#include <GraphicsDevice.h>

#include "HighResolutionTimer.h"
#include "Scene.h"


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

namespace Phoenix
{
	namespace Lib
	{
		class FrameWork
		{
		public:
			const HWND hwnd;
			const HINSTANCE instance;
			HighResolutionTimer timer;
			float elapsedTime = 0.0f;

			std::shared_ptr<GraphicsDevice> graphicsDevice;
			std::shared_ptr<SceneManager> sceneManager;

		public:
			FrameWork(HWND hwnd, HINSTANCE instance) : hwnd(hwnd), instance(instance) {}
			FrameWork(FrameWork&) = delete;
			~FrameWork() = default;
			void operator = (FrameWork&) = delete;

		public:
			int Run(Scene* firstScene);
			LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		private:
			bool Init(Scene* firstScene, bool enableMsaa = false, int subSamples = 1);
			void UnInit();
			void Update(float elapsedTime);
			void Render(float elapsedTime);

			void ImGuiInit();
			void ImGuiUnInit();
			void ImGuiUpdate();

			void CalculateFrameStats();
		};
	}
}