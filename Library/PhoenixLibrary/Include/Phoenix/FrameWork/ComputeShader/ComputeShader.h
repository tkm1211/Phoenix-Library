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

			// ���s
			virtual void Run(Graphics::IGraphicsDevice* graphicsDevice) = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix