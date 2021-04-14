#include "PlayerUI.h"
#include "Phoenix/OS/Path.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"


std::shared_ptr<PlayerUI> PlayerUI::Create()
{
	return std::make_shared<PlayerUI>();
}

void PlayerUI::Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	hp = Phoenix::Graphics::ITexture::Create();
	damage = Phoenix::Graphics::ITexture::Create();
	hpBack = Phoenix::Graphics::ITexture::Create();

	hp->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\HP\\Player\\PlayerHP.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	damage->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\HP\\DamageHP.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	hpBack->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\HP\\HPBack.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
}

void PlayerUI::Initialize()
{
	pos = Phoenix::Math::Vector2(0.0f, 0.0f);
	size = Phoenix::Math::Vector2(SizeWidth, SizeHeigth);
	hpTexPos = Phoenix::Math::Vector2(TexWidth, TexHeigth);

	damageSize = Phoenix::Math::Vector2(SizeWidth, SizeHeigth);
	damageTexPos = Phoenix::Math::Vector2(TexWidth, TexHeigth);

	hpPercent = 100.0f;
	damagePercent = 100.0f;
	startTime = 0.0f;

	downDamageGage = false;
}

void PlayerUI::Update(Phoenix::f32 hpPercent, Phoenix::f32 elapsedTime)
{
	if (this->hpPercent != hpPercent)
	{
		damagePercent = this->hpPercent;
		startTime = startTimeMax;
		downDamageGage = false;
	}

	if (0.0f < startTime)
	{
		startTime -= 1.0f * elapsedTime;
	}
	else if (damagePercent != hpPercent)
	{
		downDamageGage = true;
		startTime = 0.0f;
	}

	if (downDamageGage)
	{
		damagePercent = Phoenix::Math::f32Lerp(damagePercent, hpPercent, 0.05f * elapsedTime);

		if ((damagePercent - hpPercent) <= 0.01f)
		{
			downDamageGage = false;
			damagePercent = hpPercent;
		}
	}

	size.x = SizeWidth * (hpPercent / 100.0f);
	hpTexPos.x = TexWidth * (hpPercent / 100.0f);

	damageSize.x = SizeWidth * (damagePercent / 100.0f);
	damageTexPos.x = TexWidth * (damagePercent / 100.0f);

	this->hpPercent = hpPercent;
}

void PlayerUI::Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad)
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();
	Phoenix::Graphics::ContextDX11* contextDX11 = static_cast<Phoenix::Graphics::ContextDX11*>(context);
	Phoenix::f32 blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaToCoverageEnable), blendFactor, 0xFFFFFFFF);
	{
		quad->Draw(graphicsDevice, hpBack.get(), 0, 0, SizeWidth, SizeHeigth);
		quad->Draw(graphicsDevice, damage.get(), pos, damageSize, Phoenix::Math::Vector2(0.0f, 0.0f), damageTexPos);
		quad->Draw(graphicsDevice, hp.get(), pos, size, Phoenix::Math::Vector2(0.0f, 0.0f), hpTexPos);
	}
	context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
}