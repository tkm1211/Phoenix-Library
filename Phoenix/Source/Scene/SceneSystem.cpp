#include "SceneSystem.h"


std::unique_ptr<SceneSystem> SceneSystem::Create()
{
	return std::make_unique<SceneSystem>();
}

void SceneSystem::Init(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	this->graphicsDevice = graphicsDevice;

	AddScene<SceneTitle>();
	AddScene<SceneGame>();
	ChangeScene(SceneType::Title, false);
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
	currentScene->Update();
}

void SceneSystem::GUI()
{
	currentScene->GUI();
}

void SceneSystem::ChangeScene(SceneType sceneType, bool stack)
{
	if (stack)
	{
		stackScene = currentScene;
	}
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
}