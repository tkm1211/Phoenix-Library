#include "Scene.h"
#include "SceneSystem.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_win32.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_dx11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_internal.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"
#include "Phoenix/Math/PhoenixMath.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"


void SceneGame::Init(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	graphicsDevice = sceneSystem->GetGraphicsDevice();
	commonData = sceneSystem->GetSceneCommonData();

	player = commonData->player.get();
	boss = commonData->boss.get();
	stageModel = commonData->stageModel.get();
	basicShader = commonData->basicShader.get();
	basicSkinShader = commonData->basicSkinShader.get();
	standardShader = commonData->standardShader.get();
	camera = commonData->camera.get();

	cameraFlg = false;

	Phoenix::Graphics::DeviceDX11* device = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());
	primitive = std::make_shared<GeometricPrimitive>(device->GetD3DDevice(), 1);
}

void SceneGame::Update()
{
	// プレイヤー更新
	{
		player->Update(*camera);
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
		/*if (CircleVsCircle(Phoenix::Math::Vector2(playerPos.x, playerPos.z), Phoenix::Math::Vector2(bossPos.x, bossPos.z), player->GetRadius(), boss->GetRadius()))
		{
			Phoenix::Math::Vector3 pos;
			Phoenix::Math::Vector3 dir = player->GetPosition() - boss->GetPosition();
			dir = Phoenix::Math::Vector3Normalize(dir);
			dir.y = 0.0f;

			pos = Phoenix::Math::Vector3(bossPos.x, playerPos.y, bossPos.z) + dir * (player->GetRadius() + boss->GetRadius());
			player->SetPosition(pos);
			player->UpdateTrasform();
		}*/
		if (SphereVsSphere(Phoenix::Math::Vector3(playerPos.x, playerPos.y + 50.0f, playerPos.z), Phoenix::Math::Vector3(bossPos.x, bossPos.y + 50.0f, bossPos.z), player->GetRadius(), boss->GetRadius()))
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
			camera->FreeCamera();
		}
		else
		{
			//camera.ControllerCamera(player->GetPosition(), Phoenix::Math::Vector3(0.0f, 100.0f, 0.0f));

			Phoenix::Math::Vector3 bossPos = boss->GetPosition();
			Phoenix::Math::Vector3 playerPos = player->GetPosition();

			bossPos.y += 150.0f;
			playerPos.y += 150.0f;
			camera->LockOnCamera(bossPos, playerPos);
		}
		camera->Update();
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
	basicSkinShader->Begin(graphicsDevice, *camera);
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
	basicShader->Begin(graphicsDevice, *camera);
	basicShader->Draw(graphicsDevice, W, stageModel);
	basicShader->End(graphicsDevice);
#else
	standardShader->Begin(graphicsDevice, camera);
	standardShader->Draw(graphicsDevice, W, stageModel);
	standardShader->End(graphicsDevice);
#endif

	Phoenix::Graphics::DeviceDX11* device = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());

	// ワールド行列を作成
	Phoenix::Math::Matrix primitiveM;
	{
		Phoenix::Math::Vector3 scale = { 50.0f, 50.0f, 50.0f };
		Phoenix::Math::Vector3 rotate = { 0.0f, 0.0f, 0.0f };
		Phoenix::Math::Vector3 translate = player->GetPosition();
		translate.y += 100.0f;

		Phoenix::Math::Matrix S, R, T;
		S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
		R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
		T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

		primitiveM = S * R * T;
	}
	primitive->Render(device->GetD3DContext(), 
		Phoenix::Math::ConvertToFloat4x4FromVector4x4(primitiveM * camera->GetView() * camera->GetProjection()),
		Phoenix::Math::ConvertToFloat4x4FromVector4x4(primitiveM),
		DirectX::XMFLOAT4(1, 1, 1, 1), 
		DirectX::XMFLOAT4(0.0f, 0.6f, 0.0f, 0.5f), 
		false);

	// ワールド行列を作成
	{
		Phoenix::Math::Vector3 scale = { 75.0f, 75.0f, 75.0f };
		Phoenix::Math::Vector3 rotate = { 0.0f, 0.0f, 0.0f };
		Phoenix::Math::Vector3 translate = boss->GetPosition();
		translate.y += 100.0f;

		Phoenix::Math::Matrix S, R, T;
		S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
		R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
		T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

		primitiveM = S * R * T;
	}
	primitive->Render(device->GetD3DContext(),
		Phoenix::Math::ConvertToFloat4x4FromVector4x4(primitiveM * camera->GetView() * camera->GetProjection()),
		Phoenix::Math::ConvertToFloat4x4FromVector4x4(primitiveM),
		DirectX::XMFLOAT4(1, 1, 1, 1),
		DirectX::XMFLOAT4(0.0f, 0.6f, 0.0f, 0.5f),
		false);
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
		/*Phoenix::Graphics::DirLight* dir = static_cast<Phoenix::FrameWork::StandardShader*>(standardShader)->GetLight()->GetDefaultDirLight();
		if (ImGui::TreeNode("Light"))
		{
			ImGui::DragFloat3("dir", &dir->direction.x, 0.01f, -1.0f, 1.0f);
			ImGui::TreePop();
		}*/
	}
	ImGui::End();
}