#include "EnemiesUI.h"
#include "EnemyUI.h"
#include "Phoenix/OS/Path.h"
#include "Phoenix/FND/Util.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"


std::shared_ptr<EnemiesUI> EnemiesUI::Create()
{
	return std::make_shared<EnemiesUI>();
}

void EnemiesUI::Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	hp = Phoenix::Graphics::ITexture::Create();
	damage = Phoenix::Graphics::ITexture::Create();
	hpBack = Phoenix::Graphics::ITexture::Create();

	hp->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\HP\\Boss\\BossHP.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	damage->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\HP\\DamageHP.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	hpBack->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\HP\\HPBack.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
}

void EnemiesUI::Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::s32 enemyRange)
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();
	Phoenix::Graphics::Viewport* v = new Phoenix::Graphics::Viewport();
	context->GetViewports(1, &v);
	Phoenix::f32 width = v->width;
	Phoenix::f32 height = v->height;
	Phoenix::FND::SafeDelete(v);

	pos = Phoenix::Math::Vector2(width - EnemyUI::SizeWidth, height - EnemyUI::SizeHeigth);

	uiList.resize(enemyRange);
	currentIndex = -1;
}

void EnemiesUI::Update(Phoenix::s32 index, Phoenix::f32 hpPercent, Phoenix::f32 elapsedTime)
{
	if (index <= -1 || uiList.size() <= index)
	{
		currentIndex = index;
		return;
	}

	uiList.at(index)->Update(hpPercent, elapsedTime);
	currentIndex = index;
}

// UI‚ð’Ç‰Á
void EnemiesUI::AddUI(Phoenix::s32 index, std::shared_ptr<EnemyUI> ui)
{
	uiList.at(index) = ui;
	uiList.at(index)->Initialize();
}

void EnemiesUI::Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad)
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();
	Phoenix::Graphics::ContextDX11* contextDX11 = static_cast<Phoenix::Graphics::ContextDX11*>(context);
	Phoenix::f32 blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaToCoverageEnable), blendFactor, 0xFFFFFFFF);
	{
		for (const auto& ui : uiList)
		{
			if (!ui) continue;
			if (!ui->GetExit()) continue;

			Phoenix::Math::Vector2 size = ui->GetSize() / 5.0f;
			Phoenix::Math::Vector2 hpTexPos = ui->GetHPTexPos();

			Phoenix::Math::Vector2 damgeSize = ui->GetDamageSize() / 5.0f;
			Phoenix::Math::Vector2 damageTexPos = ui->GetDamageTexPos();

			Phoenix::Math::Vector2 pos = ui->GetPos();
			pos.x -= (EnemyUI::SizeWidth / 5.0f) / 2.0f;

			quad->Draw(graphicsDevice, hpBack.get(), pos.x, pos.y, EnemyUI::SizeWidth / 5.0f, EnemyUI::SizeHeigth / 5.0f);
			quad->Draw(graphicsDevice, damage.get(), pos, damgeSize, Phoenix::Math::Vector2(0.0f, 0.0f), damageTexPos);
			quad->Draw(graphicsDevice, hp.get(), pos, size, Phoenix::Math::Vector2(0.0f, 0.0f), hpTexPos);
		}

		if (currentIndex <= -1 || uiList.size() <= currentIndex) return;

		Phoenix::Math::Vector2 size = uiList.at(currentIndex)->GetSize();
		Phoenix::Math::Vector2 hpTexPos = uiList.at(currentIndex)->GetHPTexPos();

		Phoenix::Math::Vector2 damgeSize = uiList.at(currentIndex)->GetDamageSize();
		Phoenix::Math::Vector2 damageTexPos = uiList.at(currentIndex)->GetDamageTexPos();

		quad->Draw(graphicsDevice, hpBack.get(), pos.x, pos.y, EnemyUI::SizeWidth, EnemyUI::SizeHeigth);
		quad->Draw(graphicsDevice, damage.get(), pos, damgeSize, Phoenix::Math::Vector2(0.0f, 0.0f), damageTexPos);
		quad->Draw(graphicsDevice, hp.get(), pos, size, Phoenix::Math::Vector2(0.0f, 0.0f), hpTexPos);
	}
	context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
}