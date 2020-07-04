#pragma once

#include <memory>
#include <string>
#include "Phoenix/Graphics/Material.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/FrameWork/Renderer/Renderer.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// ���f�������_��
		//****************************************************************************
		class ModelRenderer : public Renderer
		{
		public:
			ModelRenderer() {}
			~ModelRenderer() {}

		public:
			void Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera) override;
			void Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model, IShader* shader) override;
			void End(Graphics::IGraphicsDevice* graphicsDevice) override;
		};
	} // namespace FrameWork
} // namespace Phoenix