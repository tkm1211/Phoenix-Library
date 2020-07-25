#include "Phoenix/FrameWork/FullScreenQuad/FullScreenQuad.h"


namespace Phoenix
{
	namespace FrameWork
	{
		std::unique_ptr<FullScreenQuad> FullScreenQuad::Create()
		{
			return std::make_unique<FullScreenQuad>();
		}

		bool FullScreenQuad::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{

		}

		void FullScreenQuad::Finalize()
		{
			embeddedDepthStencilState.reset();
			embeddedRasterizerState.reset();

			embeddedPixelShader[1].reset();
			embeddedPixelShader[0].reset();
			embeddedVertexShader.reset();
		}

		void FullScreenQuad::Draw
		(
			Graphics::IGraphicsDevice* graphicsDevice,
			bool useEmbeddedRasterizerState,
			bool useEmbeddedDepth_stencilState,
			bool useEmbeddedPixelShader,
			bool enableMSAA
		)
		{

		}
	}
}