#pragma once
// TODO : ADD HID
#include "Phoenix/FND/NonCopyable.h"
//#include "Phoenix/HID/Keyboard.h"
//#include "Phoenix/HID/Mouse.h"
//#include "Phoenix/HID/Pad.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// インプット
		//****************************************************************************
		class Input final : public FND::NonCopyable
		{
		public:
			// 初期化
			bool Initialize();

			// 終了化
			void Finalize();

			// 更新
			void Update();

			// キーボード取得
			//hid::IKeyboard* GetKeyboard() const { return m_keyboard.get(); }

			// マウス取得
			//hid::IMouse* GetMouse() const { return m_mouse.get(); }

			// パッド取得
			//hid::IPad* GetPad(s32 slot = 0) const { return m_pads[slot].get(); }

		private:
			//std::unique_ptr<hid::IKeyboard>		m_keyboard;
			//std::unique_ptr<hid::IMouse>		m_mouse;
			//std::unique_ptr<hid::IPad>			m_pads[2];
		};

	} // namespace FrameWork
} // namespace Phoenix