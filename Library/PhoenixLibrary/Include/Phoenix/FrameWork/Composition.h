#pragma once

#include "Phoenix/FND/Base.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// ��s�錾
		class Container;

		//****************************************************************************
		// �R���|�W�V����
		//****************************************************************************
		class Composition : public FND::Base
		{
		public:
			// ���O�擾
			virtual const char* GetName() = 0;

			// �R���X�g���N�V����
			virtual void Construct(Container* container) {}

			// ������
			virtual bool Initialize() { return true; }

			// �I����
			virtual void Finalize() {}
		};

	} // namespace FrameWork
} // namespace Phoenix