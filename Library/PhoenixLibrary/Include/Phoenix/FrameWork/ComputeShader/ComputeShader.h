#pragma once

#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/Graphics/GraphicsDevice.h"


namespace Phoenix
{
	namespace FrameWork
	{
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
		
			// �V�F�[�_�[�J�n
			virtual void Begin(Graphics::IGraphicsDevice* graphicsDevice) = 0;
		
			// �V�F�[�_�[�I��
			virtual void End(Graphics::IGraphicsDevice* graphicsDevice) = 0;
		
			// �`��
			virtual void Dispatch(Graphics::IGraphicsDevice* graphicsDevice, u32 x, u32 y, u32 z) = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix