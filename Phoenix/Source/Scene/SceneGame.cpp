#include "Scene.h"
#include "SceneSystem.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_win32.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_dx11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_internal.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FrameWork/Shader/StandardShader.h"


void SceneGame::Init(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	graphicsDevice = sceneSystem->GetGraphicsDevice();

	player = Player::Create();
	player->Init(graphicsDevice);

	boss = Boss::Create();
	boss->Init(graphicsDevice, player.get());

	const char* filename = "..\\Data\\Assets\\Model\\stage\\stage01.fbx";
	stageModel = std::make_unique<Phoenix::FrameWork::ModelObject>();
	stageModel->Initialize(graphicsDevice);
	stageModel->Load(graphicsDevice, Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), filename));

	basicShader = Phoenix::FrameWork::BasicShader::Create();
	basicShader->Initialize(graphicsDevice);

	basicSkinShader = Phoenix::FrameWork::BasicSkinShader::Create();
	basicSkinShader->Initialize(graphicsDevice);

	standardShader = Phoenix::FrameWork::StandardShader::Create();
	standardShader->Initialize(graphicsDevice);

	cameraFlg = false;
}

void SceneGame::Update()
{
	// プレイヤー更新
	{
		player->Update(camera);
	}

	// ボス更新
	{
		boss->Update();
	}

	// 当たり判定
	{
		// TODO : カプセルVSカプセルに変更
		Phoenix::Math::Vector3 playerPos = player->GetPosition();
		Phoenix::Math::Vector3 bossPos = boss->GetPosition();
		if (CircleVsCircle(Phoenix::Math::Vector2(playerPos.x, playerPos.z), Phoenix::Math::Vector2(bossPos.x, bossPos.z), player->GetRadius(), boss->GetRadius()))
		{
			Phoenix::Math::Vector3 pos;
			Phoenix::Math::Vector3 dir = player->GetPosition() - boss->GetPosition();
			dir = Phoenix::Math::Vector3Normalize(dir);
			dir.y = 0.0f;

			pos = Phoenix::Math::Vector3(bossPos.x, playerPos.y, bossPos.z) + dir * (player->GetRadius() + boss->GetRadius());
			player->SetPosition(pos);
			player->UpdateTrasform();
		}
	}

	// カメラ更新
	{
		if (cameraFlg)
		{
			camera.FreeCamera();
		}
		else
		{
			//camera.ControllerCamera(player->GetPosition(), Phoenix::Math::Vector3(0.0f, 100.0f, 0.0f));

			Phoenix::Math::Vector3 bossPos = boss->GetPosition();
			Phoenix::Math::Vector3 playerPos = player->GetPosition();

			bossPos.y += 150.0f;
			playerPos.y += 150.0f;
			camera.LockOnCamera(bossPos, playerPos);
		}
		camera.Update();
	}
}

void SceneGame::Draw()
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	// ワールド行列を作成
	Phoenix::Math::Matrix W;
	{
		Phoenix::Math::Vector3 scale = { 40.0f, 40.0f, 40.0f };
		Phoenix::Math::Vector3 rotate = { 0.0f, 0.0f, 0.0f };
		Phoenix::Math::Vector3 translate = { 0.0f, 0.0f, 0.0f };

		Phoenix::Math::Matrix S, R, T;
		S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
		R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
		T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

		W = S * R * T;
	}

	// メッシュ描画
#if 1
	basicSkinShader->Begin(graphicsDevice, camera);
	basicSkinShader->Draw(graphicsDevice, player->GetWorldMatrix(), player->GetModel());
	basicSkinShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
	basicSkinShader->End(graphicsDevice);
#else
	standardShader->Begin(graphicsDevice, camera);
	standardShader->Draw(graphicsDevice, player->GetWorldMatrix(), player->GetModel());
	standardShader->End(graphicsDevice);
#endif

#if 1
	//basicSkinShader->Begin(graphicsDevice, camera);
	//basicSkinShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
	//basicSkinShader->End(graphicsDevice);
#else
	standardShader->Begin(graphicsDevice, camera);
	standardShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
	standardShader->End(graphicsDevice);
#endif

#if 1
	basicShader->Begin(graphicsDevice, camera);
	basicShader->Draw(graphicsDevice, W, stageModel.get());
	basicShader->End(graphicsDevice);
#else
	standardShader->Begin(graphicsDevice, camera);
	standardShader->Draw(graphicsDevice, W, stageModel.get());
	standardShader->End(graphicsDevice);
#endif
}

void SceneGame::GUI()
{
	ImGui::Begin("Game");
	{
		player->GUI();
		boss->GUI();
		if (ImGui::TreeNode("Camera"))
		{
			ImGui::Checkbox("FreeCamera", &cameraFlg);
			ImGui::TreePop();
		}
		Phoenix::Graphics::DirLight* dir = static_cast<Phoenix::FrameWork::StandardShader*>(standardShader.get())->GetLight()->GetDefaultDirLight();
		if (ImGui::TreeNode("Light"))
		{
			ImGui::DragFloat3("dir", &dir->direction.x, 0.01f, -1.0f, 1.0f);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}