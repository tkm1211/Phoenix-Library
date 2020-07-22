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

	//const char* filename = "..\\Data\\Assets\\Model\\TestModel\\BlackSword03.fbx";
	const char* filename = "..\\Data\\Assets\\Model\\TestModel\\danbo_fbx\\danbo_atk.fbx";
	//const char* filename = "..\\Data\\Assets\\Model\\TestModel\\Sphere01\\Sphere01.fbx";
	//const char* filename = "..\\Data\\Assets\\Model\\TestModel\\mari\\mari.fbx";
	model = std::make_unique<Phoenix::FrameWork::ModelObject>();
	model->Initialize(graphicsDevice);
	model->Load(graphicsDevice, Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), filename));

	// 待機モーション開始
	{
		model->PlayAnimation(0, 0);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		//model->PauseAnimation(true);
	}

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

	commonData->camera->FreeCamera();
	commonData->camera->Update();
}

void SceneTitle::Draw()
{
	/*commonData->renderer->BeginRendering();
	commonData->manager->Draw();
	commonData->renderer->EndRendering();*/

	// ワールド行列を作成
	Phoenix::Math::Matrix W;
	{
		Phoenix::Math::Vector3 scale = { 1.0f, 1.0f, 1.0f };
		Phoenix::Math::Vector3 rotate = { 0.0f, 0.0f, 0.0f };
		Phoenix::Math::Vector3 translate = { 0.0f, 0.0f, 0.0f };

		Phoenix::Math::Matrix S, R, T;
		S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
		R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
		T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

		W = S * R * T;
	}

#if 0
	commonData->basicShader->Begin(graphicsDevice, *commonData->camera);
	commonData->basicShader->Draw(graphicsDevice, W, commonData->stageModel.get());
	commonData->basicShader->Draw(graphicsDevice, W, model.get());
	commonData->basicShader->End(graphicsDevice);
#else
	commonData->basicShader->Begin(graphicsDevice, *commonData->camera);
	commonData->basicShader->Draw(graphicsDevice, W, commonData->stageModel.get());
	commonData->basicShader->End(graphicsDevice);

	commonData->pbrShader->Begin(graphicsDevice, *commonData->camera);
	commonData->pbrShader->Draw(graphicsDevice, W, model.get());
	commonData->pbrShader->End(graphicsDevice);
#endif
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

	ImGui::Begin("Game");
	{
		if (ImGui::TreeNode("Shader"))
		{
			Phoenix::FrameWork::LightState* light = static_cast<Phoenix::FrameWork::PBRShader*>(commonData->pbrShader.get())->GetLight();
			Phoenix::FrameWork::MaterialState* material = static_cast<Phoenix::FrameWork::PBRShader*>(commonData->pbrShader.get())->GetMaterial();

			ImGui::DragFloat4("dir", &light->direction.x, 0.01f, -1.0f, 1.0f);
			ImGui::DragFloat4("color", &light->color.x);
			ImGui::DragFloat4("albedo", &material->albedo.x);
			ImGui::DragFloat("metallic", &material->metallic, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("roughness", &material->roughness, 0.01f, 0.0f, 1.0f);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}