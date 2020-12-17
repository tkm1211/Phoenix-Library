#include "Scene.h"
#include "SceneSystem.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_win32.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_dx11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_internal.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/FrameWork/Shader/SkyMapShader.h"
#include "Phoenix/FND/Util.h"
#include "../Source/Graphics/Texture/Win/DirectX11/TextureDX11.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/Enemy.h"
#include "../AI/MetaAI/MetaType.h"


void SceneTutorial::Construct(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	display = sceneSystem->GetDisplay();
	graphicsDevice = sceneSystem->GetGraphicsDevice();
	commonData = sceneSystem->GetSceneCommonData();

	// デバック用
	{
		stageScale = Phoenix::Math::Vector3(5.5f, 50.0, 94.0f);

		Phoenix::Graphics::DeviceDX11* device = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());
		primitive = std::make_shared<GeometricPrimitive>(device->GetD3DDevice(), 1);
		boxPrimitive = std::make_shared<GeometricPrimitive>(device->GetD3DDevice(), 0, false, &ConvertToFloat3FromVector3(stageScale));
	}

	// 共通データのアドレス取得
	{
		player = commonData->player.get();
		enemyManager = commonData->enemyManager.get();
		mannequin = commonData->mannequin.get();
		metaAI = commonData->metaAI.get();
		uiSystem = commonData->uiSystem.get();
		stageModel = commonData->stageModel.get();
		bossStageModel = commonData->bossStageModel.get();
		basicShader = commonData->basicShader.get();
		basicSkinShader = commonData->basicSkinShader.get();
		standardShader = commonData->standardShader.get();
		pbrShader = commonData->pbrShader.get();
		pbrSkinShader = commonData->pbrSkinShader.get();
		camera = commonData->camera.get();
		targetMark = commonData->targetMark.get();
		targetMarkUI = commonData->targetMarkUI.get();
	}

	// フレームバッファ
	{
		frameBuffer[0] = Phoenix::FrameWork::FrameBuffer::Create();
		frameBuffer[1] = Phoenix::FrameWork::FrameBuffer::Create();
		frameBuffer[2] = Phoenix::FrameWork::FrameBuffer::Create();

		frameBuffer[0]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), enableMSAA, 8, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
		frameBuffer[1]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
		frameBuffer[2]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::UNKNOWN);
	}

	// ポストプロセス
	{
		postProcessingEffects = Phoenix::FrameWork::PostProcessingEffects::Create();
		postProcessingEffects->Initialize(graphicsDevice);
	}

	// シャドウマップ
	{
		shadowMap = Phoenix::FrameWork::FrameBuffer::Create();
		shadowMap->Initialize(graphicsDevice, 1024 * 5, 1024 * 5, false, 1, Phoenix::Graphics::TextureFormatDx::UNKNOWN, Phoenix::Graphics::TextureFormatDx::R32_TYPELESS);

		voidPS = Phoenix::Graphics::IShader::Create();
		//voidPS->LoadPS(graphicsDevice->GetDevice(), "ShadowMapPS.cso");
		lightSpaceCamera = std::make_unique<Phoenix::Graphics::Camera>();

		// 定数バッファ作成
		{
			Phoenix::Graphics::PhoenixBufferDesc bufferDesc = {};
			Phoenix::FND::MemSet(&bufferDesc, 0, sizeof(bufferDesc));
			bufferDesc.usage = Phoenix::Graphics::PhoenixUsage::Dynamic;
			bufferDesc.bindFlags = static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixBindFlag::ConstantBuffer);
			bufferDesc.cpuAccessFlags = static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixCPUAccessFlag::CPUAccessWrite);
			bufferDesc.miscFlags = 0;
			bufferDesc.byteWidth = sizeof(ShaderConstants);
			bufferDesc.structureByteStride = 0;

			shaderConstantsBuffer = Phoenix::Graphics::IBuffer::Create();
			shaderConstantsBuffer->Initialize(graphicsDevice->GetDevice(), bufferDesc);
		}

		// サンプラー作成
		{
			comparisonSamplerState = Phoenix::Graphics::ISampler::Create();
			comparisonSamplerState->Initialize(graphicsDevice->GetDevice(), Phoenix::Graphics::SamplerState::LinearBorder, false, true);
		}
	}

	// ブルーム
	{
		quad = Phoenix::FrameWork::Quad::Create();
		quad->Initialize(graphicsDevice, Phoenix::Graphics::SamplerState::PointBorder);

		msaaResolve = Phoenix::FrameWork::MSAAResolve::Create();
		msaaResolve->Initialize(graphicsDevice);

		bloom = Phoenix::FrameWork::Bloom::Create();
		bloom->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight());
	}

	// スカイマップ
	{
		skyMap = Phoenix::FrameWork::SkyMap::Create();
		skyMap->Initialize(graphicsDevice, "..\\Data\\Assets\\Texture\\SkyMap\\AllSkyFree\\Epic_BlueSunset\\Epic_BlueSunset03.dds"); //Epic_BlueSunset\\Epic_BlueSunset03 //Night MoonBurst\\NightMoonBurst
	}

	// トーンマップ
	{
		toneMap = Phoenix::FrameWork::ToneMap::Create();
		toneMap->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight());
	}

	// IBL
	{
		/*for (int i = 0; i < 6; ++i)
		{
			skyFrameBuffer[i] = Phoenix::FrameWork::FrameBuffer::Create();
			skyFrameBuffer[i]->Initialize(graphicsDevice, 64, 64, false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
		}

		ibl = Phoenix::FrameWork::IBL::Create();
		ibl->Initialize(graphicsDevice);*/
	}

	// GPUパーティクル
	{
		testComputeShader = Phoenix::FrameWork::TestComputeShader::Create();
		bitonicSort = Phoenix::FrameWork::BitonicSort::Create();
		gpuParticle = Phoenix::FrameWork::GPUParticle::Create();
		playerHitParticle = Phoenix::FrameWork::GPUParticle::Create();
		bossHitParticle = Phoenix::FrameWork::GPUParticle::Create();
		petalParticle = Phoenix::FrameWork::GPUParticle::Create();
		soilParticle = Phoenix::FrameWork::GPUParticle::Create();
		dusterParticle = Phoenix::FrameWork::GPUParticle::Create();
	}

	{
		dissolveCB = Phoenix::Graphics::IBuffer::Create();
		{
			Phoenix::Graphics::PhoenixBufferDesc desc = {};
			Phoenix::FND::MemSet(&desc, 0, sizeof(desc));
			desc.usage = Phoenix::Graphics::PhoenixUsage::Default;
			desc.bindFlags = static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixBindFlag::ConstantBuffer);
			desc.cpuAccessFlags = 0;
			desc.miscFlags = 0;
			desc.byteWidth = sizeof(DissolveCB);
			desc.structureByteStride = 0;
			if (!dissolveCB->Initialize(graphicsDevice->GetDevice(), desc))
			{
				return;
			}
		}

		embeddedDissolvePixelShader = Phoenix::Graphics::IShader::Create();
		embeddedDissolvePixelShader->LoadPS
		(
			graphicsDevice->GetDevice(),
			"BasicMaskPS.cso"
		);

		dissolveTexture = Phoenix::Graphics::ITexture::Create();
		dissolveTexture->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\Mask\\Dissolve\\dissolve_animation2.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color::White);
	}
}

