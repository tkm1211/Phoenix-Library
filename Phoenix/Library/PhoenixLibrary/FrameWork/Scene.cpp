#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

#include "Scene.h"


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
	ImGuiNewFrame();

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
	ImGuiRender();
}

void SceneManager::ImGui()
{
	mainScene->ImGui();
}

void SceneManager::ImGuiNewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void SceneManager::ImGuiRender()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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