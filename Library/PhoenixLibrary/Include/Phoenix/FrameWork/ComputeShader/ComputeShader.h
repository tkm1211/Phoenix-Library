#pragma once

#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/Graphics/GraphicsDevice.h"


namespace Phoenix
{
	namespace FrameWork
	{
		class ComputeShaderBufferFactor
		{
		public:
			// �R���s���[�g�V�F�[�_�[�p�o�b�t�@�쐬�֐�
			static bool CreateStructuredBuffer(Graphics::IDevice* device, Phoenix::Graphics::PhoenixUsage usage, u32 bindFlags, u32 byteWidth, u32 structureByteStride, s32 miscFlags, void* initData, Graphics::IBuffer* buffer);

			// �R���s���[�g�V�F�[�_�[�p�e�N�X�`���쐬�֐�
			static bool CreateBufferSRV(Graphics::IDevice* device, Graphics::IBuffer* buffer, Graphics::ITexture* shaderResouceView, Graphics::TextureFormatDx format = Graphics::TextureFormatDx::UNKNOWN, u32 byteWidth = 0, u32 structureByteStride = 0);
			static bool CreateBufferUAV(Graphics::IDevice* device, Graphics::IBuffer* buffer, Graphics::ITexture* shaderResouceView, Graphics::TextureFormatDx format = Graphics::TextureFormatDx::UNKNOWN, u32 byteWidth = 0, u32 structureByteStride = 0);

			// �R���s���[�g�V�F�[�_�[�p�f�[�^���o�֐�
			static void CreateAndCopyToBuffer(Graphics::IDevice* device, Graphics::IContext* context, Graphics::IBuffer* buffer, Graphics::IBuffer* copyBuffer);
		};

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