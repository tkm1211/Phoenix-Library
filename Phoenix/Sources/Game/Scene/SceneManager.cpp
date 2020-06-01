#include "SceneManager.h"
#include "SceneTitle.h"


void SceneManager::Init()
{
	SetScene(new SceneTitle());
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
		stackScene->Update(this);
	}
	if (nextScene)
	{
		if (mainScene) mainScene->UnInit();
		mainScene = nextScene;
		nextScene = nullptr;
		mainScene->Init(this);
	}
	mainScene->Update(this);
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
}

void SceneManager::ReSetStackScene()
{
	mainScene = stackScene;
	stackScene = nullptr;
}