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

			// ���擾
			virtual s32 GetWidth() = 0;

			// �����擾
			virtual s32 GetHeight() = 0;

			// �E�C���h�E�n���h���擾
			virtual WindowHandle GetHandle() = 0;
		};
	} // namespace OS
} // namespace Phoenix