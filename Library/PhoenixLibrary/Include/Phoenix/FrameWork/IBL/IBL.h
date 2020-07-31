#pragma once

#include <memory>
#include "Phoenix/FrameWork/FrameBuffer/FrameBuffer.h"
#include "Phoenix/FrameWork/Object/Object.h"


namespace Phoenix
{
	namespace FrameWork
	{
		class IBL
		{
		private:
			std::unique_ptr<FrameBuffer> skyIBL;

		public:
			IBL() {}
			~IBL() { Finalize(); }

		public:
			static std::unique_ptr<IBL> Create();

			void Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			void Finalize();

			void Clear(Graphics::IGraphicsDevice* graphicsDevice, u32 index = 0, float r = 0, float g = 0, float b = 0, float a = 1);

			void Activate(Graphics::IGraphicsDevice* graphicsDevice, u32 index = 0);

			void Deactivate(Graphics::IGraphicsDevice* graphicsDevice, u32 index = 0);
		};

		class SkyMap
		{
		private:
			struct ShaderConstants
			{
				Math::Color color;
			};

		private:
			std::unique_ptr<ModelObject> model;
			std::unique_ptr<Graphics::ITexture> texture;
			std::unique_ptr<Graphics::IShader> shader;
			std::unique_ptr<Graphics::IBuffer> constantBuffer;
			std::vector<Graphics::VertexBufferKind> vbKinds;

		public:
			SkyMap() {}
			~SkyMap() {}

		public:
			static std::unique_ptr<SkyMap> Create();

			void Initialize(Graphics::IGraphicsDevice* graphicsDevice, const s8* cubemapFilename);

			void Finalize();

			void Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& world, const Math::Color& color);
		};
	}
}