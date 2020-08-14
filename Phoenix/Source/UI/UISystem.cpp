#include "UISystem.h"
#include "PlayerUI.h"


std::unique_ptr<UISystem> UISystem::Create()
{
	return std::make_unique<UISystem>();
}

void UISystem::AddUI(UI* addUI)
{
	ui.emplace_back(addUI);
}

void UISystem::Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	quad = Phoenix::FrameWork::Quad::Create();
	quad->Initialize(graphicsDevice);

	for (UI* it : ui)
	{
		it->Initialize(graphicsDevice);
	}
}

void UISystem::Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	for (UI* it : ui)
	{
		it->Draw(graphicsDevice, quad.get());
	}
}