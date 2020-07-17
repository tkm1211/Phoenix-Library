#include "Scene.h"
#include "SceneSystem.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_win32.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_dx11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_internal.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"
#include "Phoenix/Math/PhoenixMath.h"


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
	isHitCollision = false;

	Phoenix::Graphics::DeviceDX11* device = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());
	primitive = std::make_shared<GeometricPrimitive>(device->GetD3DDevice(), 1);

	// エフェクトの読込
	//auto effect = Effekseer::Effect::Create(manager, EFK_EXAMPLE_ASSETS_DIR_U16 "Laser01.efk");
	//effect = Effekseer::Effect::Create(commonData->manager, u"D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Effect\\Examples\\Resources\\Laser01.efk");
	hitEffect = Effekseer::Effect::Create(commonData->manager, u"D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Effect\\Examples\\MAGICALxSPIRAL\\HitEffect06.efk");

	// エフェクトの再生
	hitEffectHandle = commonData->manager->Play(hitEffect, 0, 0, 0);
	commonData->manager->SetPaused(hitEffectHandle, false);
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
		Phoenix::Math::Vector3 playerPos = player->GetPosition();
		Phoenix::Math::Vector3 bossPos = boss->GetPosition();
		/*Phoenix::Math::Vector3 playerPos = player->GetCollisionDatas().at(0).pos;
		Phoenix::Math::Vector3 bossPos = boss->GetCollisionDatas().at(0).pos;
		Phoenix::f32 playerRadius = player->GetCollisionDatas().at(0).radius;
		Phoenix::f32 bossRadius = boss->GetCollisionDatas().at(0).radius;*/
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
		/*if (SphereVsSphere(playerPos, bossPos, playerRadius, bossRadius))
		{
			Phoenix::Math::Vector3 pos;
			Phoenix::Math::Vector3 dir = playerPos - bossPos;
			dir = Phoenix::Math::Vector3Normalize(dir);
			dir.y = 0.0f;

			pos = Phoenix::Math::Vector3(bossPos.x, player->GetPosition().y, bossPos.z) + dir * (playerRadius + bossRadius);
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

		if (player->IsAttackJudgment())
		{
			std::vector<Phoenix::FrameWork::CollisionData>* playerDatas = player->GetCollisionDatas();
			std::vector<Phoenix::FrameWork::CollisionData>* bossDatas = boss->GetCollisionDatas();
			if (SphereVsSphere(playerDatas->at(player->GetAttackCollisionIndex()).pos, bossDatas->at(0).pos, playerDatas->at(player->GetAttackCollisionIndex()).radius, bossDatas->at(0).radius))
			{
				Phoenix::Math::Vector3 pos;
				Phoenix::Math::Vector3 dir = Phoenix::Math::Vector3Normalize(playerDatas->at(player->GetAttackCollisionIndex()).pos - bossDatas->at(0).pos);
				pos = bossDatas->at(0).pos + dir * bossDatas->at(0).radius / 2.0f;

				//hitEffectHandle = commonData->manager->Play(hitEffect, 0,0,0);
				//hitEffectHandle = commonData->manager->Play(hitEffect, pos.x, pos.y, pos.z);
				hitEffectHandle = commonData->manager->Play(hitEffect, { pos.x, pos.y, pos.z }, 20);
				commonData->manager->SetScale(hitEffectHandle, 50.0f, 50.0f, 50.0f);
				player->SetIsHit(true);
				boss->Damage(10);
			}
		}

		if (boss->IsAttackJudgment())
		{
			std::vector<Phoenix::FrameWork::CollisionData>* playerDatas = player->GetCollisionDatas();
			std::vector<Phoenix::FrameWork::CollisionData>* bossDatas = boss->GetCollisionDatas();
			if (SphereVsSphere(playerDatas->at(0).pos, bossDatas->at(boss->GetAttackCollisionIndex()).pos, playerDatas->at(0).radius, bossDatas->at(boss->GetAttackCollisionIndex()).radius))
			{
				boss->SetIsHit(true);
				player->Damage(10);
			}
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

	// エフェクト更新
	{
		// 投影行列の更新 + カメラ行列の更新
		Phoenix::Math::Matrix projection = camera->GetProjection();
		Phoenix::Math::Matrix view = camera->GetView();

		::Effekseer::Matrix44 projectionMat;
		::Effekseer::Matrix44 viewMat;

		for (Phoenix::u32 i = 0; i < 4; ++i)
		{
			for (Phoenix::u32 j = 0; j < 4; ++j)
			{
				projectionMat.Values[i][j] = projection.m[i][j];
				viewMat.Values[i][j] = view.m[i][j];
			}
		}
		commonData->renderer->SetProjectionMatrix(projectionMat);
		commonData->renderer->SetCameraMatrix(viewMat);

		// 3Dサウンド用リスナー設定の更新
		//sound->SetListener(リスナー位置, 注目点, 上方向ベクトル);

		// 再生中のエフェクトの移動等(::Effekseer::Manager経由で様々なパラメーターが設定できます。)
		//commonData->manager->AddLocation(handle, ::Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

		// 全てのエフェクトの更新
		commonData->manager->Flip();
		commonData->manager->Update();
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

#if 1
	basicShader->Begin(graphicsDevice, *camera);
	basicShader->Draw(graphicsDevice, W, stageModel);
	basicShader->End(graphicsDevice);
#else
	standardShader->Begin(graphicsDevice, camera);
	standardShader->Draw(graphicsDevice, W, stageModel);
	standardShader->End(graphicsDevice);
#endif

	// メッシュ描画
#if 1
	basicSkinShader->Begin(graphicsDevice, *camera);
	basicSkinShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
	// エフェクト描画
	{
		commonData->renderer->BeginRendering();
		commonData->manager->Draw();
		commonData->renderer->EndRendering();
	}
	basicSkinShader->Draw(graphicsDevice, player->GetWorldMatrix(), player->GetModel());
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

	if (isHitCollision)
	{
		Phoenix::Graphics::DeviceDX11* device = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());

		for (auto data : *player->GetCollisionDatas())
		{
			PrimitiveRender(device, data.pos, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), Phoenix::Math::Vector3(data.radius, data.radius, data.radius));
		}

		for (auto data : *boss->GetCollisionDatas())
		{
			PrimitiveRender(device, data.pos, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), Phoenix::Math::Vector3(data.radius, data.radius, data.radius));
		}
	}

	//// ワールド行列を作成
	//Phoenix::Math::Matrix primitiveM;
	//{
	//	Phoenix::Math::Vector3 scale = { 75.0f, 75.0f, 75.0f };
	//	Phoenix::Math::Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	//	Phoenix::Math::Vector3 translate = boss->GetPosition();
	//	translate.y += 100.0f;

	//	Phoenix::Math::Matrix S, R, T;
	//	S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
	//	R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
	//	T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

	//	primitiveM = S * R * T;
	//}
	//primitive->Render(device->GetD3DContext(),
	//	Phoenix::Math::ConvertToFloat4x4FromVector4x4(primitiveM * camera->GetView() * camera->GetProjection()),
	//	Phoenix::Math::ConvertToFloat4x4FromVector4x4(primitiveM),
	//	DirectX::XMFLOAT4(1, 1, 1, 1),
	//	DirectX::XMFLOAT4(0.0f, 0.6f, 0.0f, 0.5f),
	//	false);
}

void SceneGame::PrimitiveRender(Phoenix::Graphics::DeviceDX11* device, Phoenix::Math::Vector3 translate, Phoenix::Math::Vector3 rotate, Phoenix::Math::Vector3 scale)
{
	// ワールド行列を作成
	Phoenix::Math::Matrix W;
	{
		Phoenix::Math::Matrix S, R, T;
		S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
		R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
		T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

		W = S * R * T;
	}
	primitive->Render(device->GetD3DContext(),
		Phoenix::Math::ConvertToFloat4x4FromVector4x4(W * camera->GetView() * camera->GetProjection()),
		Phoenix::Math::ConvertToFloat4x4FromVector4x4(W),
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
		if (ImGui::TreeNode("Collision"))
		{
			ImGui::Checkbox("On", &isHitCollision);
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