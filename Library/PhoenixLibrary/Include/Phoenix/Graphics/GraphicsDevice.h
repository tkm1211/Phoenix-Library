#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"
#include "Phoenix/OS/Display.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/Graphics/Context.h"
#include "Phoenix/Graphics/SwapChain.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �O���t�B�b�N�X�f�o�C�X����C���^�[�t�F�[�X
		//****************************************************************************
		class IGraphicsDevice : public FND::Base
		{
		public:
			// ����
			static std::shared_ptr<IGraphicsDevice> Create();

			// ������
			virtual bool Initialize(OS::IDisplay* display) = 0;

			// �I����
			virtual void Finalize() = 0;

		public:
			virtual IDevice* GetDevice() = 0;
			virtual IContext* GetContext() = 0;
			virtual ISwapChain* GetSwapChain() = 0;

			// �����_�����O���ꂽ�C���[�W��\��
			virtual void Present(int syncInterval) = 0;

			// �`��J�n
			virtual void RenderBegin() = 0;

			// �`��I��
			virtual void RenderEnd() = 0;
		};
	} // namespace Graphics
} // namespace Phoenix