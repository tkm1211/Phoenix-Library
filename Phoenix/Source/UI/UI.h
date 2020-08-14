#pragma once

#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/FrameWork/Quad/Quad.h"


class UI
{
public:
	UI() {}
	virtual ~UI() {}

public:
	virtual void Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice) = 0;

	virtual void Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad) = 0;
};