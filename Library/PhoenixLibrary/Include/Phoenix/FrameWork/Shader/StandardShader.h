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
			// 生成
			static std::unique_ptr<IShader> Create();

			// 初期化
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice) override;

			// 終了化
			void Finalize() override;

			// シェーダー開始
			void Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera) override;

			// シェーダー終了
			void End(Graphics::IGraphicsDevice* graphicsDevice) override;

			// 描画
			void Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model) override;

			// 頂点バッファ種類の取得
			Graphics::VertexBufferKind* GetVectexBuferKinds() override { return vbKinds.data(); }

			// 頂点バッファ種類のサイズ
			u32 GetVectexBuferKindsSize() override { return vbKinds.size(); }

			// ライトの取得
			Graphics::ILightingState* GetLight() { return light.get(); }
		};
	}
}