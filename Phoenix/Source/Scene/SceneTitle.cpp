#include "Scene.h"
#include "SceneSystem.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_win32.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_dx11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_internal.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"
//#include "../../Effekseer/include/Effekseer/Effekseer.h"
//#include "../../Effekseer/include/EffekseerRendererDX11/EffekseerRendererDX11.h"


void SceneTitle::Init(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	graphicsDevice = sceneSystem->GetGraphicsDevice();
	commonData = sceneSystem->GetSceneCommonData();
}

void SceneTitle::Update()
{
	if (xInput[0].bBACKt)
	{
		sceneSystem->ChangeScene(SceneType::Game, false);
	}
}

void SceneTitle::Draw()
{

}

void SceneTitle::GUI()
{
	ImGui::Begin("Title");
	{
		ImGui::Text("test");
		if (ImGui::Button("Chage Scene Game"))
		{
			sceneSystem->ChangeScene(SceneType::Game, false);
		}
	}
	ImGui::End();
}