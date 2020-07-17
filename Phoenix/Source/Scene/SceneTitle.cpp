#include "Scene.h"
#include "SceneSystem.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_win32.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_dx11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_internal.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"


void SceneTitle::Init(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	graphicsDevice = sceneSystem->GetGraphicsDevice();
	commonData = sceneSystem->GetSceneCommonData();

	// エフェクトの読込
	//auto effect = Effekseer::Effect::Create(manager, EFK_EXAMPLE_ASSETS_DIR_U16 "Laser01.efk");
	//effect = Effekseer::Effect::Create(commonData->manager, u"D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Effect\\Examples\\Resources\\Laser01.efk");
	effect = Effekseer::Effect::Create(commonData->manager, u"D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Effect\\Examples\\MAGICALxSPIRAL\\HitEffect.efk");

	// エフェクトの再生
	handle = commonData->manager->Play(effect, 0, 0, 0);
}

void SceneTitle::Update()
{
	if (xInput[0].bBACKt)
	{
		sceneSystem->ChangeScene(SceneType::Game, false);
	}

	// 投影行列の更新
	//renderer->SetProjectionMatrix(::Effekseer::Matrix44);

	// カメラ行列の更新
	//renderer->SetCameraMatrix(::Effekseer::Matrix44);

	// 3Dサウンド用リスナー設定の更新
	//sound->SetListener(リスナー位置, 注目点, 上方向ベクトル);

	// 再生中のエフェクトの移動等(::Effekseer::Manager経由で様々なパラメーターが設定できます。)
	//commonData->manager->AddLocation(handle, ::Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

	// 全てのエフェクトの更新
	commonData->manager->Update();
}

void SceneTitle::Draw()
{
	commonData->renderer->BeginRendering();
	commonData->manager->Draw();
	commonData->renderer->EndRendering();
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
		if (ImGui::Button("play"))
		{
			// エフェクトの再生
			handle = commonData->manager->Play(effect, 0, 0, 0);
		}
	}
	ImGui::End();
}