void SceneTutorial::Initialize()
{
	// デバック用
	{
		texSize = Phoenix::Math::Vector2(256.0f, 256.0f);

		cameraFlg = false;
		lockOnCamera = false;
		isHitCollision = false;
		isUpdate = true;
		isPlayerUpdate = true;
		isBossUpdate = true;
		enableMSAA = false;
		shadowBlend = false;
		bloomBlend = true;
		isPBR = true;

		for (int i = 0; i < 10; ++i)
		{
			active[i] = false;
		}
	}

	// 共通データの初期化
	{
		player->Initialize();
		enemyManager->Initialize();
		mannequin->Initialize();

		player->SetPosition(Phoenix::Math::Vector3(0.0f, 0.0f, 75.0f));

		camera->SetEye(Phoenix::Math::Vector3(0.0f, 0.0f, 10.0f));
		camera->SetRotateX(0.5f);
		camera->SetRotateY(0.0f);

		currentShader = pbrShader;

		tempCameraFouce = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		sphereLinearSpeed = 0.0f;
		distanceToFouceFromCamera = 0.0f;

		isCameraShake = false;
		shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		shakeWidth = 0.0f;
		shakeHeight = 0.0f;
		cameraShakeCnt = 0;
		cameraShakeMaxCnt = 0;

		playerBehaviorScore = 0;
		oldPlayerBehaviorScore = 0;

		Phoenix::FrameWork::Transform transform;
		transform.SetTranslate({ 0,0,65.0f });
		transform.SetRotate({ 0,0,0,1 });
		transform.SetScale({ 1.0f,1.0f,1.0f });
		enemyManager->AddEnemy(transform);
		enemyManager->SetBattleEnemy(0);
	}

	// GPUパーティクル
	{
		testComputeShader->Initialize(graphicsDevice);
		bitonicSort->Initialize(graphicsDevice);
		gpuParticle->Initialize(graphicsDevice, "SimulateCS.cso", "..\\Data\\Assets\\Texture\\Effect\\Fire\\FireOrigin.png"); // particle
		playerHitParticle->Initialize(graphicsDevice, "SimulateCS.cso", "..\\Data\\Assets\\Texture\\Effect\\Fire\\FireOrigin02.png", true); // PlayerHitEffectCS
		bossHitParticle->Initialize(graphicsDevice, "SimulateCS.cso", "..\\Data\\Assets\\Texture\\Effect\\Fire\\FireOrigin02.png", true); // PlayerHitEffectCS
		petalParticle->Initialize(graphicsDevice, "PetalEffectCS.cso", "..\\Data\\Assets\\Texture\\Effect\\JumpAttack\\Petal01.png", false);
		soilParticle->Initialize(graphicsDevice, "SoilEffectCS.cso", "..\\Data\\Assets\\Texture\\Effect\\JumpAttack\\Soil01.png", false);
		//dusterParticle->Initialize(graphicsDevice, "DusterEffectCS.cso", "..\\Data\\Assets\\Texture\\Effect\\Duster\\Duster02.png");
	}
}

void SceneTutorial::Update(Phoenix::f32 elapsedTime)
{
	bool onFade = sceneSystem->GetOnFade();
	bool onControl = player->GetAlive();

	// プレイヤー更新
	Phoenix::Math::Vector3 oldPlayerPos = player->GetPosition();
	if (isUpdate && !isHitStop)
	{
		player->Update(*camera, !onFade && isPlayerUpdate, elapsedTime);
	}

	// エネミーマネージャー更新
	if (isUpdate)
	{
		enemyManager->Update(onControl);
	}

	// ボス更新
	if (isUpdate && !isHitStop)
	{
		mannequin->Update(*camera, true);
	}

	if (isUpdate)
	{
		metaAI->Update();
	}

	// ゲームジャッジ
	if (!onFade)
	{
		if (player->GetPosition().z <= 25.0f)
		{
			sceneSystem->ChangeScene(SceneType::Game, false, true);
		}

		/*if (boss->GetHP() <= 0)
		{
			sceneSystem->ChangeScene(SceneType::GameClear, false, true);
		}*/
	}

	// 当たり判定
	{
		JudgeHit();
	}

	// カメラ更新
	{
		UpdateCamera();
	}

	// UI更新
	{
		player->UpdateUI();
		//boss->UpdateUI();
	}

	Phoenix::FrameWork::LightState* light = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetLight();
	light->direction = Phoenix::Math::Vector4(-camera->GetFront(), 1.0f);

	// フェード中
	if (sceneSystem->GetOnFade()) return;

	if (isPBR)
	{
		currentShader = pbrShader;
	}
	else
	{
		currentShader = basicSkinShader;
	}

	if (isUpdate)
	{
		//gpuParticle->Burst(10);
		//gpuParticle->UpdateCPU(graphicsDevice, particlePos, 1.0f / 60.0f);
		//gpuParticle->UpdateGPU(graphicsDevice, Phoenix::Math::MatrixIdentity(), 1.0f / 60.0f);

		playerHitParticle->UpdateCPU(graphicsDevice, particlePos, 1.0f / 60.0f);
		playerHitParticle->UpdateGPU(graphicsDevice, Phoenix::Math::MatrixIdentity(), 1.0f / 60.0f);

		bossHitParticle->UpdateCPU(graphicsDevice, bossHitParticlePos, 1.0f / 60.0f);
		bossHitParticle->UpdateGPU(graphicsDevice, Phoenix::Math::MatrixIdentity(), 1.0f / 60.0f);

		petalParticle->UpdateCPU(graphicsDevice, jumpAttackParticlePos, 1.0f / 60.0f);
		petalParticle->UpdateGPU(graphicsDevice, Phoenix::Math::MatrixIdentity(), 1.0f / 60.0f);

		soilParticle->UpdateCPU(graphicsDevice, jumpAttackParticlePos, 1.0f / 60.0f);
		soilParticle->UpdateGPU(graphicsDevice, Phoenix::Math::MatrixIdentity(), 1.0f / 60.0f);

		//dusterParticle->UpdateCPU(graphicsDevice, particlePos, 1.0f / 60.0f);
		//dusterParticle->UpdateGPU(graphicsDevice, Phoenix::Math::MatrixIdentity(), 1.0f / 60.0f);
	}

	if (isHitStop)
	{
		if (hitStopMaxCnt <= hitStopCnt++)
		{
			isHitStop = false;
			hitStopCnt = 0;
		}
	}
	if (isCameraShake)
	{
		Phoenix::Math::Vector3 right = camera->GetRight();
		Phoenix::f32 len = static_cast<Phoenix::f32>(cameraShakeCnt) / static_cast<Phoenix::f32>(cameraShakeMaxCnt);
		//shakeHeight *= cameraShakeCnt % 5 == 0 ? -1.0f : 1.0f;

		//shake = right * (shakeWidth * (1.0f - len));
		//shake.y = shakeHeight * len;

		shake = right * (cosf(450.0f * len * 0.01745f) * (shakeWidth * (1.0f - len)));
		shake.y = cosf(450.0f * len * 0.01745f) * (shakeHeight * (1.0f - len));

		camera->SetCameraShake(shake);

		if (cameraShakeMaxCnt <= cameraShakeCnt++)
		{
			isCameraShake = false;
			cameraShakeCnt = 0;
			shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
			camera->SetCameraShake(shake);
		}
	}

	// サウンド再生
	{
		commonData->bgm->Update();
		commonData->se->Update();
	}

	// UI Update
	/*{
		Phoenix::f32 size = 128.0f / 4.0f;

		Phoenix::Math::Vector3 bossPos = mannequin->GetPosition();
		bossPos.y += 1.5f;

		Phoenix::Math::Vector3 screenPos = WorldToScreen(bossPos);

		targetMarkUI->Update(Phoenix::Math::Vector2(screenPos.x, screenPos.y));
	}*/

	// メタAI通知
	{
		if (oldPlayerBehaviorScore < playerBehaviorScore)
		{
			metaAI->Sensor(static_cast<Phoenix::s32>(MetaType::Battle), playerBehaviorScore);
		}

		// 過去のスコア保存
		oldPlayerBehaviorScore = playerBehaviorScore;

		playerBehaviorScore -= 1;

		if (playerBehaviorScore < 0)
		{
			playerBehaviorScore = 0;
		}
	}
}

