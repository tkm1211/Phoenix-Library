#include "SceneManager.h"
#include "SceneTitle.h"


void SceneManager::Init(Scene* firstScene, GraphicsDevice* pGraphicsDevice)
{
	graphicsDevice = pGraphicsDevice;
	SetScene(firstScene);
}

void SceneManager::UnInit()
{
	mainScene->UnInit();
}

void SceneManager::Update()
{
	// XVˆ—
	if (stackScene)
	{
		stackScene->Update();
	}
	if (nextScene)
	{
		if (mainScene) mainScene->UnInit();
		mainScene = nextScene;
		nextScene = nullptr;
		mainScene->Init();
	}
	mainScene->Update();
}

void SceneManager::Render()
{
	mainScene->Render();
}

void SceneManager::ImGui()
{
	mainScene->ImGui();
}

void SceneManager::SetScene(Scene* scene, bool nowSceneStack)
{
	if (nowSceneStack)
	{
		stackScene = scene;
	}

	nextScene = scene;
	nextScene->SetSceneManger(this);
	nextScene->SetGraphicsDevice(graphicsDevice);
}

void SceneManager::ReSetStackScene()
{
	mainScene = stackScene;
	stackScene = nullptr;
}