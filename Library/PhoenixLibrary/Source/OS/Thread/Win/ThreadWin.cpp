#include "pch.h"
#include "ThreadWin.h"
#include <process.h>
#include "Phoenix/FND/Logger.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// Windows�ŃX���b�h����N���X
		//****************************************************************************
		// ����
		std::unique_ptr<IThread> IThread::Create()
		{
			return std::make_unique<ThreadWin>();
		}

		// �J�����g�X���b�h���w�莞�Ԓ�~
		void IThread::Sleep(u32 milliSec)
		{
			::Sleep(milliSec);
		}

		// ������
		bool ThreadWin::Initialize()
		{
			return true;
		}

		// �I����
		void ThreadWin::Finalize()
		{
			if (CheakExec(false) == ErrExec)
			{
				worker->Exit();
			}

			if (handle)
			{
				if (run)
				{
					DWORD ret;
					do
					{ // �I�����m�F�ł���܂ŁA�I���R�}���h�𔭍s
						if (CheakExec(false) == ErrExec)
						{
							worker->Exit();	// ���[�J�[�v���O�����̏I��
						}

						ret = ::WaitForSingleObject(handle, 10);
					} while (ret == WAIT_TIMEOUT);
				}

				::CloseHandle(handle);
				handle = NULL;
			}
		}

		// �X���b�h�������s
		bool ThreadWin::Execute(IThreadWorker* worker)
		{
			this->worker = worker;

			unsigned stackSize = 0;

			if (CheakExec(false) == ErrExec)
			{
				PHOENIX_LOG_OS_ERROR("�X���b�h�����d���s���Ă��܂��B\n");
				return false;
			}

			handle = (HANDLE)_beginthreadex
			(
				LPSECURITY_ATTRIBUTES(0), 	// �Z�L�����e�B�A�g���r���[�g
				stackSize,				  	// �X�^�b�N�T�C�Y
				ThreadProc,				  	// �v���O�����J�n
				this,					  	// �p�����[�^
				CREATE_SUSPENDED,		  	// ���f��Ԃł̋N��
				&id						  	// �X���b�hID�i�[
			);

			if (!handle)
			{
				PHOENIX_LOG_OS_ERROR("�X���b�h���쐬�ł��܂���ł����B\n");
				return false;
			}

			if (::ResumeThread(handle) > 1)
			{
				PHOENIX_LOG_OS_ERROR("�X���b�h�����s����܂���ł����B\n");
				return false;
			}

			run = true;

			return true;
		}

		// �X���b�h���������s��������
		bool ThreadWin::IsExecute()
		{
			switch (CheakExec(false))
			{
			case ErrInit:
			case ErrRun:
			case ErrFail:
			case ErrExec:
			case ErrEnd:
				return false;
			case ErrExit:
				return true;
			}

			return false;
		}

		// �X���b�h���I������܂őҋ@
		void ThreadWin::Join()
		{
			CheakExec(true);
		}

		// �D��x�̐ݒ�
		void ThreadWin::SetPriorty(ThreadPriority priority)
		{
			HANDLE dupHandle;
			if (!::DuplicateHandle(GetCurrentProcess(), handle, GetCurrentProcess(), &dupHandle, THREAD_SET_INFORMATION, true, 0))
			{
				return;
			}

			int nPriority = 0;
			switch (priority)
			{
			case ThreadPriority::Normal:	nPriority = THREAD_PRIORITY_NORMAL;			break;	// �W���v���C�I���e�B
			case ThreadPriority::Idle:		nPriority = THREAD_PRIORITY_IDLE;			break;	// �A�C�h�����O
			case ThreadPriority::Lowest:	nPriority = THREAD_PRIORITY_LOWEST;			break;	// �W��-2
			case ThreadPriority::Below:		nPriority = THREAD_PRIORITY_BELOW_NORMAL;	break;	// �W��-1
			case ThreadPriority::Above:		nPriority = THREAD_PRIORITY_ABOVE_NORMAL;	break;	// �W��+1
			case ThreadPriority::Highest:	nPriority = THREAD_PRIORITY_HIGHEST;		break;	// �W��+2
			}

			::SetThreadPriority(dupHandle, nPriority);
			::CloseHandle(dupHandle);
		}
		
		// ����`�F�b�N
		ThreadWin::ErrCode ThreadWin::CheakExec(bool block)
		{
			if (worker == nullptr)
			{
				return ErrInit;			// ����������Ă��܂���
			}
			if (!run)
			{
				return ErrRun;			// ����������Ă��܂���
			}

			DWORD exitCode;
			if (!::GetExitCodeThread(handle, &exitCode))
			{
				run = false;
				return ErrFail;			// ���퓮�삵�Ă��܂���
			}
			if (exitCode == STILL_ACTIVE)
			{
				// �u���b�L���O
				if (block)
				{
					// �I���҂�
					::WaitForSingleObject(handle, INFINITE);
					run = false;
					return ErrExit;		// �I������
				}
				return ErrExec;			// ���s��
			}
			run = false;

			return ErrEnd;				// �I�����Ă܂�
		}

		// �X���b�h�����֐�
		unsigned __stdcall ThreadWin::ThreadProc(void* param)
		{
			ThreadWin* thread = reinterpret_cast<ThreadWin*>(param);

			thread->worker->Execute();

			_endthreadex(0);

			return 0;
		}
	} // namespace OS
} // namespace Phoenix