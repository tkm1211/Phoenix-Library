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
		logo[0] = Phoenix::Graphics::ITexture::Create();
		logo[1] = Phoenix::Graphics::ITexture::Create();

		quad->Initialize(graphicsDevice);
		logo[0]->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\GameOver\\Replay.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
		logo[1]->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\GameOver\\Title.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void SceneGameOver::Initialize()
{
	isChangeScene = false;
	isRight = false;
}

void SceneGameOver::Update(Phoenix::f32 elapsedTime)
{
	if (isChangeScene) return;

	if (isRight && xInput[0].bLEFTt)
	{
		isRight = false;
	}
	else if (!isRight && xInput[0].bRIGHTt)
	{
		isRight = true;
	}

	if (xInput[0].bAt || xInput[0].bBt || xInput[0].bXt || xInput[0].bYt || xInput[0].bRBt || xInput[0].bLBt || xInput[0].bRTt || xInput[0].bLTt || xInput[0].bSTARTt || xInput[0].bBACKt)
	{
		isChangeScene = true;
		if (!isRight) sceneSystem->ChangeScene(SceneType::Game, false, true);
		else if (isRight) sceneSystem->ChangeScene(SceneType::Title, false, true);
	}
}

void SceneGameOver::Draw(Phoenix::f32 elapsedTime)
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	Phoenix::f32 width = static_cast<Phoenix::f32>(display->GetWidth());
	Phoenix::f32 height = static_cast<Phoenix::f32>(display->GetHeight());

	// Draw Game Over Logo.
	{
		if (!isRight) quad->Draw(graphicsDevice, logo[0].get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f));
		else if (isRight) quad->Draw(graphicsDevice, logo[1].get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f));
	}
}

void SceneGameOver::GUI()
{

}