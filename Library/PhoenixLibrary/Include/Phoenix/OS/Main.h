#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// ���C�����[�v����C���^�[�t�F�[�X
		//****************************************************************************
		class IMain : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IMain> Create();

			// ������
			virtual bool Initialize(uintPtr instance) = 0;
			
			// �I����
			virtual void Finalize() = 0;
			
			// ���[�v����
			virtual void Run() = 0;

			//// �I���v��
			//virtual void Exit() = 0;

			//// �I���v����Ԃ��m�F
			//virtual bool IsExit() = 0;

			//// ���O�擾
			//virtual const char* GetName() = 0;

			//// ���O�ݒ�
			//virtual void SetName(const char* name) = 0;

			//// �C���X�^���X�n���h�����擾
			//virtual uintPtr Handle() = 0;
		};
	} // namespace OS
} // namespace Phoenix