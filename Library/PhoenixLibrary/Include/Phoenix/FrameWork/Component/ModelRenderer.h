#pragma once

#include <memory>
#include <string>
#include "Phoenix/Graphics/Material.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/FrameWork/Component/Renderer.h"
#include "Phoenix/FrameWork/Material.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// ƒ‚ƒfƒ‹ƒŒƒ“ƒ_ƒ‰
		//****************************************************************************
		class ModelRenderer : public Renderer
		{
		public:
			ModelRenderer() {}
			~ModelRenderer() {}

		public:
			void Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera, Graphics::IBuffer* buffers[], u32 size) override;
			void Draw(Graphics::IGraphicsDevice* graphicsDevice, ModelObject* model, bool skining = false) override;
			void End(Graphics::IGraphicsDevice* graphicsDevice) override;
		};
	} // namespace FrameWork
} // namespace Phoenix