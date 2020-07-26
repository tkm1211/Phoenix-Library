#pragma once

#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Graphics/Buffer.h"
#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/Graphics/RenderState.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Texture.h"
#include "Phoenix/FrameWork/FrameBuffer/FrameBuffer.h"

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
			virtual ~FullScreenQuad() { Finalize(); }

		public:
			static std::unique_ptr<FullScreenQuad> Create();

			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			void Finalize();

			void Draw
			(
				Graphics::IGraphicsDevice* graphicsDevice,
				bool useEmbeddedRasterizerState = true,
				bool useEmbeddedDepthStencilState = true,
				bool useEmbeddedPixelShader = false,
				bool enableMSAA = false
			);
		};

		class Bloom : FullScreenQuad
		{
		private:
			enum { LINEAR_BORDER, POINT, LINEAR, ANISOTROPIC };

			struct ShaderConstants
			{
				float glowExtractionThreshold = 0.85f;
				float blurConvolutionIntensity = 0.06f;
				float lensFlareThreshold = 1.000f;
				float lensFlareGhostDispersal = 0.300f; //dispersion factor
				int numberOfGhosts = 6;
				float lensFlareIntensity = 0.35f;
				int options[2] = { 1 };
			};

		private:
			// if you change value of 'number_of_downsampled', you must change 'number_of_downsampled' in bloom.hlsli to this same value.
			const static u32 numberOfDownsampled = 6;

		public:
			ShaderConstants shaderContants;

		private:
			std::unique_ptr<Graphics::ISampler> samplerState[4];

			std::unique_ptr<Graphics::IBlend> blendState;

			std::unique_ptr<Graphics::IShader> glowExtractionPS;
			std::unique_ptr<Graphics::IShader> gaussianBlurHorizontalPS;
			std::unique_ptr<Graphics::IShader> gaussianBlurVerticalPS;
			std::unique_ptr<Graphics::IShader> gaussianBlurConvolutionPS;
			std::unique_ptr<Graphics::IShader> gaussianBlurDownsamplingPS;

			std::unique_ptr<Graphics::IShader> lensFlarePS;
			std::unique_ptr<Graphics::ITexture> gradientMap;
			std::unique_ptr<Graphics::ITexture> noiseMap;

			std::unique_ptr<Graphics::IBuffer> constantBuffer;

			std::unique_ptr<FrameBuffer> glowExtraction;
			std::unique_ptr<FrameBuffer> gaussianBlur[numberOfDownsampled][2];
			std::unique_ptr<FrameBuffer> lensFlare;

		public:
			Bloom() : FullScreenQuad() {}
			~Bloom() {}

		public:
			static std::unique_ptr<Bloom> Create();

			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice, u32 width, u32 height);

			void Finalize();

			void Generate(Graphics::IGraphicsDevice* graphicsDevice, Graphics::ITexture* hdrTexture, bool enableLensFlare);

			void Draw(Graphics::IGraphicsDevice* graphicsDevice);
		};
	}
}