#include "TutorialUI.h"
#include "Phoenix/OS/Path.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"


// 生成
std::shared_ptr<TutorialUI> TutorialUI::Create()
{
    return std::make_shared<TutorialUI>();
}

// 初期化
void TutorialUI::Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	attack = Phoenix::Graphics::ITexture::Create();
	attackDetail = Phoenix::Graphics::ITexture::Create();

	dedge = Phoenix::Graphics::ITexture::Create();
	dedgeDetail = Phoenix::Graphics::ITexture::Create();

	lockOn = Phoenix::Graphics::ITexture::Create();
	lockOnDetail = Phoenix::Graphics::ITexture::Create();

	attack->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\AttackUI.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	attackDetail->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\AttackUI2.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));

	dedge->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\AvoidUI.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	dedgeDetail->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\AvoidUI2.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));

	lockOn->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\LockOnUI.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	lockOnDetail->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\LockOnUI2.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));

	state = 0;
	waitCnt = 0.0f;

	toDraw = false;
	for (int i = 0; i < 4; ++i)
	{
		cleared[i] = false;
	}
}

// 更新
void TutorialUI::Update(Phoenix::f32 elapsedTime)
{
	switch (state)
	{
	case 0:
		TransitionState((cleared[0] && cleared[1]), elapsedTime);
		break;

	case 1:
		TransitionState(cleared[2], elapsedTime);
		break;

	case 2:
		TransitionState(cleared[3], elapsedTime);
		break;

	case 3:
		toDraw = false;
		break;
	default: break;
	}
}

// ステート移行
void TutorialUI::TransitionState(bool condition, Phoenix::f32 elapsedTime)
{
	if (condition)
	{
		if (WaitMaxCnt <= waitCnt)
		{
			++state;
			waitCnt = 0.0f;
		}
		else
		{
			waitCnt += 1.0f * elapsedTime;
		}
	}
}

// 描画
void TutorialUI::Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad)
{
	if (!toDraw) return;

	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();
	Phoenix::Graphics::ContextDX11* contextDX11 = static_cast<Phoenix::Graphics::ContextDX11*>(context);
	Phoenix::f32 blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaBlend), blendFactor, 0xFFFFFFFF);
	{
		switch (state)
		{
		case 0:
			quad->Draw(graphicsDevice, attack.get(), 0.0f, 0.0f, SizeWidth, SizeHeigth);
			quad->Draw(graphicsDevice, attackDetail.get(), 0.0f, 0.0f, SizeWidth, SizeHeigth);
			break;

		case 1:
			quad->Draw(graphicsDevice, dedge.get(), 0.0f, 0.0f, SizeWidth, SizeHeigth);
			quad->Draw(graphicsDevice, dedgeDetail.get(), 0.0f, 0.0f, SizeWidth, SizeHeigth);
			break;

		case 2:
			quad->Draw(graphicsDevice, lockOn.get(), 0.0f, 0.0f, SizeWidth, SizeHeigth);
			quad->Draw(graphicsDevice, lockOnDetail.get(), 0.0f, 0.0f, SizeWidth, SizeHeigth);
			break;
		default: break;
		}
	}
	context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
}

void TutorialUI::ClearWeakAttack()
{
	if (state == 0) cleared[0] = true;
}

void TutorialUI::ClearStrongAttack()
{
	if (state == 0) cleared[1] = true;
}

void TutorialUI::ClearDedge()
{
	if (state == 1) cleared[2] = true;
}

void TutorialUI::ClearLockOn()
{
	if (state == 2) cleared[3] = true;
}

void TutorialUI::ToDraw(bool toDraw)
{
	this->toDraw = toDraw;
}