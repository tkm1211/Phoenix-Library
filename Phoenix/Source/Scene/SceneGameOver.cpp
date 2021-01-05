#include "Scene.h"
#include "SceneSystem.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"


void SceneGameOver::Construct(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	display = sceneSystem->GetDisplay();
	graphicsDevice = sceneSystem->GetGraphicsDevice();
	commonData = sceneSystem->GetSceneCommonData();

	// ゲームオーバー
	{
		quad = Phoenix::FrameWork::Quad::Create();
		logo = Phoenix::Graphics::ITexture::Create();
		button[0] = Phoenix::Graphics::ITexture::Create();
		button[1] = Phoenix::Graphics::ITexture::Create();

		quad->Initialize(graphicsDevice);
		logo->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\GameOver\\Logo.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
		button[0]->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\GameOver\\Replay.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
		button[1]->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\GameOver\\Title.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// フレームバッファ
	{
		frameBuffer = Phoenix::FrameWork::FrameBuffer::Create();
		frameBuffer->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
	}

	// トーンマップ
	{
		toneMap = Phoenix::FrameWork::ToneMap::Create();
		toneMap->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight());
	}
}

void SceneGameOver::Initialize()
{
	isChangeScene = false;
	isRight = false;

	bright = 0.1f;
	contrast = 2.0f;
	saturate = 0.5f;
	screenColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	isTurn = true;
}

void SceneGameOver::Update(Phoenix::f32 elapsedTime)
{
	if (sceneSystem->GetOnFade()) return;

	if (isChangeScene) return;

	if (isRight && xInput[0].bLEFTt || xInput[0].sX < 0)
	{
		isRight = false;
	}
	else if (!isRight && xInput[0].bRIGHTt || 0 < xInput[0].sX)
	{
		isRight = true;
	}

	if ((GetKeyState(VK_SPACE) & 1) || xInput[0].bAt || xInput[0].bBt || xInput[0].bXt || xInput[0].bYt || xInput[0].bRBt || xInput[0].bLBt || xInput[0].bRTt || xInput[0].bLTt || xInput[0].bSTARTt || xInput[0].bBACKt)
	{
		isChangeScene = true;
		if (!isRight) sceneSystem->ChangeScene(SceneType::Game, false, true);
		else if (isRight) sceneSystem->ChangeScene(SceneType::Title, false, true);
	}

	if (isTurn)
	{
		bright += addBright * elapsedTime;
		if (0.25f <= bright)
		{
			bright = 0.25f;
			isTurn = false;
		}
	}
	else
	{
		bright -= addBright * elapsedTime;
		if (bright <= 0.1f)
		{
			bright = 0.1f;
			isTurn = true;
		}
	}
}

void SceneGameOver::Draw(Phoenix::f32 elapsedTime)
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	Phoenix::f32 width = static_cast<Phoenix::f32>(display->GetWidth());
	Phoenix::f32 height = static_cast<Phoenix::f32>(display->GetHeight());

	frameBuffer->Clear(graphicsDevice, 0, 0.0f, 0.0f, 0.0f, 1.0f);
	frameBuffer->Activate(graphicsDevice);
	{
		// Draw Game Over Logo.
		{
			quad->SetBright(bright);
			quad->SetContrast(contrast);
			quad->SetSaturate(saturate);
			quad->SetScreenColor(screenColor);
			quad->Draw(graphicsDevice, logo.get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, true, true, true, true, true, true, false, false);
			if (!isRight) quad->Draw(graphicsDevice, button[0].get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f));
			else if (isRight) quad->Draw(graphicsDevice, button[1].get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f));
		}
	}
	frameBuffer->Deactivate(graphicsDevice);

	quad->Draw(graphicsDevice, frameBuffer->renderTargerSurface[0]->GetTexture(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height));

	//toneMap->Draw(graphicsDevice, frameBuffer->renderTargerSurface[0]->GetTexture(), elapsedTime);
}

void SceneGameOver::GUI()
{

}