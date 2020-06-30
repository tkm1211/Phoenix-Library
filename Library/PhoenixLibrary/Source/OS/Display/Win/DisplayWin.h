#pragma once

#include <Windows.h>
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_win32.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_dx11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_internal.h"
#include "Phoenix/OS/Display.h"


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// Windows版ディスプレイ操作クラス
		//****************************************************************************
		class DisplayWin final : public IDisplay
		{
		private:
			wchar_t		name[256] = { 0 };
			HWND		hwnd = nullptr;
			HINSTANCE	hInstaice = nullptr;
			int			width = 0;
			int			height = 0;

		public:
			DisplayWin() {}
			~DisplayWin() override { Finalize(); }

		public:
			bool Initialize(const wchar_t* name, s32 width, s32 height, uintPtr instance) override;
			void Finalize() override;
			Handle GetHWND() override;
			int GetWidth() override;
			int GetHeight() override;
			WindowHandle GetHandle() override;

		public:
			static LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		};
	} // namespace OS
} // namespace Phoenix