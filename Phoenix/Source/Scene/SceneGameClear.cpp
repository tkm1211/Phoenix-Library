#include "Scene.h"
#include "SceneSystem.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"


void SceneGameClear::Construct(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	display = sceneSystem->GetDisplay();
	graphicsDevice = sceneSystem->GetGraphicsDevice();
	commonData = sceneSystem->GetSceneCommonData();

	// ゲームクリア
	{
		quad = Phoenix::FrameWork::Quad::Create();
		logo = Phoenix::Graphics::ITexture::Create();
		button = Phoenix::Graphics::ITexture::Create();

		quad->Initialize(graphicsDevice);
		logo->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\GameClear\\Logo.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
		button->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\GameClear\\Button.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
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

void SceneGameClear::Initialize()
{
	isChangeScene = false;

	bright = 0.1f;
	contrast = 2.0f;
	saturate = 0.5f;
	screenColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	isTurn = true;
}

void SceneGameClear::Update(Phoenix::f32 elapsedTime)
{
	if (sceneSystem->GetOnFade()) return;

	if (isChangeScene) return;

	if (xInput[0].bAt || xInput[0].bBt || xInput[0].bXt || xInput[0].bYt || xInput[0].bRBt || xInput[0].bLBt || xInput[0].bRTt || xInput[0].bLTt || xInput[0].bSTARTt || xInput[0].bBACKt)
	{
		isChangeScene = true;
		sceneSystem->ChangeScene(SceneType::Title, false, true);
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

void SceneGameClear::Draw(Phoenix::f32 elapsedTime)
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	Phoenix::f32 width = static_cast<Phoenix::f32>(display->GetWidth());
	Phoenix::f32 height = static_cast<Phoenix::f32>(display->GetHeight());

	frameBuffer->Clear(graphicsDevice, 0, 0.0f, 0.0f, 0.0f, 1.0f);
	frameBuffer->Activate(graphicsDevice);
	{
		// Draw Game Clear Logo.
		{
			quad->SetBright(bright);
			quad->SetContrast(contrast);
			quad->SetSaturate(saturate);
			quad->SetScreenColor(screenColor);
			quad->Draw(graphicsDevice, logo.get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, true, true, true, true, true, true, false, false);
			quad->Draw(graphicsDevice, button.get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f));
		}
	}
	frameBuffer->Deactivate(graphicsDevice);

	quad->Draw(graphicsDevice, frameBuffer->renderTargerSurface[0]->GetTexture(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height));

	//toneMap->Draw(graphicsDevice, frameBuffer->renderTargerSurface[0]->GetTexture(), elapsedTime);
}

void SceneGameClear::GUI()
{
	/*if (ImGui::TreeNode("Screen Filter"))
	{
		ImGui::SliderFloat("bright", &bright, 0.0f, 1.0f);
		ImGui::SliderFloat("contrast", &contrast, 0.0f, 10.0f);
		ImGui::SliderFloat("saturate", &saturate, 0.0f, 1.0f);
		ImGui::SliderFloat4("screenColor", &screenColor.r, 0.0f, 2.0f);

		ImGui::TreePop();
	}*/
}