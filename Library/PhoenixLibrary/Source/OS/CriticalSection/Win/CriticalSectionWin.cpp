#include "pch.h"
#include "Phoenix/FND/Logger.h"
#include "CriticalSectionWin.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// Windows��critical�Z�N�V��������N���X
		//****************************************************************************
		// ����
		std::unique_ptr<ICriticalSection> ICriticalSection::Create()
		{
			return std::make_unique<CriticalSectionWin>();
		}

		// ������
		bool CriticalSectionWin::Initialize()
		{
			try
			{
				// �N���e�B�J���Z�N�V�����o�^
				::InitializeCriticalSection(&criticalSection);
			}
			catch (...)
			{
				// �N���e�B�J���Z�N�V�����o�^�ł̗�O�G���[�́A������catch����
				PHOENIX_LOG_OS_ERROR("InitializeCriticalSection() : Failed!!\n");
				return false;
			}
			initialize = true;

			return true;
		}

		// �I����
		void CriticalSectionWin::Finalize()
		{
			if (!initialize)
			{
				return;
			}
			initialize = false;

			::DeleteCriticalSection(&criticalSection);
		}

		// ���L���擾�҂�
		void CriticalSectionWin::Enter()
		{
			try {
				// �N���e�B�J���Z�N�V�������L���擾
				::EnterCriticalSection(&criticalSection);
			}
			catch (...) {
				// �N���e�B�J���Z�N�V�������L�ł̗�O�G���[�́A������catch����
				PHOENIX_LOG_OS_ERROR("EnterCriticalSection() : Failed!!\n");
				return;
			}
		}

		// ���L�����
		void CriticalSectionWin::Leave()
		{
			try {
				// �N���e�B�J���Z�N�V�������L�����
				::LeaveCriticalSection(&criticalSection);
			}
			catch (...) {
				// �N���e�B�J���Z�N�V�������L�ł̗�O�G���[�́A������catch����
				PHOENIX_LOG_OS_ERROR("LeaveCriticalSection() : Failed!!\n");
				return;
			}
		}
	}
}