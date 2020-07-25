#pragma once

#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Graphics/Buffer.h"
#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/Graphics/RenderState.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Texture.h"

#include <d3d11.h>
#include <wrl.h>


namespace Phoenix
{
	namespace FrameWork
	{
		class Quad
		{
		private:
			struct vertex
			{
				Math::Vector3 position;
				Math::Vector2 texcoord;
				Math::Vector4 color;
			};

		private:
			Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
			std::unique_ptr<Graphics::IBuffer> vertexBuffer;

			std::unique_ptr<Graphics::IShader> embeddedVertexShader;
			std::unique_ptr<Graphics::IShader> embeddedPixelShader[2];

			std::unique_ptr<Graphics::IRasterizer> embeddedRasterizerState;
			std::unique_ptr<Graphics::IDepthStencil> embeddedDepthStencilState;
			std::unique_ptr<Graphics::ISampler> embeddedSamplerState;

		public:
			Quad() {}
			 ~Quad() {}

		public:
			static std::unique_ptr<Quad> Create();

			bool Initialize
			(
				Graphics::IGraphicsDevice* graphicsDevice,
				Graphics::SamplerState samplerState = Graphics::SamplerState::AnisotropicBorder,
				Math::Color samplerBoarderColor = Math::Color::Black
			);

			void Finalize();

			void Draw
			(
				Graphics::IGraphicsDevice* graphicsDevice,
				Graphics::ITexture* shaderResourceView,
				float dx, float dy, float dw, float dh,
				float sx, float sy, float sw, float sh,
				float angle/*degree*/,
				float r, float g, float b, float a,
				bool useEmbeddedVertexShader = true,
				bool useEmbeddedPixelShader = true,
				bool useEmbeddedRasterizerState = true,
				bool useEmbeddedDepthStencilState = true,
				bool useEmbeddedSamplerState = true
			) const;

			void Draw
			(
				Graphics::IGraphicsDevice* graphicsDevice,
				Graphics::ITexture* shaderResourceView,
				float dx, float dy, float dw, float dh,
				float angle = 0/*degree*/,
				float r = 1, float g = 1, float b = 1, float a = 1,
				bool useEmbeddedVertexShader = true,
				bool useEmbeddedPixelShader = true,
				bool useEmbeddedRasterizerState = true,
				bool useEmbeddedDepthStencilState = true,
				bool useEmbeddedSamplerState = true
			) const;
		};

		class FullScreenQuad
		{
		private:
			std::unique_ptr<Graphics::IShader> embeddedVertexShader;
			std::unique_ptr<Graphics::IShader> embeddedPixelShader[2];

			std::unique_ptr<Graphics::IRasterizer> embeddedRasterizerState;
			std::unique_ptr<Graphics::IDepthStencil> embeddedDepthStencilState;

		public:
			FullScreenQuad() {}
			~FullScreenQuad() { Finalize(); }

		public:
			static std::unique_ptr<FullScreenQuad> Create();

			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			void Finalize();

			void Draw
			(
				Graphics::IGraphicsDevice* graphicsDevice,
				bool useEmbeddedRasterizerState = true,
				bool useEmbeddedDepth_stencilState = true,
				bool useEmbeddedPixelShader = false,
				bool enableMSAA = false
			);
		};
	}
}