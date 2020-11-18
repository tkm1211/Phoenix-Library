#pragma once

#include <memory>
#include <vector>
#include "UI.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/FrameWork/Quad/Quad.h"


class UISystem
{
private:
	std::vector<std::shared_ptr<UI>> ui;
	std::shared_ptr<Phoenix::FrameWork::Quad> quad;

public:
	UISystem() {}
	~UISystem() {}

public:
	static std::unique_ptr<UISystem> Create();

	void AddUI(std::shared_ptr<UI> addUI);

	void Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);

	void Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
};