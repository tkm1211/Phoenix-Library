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

	// ƒQ[ƒ€ƒNƒŠƒA
	{
		quad = Phoenix::FrameWork::Quad::Create();
		logo = Phoenix::Graphics::ITexture::Create();

		quad->Initialize(graphicsDevice);
		logo->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\GameClear\\GameClear.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void SceneGameClear::Initialize()
{
	isChangeScene = false;
}

void SceneGameClear::Update(Phoenix::f32 elapsedTime)
{
	if (isChangeScene) return;

	if (xInput[0].bAt || xInput[0].bBt || xInput[0].bXt || xInput[0].bYt || xInput[0].bRBt || xInput[0].bLBt || xInput[0].bRTt || xInput[0].bLTt || xInput[0].bSTARTt || xInput[0].bBACKt)
	{
		isChangeScene = true;
		sceneSystem->ChangeScene(SceneType::Title, false, true);
	}
}

void SceneGameClear::Draw(Phoenix::f32 elapsedTime)
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	Phoenix::f32 width = static_cast<Phoenix::f32>(display->GetWidth());
	Phoenix::f32 height = static_cast<Phoenix::f32>(display->GetHeight());

	// Draw Game Clear Logo.
	{
		quad->Draw(graphicsDevice, logo.get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f));
	}
}

void SceneGameClear::GUI()
{

}