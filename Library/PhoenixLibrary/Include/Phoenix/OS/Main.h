#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"


namespace Phoenix
{
	namespace OS
	{
		//============================================================================
		// ���C�����[�v����C���^�[�t�F�[�X
		//----------------------------------------------------------------------------
		class IMain : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IMain> Create();

			// ������
			virtual bool Initialize(const wchar_t* name, s32 width, s32 height, uintPtr instance) = 0;
			
			// �I����
			virtual void Finalize() = 0;
			
			// ���[�v����
			virtual void Run() = 0;
		};
	} // namespace OS
} // namespace Phoenix