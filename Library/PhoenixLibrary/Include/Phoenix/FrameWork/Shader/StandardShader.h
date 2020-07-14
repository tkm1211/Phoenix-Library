#pragma once

#include "Shader.h"
#include "Phoenix/Graphics/Light.h"


namespace Phoenix
{
	namespace FrameWork
	{
		class StandardShader : public IShader
		{
		private:
			struct CbScene
			{
				Phoenix::Math::Vector3 eye;
				Phoenix::f32 padding;
			};

			struct CbMaterial
			{
				Phoenix::Graphics::LightEnvironment lightEnv;
				Phoenix::Math::Color emissive;
				Phoenix::Math::Color diffuse;
				Phoenix::Math::Color specular;
			};

		private:
			std::unique_ptr<Graphics::IShader> shader;
			std::unique_ptr<Graphics::ILightingState> light;
			std::unique_ptr<Graphics::IBuffer> cbScene;
			std::unique_ptr<Graphics::IBuffer> cbMaterial;
			std::vector<Graphics::VertexBufferKind> vbKinds;

		public:
			StandardShader() {}
			~StandardShader() override { Finalize(); }

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

			// ���C�g�̎擾
			Graphics::ILightingState* GetLight() { return light.get(); }
		};
	}
}