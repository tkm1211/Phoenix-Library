#pragma once

#include "Phoenix/Graphics/Surface.h"
#include "Phoenix/Graphics/Viewport.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Shader.h"
#include "../Source/Graphics/Texture/Win/DirectX11/TextureDX11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_win32.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_dx11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_internal.h"

#include <d3d11.h>
#include <wrl.h>


namespace Phoenix
{
	namespace FrameWork
	{
		class FrameBuffer
		{
		private:
			static const s32 ViewportCount = 16;

		public:
			std::unique_ptr<Graphics::IRenderTargetSurface> renderTargerSurface;
			std::unique_ptr<Graphics::IDepthStencilSurface> depthStencilSurface;

			std::unique_ptr<Graphics::IRenderTargetSurface> cachedRenderTargerSurface;
			std::unique_ptr<Graphics::IDepthStencilSurface> cachedDepthStencilSurface;

			u32 numberOfStoredViewports;
			
			Graphics::Viewport viewport = {};
			Graphics::Viewport* cachedViewports[ViewportCount];

		public:
			FrameBuffer() {}
			~FrameBuffer() { Finalize(); }

		public:
			static std::unique_ptr<FrameBuffer> Create();

			bool Initialize
			(
				Graphics::IGraphicsDevice* graphicsDevice,
				u32 width,
				u32 height,
				bool enableMSAA = false,
				int subsamples = 1,
				Graphics::TextureFormatDx renderTargetTexture2dFormat = Graphics::TextureFormatDx::R8G8B8A8_UNORM,
				Graphics::TextureFormatDx depthStencilTexture2dFormat = Graphics::TextureFormatDx::R24G8_TYPELESS,
				bool needRenderTargetShaderResourceView = true,
				bool needDepthStencilShaderResourceView = true,
				bool generateMips = false
			);

			void Finalize();

			void Clear(Graphics::IGraphicsDevice* graphicsDevice, float r = 0, float g = 0, float b = 0, float a = 1, float depth = 1, u8 stencil = 0);

			//clear only 'render_target_view'
			void ClearRenderTargetView(Graphics::IGraphicsDevice* graphicsDevice, float r = 0, float g = 0, float b = 0, float a = 1);

			//clear only 'depth_stencil_view'
			void ClearDepthStencilView(Graphics::IGraphicsDevice* graphicsDevice, FLOAT depth = 1, UINT8 stencil = 0);

			void Activate(Graphics::IGraphicsDevice* graphicsDevice);

			//activate only 'render_target_view'
			void ActivateRenderTargetView(Graphics::IGraphicsDevice* graphicsDevice);

			//activate only 'depth_stencil_view'
			void ActivateDepthStencilView(Graphics::IGraphicsDevice* graphicsDevice);

			void Deactivate(Graphics::IGraphicsDevice* graphicsDevice);

			void DrawSRV(bool isDrawRTV, bool isDrawDSV)
			{
				if (isDrawRTV)
				{
					ID3D11ShaderResourceView* srv = static_cast<Graphics::TextureDX11*>(renderTargerSurface->GetTexture())->GetD3DShaderResourceView();
					ImGui::Image(srv, ImVec2(100.0f, 100.0f));
				}
				if (isDrawDSV)
				{
					ImGui::Image(static_cast<ID3D11ShaderResourceView*>(depthStencilSurface->GetTexture()->Handle()), ImVec2(256.0f, 256.0f));
				}
			}

			Graphics::IRenderTargetSurface* GetRenderTargetSurface() { return renderTargerSurface.get(); }
			Graphics::IDepthStencilSurface* GetDepthStencilSurface() { return depthStencilSurface.get(); }
		};

		class MSAAResolve
		{
		private:
			std::unique_ptr<Graphics::IShader> embeddedShader;

			std::unique_ptr<Graphics::IRasterizer> embeddedRasterizerState;
			std::unique_ptr<Graphics::IDepthStencil> embeddedDepthStencilState;

		public:
			MSAAResolve() {}
			~MSAAResolve() { Finalize(); }

		public:
			static std::unique_ptr<MSAAResolve> Create();

			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			void Finalize();

			void Resolve(Graphics::IGraphicsDevice* graphicsDevice, const FrameBuffer* source, FrameBuffer* destination);
		};
	}
}