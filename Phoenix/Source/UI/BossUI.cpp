#include "BossUI.h"
#include "Phoenix/OS/Path.h"
#include "Phoenix/FND/Util.h"


std::shared_ptr<BossUI> BossUI::Create()
{
	return std::make_shared<BossUI>();
}

void BossUI::Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	hp = Phoenix::Graphics::ITexture::Create();
	hpBack = Phoenix::Graphics::ITexture::Create();

	hp->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\HP\\Boss\\BossHP.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	hpBack->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\HP\\HPBack.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));

	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();
	Phoenix::Graphics::Viewport* v = new Phoenix::Graphics::Viewport();
	context->GetViewports(1, &v);
	Phoenix::f32 width = v->width;
	Phoenix::f32 height = v->height;
	Phoenix::FND::SafeDelete(v);

	pos = Phoenix::Math::Vector2(width - TexWidth, height - TexHeigth);
	size = Phoenix::Math::Vector2(TexWidth, TexHeigth);
	hpTexPos = Phoenix::Math::Vector2(TexWidth, TexHeigth);
}

void BossUI::Update(Phoenix::f32 hpPercent)
{
	hpTexPos.x = TexWidth * (hpPercent / 100.0f);
}

void BossUI::Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad)
{
	quad->Draw(graphicsDevice, hpBack.get(), pos.x, pos.y, TexWidth, TexHeigth);
	quad->Draw(graphicsDevice, hp.get(), pos, hpTexPos, Phoenix::Math::Vector2(0.0f, 0.0f), hpTexPos);
}