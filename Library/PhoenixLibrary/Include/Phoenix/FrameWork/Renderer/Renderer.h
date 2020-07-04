#pragma once

#include "Phoenix/Math/Color.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Shader/Shader.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �����_�����
		//****************************************************************************
		class Renderer
		{
		public:
			Renderer() {}
			virtual ~Renderer() {}

		public:
			virtual void Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera) = 0;
			virtual void Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model, IShader* shader) = 0;
			virtual void End(Graphics::IGraphicsDevice* graphicsDevice) = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix