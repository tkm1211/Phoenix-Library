#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>

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
	ImGui::Begin("Vector3");
	ImGui::Text("%f, %f, %f", vector3.x, vector3.y, vector3.z);
	ImGui::End();
}

void SceneTitle::SetSceneManger(SceneManager* pSceneManager)
{
	sceneManager = pSceneManager;
}

void SceneTitle::SetGraphicsDevice(GraphicsDevice* pGraphicsDevice)
{
	graphicsDevice = pGraphicsDevice;
}