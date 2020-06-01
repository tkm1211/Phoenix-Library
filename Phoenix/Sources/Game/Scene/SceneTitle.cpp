#include "SceneTitle.h"


void SceneTitle::Init(SceneManager* sceneManager)
{
	vector3 = Phoenix::Lib::Vector3(0.0f, 0.0f, 0.0f);
}

void SceneTitle::UnInit()
{

}

void SceneTitle::Update(SceneManager* sceneManager)
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