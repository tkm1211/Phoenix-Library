#pragma once

#include "Phoenix/Math/Color.h"
//#include "Phoenix/Math/Frustum.h"
#include "Phoenix/FrameWork/Component.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/Graphics/Camera.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// レンダラ基底
		//****************************************************************************
		class Renderer
		{
		public:
			Renderer() {}
			virtual ~Renderer() {}

		public:
			virtual void Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera) = 0;
			virtual void Draw(Graphics::IGraphicsDevice* graphicsDevice, ModelObject* model, bool skining = false) = 0;
			virtual void End(Graphics::IGraphicsDevice* graphicsDevice) = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix