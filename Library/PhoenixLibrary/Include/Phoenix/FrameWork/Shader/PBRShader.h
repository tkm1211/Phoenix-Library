#pragma once

#include "Shader.h"


namespace Phoenix
{
	namespace FrameWork
	{
		class LightState
		{
		public:
			Math::Vector4 direction = { 0.0f, 0.0f, 0.0f, 0.0f };
			Math::Vector4 color = { 0.0f, 0.0f, 0.0f, 0.0f };

		public:
			LightState() {}
			~LightState() {}
		};

		class MaterialState
		{
		public:
			Math::Vector4 albedo = { 0.0f, 0.0f, 0.0f, 0.0f };
			f32 metallic = 0.0f;
			f32 roughness = 0.0f;

		public:
			MaterialState() {}
			~MaterialState() {}
		};

		class PBRShader : public IShader
		{
		private:
			struct DirectionalLight
			{
				Math::Vector4 direction;
				Math::Vector4 color;
			};

			struct CbMaterial
			{
				Math::Vector4 albedo;
				f32 metallic;
				f32 roughness;
				f32 padding01;
				f32 padding02;
			};

			struct CbScene
			{
				DirectionalLight dirLight;
				Math::Vector4 viewPos;
			};

		private:
			std::unique_ptr<Graphics::IShader> shader;
			std::unique_ptr<Graphics::IBuffer> cbMaterial;
			std::unique_ptr<Graphics::IBuffer> cbScene;
			std::vector<Graphics::VertexBufferKind> vbKinds;

			std::unique_ptr<LightState> light;
			std::unique_ptr<MaterialState> material;

		public:
			PBRShader() {}
			~PBRShader() override { Finalize(); }

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
			LightState* GetLight() { return light.get(); }

			// �}�e���A���̎擾
			MaterialState* GetMaterial() { return material.get(); }
		};
	}
}