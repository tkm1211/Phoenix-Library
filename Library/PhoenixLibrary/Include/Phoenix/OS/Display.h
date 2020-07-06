#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"


namespace Phoenix
{
	namespace OS
	{
		// �E�C���h�E�n���h��
		typedef uintPtr WindowHandle;

#ifdef WIN32
		typedef HWND Handle;
#endif

		//****************************************************************************
		// �f�B�X�v���C����C���^�[�t�F�[�X
		//****************************************************************************
		class IDisplay : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IDisplay> Create();

			// ������
			virtual bool Initialize(const wchar_t* name, s32 width, s32 height, uintPtr instance) = 0;
			
			// �I����
			virtual void Finalize() = 0;

			// �n���h���擾
			virtual Handle GetHWND() = 0;

			// ���擾
			virtual s32 GetWidth() = 0;

			// �����擾
			virtual s32 GetHeight() = 0;

			// �E�C���h�E�n���h���擾
			virtual WindowHandle GetHandle() = 0;

			// �^�C�}�[�`�b�N
			virtual void TimerTick() = 0;

			// �^�C�}�[�C���^�[�o��
			virtual f32 TimerInterval() = 0;

			// �t���[�����[�g�̕\��
			virtual void CalculateFrameStats() = 0;
		};
	} // namespace OS
} // namespace Phoenix