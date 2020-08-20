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
		public:
			static const u32 particleMaxSize = 32;

		private:
			std::unique_ptr<Graphics::IComputeShader> shader;

			std::unique_ptr<Graphics::IBuffer> particleBuffer01;
			std::unique_ptr<Graphics::IBuffer> particleBuffer02;
			std::unique_ptr<Graphics::IBuffer> particleBufferResult;

			std::unique_ptr<Graphics::ITexture> particleBufferSRV01;
			std::unique_ptr<Graphics::ITexture> particleBufferSRV02;
			std::unique_ptr<Graphics::ITexture> particleBufferUAV;

			ParticleParameter particle[particleMaxSize];

		public:
			ParticleParameter resultParticle[particleMaxSize];

		public:
			TestComputeShader() {}
			~TestComputeShader() {}

		public:
			// ����
			static std::unique_ptr<IComputeShader> Create();

			// ������
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice) override;

			// �I����
			void Finalize() override;

			// ���s
			void Run(Graphics::IGraphicsDevice* graphicsDevice) override;
		};
	}
}