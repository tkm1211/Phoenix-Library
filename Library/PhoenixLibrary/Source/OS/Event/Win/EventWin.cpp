#include "pch.h"
#include "Phoenix/FND/Logger.h"
#include "EventWin.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// Windows�ŃC�x���g����N���X
		//****************************************************************************
		// ����
		std::unique_ptr<IEvent> IEvent::Create()
		{
			return std::make_unique<EventWIN>();
		}

		// ������
		bool EventWIN::Initialize(bool manual)
		{
			static int id = 0;
			handle = ::CreateEventA(nullptr, manual, false, nullptr);

			if (handle == nullptr)
			{
				PHOENIX_LOG_OS_ERROR("CreateEvent() : Failed!!\n");
				return false;
			}

			return true;
		}

		// ���
		void EventWIN::Finalize()
		{
			if (handle != nullptr)
			{
				::CloseHandle(handle);
				handle = nullptr;
			}
		}

		// �V�O�i���ݒ�
		void EventWIN::Set()
		{
			::SetEvent(handle);
		}

		// �V�O�i������
		void EventWIN::Reset()
		{
			::ResetEvent(handle);
		}

		// �V�O�i���ݒ�҂�
		void EventWIN::Wait(f32 time)
		{
			DWORD milliSeconds = (time > 0.0001f) ? (DWORD)(time * 1000.0f) : INFINITE;

			::WaitForSingleObject(handle, milliSeconds);
		}

	}	// namespace os
}	// namespace ys