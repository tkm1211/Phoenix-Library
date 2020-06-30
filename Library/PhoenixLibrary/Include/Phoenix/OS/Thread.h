#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// �X���b�h�D��x
		//****************************************************************************
		enum class ThreadPriority
		{
			Normal,		// �W���v���C�I���e�B
			Idle,  		// �A�C�h�����O
			Lowest,		// �W��-2
			Below, 		// �W��-1
			Above, 		// �W��+1
			Highest		// �W��+2
		};

		//****************************************************************************
		// �X���b�h���[�J�[�C���^�[�t�F�[�X
		//****************************************************************************
		class IThreadWorker
		{
		public:
			// �X���b�h���s���ɌĂ΂��
			virtual void Execute() = 0;

			// �X���b�h�������I��
			virtual void Exit() = 0;
		};
		//****************************************************************************
		// �X���b�h����C���^�[�t�F�[�X
		//****************************************************************************
		class IThread : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IThread> Create();

			// ������
			virtual bool Initialize() = 0;

			// �I����
			virtual void Finalize() = 0;

			// �X���b�h�������s
			virtual bool Execute(IThreadWorker* worker) = 0;

			// �X���b�h���������s��������
			virtual bool IsExecute() = 0;

			// �X���b�h���I������܂őҋ@
			virtual void Join() = 0;

			// �D��x�̐ݒ�
			virtual void SetPriorty(ThreadPriority priority) = 0;

			// �J�����g�X���b�h���w�莞�Ԓ�~
			static void Sleep(u32 milliSec);
		};
	} // namespace OS
} // namespace Phoenix