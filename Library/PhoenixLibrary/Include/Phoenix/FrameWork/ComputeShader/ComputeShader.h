#pragma once

#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/Graphics/GraphicsDevice.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �R���s���[�g�V�F�[�_�[�p�o�b�t�@�쐬�֐�
		//****************************************************************************
		bool CreateStructuredBuffer(Graphics::IDevice* device, u32 elementSize, u32 count, void* initData, Graphics::IBuffer* buffer);

		//****************************************************************************
		// �R���s���[�g�V�F�[�_�[�p�e�N�X�`���쐬�֐�
		//****************************************************************************
		bool CreateBufferSRV(Graphics::IDevice* device, Graphics::IBuffer* buffer, Graphics::ITexture* shaderResouceView);

		bool CreateBufferUAV(Graphics::IDevice* device, Graphics::IBuffer* buffer, Graphics::ITexture* shaderResouceView);

		//****************************************************************************
		// �R���s���[�g�V�F�[�_�[�p�f�[�^���o�֐�
		//****************************************************************************
		void CreateAndCopyToBuffer(Graphics::IDevice* device, Graphics::IContext* context, Graphics::IBuffer* buffer, Graphics::IBuffer* copyBuffer);

		//****************************************************************************
		// �R���s���[�g�V�F�[�_�[���N���X
		//****************************************************************************
		class IComputeShader
		{
		public:
			IComputeShader() {}
			virtual ~IComputeShader() {}
		
		public:
			// ����
			// static std::unique_ptr<IShader> Create(); // �e�p���N���X�ō쐬���Ă��������B
		
			// ������
			virtual bool Initialize(Graphics::IGraphicsDevice* graphicsDevice) = 0;
		
			// �I����
			virtual void Finalize() = 0;

			// ���s
			virtual void Run(Graphics::IGraphicsDevice* graphicsDevice) = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix