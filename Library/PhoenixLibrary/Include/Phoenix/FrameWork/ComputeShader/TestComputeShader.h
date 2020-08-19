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
			// ����
			static std::unique_ptr<IComputeShader> Create();

			// ������
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice) override;

			// �I����
			void Finalize() override;

			// �V�F�[�_�[�J�n
			void Begin(Graphics::IGraphicsDevice* graphicsDevice) override;

			// �V�F�[�_�[�I��
			void End(Graphics::IGraphicsDevice* graphicsDevice) override;

			// ���s
			void Dispatch(Graphics::IGraphicsDevice* graphicsDevice, u32 x, u32 y, u32 z) override;
		};
	}
}