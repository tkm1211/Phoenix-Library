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
		// �C���v�b�g
		//****************************************************************************
		class Input final : public FND::NonCopyable
		{
		public:
			// ������
			bool Initialize();

			// �I����
			void Finalize();

			// �X�V
			void Update();

			// �L�[�{�[�h�擾
			//hid::IKeyboard* GetKeyboard() const { return m_keyboard.get(); }

			// �}�E�X�擾
			//hid::IMouse* GetMouse() const { return m_mouse.get(); }

			// �p�b�h�擾
			//hid::IPad* GetPad(s32 slot = 0) const { return m_pads[slot].get(); }

		private:
			//std::unique_ptr<hid::IKeyboard>		m_keyboard;
			//std::unique_ptr<hid::IMouse>		m_mouse;
			//std::unique_ptr<hid::IPad>			m_pads[2];
		};

	} // namespace FrameWork
} // namespace Phoenix