#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �f�o�C�X����C���^�[�t�F�[�X
		//****************************************************************************
		class IDevice : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IDevice> Create();

			// ������
			virtual bool Initialize() = 0;

			// �I����
			virtual void Finalize() = 0;
		};
	} // namespace Graphics
} // namespace Phoenix