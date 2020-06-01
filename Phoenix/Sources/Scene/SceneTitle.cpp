#include "SceneTitle.h"


void SceneTitle::Init()
{
	vector3 = Phoenix::Lib::Vector3(0.0f, 0.0f, 0.0f);
}

void SceneTitle::UnInit()
{

}

void SceneTitle::Update()
{
	vector3.x++;
	vector3.y++;
	vector3.z++;
}

void SceneTitle::Render()
{

}

void SceneTitle::ImGui()
{

}

void SceneTitle::SetSceneManger(SceneManager* pSceneManager)
{
	sceneManager = pSceneManager;
}

void SceneTitle::SetGraphicsDevice(GraphicsDevice* pGraphicsDevice)
{
	graphicsDevice = pGraphicsDevice;
}