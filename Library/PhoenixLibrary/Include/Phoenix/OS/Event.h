#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// �C�x���g����C���^�[�t�F�[�X
		//****************************************************************************
		class IEvent : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IEvent> Create();

			// ������
			virtual bool Initialize(bool manual = false) = 0;

			// �I����
			virtual void Finalize() = 0;

			// �V�O�i���ݒ�
			virtual void Set() = 0;

			// �V�O�i������
			virtual void Reset() = 0;

			// �V�O�i���ݒ�҂�
			virtual void Wait(f32 time = 0.0f) = 0;
		};
	}
}