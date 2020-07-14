#pragma once

#include "Shader.h"


namespace Phoenix
{
	namespace FrameWork
	{
		class BasicSkinShader : public IShader
		{
		private:
			struct CbMaterial
			{
				Phoenix::Math::Color color;
			};

		private:
			std::unique_ptr<Graphics::IShader> shader;
			std::unique_ptr<Graphics::IBuffer> cbMatrial;
			std::vector<Graphics::VertexBufferKind> vbKinds;

		public:
			BasicSkinShader() {}
			~BasicSkinShader() override { Finalize(); }

		public:
			// ����
			static std::unique_ptr<IShader> Create();

			// ������
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice) override;

			// �I����
			void Finalize() override;

			// �V�F�[�_�[�J�n
			void Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera) override;

			// �V�F�[�_�[�I��
			void End(Graphics::IGraphicsDevice* graphicsDevice) override;

			// �`��
			void Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model) override;

			// ���_�o�b�t�@��ނ̎擾
			Graphics::VertexBufferKind* GetVectexBuferKinds() override { return vbKinds.data(); }

			// ���_�o�b�t�@��ނ̃T�C�Y
			sizeT GetVectexBuferKindsSize() override { return vbKinds.size(); }
		};
	}
}