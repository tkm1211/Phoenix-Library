#pragma once

#include "Phoenix/Graphics/GraphicsDevice.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�ŃO���t�B�b�N�X�f�o�C�X����I�u�W�F�N�g
		//****************************************************************************
		class GraphicsDeviceDX11 final : public IGraphicsDevice
		{
		private:
			std::unique_ptr<IDevice> device;
			std::unique_ptr<IContext> context;
			std::unique_ptr<ISwapChain> swapChain;

		public:
			// ������
			bool Initialize(OS::IDisplay* display) override;

			// �I����
			void Finalize() override;

		public:
			IDevice* GetDevice() override { return device.get(); }
			IContext* GetContext() override { return context.get(); }
			ISwapChain* GetSwapChain() override { return swapChain.get(); }

			// �����_�����O���ꂽ�C���[�W��\��
			void Present(int syncInterval);

			// �`��J�n
			void RenderBegin();

			// �`��I��
			void RenderEnd();
		};
	} // namespace Graphics
} // namespace Phoenix