void SceneTutorial::JudgeHit()
{
	Phoenix::Math::Vector3 playerPos = player->GetPosition();
	Phoenix::Math::Vector3 mannequinPos = mannequin->GetPosition();
	Phoenix::f32 playerRadius = player->GetRadius();
	Phoenix::f32 mannequinRadius = mannequin->GetRadius();

	// 押し出し
	{
		JudgeExtrude(playerPos, mannequinPos, playerRadius, mannequinRadius);
	}

	// ヒット
	{
		// プレイヤー攻撃
		if (player->IsAttackJudgment().at(0) && !isHitStop)
		{
			const std::vector<Phoenix::FrameWork::CollisionData> playerDatas = player->GetCollisionDatas();
			const std::vector<Phoenix::FrameWork::CollisionData> mannequinDatas = mannequin->GetCollisionDatas();
			if (SphereVsSphere(playerDatas.at(player->GetAttackCollisionIndex()).pos, mannequinDatas.at(0).pos, playerDatas.at(player->GetAttackCollisionIndex()).radius, mannequinDatas.at(0).radius))
			{
				Phoenix::Math::Vector3 pos;
				Phoenix::Math::Vector3 normal;
				Phoenix::Math::Vector3 dir = Phoenix::Math::Vector3Normalize(playerDatas.at(player->GetAttackCollisionIndex()).pos - mannequinDatas.at(0).pos);
				pos = mannequinDatas.at(0).pos + dir * mannequinDatas.at(0).radius / 2.0f;
				normal = Phoenix::Math::Vector3Normalize(playerPos - mannequinPos); // playerDatas->at(player->GetAttackCollisionIndex()).pos

				player->SetIsHit(true);
				//boss->Damage(10);

				playerBehaviorScore += 100;

				// Burst Particle.
				{
					playerHitParticle->Burst(150);
					playerHitParticle->SetParticleLife(1.5f);
					playerHitParticle->SetParticleSize(0.004f);
					playerHitParticle->SetParticleScale(1.0f);
					playerHitParticle->SetParticleNormal(Phoenix::Math::Vector4(normal, 0.0f));
					playerHitParticle->SetParticleColor(Phoenix::Math::Color(245.0f / 255.0f, 69.0f / 255.0f, 33.0f / 255.0f, 1.0f)); // particleMainColor Phoenix::Math::Color(245.0f / 255.0f, 69.0f / 255.0f, 33.0f / 255.0f, 1.0f) // 40.0f / 255.0f, 40.0f / 255.0f, 148.0f / 255.0f, 1.0f
					//playerHitParticle->SetParticleColor(Phoenix::Math::Color(40.0f / 255.0f, 40.0f / 255.0f, 148.0f / 255.0f, 1.0f)); // particleMainColor Phoenix::Math::Color(245.0f / 255.0f, 69.0f / 255.0f, 33.0f / 255.0f, 1.0f) // 40.0f / 255.0f, 40.0f / 255.0f, 148.0f / 255.0f, 1.0f
					//playerHitParticle->SetParticleColor(Phoenix::Math::Color(33.0f / 255.0f, 245.0f / 255.0f, 148.0f / 255.0f, 1.0f)); // particleMainColor Phoenix::Math::Color(245.0f / 255.0f, 69.0f / 255.0f, 33.0f / 255.0f, 1.0f) // 40.0f / 255.0f, 40.0f / 255.0f, 148.0f / 255.0f, 1.0f

					particlePos = playerDatas.at(player->GetAttackCollisionIndex()).pos;
				}

				// Set Point Light
				{
					Phoenix::FrameWork::PointLightState* point = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetPointLight();
					point->position = Phoenix::Math::Vector4(particlePos, 0.0f);
					point->color = Phoenix::Math::Vector4(50.0f, 50.0f, 50.0f, 1.0f);
					point->distance = pointLightDistance;
					point->decay = 1.0f;
					grayScale = point->color.x;
					playerAttackEndCount = 0.0f;
					onPointLight = true;
				}

				// Set Hit Stop
				if (player->GetAttackCollisionIndex() == 3)
				{
					isHitStop = true;
					hitStopCnt = 0;
				}

				// Set Camera Shake
				{
					if (!isCameraShake)
					{
						if (player->GetAttackCollisionIndex() == 1)
						{
							isCameraShake = true;
							shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
							cameraShakeCnt = 0;

							shakeWidth = 0.0f;
							shakeHeight = 0.10f;
							cameraShakeMaxCnt = 7;

							SetXInputVibration(1.0f, 0.0f, 5);
						}
						else if (player->GetAttackCollisionIndex() == 2)
						{
							isCameraShake = true;
							shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
							cameraShakeCnt = 0;

							shakeWidth = 0.10f;
							shakeHeight = 0.0f;
							cameraShakeMaxCnt = 7;

							SetXInputVibration(0.0f, 1.0f, 5);
						}
						else if (player->GetAttackCollisionIndex() == 3)
						{
							isCameraShake = true;
							shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
							cameraShakeCnt = 0;

							shakeWidth = 0.55f;
							shakeHeight = 0.0f;
							cameraShakeMaxCnt = 10;

							SetXInputVibration(1.0f, 1.0f, 10);
						}
					}
				}
			}
		}
		else if (onPointLight && !isHitStop)
		{
			Phoenix::FrameWork::PointLightState* point = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetPointLight();

			if (playerAttackEndMaxCount <= playerAttackEndCount++)
			{
				point->position = Phoenix::Math::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
				point->color = Phoenix::Math::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
				point->distance = 0.0f;
				point->decay = 0.0f;
				playerAttackEndCount = 0.0f;
				onPointLight = false;
			}
			else
			{
				//point->distance = pointLightDistance * ((playerAttackEndMaxCount - playerAttackEndCount) / playerAttackEndMaxCount);

				point->color.x -= grayScale / playerAttackEndMaxCount;
				point->color.y -= grayScale / playerAttackEndMaxCount;
				point->color.z -= grayScale / playerAttackEndMaxCount;
			}
		}

		// エネミー攻撃
		/*if (!player->Invincible() && !isHitStop)
		{
			const std::vector<Phoenix::FrameWork::CollisionData>* playerDatas = player->GetCollisionDatas();
			const std::vector<Phoenix::FrameWork::CollisionData>* mannequinDatas = mannequin->GetCollisionDatas();
			if (!isCameraShake)
			{
				isCameraShake = true;
				shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
				cameraShakeCnt = 0;

				shakeWidth = 0.0f;
				shakeHeight = 0.75f;
				cameraShakeMaxCnt = 20;

				petalParticle->Burst(50);
				petalParticle->SetParticleLife(1.0f);
				petalParticle->SetParticleSize(0.07f);
				petalParticle->SetParticleScale(0.75f);
				petalParticle->SetParticleNormal(Phoenix::Math::Vector4(0.0f, 1.0f, 0.0f, 0.0f));
				petalParticle->SetParticleColor(Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f)); // particleMainColor

				soilParticle->Burst(50);
				soilParticle->SetParticleLife(2.0f);
				soilParticle->SetParticleSize(0.07f);
				soilParticle->SetParticleScale(0.75f);
				soilParticle->SetParticleNormal(Phoenix::Math::Vector4(0.0f, 1.0f, 0.0f, 0.0f));
				soilParticle->SetParticleColor(Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f)); // particleMainColor

				jumpAttackParticlePos = bossPos;

				SetXInputVibration(1.0f, 1.0f, 30);
			}
			if (SphereVsSphere(playerDatas->at(0).pos, bossDatas->at(boss->GetAttackCollisionIndex()).pos, playerDatas->at(0).radius, bossDatas->at(boss->GetAttackCollisionIndex()).radius))
			{
				Phoenix::Math::Vector3 normal;
				normal = Phoenix::Math::Vector3Normalize(playerPos - bossPos); // playerDatas->at(player->GetAttackCollisionIndex()).pos

				boss->SetIsHit(true);
				player->Damage(10);

				// Burst Particle.
				if (!boss->IsJumpAttack())
				{
					bossHitParticle->Burst(100);
					bossHitParticle->SetParticleLife(1.0f);
					bossHitParticle->SetParticleSize(0.07f);
					bossHitParticle->SetParticleScale(0.25f);
					bossHitParticle->SetParticleNormal(Phoenix::Math::Vector4(normal, 0.0f));
					bossHitParticle->SetParticleColor(Phoenix::Math::Color(33.0f / 255.0f, 245.0f / 255.0f, 148.0f / 255.0f, 1.0f)); // particleMainColor

					bossHitParticlePos = bossPos + ((playerPos - bossPos) * 0.5f);
					bossHitParticlePos.y = bossDatas->at(boss->GetAttackCollisionIndex()).pos.y;
				}
			}
		}*/
	}

	playerPos = player->GetPosition();
	Phoenix::f32 playerDis = Phoenix::Math::Vector2Length(Phoenix::Math::Vector2(playerPos.x, playerPos.z));
	Phoenix::Math::Vector2 playerNormal = Phoenix::Math::Vector2Normalize(Phoenix::Math::Vector2(playerPos.x, playerPos.z));

	if (playerPos.x <= -stageScale.x)
	{
		playerPos.x = -stageScale.x;
	}
	else if (stageScale.x <= playerPos.x)
	{
		playerPos.x = stageScale.x;
	}
	if (playerPos.z <= -stageScale.z)
	{
		playerPos.z = -stageScale.z;
	}
	else if (stageScale.z <= playerPos.z)
	{
		playerPos.z = stageScale.z;
	}

	player->SetPosition(playerPos);
}

