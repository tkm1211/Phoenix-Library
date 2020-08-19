#pragma once

#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/Graphics/Buffer.h"
#include "Phoenix/FrameWork/ComputeShader/ComputeShader.h"


namespace Phoenix
{
	namespace FrameWork
	{
		struct ParticleParameter
		{
			Math::Vector3 pos;
		};

		class TestComputeShader : public IComputeShader
		{
		private:
			static const u32 particleMaxSize = 10;

		private:
			std::unique_ptr<Graphics::IComputeShader> shader;
			std::unique_ptr<Graphics::IBuffer> srv;
			std::unique_ptr<Graphics::IBuffer> uav;

			ParticleParameter particle[particleMaxSize];
			ParticleParameter resultParticle;

		public:
			TestComputeShader() {}
			~TestComputeShader() {}

		public:
			// 生成
			static std::unique_ptr<IComputeShader> Create();

			// 初期化
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice) override;

			// 終了化
			void Finalize() override;

			// シェーダー開始
			void Begin(Graphics::IGraphicsDevice* graphicsDevice) override;

			// シェーダー終了
			void End(Graphics::IGraphicsDevice* graphicsDevice) override;

			// 実行
			void Dispatch(Graphics::IGraphicsDevice* graphicsDevice, u32 x, u32 y, u32 z) override;
		};
	}
}