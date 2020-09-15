#include "SceneSystem.h"
#include "FadeSystem.h"
#include "../Player/Player.h"
#include "../Boss/Boss.h"
#include "Phoenix/Types.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Shader/Shader.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FrameWork/Shader/StandardShader.h"


std::unique_ptr<SceneSystem> SceneSystem::Create()
{
	return std::make_unique<SceneSystem>();
}

void SceneSystem::Init(Phoenix::OS::IDisplay* display, Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	this->display = display;
	this->graphicsDevice = graphicsDevice;
	commonData = SceneCommonData::Create();
	commonData->Initialize(graphicsDevice);

	fadeSystem = FadeSystem::Create();
	fadeSystem->Initialize(graphicsDevice);

	AddScene<SceneTitle>();
	AddScene<SceneGame>();
	ChangeScene(SceneType::Title, false, false);
}

void SceneSystem::Update()
{
	// XVˆ—
	if (stackScene)
	{
		stackScene->Update();
	}
	if (nextScene)
	{
		currentScene = nextScene;
		nextScene = nullptr;
		currentScene->Init(this);
	}
	fadeSystem->Update(this);
	currentScene->Update();
}

void SceneSystem::GUI()
{
	currentScene->GUI();
}

void SceneSystem::ChangeScene(SceneType sceneType, bool stack, bool fade)
{
	if (stack)
	{
		stackScene = currentScene;
	}

	if (fade)
	{
		fadeSystem->OnFade(sceneType);
	}
	else
	{
		SetScene(sceneType);
	}
}

void SceneSystem::SetScene(SceneType sceneType)
{
	nextScene = GetScene(sceneType);
}

void SceneSystem::ReSetStackScene()
{
	currentScene = stackScene;
	stackScene = nullptr;
}

void SceneSystem::Draw()
{
	if (stackScene)
	{
		stackScene->Draw();
	}
	currentScene->Draw();
	fadeSystem->Draw(graphicsDevice);
}

bool SceneSystem::GetOnFade()
{
	return fadeSystem->GetWorking();
}