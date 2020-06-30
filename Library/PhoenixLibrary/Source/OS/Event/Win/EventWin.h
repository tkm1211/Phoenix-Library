#pragma once

#include <Windows.h>
#include "Phoenix/OS/Event.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// Windows�ŃC�x���g����N���X
		//****************************************************************************
		class EventWIN final : public IEvent
		{
		private:
			HANDLE handle = nullptr;

		public:
			~EventWIN() override { Finalize(); }

		public:
			// ������
			bool Initialize(bool manual = false) override;

			// �I����
			void Finalize() override;

			// �V�O�i���ݒ�
			void Set() override;

			// �V�O�i������
			void Reset() override;

			// �V�O�i���ݒ�҂�
			void Wait(f32 time = 0.0f) override;
		};
	}
}