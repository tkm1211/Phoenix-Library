#include "PlayerUI.h"
#include "Phoenix/OS/Path.h"


std::shared_ptr<PlayerUI> PlayerUI::Create()
{
	return std::make_shared<PlayerUI>();
}

void PlayerUI::Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	hp = Phoenix::Graphics::ITexture::Create();
	hpBack = Phoenix::Graphics::ITexture::Create();

	hp->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\HP\\Player\\PlayerHP.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	hpBack->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\HP\\HPBack.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));

	pos = Phoenix::Math::Vector2(0.0f, 0.0f);
	size = Phoenix::Math::Vector2(TexWidth, TexHeigth);
	hpTexPos = Phoenix::Math::Vector2(TexWidth, TexHeigth);
}

void PlayerUI::Update(Phoenix::f32 hpPercent)
{
	hpTexPos.x = TexWidth * (hpPercent / 100.0f);
}

void PlayerUI::Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad)
{
	quad->Draw(graphicsDevice, hpBack.get(), 0, 0, TexWidth, TexHeigth);
	quad->Draw(graphicsDevice, hp.get(), pos, hpTexPos, Phoenix::Math::Vector2(0.0f, 0.0f), hpTexPos);
}