void SceneTutorial::JudgeExtrude(Phoenix::Math::Vector3 playerPos, Phoenix::Math::Vector3 enemyPos, Phoenix::f32 playerRadius, Phoenix::f32 enemyRadius)
{
	if (SphereVsSphere(Phoenix::Math::Vector3(playerPos.x, playerPos.y + 0.5f, playerPos.z), Phoenix::Math::Vector3(enemyPos.x, enemyPos.y + 0.5f, enemyPos.z), playerRadius, enemyRadius))
	{
		Phoenix::Math::Vector3 pos;
		Phoenix::Math::Vector3 dir = playerPos - enemyPos;
		dir = Phoenix::Math::Vector3Normalize(dir);
		dir.y = 0.0f;

		pos = Phoenix::Math::Vector3(enemyPos.x, playerPos.y, enemyPos.z) + dir * (playerRadius + enemyRadius);
		player->SetPosition(pos);
		player->UpdateTrasform();
	}
}

void SceneTutorial::UpdateCamera()
{
	if (xInput[0].bLBt && !isHitStop)
	{
#if 0
		lockOnCamera = !lockOnCamera;
#else
		camera->SetTargetPos(mannequin->GetPosition(), Phoenix::Math::Vector3(0.0f, 1.25f, 0.0f));
		targetMarkUI->LockOnTarget();
#endif
	}

	if (cameraFlg && !isHitStop)
	{
		static bool isInit = false;
		if (!isInit)
		{
			camera->SetRotateX(0.0f);
			camera->SetRotateY(-3.14f);
			isInit = true;
		}
		camera->FreeCamera();
	}
	else
	{
		//inTerritory = false;

		/*for (const auto& enemy : enemyManager->GetEnemies())
		{
			if (enemy->GetAlive())
			{
				UpdateGameCamera(player->GetPosition(), enemy->GetTransform().GetTranslate());
			}
		}*/

		UpdateGameCamera(player->GetPosition(), mannequin->GetPosition());
	}
	//camera->Update();
}

void SceneTutorial::UpdateGameCamera(Phoenix::Math::Vector3 playerPos, Phoenix::Math::Vector3 enemyPos)
{
	Phoenix::Math::Vector3 enemyToPlayerVec = enemyPos - playerPos;
	Phoenix::f32 len = Phoenix::Math::Vector3Length(enemyToPlayerVec);

	static Phoenix::f32 lerp = 1.0f;
	static Phoenix::f32 cameraLen = 6.0f;
	if (len <= 5.0f)
	{
		cameraLen = Phoenix::Math::f32Lerp(cameraLen, 8.5f, 0.05f);

		enemyToPlayerVec = Phoenix::Math::Vector3Normalize(enemyToPlayerVec);
		camera->ControllerCamera02(playerPos + enemyToPlayerVec * (len * 0.5f), Phoenix::Math::Vector3(0.0f, 1.25f, 0.0f), cameraLen, 0.05f);
		player->InEnemyTerritory(true);
		player->SetTargetPos(mannequin->GetPosition());
		lerp = 0.01f;

		inTerritory = true;
	}
	else/* if (!inTerritory)*/
	{
		lerp = Phoenix::Math::f32Lerp(lerp, 1.0f, 0.01f);
		cameraLen = Phoenix::Math::f32Lerp(cameraLen, 6.0f, 0.05f);

		camera->ControllerCamera02(player->GetPosition(), Phoenix::Math::Vector3(0.0f, 1.5f, 0.0f), cameraLen, lerp);
		player->InEnemyTerritory(false);
		player->SetTargetPos(Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f));
	}
}

void SceneTutorial::Draw(Phoenix::f32 elapsedTime)
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	Phoenix::Graphics::Viewport* v = new Phoenix::Graphics::Viewport();
	context->GetViewports(1, &v);
	float aspectRatio = v->width / v->height;
	Phoenix::FND::SafeDelete(v);

#if 1
	// Generate ShadowMap
	{
		shadowMap->Clear(graphicsDevice, 0, 1.0f, 1.0f, 1.0f, 1.0f);
		shadowMap->Activate(graphicsDevice);
		{
			float distance = dis;
			Phoenix::FrameWork::LightState* light = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetSunLight();
			Phoenix::Math::Vector4 L = Phoenix::Math::Vector4Normalize(-light->direction);
			Phoenix::Math::Vector4 P = Phoenix::Math::Vector4(player->GetPosition(), 1.0f);
			Phoenix::Math::Vector4 Eye = P - distance * L;

			lightSpaceCamera->SetEye(Phoenix::Math::Vector3(Eye.x, Eye.y, Eye.z));
			lightSpaceCamera->SetFocus(Phoenix::Math::Vector3(P.x, P.y, P.z));

			Phoenix::Math::Vector3 up, right;
			{
				right = Phoenix::Math::Vector3Cross(Phoenix::Math::Vector3(L.x, L.y, L.z), Phoenix::Math::Vector3::OneY);
				right = Phoenix::Math::Vector3Normalize(right);

				up = Phoenix::Math::Vector3Cross(right, Phoenix::Math::Vector3(L.x, L.y, L.z));
				up = Phoenix::Math::Vector3Normalize(up);

				right = Phoenix::Math::Vector3Cross(Phoenix::Math::Vector3(L.x, L.y, L.z), Phoenix::Math::Vector3::OneY);
				up = Phoenix::Math::Vector3Cross(right, Phoenix::Math::Vector3(L.x, L.y, L.z));
			}

			Phoenix::Math::Matrix projection = Phoenix::Math::MatrixOrtho(width, height /*/ aspectRatio*/, nearZ, farZ);
			lightSpaceCamera->SetProjection(projection);
			lightSpaceCamera->SetLookAt(lightSpaceCamera->GetEye(), lightSpaceCamera->GetFocus(), up);

			// Draw player and boss.
			{
				if (currentShader)
				{
					currentShader->Begin(graphicsDevice, *lightSpaceCamera);
					voidPS->ActivatePS(graphicsDevice->GetDevice());
					{
						currentShader->Draw(graphicsDevice, mannequin->GetWorldMatrix(), mannequin->GetModel());
						currentShader->Draw(graphicsDevice, player->GetWorldMatrix(), player->GetModel());
						/*for (auto enemy : enemyManager->GetEnemies())
						{
							if (enemy->GetAlive())
							{
								currentShader->Draw(graphicsDevice, enemy->GetWorldMatrix(), enemy->GetModel());
							}
						}*/
					}
					voidPS->DeactivatePS(graphicsDevice->GetDevice());
					currentShader->End(graphicsDevice);
				}
			}

			// Draw stage.
			{
				// ワールド行列を作成
				Phoenix::Math::Matrix W;
				{
					Phoenix::Math::Vector3 scale = { 40.0f, 40.0f, 40.0f }; // TODO : 修正
					Phoenix::Math::Vector3 rotate = { 0.0f, 0.0f, 0.0f };
					Phoenix::Math::Vector3 translate = { 0.0f, 0.0f, 0.0f };

					Phoenix::Math::Matrix S, R, T;
					S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
					R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
					T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

					W = S * R * T;
				}

				basicShader->Begin(graphicsDevice, *lightSpaceCamera);
				voidPS->ActivatePS(graphicsDevice->GetDevice());
				{
					basicShader->Draw(graphicsDevice, W, stageModel);
				}
				voidPS->DeactivatePS(graphicsDevice->GetDevice());
				basicShader->End(graphicsDevice);
			}
		}
		shadowMap->Deactivate(graphicsDevice);
	}

	// Work No_0 framebuffer.
	{
		frameBuffer[0]->Clear(graphicsDevice, 0, 0.5f, 0.5f, 0.5f, 1.0f);
		frameBuffer[0]->Activate(graphicsDevice);
		{
			// Set Shadow Data.
			{
				shaderContexts.lightViewProjection = Phoenix::Math::MatrixTranspose(/*Phoenix::Math::MatrixInverse*/(lightSpaceCamera->GetView() * lightSpaceCamera->GetProjection()));
				Phoenix::Graphics::PhoenixMap map = Phoenix::Graphics::PhoenixMap::WriteDiscard;
				Phoenix::Graphics::PhoenixMappedSubresource mapedBuffer;
				{
					context->Map(shaderConstantsBuffer.get(), 0, map, 0, &mapedBuffer);
					Phoenix::FND::MemCpy(mapedBuffer.data, &shaderContexts, sizeof(ShaderConstants));
					context->Unmap(shaderConstantsBuffer.get(), 0);
				}
				Phoenix::Graphics::IBuffer* psCBuffer[] =
				{
					shaderConstantsBuffer.get()
				};
				context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Pixel, 3, Phoenix::FND::ArraySize(psCBuffer), psCBuffer);

				Phoenix::Graphics::ISampler* samplers[] =
				{
					comparisonSamplerState.get()
				};
				context->SetSamplers(Phoenix::Graphics::ShaderType::Pixel, 3, 1, samplers);

				Phoenix::Graphics::ITexture* texture[] = { shadowMap->GetDepthStencilSurface()->GetTexture() };
				graphicsDevice->GetContext()->SetShaderResources(Phoenix::Graphics::ShaderType::Pixel, 8, 1, texture);
			}

			// Draw skymap.
			{
				Phoenix::FrameWork::LightState* light = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetLight();
				Phoenix::Math::Color color = { 1.0f, 1.0f, 1.0f, 1.0f };
				float skyDimension = 50000;
				// ワールド行列を作成
				Phoenix::Math::Matrix skyWorldM;
				{
					Phoenix::Math::Vector3 scale = { skyDimension, skyDimension, skyDimension };
					Phoenix::Math::Vector3 rotate = { 0.0f, 90.0f * 0.01745f, 0.0f };
					Phoenix::Math::Vector3 translate = { 0.0f, 0.0f, 0.0f };

					Phoenix::Math::Matrix S, R, T;
					S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
					R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
					T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

					skyWorldM = S * R * T;
				}
				skyMap->Draw(graphicsDevice, skyWorldM, *camera, light->direction, color);
			}

			// Draw stage.
			{
				// ワールド行列を作成
				Phoenix::Math::Matrix W;
				{
					Phoenix::Math::Vector3 scale = { 1.0f, 1.0f, 1.0f };
					Phoenix::Math::Vector3 rotate = { 0.0f, 0.0f, 0.0f };
					Phoenix::Math::Vector3 translate = { 0.0f, -22.10f, 0.0f };

					Phoenix::Math::Matrix S, R, T;
					S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
					R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
					T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

					W = S * R * T;
				}
#if 0
				basicShader->Begin(graphicsDevice, *camera);
				basicShader->Draw(graphicsDevice, W, bossStageModel);
				basicShader->End(graphicsDevice);
#elif 0
				standardShader->Begin(graphicsDevice, camera);
				standardShader->Draw(graphicsDevice, W, bossStageModel);
				standardShader->End(graphicsDevice);
#else
				pbrSkinShader->Begin(graphicsDevice, *camera);
				pbrSkinShader->Draw(graphicsDevice, W, stageModel);
				pbrSkinShader->End(graphicsDevice);
#endif
			}

			// Draw player and boss.
			{
				if (currentShader)
				{
					Phoenix::FrameWork::MaterialState* material = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetMaterial();
					Phoenix::f32 metallic  = material->metallic;
					Phoenix::f32 roughness = material->roughness;

					material->metallic = 1.0f;
					material->roughness = 1.0f;

					currentShader->Begin(graphicsDevice, *camera);
					currentShader->Draw(graphicsDevice, mannequin->GetWorldMatrix(), mannequin->GetModel());
					currentShader->End(graphicsDevice);

					/*currentShader->Begin(graphicsDevice, *camera);
					for (auto enemy : enemyManager->GetEnemies())
					{
						if (enemy->GetAlive())
						{
							currentShader->Draw(graphicsDevice, enemy->GetWorldMatrix(), enemy->GetModel());
						}
					}
					currentShader->End(graphicsDevice);*/

					// Draw Effect.
					{
						Phoenix::Graphics::ContextDX11* contextDX11 = static_cast<Phoenix::Graphics::ContextDX11*>(context);
						Phoenix::f32 blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
						context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaBlend), blendFactor, 0xFFFFFFFF);
						{
							gpuParticle->Draw(graphicsDevice, *camera);
							playerHitParticle->Draw(graphicsDevice, *camera);
							bossHitParticle->Draw(graphicsDevice, *camera);
							petalParticle->Draw(graphicsDevice, *camera);
							soilParticle->Draw(graphicsDevice, *camera);
							//dusterParticle->Draw(graphicsDevice, *camera);
						}
						context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
					}

					material->metallic = metallic;
					material->roughness = roughness;

					currentShader->Begin(graphicsDevice, *camera);
					currentShader->Draw(graphicsDevice, player->GetWorldMatrix(), player->GetModel());
					currentShader->End(graphicsDevice);
				}
			}

			// Draw collision primitive.
			if (isHitCollision)
			{
				Phoenix::Graphics::DeviceDX11* device = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());

				Phoenix::Graphics::ContextDX11* contextDX11 = static_cast<Phoenix::Graphics::ContextDX11*>(context);
				context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::Opaque), 0, 0xFFFFFFFF);

				for (const auto data : player->GetCollisionDatas())
				{
					PrimitiveRender(device, data.pos, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), Phoenix::Math::Vector3(data.radius, data.radius, data.radius));
				}

				for (const auto data : mannequin->GetCollisionDatas())
				{
					PrimitiveRender(device, data.pos, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), Phoenix::Math::Vector3(data.radius, data.radius, data.radius));
				}

				for (auto enemy : enemyManager->GetEnemies())
				{
					const auto datas = *enemy->GetCollisionDatas();
					for (const auto data : datas)
					{
						PrimitiveRender(device, data.pos, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), Phoenix::Math::Vector3(data.radius, data.radius, data.radius));
					}
				}

				PrimitiveRender(device, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), Phoenix::Math::Vector3(1.0f, 1.0f, 1.0f), true);

				context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
			}
		}
		frameBuffer[0]->Deactivate(graphicsDevice);
	}

	// Generate Bloom Texture.
	Phoenix::u32 resolvedFramebuffer = 0;
	{
		if (enableMSAA)
		{
			resolvedFramebuffer = 1;

			msaaResolve->Resolve(graphicsDevice, frameBuffer[0].get(), frameBuffer[resolvedFramebuffer].get());

			bloom->Generate(graphicsDevice, frameBuffer[resolvedFramebuffer]->GetRenderTargetSurface()->GetTexture(), false);

			frameBuffer[resolvedFramebuffer]->Activate(graphicsDevice);
			{
				bloom->Draw(graphicsDevice);
			}
			frameBuffer[resolvedFramebuffer]->Deactivate(graphicsDevice);
		}
		else
		{
			bloom->Generate(graphicsDevice, frameBuffer[resolvedFramebuffer]->GetRenderTargetSurface()->GetTexture(), false);

			resolvedFramebuffer = 1;

			frameBuffer[resolvedFramebuffer]->Activate(graphicsDevice);
			{
				bloom->Draw(graphicsDevice);
			}
			frameBuffer[resolvedFramebuffer]->Deactivate(graphicsDevice);
		}
	}

	// Blend Bloom.
	if (bloomBlend)
	{
		resolvedFramebuffer = 2;
		frameBuffer[resolvedFramebuffer]->Activate(graphicsDevice);
		{
			bloom->Blend(graphicsDevice, frameBuffer[0]->GetRenderTargetSurface()->GetTexture(), frameBuffer[1]->GetRenderTargetSurface()->GetTexture());
		}
		frameBuffer[resolvedFramebuffer]->Deactivate(graphicsDevice);

		//quad->Draw(graphicsDevice, frameBuffer[resolvedFramebuffer]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f, static_cast<Phoenix::f32>(display->GetWidth()), static_cast<Phoenix::f32>(display->GetHeight()));

		toneMap->Draw(graphicsDevice, frameBuffer[resolvedFramebuffer]->renderTargerSurface[0]->GetTexture(), elapsedTime);
	}
	else
	{
		quad->Draw(graphicsDevice, frameBuffer[0]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f, static_cast<Phoenix::f32>(display->GetWidth()), static_cast<Phoenix::f32>(display->GetHeight()));
	}
#endif
	// Draw UI and Effect.
	{
#if 1
		//if (lockOnCamera)
		//{
		//	/*Phoenix::Graphics::ContextDX11* contextDX11 = static_cast<Phoenix::Graphics::ContextDX11*>(context);
		//	context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);*/

		//	Phoenix::f32 size = 128.0f / 4.0f;

		//	Phoenix::Math::Vector3 bossPos = boss->GetPosition();
		//	bossPos.y += 1.5f;

		//	Phoenix::Math::Vector3 screenPos = WorldToScreen(bossPos);
		//	screenPos.x -= size / 2.0f;

		//	quad->Draw(graphicsDevice, targetMark, screenPos.x, screenPos.y, size, size);
		//}

		// Draw Effect.
		//{
		//	//Phoenix::Graphics::ContextDX11* contextDX11 = static_cast<Phoenix::Graphics::ContextDX11*>(context);
		//	//context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::Additive), 0, 0xFFFFFFFF);
		//	{
		//		gpuParticle->Draw(graphicsDevice, *camera);
		//		playerHitParticle->Draw(graphicsDevice, *camera);
		//	}
		//	//context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
		//}
#endif

#if 1
		if (isDrawUI) uiSystem->Draw(graphicsDevice);
#endif
	}

	// Draw frameBuffer Texture.
	{
		if (active[0]) quad->Draw(graphicsDevice, shadowMap->GetDepthStencilSurface()->GetTexture(), texSize.x * 0, 0, texSize.x, texSize.y);
		//if (active[0]) quad->Draw(graphicsDevice, frameBuffer[0]->renderTargerSurface[0]->GetTexture(), texSize.x * 0, 0, texSize.x, texSize.y);
		if (active[1]) quad->Draw(graphicsDevice, frameBuffer[1]->renderTargerSurface[0]->GetTexture(), texSize.x * 1, 0, texSize.x, texSize.y);
		if (active[2]) quad->Draw(graphicsDevice, frameBuffer[1]->depthStencilSurface->GetTexture(), texSize.x * 2, 0, texSize.x, texSize.y);
		if (active[3]) quad->Draw(graphicsDevice, frameBuffer[0]->renderTargerSurface[0]->GetTexture(), texSize.x * 3, 0, texSize.x, texSize.y);
		/*if (active[3]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[0]->GetTexture(), texSize.x * 3, 0, texSize.x, texSize.y);
		if (active[4]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[1]->GetTexture(), texSize.x * 4, 0, texSize.x, texSize.y);
		if (active[5]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[2]->GetTexture(), texSize.x * 5, 0, texSize.x, texSize.y);
		if (active[6]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[3]->GetTexture(), texSize.x * 6, 0, texSize.x, texSize.y);
		if (active[7]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[4]->GetTexture(), texSize.x * 7, 0, texSize.x, texSize.y);
		if (active[8]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[5]->GetTexture(), texSize.x * 8, 0, texSize.x, texSize.y);*/
	}
}

