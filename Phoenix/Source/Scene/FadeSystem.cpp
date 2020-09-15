#include "FadeSystem.h"
#include "Phoenix/FND/Util.h"


std::shared_ptr<FadeSystem> FadeSystem::Create()
{
	return std::make_shared<FadeSystem>();
}

bool FadeSystem::Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	working = false;
	isTrun = false;
	type = SceneType::Title;
	dissolveThreshold = 1.0f;

	quad = Phoenix::FrameWork::Quad::Create();
	quad->Initialize(graphicsDevice);
	quad->LoadDissolveTexture(graphicsDevice, "..\\Data\\Assets\\Texture\\Mask\\Dissolve\\dissolve_animation2.png");

	fade = Phoenix::Graphics::ITexture::Create();
	bool check = fade->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\Fade\\Fade01.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));

	return true;
}

void FadeSystem::Finalize()
{
	fade.reset();
	quad.reset();
}

void FadeSystem::Update(SceneSystem* sceneSystem)
{
	if (!working) return;

	if (!isTrun)
	{
		dissolveThreshold -= dissolveSpeed;
		if (dissolveThreshold <= 0.0f)
		{
			sceneSystem->SetScene(type);
			isTrun = true;
		}
	}
	else
	{
		dissolveThreshold += dissolveSpeed;
		if (1.0f <= dissolveThreshold)
		{
			working = false;
			isTrun = false;
			dissolveThreshold = 0.0f;
		}
	}
}

void FadeSystem::Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	if (!working) return;

	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	Phoenix::Graphics::Viewport* v = new Phoenix::Graphics::Viewport();
	context->GetViewports(1, &v);

	Phoenix::f32 width = v->width;
	Phoenix::f32 height = v->height;

	Phoenix::FND::SafeDelete(v);

	quad->SetDissolveThreshold(dissolveThreshold);
	//quad->Draw(graphicsDevice, fade.get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f));
	quad->Draw(graphicsDevice, fade.get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, true, true, true, true, true, true);
}

void FadeSystem::OnFade(SceneType sceneType)
{
	working = true;
	type = sceneType;
}