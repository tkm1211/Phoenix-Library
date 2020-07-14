#pragma once

#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Mesh.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/Graphics/Camera.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �V�F�[�_�[���N���X
		//****************************************************************************
		class IShader
		{
		public:
			IShader() {}
			virtual ~IShader() {}

		public:
			// ����
			// static std::unique_ptr<IShader> Create(); // �e�p���N���X�ō쐬���Ă��������B

			// ������
			virtual bool Initialize(Graphics::IGraphicsDevice* graphicsDevice) = 0;

			// �I����
			virtual void Finalize() = 0;

			// �V�F�[�_�[�J�n
			virtual void Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera) = 0;

			// �V�F�[�_�[�I��
			virtual void End(Graphics::IGraphicsDevice* graphicsDevice) = 0;

			// �`��
			virtual void Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model) = 0;

			// ���_�o�b�t�@��ނ̎擾
			virtual Graphics::VertexBufferKind* GetVectexBuferKinds() = 0;

			// ���_�o�b�t�@��ނ̃T�C�Y
			virtual sizeT GetVectexBuferKindsSize() = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix