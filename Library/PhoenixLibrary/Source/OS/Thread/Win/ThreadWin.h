#pragma once

#include "Phoenix/OS/Thread.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// Windows�ŃX���b�h����N���X
		//****************************************************************************
		class ThreadWin : public IThread
		{
		private:
			enum ErrCode
			{
				ErrInit,	// ����������Ă��Ȃ�
				ErrRun,		// ���s���Ă��Ȃ�
				ErrFail,	// ���퓮�삵�Ă��Ȃ�
				ErrExec,	// ���s��
				ErrExit,	// �I������
				ErrEnd		// �I����
			};

		private:
			IThreadWorker* worker = nullptr;
			HANDLE handle = nullptr;
			unsigned id = 0;
			bool run = false;

		public:
			~ThreadWin() override { Finalize(); }

		public:
			// ������
			bool Initialize() override;

			// �I����
			void Finalize() override;

			// �X���b�h�������s
			bool Execute(IThreadWorker* worker) override;

			// �X���b�h���������s��������
			bool IsExecute() override;

			// �X���b�h���I������܂őҋ@
			void Join() override;

			// �D��x�̐ݒ�
			void SetPriorty(ThreadPriority priority) override;

		private:
			// ����`�F�b�N
			ErrCode CheakExec(bool block);

			// �X���b�h�����֐�
			static unsigned __stdcall ThreadProc(void* param);
		};
	} // namespace OS
} // namespace Phoenix