void SceneTutorial::GUI()
{
#if	defined(PHOENIX_TARGET_DEBUG)
	ImGui::Begin("Tutorial");
	{
		/*ImGui::Text("%d", hit);
		ImGui::Checkbox("Update", &isUpdate);
		ImGui::Checkbox("PlayerUpdate", &isPlayerUpdate);
		ImGui::Checkbox("BossUpdate", &isBossUpdate);*/
		{
			player->GUI();
		}
		{
			mannequin->GUI();
		}
		{
			enemyManager->GUI();
		}
		if (ImGui::TreeNode("Camera"))
		{
			ImGui::Checkbox("FreeCamera", &cameraFlg);
			ImGui::Checkbox("LockCamera", &lockOnCamera);
			ImGui::DragFloat("MoveSpeed", &camera->speed, 0.1f, 0.1f, 50.0f);
			ImGui::DragFloat("adjustY", &adjustY, 0.1f);
			ImGui::DragFloat("adjustRight", &camera->adjustRight, 0.1f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Collision"))
		{
			ImGui::Checkbox("On", &isHitCollision);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Shader"))
		{
			Phoenix::FrameWork::LightState* light = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetLight();
			Phoenix::FrameWork::LightState* sunLight = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetSunLight();
			Phoenix::FrameWork::MaterialState* material = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetMaterial();
			Phoenix::FrameWork::MaterialState* materialSkin = static_cast<Phoenix::FrameWork::PBRSkinShader*>(pbrSkinShader)->GetMaterial();
			ImGuiColorEditFlags flag = ImGuiColorEditFlags_Float; // 0 ~ 255表記ではなく、0.0 ~ 1.0表記にします。

			ImGui::Checkbox("OnPBR", &isPBR);
			ImGui::DragFloat4("dir", &light->direction.x);
			ImGui::DragFloat4("sunDir", &sunLight->direction.x, 0.01f);
			ImGui::DragFloat4("color", &light->color.x);
			ImGui::ColorEdit4("albedo", material->albedo, flag);
			ImGui::DragFloat("metallic", &material->metallic, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("roughness", &material->roughness, 0.01f, 0.0f, 1.0f);
			if (ImGui::TreeNode("Stage"))
			{
				ImGui::ColorEdit4("albedo", materialSkin->albedo, flag);
				ImGui::DragFloat("metallic", &materialSkin->metallic, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("roughness", &materialSkin->roughness, 0.01f, 0.0f, 1.0f);
				ImGui::TreePop();
			}
			ImGui::TreePop();

		}
		if (ImGui::TreeNode("UI"))
		{
			ImGui::Checkbox("DrawUI", &isDrawUI);
			if (ImGui::TreeNode("TargetMark"))
			{
				if (ImGui::Button("On"))
				{
					targetMarkUI->LockOnTarget();
				}

				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Shadow"))
		{
			ImGui::Checkbox("On", &shadowBlend);
			ImGui::DragFloat3("color", &postProcessingEffects->shaderContexts.shadowMap.color.x, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("bias", &postProcessingEffects->shaderContexts.shadowMap.bias, 0.001f, 0.0f, 1.0f);

			ImGui::DragFloat("dis", &dis, 0.1f);
			ImGui::DragFloat("width", &width, 1.0f);
			ImGui::DragFloat("height", &height, 1.0f);
			ImGui::DragFloat("nearZ", &nearZ, 0.01f);
			ImGui::DragFloat("farZ", &farZ, 0.1f);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Bloom"))
		{
			ImGui::Checkbox("On", &bloomBlend);
			ImGui::DragFloat("glowExtractionThreshold", &bloom->shaderContants.glowExtractionThreshold, 0.01f, 0.0f, 5.0f);
			ImGui::DragFloat("blurConvolutionIntensity", &bloom->shaderContants.blurConvolutionIntensity, 0.01f, 0.0f, 5.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("FrameBuffer"))
		{
			if (ImGui::TreeNode("Flag"))
			{
				const Phoenix::s8* name[10] =
				{ "On FrameBuffer00", "On FrameBuffer01", "On FrameBuffer02", "On FrameBuffer03", "On FrameBuffer04",
				  "On FrameBuffer05", "On FrameBuffer06", "On FrameBuffer07", "On FrameBuffer08", "On FrameBuffer09" };
				for (int i = 0; i < 10; ++i)
				{
					ImGui::Checkbox(name[i], &active[i]);
				}
				ImGui::TreePop();
			}
			ImGui::DragFloat2("textureSize", &texSize.x);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ComputeShader"))
		{
			static bool isRun = false;
			ImGui::Checkbox("On", &isRun);
			if (isRun)
			{
				testComputeShader->Run(graphicsDevice);
			}

			Phoenix::FrameWork::TestComputeShader* test = static_cast<Phoenix::FrameWork::TestComputeShader*>(testComputeShader.get());
			for (int i = 0; i < test->particleMaxSize; i++)
			{
				ImGui::Text("resutl : %f, %f, %f", test->resultParticle[i].pos.x, test->resultParticle[i].pos.y, test->resultParticle[i].pos.z);
			}

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("BitonicSort"))
		{
			static Phoenix::u32 size = 512;
			static bool isInit = false;
			static Phoenix::FrameWork::SortData datas[512];

			if (!isInit)
			{
				for (Phoenix::u32 i = 0; i < size; i++)
				{
					datas[i].key = 0.0f;
					datas[i].index = i;
				}
				isInit = true;
			}

			if (ImGui::Button("Run"))
			{
				for (Phoenix::u32 i = 0; i < size; i++)
				{
					datas[i].key = static_cast<Phoenix::f32>(rand() % size);
					datas[i].index = i;
				}

				bitonicSort->Run(graphicsDevice, &datas[0], size);
			}

			for (Phoenix::u32 i = 0; i < size; i++)
			{
				ImGui::Text("resutl : %f, %d", datas[i].key, datas[i].index);
			}

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("GPUParticle"))
		{
			if (ImGui::Button("Burst"))
			{
				gpuParticle->Burst(100);
				gpuParticle->SetParticleLife(particleLife);
				gpuParticle->SetParticleSize(particleSize);
				gpuParticle->SetParticleScale(particleScale);
				gpuParticle->SetParticleNormal(particleNormal);
				gpuParticle->SetParticleColor(particleMainColor);
			}

			if (ImGui::Button("Hit Particle Burst"))
			{
				gpuParticle->Burst(100);
				gpuParticle->SetParticleLife(1.0f);
				gpuParticle->SetParticleSize(0.02f);
				gpuParticle->SetParticleScale(0.25f);
				gpuParticle->SetParticleNormal(particleNormal);
				gpuParticle->SetParticleColor(particleMainColor);
			}

			ImGui::DragFloat3("Pos", &particlePos.x, 0.1f);
			ImGui::DragFloat3("noemal", &particleNormal.x, 0.1f);
			ImGui::DragFloat("life", &particleLife, 0.1f);
			ImGui::DragFloat("size", &particleSize, 0.1f);
			ImGui::DragFloat("scale", &particleScale, 0.1f);
			ImGui::ColorEdit4("color", particleMainColor);

			//if (ImGui::TreeNode("JumpAttackParticle"))
			//{
			//	if (ImGui::Button("Burst"))
			//	{
			//		jumpAttackParticle->Burst(50);
			//		jumpAttackParticle->SetParticleLife(particleLife);
			//		jumpAttackParticle->SetParticleSize(particleSize);
			//		jumpAttackParticle->SetParticleScale(particleScale);
			//		jumpAttackParticle->SetParticleNormal(Phoenix::Math::Vector4(0.0f, 1.0f, 0.0f, 0.0f));
			//		jumpAttackParticle->SetParticleColor(Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f)); // particleMainColor

			//		jumpAttackParticlePos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
			//	}

			//	ImGui::DragFloat3("Pos", &jumpAttackParticlePos.x, 0.1f);
			//	ImGui::DragFloat3("normal", &particleNormal.x, 0.1f);
			//	ImGui::DragFloat("life", &particleLife, 0.1f);
			//	ImGui::DragFloat("size", &particleSize, 0.1f);
			//	ImGui::DragFloat("scale", &particleScale, 0.1f);
			//	ImGui::ColorEdit4("color", particleMainColor);

			//	ImGui::TreePop();
			//}

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ToneMap"))
		{
			ImGui::SliderFloat("whitePoint", &toneMap->shaderConstant.whitePoint, 0.0f, 30.0f);
			ImGui::SliderFloat("averageGray", &toneMap->shaderConstant.averageGray, 0.0f, 3.0f);
			ImGui::SliderFloat("brightness", &toneMap->shaderConstant.brightness, -1.0f, 1.0f);
			ImGui::SliderFloat("contrast", &toneMap->shaderConstant.contrast, -1.0f, 1.0f);
			ImGui::SliderFloat("hue", &toneMap->shaderConstant.hue, -1.0f, 1.0f);
			ImGui::SliderFloat("saturation", &toneMap->shaderConstant.saturation, -1.0f, 1.0f);
			ImGui::SliderFloat("amount", &toneMap->shaderConstant.amount, 0.0f, 1.0f, "sepia = %.3f");
			ImGui::SliderFloat("offset", &toneMap->shaderConstant.offset, 0.0f, 10.0f, "vignette = %.3f");
			ImGui::SliderFloat("darkness", &toneMap->shaderConstant.darkness, 0.0f, 1.0f, "vignette = %.3f");

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Pad"))
		{
			if (ImGui::Button("On Vibration"))
			{
				SetXInputVibration(vibrationRight, vibrationLeft, vibrationMaxCnt);
			}

			ImGui::SliderFloat("shakeRight", &vibrationRight, 0.0f, 65535.0f);
			ImGui::SliderFloat("shakeLeft", &vibrationLeft, 0.0f, 65535.0f);
			ImGui::DragInt("shakeMaxCnt", &vibrationMaxCnt);

			ImGui::TreePop();
		}
	}
	ImGui::End();
#endif
}

void SceneTutorial::PrimitiveRender(Phoenix::Graphics::DeviceDX11* device, Phoenix::Math::Vector3 translate, Phoenix::Math::Vector3 rotate, Phoenix::Math::Vector3 scale, bool isBox)
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

	if (isBox)
	{
		boxPrimitive->Render
		(
			device->GetD3DContext(),
			Phoenix::Math::ConvertToFloat4x4FromVector4x4(W * camera->GetView() * camera->GetProjection()),
			Phoenix::Math::ConvertToFloat4x4FromVector4x4(W),
			DirectX::XMFLOAT4(1, 1, 1, 1),
			DirectX::XMFLOAT4(0.0f, 0.6f, 0.0f, 0.5f),
			false
		);
	}
	else
	{
		primitive->Render
		(
			device->GetD3DContext(),
			Phoenix::Math::ConvertToFloat4x4FromVector4x4(W * camera->GetView() * camera->GetProjection()),
			Phoenix::Math::ConvertToFloat4x4FromVector4x4(W),
			DirectX::XMFLOAT4(1, 1, 1, 1),
			DirectX::XMFLOAT4(0.0f, 0.6f, 0.0f, 0.5f),
			false
		);
	}
}

Phoenix::Math::Vector3 SceneTutorial::WorldToScreen(const Phoenix::Math::Vector3& worldPosition)
{
	Phoenix::Math::Vector3 screenPosition;

	// ビューポート
	Phoenix::Graphics::Viewport* v = new Phoenix::Graphics::Viewport();
	graphicsDevice->GetContext()->GetViewports(1, &v);
	float viewportX = 0.0f;
	float viewportY = 0.0f;
	float viewportW = static_cast<Phoenix::f32>(v->width);
	float viewportH = static_cast<Phoenix::f32>(v->height);
	float viewportMinZ = 0.0f;
	float viewportMaxZ = 1.0f;
	Phoenix::FND::SafeDelete(v);

	// ビュー行列
	Phoenix::Math::Matrix V = camera->GetView();

	// プロジェクション行列
	Phoenix::Math::Matrix P = camera->GetProjection();

	// ワールド行列
	Phoenix::Math::Matrix W = Phoenix::Math::MatrixIdentity(); //移動成分はいらないので単位行列を入れておく。

	// ワールド座標からNDC座標へ変換
	Phoenix::Math::Matrix WVP = W * V * P;
	Phoenix::Math::Vector3 ndcPosition = Phoenix::Math::Vector3TransformCoord(worldPosition, WVP); // Vector3TransformCoordで1.0fで[x,y,z,w]が割られている。

	// NDC座標からスクリーン座標へ変換
	screenPosition.x = ((ndcPosition.x + 1.0f) / 2.0f) * viewportW;
	screenPosition.y = viewportH - (((ndcPosition.y + 1.0f) / 2.0f) * viewportH);
	screenPosition.z = viewportMinZ + ndcPosition.z * (viewportMaxZ - viewportMinZ);

	return screenPosition;
}