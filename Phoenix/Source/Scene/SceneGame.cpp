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
#include "../Enemy/Enemy.h"
#include "../AI/MetaAI/MetaType.h"
#include "Phoenix/OS/StartUp.h"


void SceneGame::Construct(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	display = sceneSystem->GetDisplay();
	graphicsDevice = sceneSystem->GetGraphicsDevice();
	commonData = sceneSystem->GetSceneCommonData();

	// デバック用
	{
		Phoenix::Graphics::DeviceDX11* device = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());
		primitive = std::make_shared<GeometricPrimitive>(device->GetD3DDevice(), 1);
		cylinderPrimitive = std::make_shared<GeometricPrimitive>(device->GetD3DDevice(), 2);
	}

	// 共通データのアドレス取得
	{
		player = commonData->player.get();
		//boss = commonData->boss.get();
		enemyManager = commonData->enemyManager.get();
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
		soundSystem = commonData->soundSystem.get();
	}

	// フレームバッファ
	{
		frameBuffer[0] = Phoenix::FrameWork::FrameBuffer::Create();
		frameBuffer[1] = Phoenix::FrameWork::FrameBuffer::Create();
		frameBuffer[2] = Phoenix::FrameWork::FrameBuffer::Create();
		frameBuffer[3] = Phoenix::FrameWork::FrameBuffer::Create();
		frameBuffer[4] = Phoenix::FrameWork::FrameBuffer::Create();
		frameBuffer[5] = Phoenix::FrameWork::FrameBuffer::Create();

		frameBuffer[0]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), enableMSAA, 8, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
		frameBuffer[1]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
		frameBuffer[2]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::UNKNOWN);
		frameBuffer[3]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
		frameBuffer[4]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
		frameBuffer[5]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
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

	// モーションブラー
	{
		motionBlur = Phoenix::FrameWork::MotionBlur::Create();
		motionBlur->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight());
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
		//dusterParticle[0] = Phoenix::FrameWork::GPUParticle::Create();
		//dusterParticle[1] = Phoenix::FrameWork::GPUParticle::Create();
		//dusterParticle[2] = Phoenix::FrameWork::GPUParticle::Create();
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

		pbrDissolvePixelShader = Phoenix::Graphics::IShader::Create();
		pbrDissolvePixelShader->LoadPS
		(
			graphicsDevice->GetDevice(),
			"PhysicallyBasedRenderingDissolvePS.cso"
		);

		bossRedTexture = Phoenix::Graphics::ITexture::Create();
		bossRedTexture->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\Boss\\Mutant_diffuse_Red1.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color::White);

		dissolveTexture = Phoenix::Graphics::ITexture::Create();
		dissolveTexture->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\Mask\\Dissolve\\dissolve_animation1_2.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color::White);

		emissiveTexture = Phoenix::Graphics::ITexture::Create();
		emissiveTexture->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\Mask\\Dissolve\\dissolve_edgecolor_gray.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color::White);

		dissolveThreshold = 1.15f;
		dissolveEmissiveWidth = 0.01f;
		isTurn = false;
	}
}

void SceneGame::Initialize()
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

		isDrawUI = true;
		onPlayerEditor = false;
	}

	// 共通データの初期化
	{
		player->Initialize();
		//boss->Initialize();

		enemyManager->Initialize();
		metaAI->Initialize();

		camera->SetEye(Phoenix::Math::Vector3(0.0f, 0.0f, 10.0f));
		camera->SetRotateX(-0.225f);
		camera->SetRotateY(0.0f);

		currentShader = pbrShader;

		tempCameraFouce = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		sphereLinearSpeed = 0.0f;
		distanceToFouceFromCamera = 0.0f;

		isCameraShake = false;
		shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		shakeWidth = 0.0f;
		shakeHeight = 0.0f;
		cameraShakeCnt = 0.0f;
		cameraShakeMaxCnt = 0.0f;

		playerBehaviorScore = 0;
		oldPlayerBehaviorScore = 0;
		WeakAttackScore = 10;
		StrongAttackScore = 30;

		isSlow = false;
		slowTimeCnt = 0.0f;

		isMotionBlur = false;
	}

	// ラウンド
	{
		roundCnt = 0;
		roundSwitch = false;
		roundFadeColor = 1.0f;
		roundFadeSwitch = false;

		RoundInitialize();
	}

	// GPUパーティクル
	{
		testComputeShader->Initialize(graphicsDevice);
		bitonicSort->Initialize(graphicsDevice);
		gpuParticle->Initialize(graphicsDevice, "SimulateCS.cso", "..\\Data\\Assets\\Texture\\Effect\\Fire\\FireOrigin.png"); // particle
		playerHitParticle->Initialize(graphicsDevice, "SimulateCS.cso", "..\\Data\\Assets\\Texture\\Effect\\Fire\\FireOrigin02.png", true); // PlayerHitEffectCS Fire\\FireOrigin02
		bossHitParticle->Initialize(graphicsDevice, "SimulateCS.cso", "..\\Data\\Assets\\Texture\\Effect\\Fire\\FireOrigin02.png", true); // PlayerHitEffectCS
		petalParticle->Initialize(graphicsDevice, "PetalEffectCS.cso", "..\\Data\\Assets\\Texture\\Effect\\JumpAttack\\Petal01.png", false);
		soilParticle->Initialize(graphicsDevice, "SoilEffectCS.cso", "..\\Data\\Assets\\Texture\\Effect\\JumpAttack\\Soil01.png", false);
		//dusterParticle[0]->Initialize(graphicsDevice, "DusterEffectCS.cso", "..\\Data\\Assets\\Texture\\Effect\\Duster\\Duster05.png", false);
		//dusterParticle[1]->Initialize(graphicsDevice, "DusterEffectCS.cso", "..\\Data\\Assets\\Texture\\Effect\\Duster\\Duster06.png", false);
		//dusterParticle[2]->Initialize(graphicsDevice, "DusterEffectCS.cso", "..\\Data\\Assets\\Texture\\Effect\\Duster\\Duster07.png", false);
	}
}

void SceneGame::RoundInitialize()
{
	// 共通データの初期化
	{
		player->Initialize();
		enemyManager->Initialize();
		metaAI->Initialize();

		camera->SetEye(Phoenix::Math::Vector3(0.0f, 0.0f, 10.0f));
		camera->SetRotateX(-0.225f);
		camera->SetRotateY(0.0f);
	}

	// エネミー追加
	{
		auto AddEneies = [&](Phoenix::s32 nextRoundCnt)
		{
			Phoenix::FrameWork::Transform transform;

			switch (nextRoundCnt)
			{
			case 0:
				transform.SetTranslate({ 0,0,-5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(0);
				break;
			case 1:
				transform.SetTranslate({ 0,0,-5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(0);

				transform.SetTranslate({ 5,0,-5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(1);
				break;
			case 2:
				transform.SetTranslate({ 0,0,-5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(0);

				transform.SetTranslate({ 5,0,-5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(1);

				transform.SetTranslate({ -5,0,-5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(2);
				break;
			case 3:
				transform.SetTranslate({ 0,0,-5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(0);

				transform.SetTranslate({ 5,0,-5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(1);

				transform.SetTranslate({ -5,0,-5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(2);

				transform.SetTranslate({ -5,0,5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(3);
				break;
			case 4:
				transform.SetTranslate({ 0,0,-5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(0);

				transform.SetTranslate({ 5,0,-5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(1);

				transform.SetTranslate({ -5,0,-5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(2);

				transform.SetTranslate({ -5,0,5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(3);

				transform.SetTranslate({ 5,0,5 });
				transform.SetRotate({ 0,0,0,1 });
				transform.SetScale({ 1.0f,1.0f,1.0f });
				enemyManager->AddEnemy(transform);
				enemyManager->SetBattleEnemy(4);
				break;
			default: break;
			}
		};

		enemyManager->ResetEnemies();
		AddEneies(roundCnt);
	}

	// 再生
	{
		soundSystem->Play(SoundType::BGM_Game, true);
	}
}

void SceneGame::Update(Phoenix::f32 elapsedTime)
{
	// フラグ更新
	{
		onFade = sceneSystem->GetOnFade();
		onControl = player->GetAlive();
	}

	// スローモーション更新
	{
		UpdateSlow(elapsedTime);
	}

	// GUI起動
	{
		StartUpGUI();
	}

	// サウンド更新
	{
		soundSystem->Update();
	}

	// プレイヤー更新
	{
		UpdatePlayer(elapsedTime);
	}

	// メタAI更新
	{
		UpdateMetaAI(elapsedTime);
	}

	// エネミー更新
	{
		UpdateEnemyManager(elapsedTime);
	}

	// ラウンド移行
	if (roundSwitch)
	{
		UpdateRound(elapsedTime);
		return;
	}

	// ゲームジャッジ
	{
		JudgeGame();
	}

	// 当たり判定
	{
		JudgeCollision(elapsedTime);
	}

	// カメラ更新
	{
		UpdateCamera(elapsedTime);
	}

	// UI更新
	{
		UpdateUI();
	}

	// 平行光更新
	{
		UpdateDirectionLight(elapsedTime);
	}

	// フェード中
	if (sceneSystem->GetOnFade())
	{
		return;
	}

	// シェーダー切り替え
	{
		SwitchShader();
	}

	// パーティクル更新
	{
		UpdateParticle(elapsedTime);
	}

	// ヒットストップ更新
	{
		UpdateHitStop(elapsedTime);
	}
	
	// カメラシェイク更新
	{
		UpdateCameraShake(elapsedTime);
	}

	// テストサウンド再生
	{
		commonData->bgm->Update();
		commonData->se->Update();
	}

	// メタAI通知
	{
		NoticeMetaAI();
	}
}

void SceneGame::SearchNearEnemy(Phoenix::Math::Vector3& nearEnemyPos, Phoenix::Math::Vector3& centerOfGravity, Phoenix::s32& enemyCount)
{
	inTerritory = false;

	nearEnemyIndex = -1;
	nearIndex = -1;
	drawEnemyUIIndex = -1;

	Phoenix::f32 nearEnemyLen = -1.0f;
	Phoenix::f32 nearLen = -1.0f;
	Phoenix::Math::Vector3 playerPos = player->GetPosition();

	Phoenix::s32 count = 0;
	for (const auto& enemy : enemyManager->GetEnemies())
	{
		if (enemy->GetAlive())
		{
			Phoenix::Math::Vector3 enemyPos = enemy->GetPosition();

			Phoenix::Math::Vector3 enemyToPlayerVec = enemyPos - playerPos;
			Phoenix::f32 len = Phoenix::Math::Vector3Length(enemyToPlayerVec);

			if (nearIndex == -1 || len < nearLen)
			{
				nearLen = len;
				nearIndex = count;
			}
			if (enemy->InDistanceHitByAttack())
			{
				if (nearEnemyIndex == -1 || len < nearEnemyLen)
				{
					nearEnemyIndex = count;
					nearEnemyPos = enemy->GetPosition();
				}

				inTerritory = true;
				centerOfGravity += enemyPos;
				++enemyCount;
			}
		}

		++count;
	}
}

void SceneGame::UpdateTargetPos(Phoenix::Math::Vector3 nearEnemyPos, Phoenix::Math::Vector3 centerOfGravity, Phoenix::s32 enemyCount)
{
	Phoenix::Math::Quaternion rotate = player->GetRotate();
	Phoenix::Math::Matrix m = Phoenix::Math::MatrixRotationQuaternion(&rotate);
	Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(m._31, m._32, m._33);
	forward.y = 0.0f;
	if (nearEnemyIndex != -1)
	{
		Phoenix::Math::Vector3 dir = nearEnemyPos - player->GetPosition();
		dir = Phoenix::Math::Vector3Normalize(dir);
		dir.y = 0.0f;

		Phoenix::f32 angle;
		angle = acosf(Phoenix::Math::Vector3Dot(dir, forward));

		if (1e-8f < fabs(angle))
		{
			angle /= 0.01745f;

			if (angle <= 45.0f)
			{
				drawEnemyUIIndex = nearEnemyIndex;
				player->SetTargetPos(enemyManager->GetEnemies().at(nearEnemyIndex)->GetPosition());
			}
		}
	}
	else if (0 <= nearIndex)
	{
		player->SetTargetPos(enemyManager->GetEnemies().at(nearIndex)->GetPosition());
	}
}

void SceneGame::UpdatePlayer(Phoenix::f32 elapsedTime)
{
	if (isUpdate && !isHitStop)
	{
		Phoenix::s32 alive = 0;
		for (const auto& enemy : enemyManager->GetEnemies())
		{
			if (enemy->GetEnable() && enemy->GetAlive())
			{
				++alive;
			}
		}

		player->Update(*camera, !onFade && isPlayerUpdate && !roundSwitch, elapsedTime/*&& (alive != 0)*/);
	}
}

void SceneGame::UpdateEnemyManager(Phoenix::f32 elapsedTime)
{
	if (isUpdate && isBossUpdate && !isHitStop)
	{
		enemyManager->Update(onControl && !roundSwitch, elapsedTime);
	}
}

void SceneGame::UpdateMetaAI(Phoenix::f32 elapsedTime)
{
	if (isUpdate && isBossUpdate && !isHitStop)
	{
		if (!onFade && onControl && !roundSwitch)
		{
			metaAI->Update(elapsedTime);
		}
	}
}

void SceneGame::UpdateCamera(Phoenix::f32 elapsedTime)
{
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
		static Phoenix::f32 lerp = 1.0f;
		static Phoenix::f32 cameraLen = 6.0f;

		bool onFade = sceneSystem->GetOnFade();

		Phoenix::s32 enemyCount = 0;

		Phoenix::Math::Vector3 playerPos = player->GetPosition();
		Phoenix::Math::Vector3 nearEnemyPos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		Phoenix::Math::Vector3 centerOfGravity = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);

		// 一番近いエネミーを探す
		{
			SearchNearEnemy(nearEnemyPos, centerOfGravity, enemyCount);
		}

		// 一番近いエネミーの座標をプレイヤーに設定
		{
			UpdateTargetPos(nearEnemyPos, centerOfGravity, enemyCount);
		}

		Phoenix::f32 adjustLen = 0.0f;
		if (isSlow)
		{
			adjustLen = -4.0f;
		}

		if (inTerritory)
		{
			Phoenix::Math::Vector3 enemyToPlayerVec = (centerOfGravity / static_cast<Phoenix::f32>(enemyCount)) - playerPos;
			Phoenix::f32 len = Phoenix::Math::Vector3Length(enemyToPlayerVec);

			if (len <= 5.0f)
			{
				Phoenix::Math::Quaternion rotate = player->GetRotate();
				Phoenix::Math::Matrix m = Phoenix::Math::MatrixRotationQuaternion(&rotate);
				Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(m._31, m._32, m._33);
				forward.y = 0.0f;

				cameraLen = Phoenix::Math::f32Lerp(cameraLen, len + 6.0f + adjustLen, 0.05f * elapsedTime);

				enemyToPlayerVec = Phoenix::Math::Vector3Normalize(enemyToPlayerVec);
				camera->ControllerCamera02(!onFade && !roundSwitch, playerPos + enemyToPlayerVec * (len * 0.5f), Phoenix::Math::Vector3(0.0f, 1.25f, 0.0f), cameraLen, elapsedTime, 0.05f, (player->GetAnimationState() == Player::AnimationState::Idle), forward);
				lerp = 0.01f;
			}
		}
		else
		{
			lerp = Phoenix::Math::f32Lerp(lerp, 1.0f, 0.01f * elapsedTime);
			cameraLen = Phoenix::Math::f32Lerp(cameraLen, 6.5f + adjustLen, 0.05f * elapsedTime);

			camera->ControllerCamera02(!onFade && !roundSwitch, player->GetPosition(), Phoenix::Math::Vector3(0.0f, 1.5f, 0.0f), cameraLen, elapsedTime, lerp);
		}
	}
}

void SceneGame::UpdateSlow(Phoenix::f32& elapsedTime)
{
	if (isSlow)
	{
		if (100.0f <= slowTimeCnt || !player->IsJustDedge())
		{
			isSlow = false;
			slowTimeCnt = 0.0f;

			player->SetIsJustDedge(false);
		}
		else
		{
			slowTimeCnt += 1.0f * elapsedTime;
			elapsedTime *= 0.15f;
		}
	}
}

void SceneGame::UpdateRound(Phoenix::f32 elapsedTime)
{
	if (!roundFadeSwitch)
	{
		roundFadeColor -= 0.01f * elapsedTime;
		if (roundFadeColor <= 0.0f)
		{
			roundFadeSwitch = true;
			roundFadeColor = 0.0f;
			RoundInitialize();
		}
	}
	else
	{
		roundFadeColor += 0.01f * elapsedTime;
		if (1.0f <= roundFadeColor)
		{
			roundSwitch = false;
			roundFadeSwitch = false;
			roundFadeColor = 1.0f;
		}
	}
	screenColor = Phoenix::Math::Color(roundFadeColor, roundFadeColor, roundFadeColor, 1.0f);

	UpdateCamera(elapsedTime);
	UpdateUI();
}

void SceneGame::UpdateHitLight(Phoenix::f32 elapsedTime)
{
	if (onPointLight && !isHitStop)
	{
		Phoenix::FrameWork::PointLightState* point = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetPointLight();

		if (playerAttackEndMaxCount <= playerAttackEndCount)
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

			point->color.x -= grayScale / playerAttackEndMaxCount * elapsedTime;
			point->color.y -= grayScale / playerAttackEndMaxCount * elapsedTime;
			point->color.z -= grayScale / playerAttackEndMaxCount * elapsedTime;

			playerAttackEndCount += 1.0f * elapsedTime;
		}
	}
}

void SceneGame::UpdateDirectionLight(Phoenix::f32 elapsedTime)
{
	Phoenix::FrameWork::LightState* light = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetLight();
	light->direction = Phoenix::Math::Vector4(-camera->GetFront(), 1.0f);
}

void SceneGame::UpdateParticle(Phoenix::f32 elapsedTime)
{
	if (isUpdate)
	{
		//gpuParticle->Burst(10);
		//gpuParticle->UpdateCPU(graphicsDevice, particlePos, 1.0f / 60.0f);
		//gpuParticle->UpdateGPU(graphicsDevice, Phoenix::Math::MatrixIdentity(), 1.0f / 60.0f);
		playerHitParticle->SetParticleFixedTimeStep(elapsedTime / 60.0f);
		playerHitParticle->UpdateCPU(graphicsDevice, particlePos, 1.0f / 60.0f);
		playerHitParticle->UpdateGPU(graphicsDevice, Phoenix::Math::MatrixIdentity(), 1.0f / 60.0f);

		bossHitParticle->SetParticleFixedTimeStep(elapsedTime / 60.0f);
		bossHitParticle->UpdateCPU(graphicsDevice, bossHitParticlePos, 1.0f / 60.0f);
		bossHitParticle->UpdateGPU(graphicsDevice, Phoenix::Math::MatrixIdentity(), 1.0f / 60.0f);

		petalParticle->SetParticleFixedTimeStep(elapsedTime / 60.0f);
		petalParticle->UpdateCPU(graphicsDevice, jumpAttackParticlePos, 1.0f / 60.0f);
		petalParticle->UpdateGPU(graphicsDevice, Phoenix::Math::MatrixIdentity(), 1.0f / 60.0f);

		soilParticle->SetParticleFixedTimeStep(elapsedTime / 60.0f);
		soilParticle->UpdateCPU(graphicsDevice, jumpAttackParticlePos, 1.0f / 60.0f);
		soilParticle->UpdateGPU(graphicsDevice, Phoenix::Math::MatrixIdentity(), 1.0f / 60.0f);

		/*for (Phoenix::s32 i = 0; i < 3; ++i)
		{
			dusterParticle[i]->UpdateCPU(graphicsDevice, particlePos, 1.0f / 60.0f);
			dusterParticle[i]->UpdateGPU(graphicsDevice, Phoenix::Math::MatrixIdentity(), 1.0f / 60.0f);
		}*/
	}
}

void SceneGame::UpdateHitStop(Phoenix::f32 elapsedTime)
{
	if (isHitStop)
	{
		if (hitStopMaxCnt <= hitStopCnt)
		{
			isHitStop = false;
			hitStopCnt = 0.0f;
		}
		else
		{
			hitStopCnt += 1.0f * elapsedTime;
		}
	}
}

void SceneGame::UpdateCameraShake(Phoenix::f32 elapsedTime)
{
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

		if (cameraShakeMaxCnt <= cameraShakeCnt)
		{
			isCameraShake = false;
			cameraShakeCnt = 0;
			shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
			camera->SetCameraShake(shake);
		}
		else
		{
			cameraShakeCnt += 1.0f * elapsedTime;
		}
	}

	isMotionBlur = isCameraShake;
}

void SceneGame::UpdateUI()
{
	player->UpdateUI();
	enemyManager->UpdateUI(nearEnemyIndex);

	for (const auto& enemy : enemyManager->GetEnemies())
	{
		//Phoenix::Math::Vector3 frontVec = camera->GetFocus() - camera->GetEye();
		Phoenix::Math::Vector3 frontVec = camera->GetFront();
		Phoenix::Math::Vector3 enemyFromCameraVec = Phoenix::Math::Vector3Normalize(enemy->GetPosition() - camera->GetEye());
		frontVec.y = 0.0f;
		enemyFromCameraVec.y = 0.0f;

		Phoenix::f32 angle;
		angle = acosf(Phoenix::Math::Vector3Dot(frontVec, enemyFromCameraVec));
		if (1e-8f < fabs(angle))
		{
			if ((90.0f * 0.01745f) <= angle)
			{
				enemy->UpdateUI(Phoenix::Math::Vector2(-1920.0f, -1080.0f)); // 画面にエネミーが映っていないときにUIが表示されるため外に描画して隠している。
				continue;
			}
		}
		Phoenix::Math::Vector3 pos = enemy->GetPosition();
		pos.y += 2.15f;

		Phoenix::Math::Vector3 screenPos = WorldToScreen(pos);
		enemy->UpdateUI(Phoenix::Math::Vector2(screenPos.x, screenPos.y));
	}
}

void SceneGame::JudgeCollision(Phoenix::f32 elapsedTime)
{
	// エネミー同士を押し出し
	{
		PushingOutEnemies();
	}

	// プレイヤーとエネミーを押し出し
	{
		PushingOutPlayerAndEnemies();
	}

	// プレイヤーとエネミーの攻撃判定
	{
		JudgeHitPlayerAndEnemies();
	}

	// エネミーとステージの押し出し
	{
		PushingOutEnemiesAndStage();
	}

	// プレイヤーとステージの押し出し
	{
		PushingOutPlayerAndStage();
	}

	// 攻撃ヒットライトの更新
	{
		UpdateHitLight(elapsedTime);
	}
}

void SceneGame::JudgeGame()
{
	if (!onFade)
	{
		if (player->GetDeath())
		{
			sceneSystem->ChangeScene(SceneType::GameOver, false, true);

			soundSystem->Stop(SoundType::BGM_Game, true);
			soundSystem->Stop(SoundType::SE_Player_Walk, true);
			soundSystem->Stop(SoundType::SE_Player_Attack_Punch_Swing, true);
			soundSystem->Stop(SoundType::SE_Player_Attack_Punch_Hit_Right, true);
			soundSystem->Stop(SoundType::SE_Player_Attack_Punch_Hit_Heavy, true);
			soundSystem->Stop(SoundType::SE_Player_Attack_Kick_Swing, true);
			soundSystem->Stop(SoundType::SE_Player_Attack_Kick_Hit_Right, true);
			soundSystem->Stop(SoundType::SE_Player_Attack_Kick_Hit_Heavy, true);
		}

		Phoenix::s32 enable = 0;
		Phoenix::s32 alive = 0;
		Phoenix::s32 death = 0;
		for (const auto& enemy : enemyManager->GetEnemies())
		{
			if (enemy->GetEnable()) ++enable;
			else continue;

			if (!enemy->GetAlive()) ++alive;
			else continue;

			if (enemy->GetDeath()) ++death;
		}

		if (enable == alive && alive == death && death == enable && enable != 0)
		{
			++roundCnt;

			if (roundCnt == roundMax)
			{
				sceneSystem->ChangeScene(SceneType::GameClear, false, true);
			}
			else
			{
				roundSwitch = true;
			}

			soundSystem->Stop(SoundType::BGM_Game, true);
			soundSystem->Stop(SoundType::SE_Player_Walk, true);
			soundSystem->Stop(SoundType::SE_Player_Attack_Punch_Swing, true);
			soundSystem->Stop(SoundType::SE_Player_Attack_Punch_Hit_Right, true);
			soundSystem->Stop(SoundType::SE_Player_Attack_Punch_Hit_Heavy, true);
			soundSystem->Stop(SoundType::SE_Player_Attack_Kick_Swing, true);
			soundSystem->Stop(SoundType::SE_Player_Attack_Kick_Hit_Right, true);
			soundSystem->Stop(SoundType::SE_Player_Attack_Kick_Hit_Heavy, true);
		}
	}
}

void SceneGame::PushingOutEnemies()
{
	Phoenix::Math::Vector3 playerPos = player->GetPosition();

	auto& enemies = enemyManager->GetEnemies();
	for (Phoenix::s32 i = 0; i < enemies.size(); ++i)
	{
		if (!enemies.at(i)->GetAlive()) continue;

		for (Phoenix::s32 j = i + 1; j < enemies.size(); ++j)
		{
			if (i == j) continue;
			if (!enemies.at(j)->GetAlive()) continue;

			Phoenix::Math::Vector3 enemy01Pos = enemies.at(i)->GetPosition();
			Phoenix::Math::Vector3 enemy02Pos = enemies.at(j)->GetPosition();

			Phoenix::f32 enemy01Radius = enemies.at(i)->GetRadius();
			Phoenix::f32 enemy02Radius = enemies.at(j)->GetRadius();

			if (SphereVsSphere(Phoenix::Math::Vector3(enemy01Pos.x, enemy01Pos.y + 0.5f, enemy01Pos.z), Phoenix::Math::Vector3(enemy02Pos.x, enemy02Pos.y + 0.5f, enemy02Pos.z), enemy01Radius, enemy02Radius))
			{
				Phoenix::Math::Vector3 centerOfGravity = (enemy01Pos + enemy02Pos) * 0.5f;

				Phoenix::Math::Vector3 pos;
				Phoenix::Math::Vector3 dir;

				if (enemies.at(i)->UnderAttack())
				{
					// enemy02
					{
						dir = enemy02Pos - enemy01Pos;
						dir = Phoenix::Math::Vector3Normalize(dir);
						dir.y = 0.0f;

						pos = Phoenix::Math::Vector3(enemy01Pos.x, enemy01Pos.y, enemy01Pos.z) + dir * (enemy02Radius + enemy01Radius);
						enemies.at(j)->SetTranslate(pos);
						enemies.at(j)->UpdateTransform();
					}
				}
				else if (enemies.at(j)->UnderAttack())
				{
					// enemy01
					{
						dir = enemy01Pos - enemy02Pos;
						dir = Phoenix::Math::Vector3Normalize(dir);
						dir.y = 0.0f;

						pos = Phoenix::Math::Vector3(enemy02Pos.x, enemy02Pos.y, enemy02Pos.z) + dir * (enemy01Radius + enemy02Radius);
						enemies.at(i)->SetTranslate(pos);
						enemies.at(i)->UpdateTransform();
					}
				}
				else
				{
					// enemy01
					{
						dir = enemy01Pos - centerOfGravity;
						dir = Phoenix::Math::Vector3Normalize(dir);
						dir.y = 0.0f;

						pos = Phoenix::Math::Vector3(centerOfGravity.x, centerOfGravity.y, centerOfGravity.z) + dir * (enemy01Radius);
						enemies.at(i)->SetTranslate(pos);
						enemies.at(i)->UpdateTransform();
					}

					// enemy02
					{
						dir = enemy02Pos - centerOfGravity;
						dir = Phoenix::Math::Vector3Normalize(dir);
						dir.y = 0.0f;

						pos = Phoenix::Math::Vector3(centerOfGravity.x, centerOfGravity.y, centerOfGravity.z) + dir * (enemy02Radius);
						enemies.at(j)->SetTranslate(pos);
						enemies.at(j)->UpdateTransform();
					}
				}
			}
		}
	}
}

void SceneGame::PushingOutPlayerAndEnemies()
{
	Phoenix::s32 index = -1;
	Phoenix::Math::Vector3 playerPos = player->GetPosition();

	for (const auto& enemy : enemyManager->GetEnemies())
	{
		++index;

		if (!enemy->GetAlive()) continue;

		Phoenix::Math::Vector3 enemyPos = enemy->GetPosition();

		Phoenix::f32 playerRadius = player->GetRadius();
		Phoenix::f32 enemyRadius = enemy->GetRadius();

		if (SphereVsSphere(Phoenix::Math::Vector3(playerPos.x, playerPos.y + 0.5f, playerPos.z), Phoenix::Math::Vector3(enemyPos.x, enemyPos.y + 0.5f, enemyPos.z), playerRadius, enemyRadius))
		{
#if 0
			Phoenix::Math::Vector3 pos;
			Phoenix::Math::Vector3 dir = player->GetPosition() - enemy->GetPosition();
			dir = Phoenix::Math::Vector3Normalize(dir);
			dir.y = 0.0f;

			pos = Phoenix::Math::Vector3(enemyPos.x, playerPos.y, enemyPos.z) + dir * (player->GetRadius() + enemy->GetRadius());
			player->SetPosition(pos);
			player->UpdateTrasform();
#elif 1
			Phoenix::Math::Vector3 pos;
			Phoenix::Math::Vector3 dir = enemyPos - playerPos;
			dir = Phoenix::Math::Vector3Normalize(dir);
			dir.y = 0.0f;

			pos = Phoenix::Math::Vector3(playerPos.x, enemyPos.y, playerPos.z) + dir * (playerRadius + enemyRadius);
			enemy->SetTranslate(pos);
			enemy->UpdateTransform();
#else
			Phoenix::Math::Vector3 centerOfGravity = (playerPos + enemyPos) * 0.5f;

			Phoenix::Math::Vector3 pos;
			Phoenix::Math::Vector3 dir;

			// player
			{
				dir = playerPos - centerOfGravity;
				dir = Phoenix::Math::Vector3Normalize(dir);
				dir.y = 0.0f;

				pos = Phoenix::Math::Vector3(centerOfGravity.x, centerOfGravity.y, centerOfGravity.z) + dir * (playerRadius);
				player->SetPosition(pos);
				player->UpdateTrasform();
			}

			// enemy
			{
				dir = enemyPos - centerOfGravity;
				dir = Phoenix::Math::Vector3Normalize(dir);
				dir.y = 0.0f;

				pos = Phoenix::Math::Vector3(centerOfGravity.x, centerOfGravity.y, centerOfGravity.z) + dir * (enemyRadius);
				enemy->SetTranslate(pos);
				enemy->UpdateTrasform();
			}
#endif
		}
	}
}

void SceneGame::PushingOutPlayerAndStage()
{
	Phoenix::Math::Vector3 playerPos = player->GetPosition();
	Phoenix::f32 playerDis = Phoenix::Math::Vector2Length(Phoenix::Math::Vector2(playerPos.x, playerPos.z));
	Phoenix::Math::Vector2 playerNormal = Phoenix::Math::Vector2Normalize(Phoenix::Math::Vector2(playerPos.x, playerPos.z));

	if (stageRadius <= playerDis)
	{
		player->SetPosition(Phoenix::Math::Vector3(playerNormal.x * stageRadius, playerPos.y, playerNormal.y * stageRadius));

	}
}

void SceneGame::PushingOutEnemiesAndStage()
{
	Phoenix::Math::Vector3 playerPos = player->GetPosition();

	for (const auto& enemy : enemyManager->GetEnemies())
	{
		if (!enemy->GetAlive()) continue;

		Phoenix::Math::Vector3 enemyPos = enemy->GetPosition();
		Phoenix::f32 bossDis = Phoenix::Math::Vector2Length(Phoenix::Math::Vector2(enemyPos.x, enemyPos.z));
		Phoenix::Math::Vector2 enemyNormal = Phoenix::Math::Vector2Normalize(Phoenix::Math::Vector2(enemyPos.x, enemyPos.z));
		if (stageRadius <= bossDis)
		{
			enemy->SetTranslate(Phoenix::Math::Vector3(enemyNormal.x * stageRadius, enemyPos.y, enemyNormal.y * stageRadius));

			if (SphereVsSphere(Phoenix::Math::Vector3(playerPos.x, playerPos.y + 0.5f, playerPos.z), Phoenix::Math::Vector3(enemyPos.x, enemyPos.y + 0.5f, enemyPos.z), player->GetRadius(), enemy->GetRadius()))
			{
#if 1
				Phoenix::Math::Vector3 pos;
				Phoenix::Math::Vector3 dir = player->GetPosition() - enemy->GetPosition();
				dir = Phoenix::Math::Vector3Normalize(dir);
				dir.y = 0.0f;

				pos = Phoenix::Math::Vector3(enemyPos.x, playerPos.y, enemyPos.z) + dir * (player->GetRadius() + enemy->GetRadius());
				player->SetPosition(pos);
				player->UpdateTrasform();
#else
				Phoenix::Math::Vector3 pos;
				Phoenix::Math::Vector3 dir = enemy->GetPosition() - player->GetPosition();
				dir = Phoenix::Math::Vector3Normalize(dir);
				dir.y = 0.0f;

				pos = Phoenix::Math::Vector3(playerPos.x, enemyPos.y, playerPos.z) + dir * (player->GetRadius() + enemy->GetRadius());
				enemy->SetTranslate(pos);
				enemy->UpdateTrasform();
#endif
			}
		}
	}
}

void SceneGame::JudgeHitPlayerAndEnemies()
{
	Phoenix::s32 index = -1;
	Phoenix::Math::Vector3 playerPos = player->GetPosition();

	for (const auto& enemy : enemyManager->GetEnemies())
	{
		++index;

		if (!enemy->GetAlive()) continue;

		Phoenix::Math::Vector3 enemyPos = enemy->GetPosition();

		static bool playSE = true;
		static Phoenix::s32 hitCollisionIndex = -1;
		static Phoenix::s32 hitAttackPower = -1;
		static Phoenix::s32 stopCount = 0;

		if (hitCollisionIndex != -1 && hitAttackPower != -1)
		{
			if (15 <= stopCount++)
			{
				playSE = true;
				stopCount = 0;
				hitCollisionIndex = -1;
				hitAttackPower = -1;
			}
		}

		if (player->IsAttackJudgment().at(index) && !isHitStop && enemy->GetBattleState() != BattleEnemyState::Dedge)
		{
			const std::vector<Phoenix::FrameWork::CollisionData> playerDatas = player->GetCollisionDatas();
			const std::vector<Phoenix::FrameWork::CollisionData>* enemyDatas = enemy->GetCollisionDatas();

			Phoenix::Math::Vector3 cp1 = Phoenix::Math::Vector3(enemyDatas->at(0).pos.x, enemyDatas->at(0).pos.y - 1.0f, enemyDatas->at(0).pos.z);
			Phoenix::Math::Vector3 cp2 = Phoenix::Math::Vector3(enemyDatas->at(0).pos.x, enemyDatas->at(0).pos.y + 1.0f, enemyDatas->at(0).pos.z);

			//if (SphereVsSphere(playerDatas.at(player->GetAttackCollisionIndex()).pos, enemyDatas->at(0).pos, playerDatas.at(player->GetAttackCollisionIndex()).radius, enemyDatas->at(0).radius))
			if (SphereVsCapsule(playerDatas.at(player->GetAttackCollisionIndex()).pos, cp1, cp2, playerDatas.at(player->GetAttackCollisionIndex()).radius, enemyDatas->at(0).radius))
			{
				Phoenix::Math::Vector3 pos;
				Phoenix::Math::Vector3 normal;
				Phoenix::Math::Vector3 dir = Phoenix::Math::Vector3Normalize(playerDatas.at(player->GetAttackCollisionIndex()).pos - enemyDatas->at(0).pos);
				pos = enemyDatas->at(0).pos + dir * enemyDatas->at(0).radius / 2.0f;
				normal = Phoenix::Math::Vector3Normalize(playerPos - enemyPos); // playerDatas->at(player->GetAttackCollisionIndex()).pos

				player->SetIsHit(index);
				enemy->Damage(player->GetAttackDamage());

				// SE再生
				if (playSE)
				{
					if (player->GetAttackCollisionIndex() == 1 || player->GetAttackCollisionIndex() == 2)
					{
						if (player->GetAttackPower() == 0)
						{
							soundSystem->Play(SoundType::SE_Player_Attack_Punch_Hit_Right);
							playSE = false;
							hitCollisionIndex = player->GetAttackCollisionIndex();
							hitAttackPower = player->GetAttackPower();
						}
						else if (player->GetAttackPower() == 1)
						{
							soundSystem->Play(SoundType::SE_Player_Attack_Punch_Hit_Heavy);
							playSE = false;
							hitCollisionIndex = player->GetAttackCollisionIndex();
							hitAttackPower = player->GetAttackPower();
						}
					}
					else if (player->GetAttackCollisionIndex() == 3 || player->GetAttackCollisionIndex() == 4)
					{
						if (player->GetAttackPower() == 0)
						{
							soundSystem->Play(SoundType::SE_Player_Attack_Kick_Hit_Right);
							playSE = false;
							hitCollisionIndex = player->GetAttackCollisionIndex();
							hitAttackPower = player->GetAttackPower();
						}
						else if (player->GetAttackPower() == 1)
						{
							soundSystem->Play(SoundType::SE_Player_Attack_Kick_Hit_Heavy);
							playSE = false;
							hitCollisionIndex = player->GetAttackCollisionIndex();
							hitAttackPower = player->GetAttackPower();
						}
					}
				}

				// Burst Particle.
				{
					playerHitParticle->Burst(150);
					playerHitParticle->SetParticleLife(1.5f);
					playerHitParticle->SetParticleSize(0.004f);
					playerHitParticle->SetParticleScale(1.0f);
					playerHitParticle->SetParticleMotionBlurAmount(50.0f);
					playerHitParticle->SetParticleNormal(Phoenix::Math::Vector4(normal, 0.0f));
					playerHitParticle->SetParticleColor(Phoenix::Math::Color(245.0f / 255.0f, 69.0f / 255.0f, 33.0f / 255.0f, 1.0f)); // particleMainColor Phoenix::Math::Color(245.0f / 255.0f, 69.0f / 255.0f, 33.0f / 255.0f, 1.0f) // 40.0f / 255.0f, 40.0f / 255.0f, 148.0f / 255.0f, 1.0f
					//playerHitParticle->SetParticleColor(Phoenix::Math::Color(40.0f / 255.0f, 40.0f / 255.0f, 148.0f / 255.0f, 1.0f)); // particleMainColor Phoenix::Math::Color(245.0f / 255.0f, 69.0f / 255.0f, 33.0f / 255.0f, 1.0f) // 40.0f / 255.0f, 40.0f / 255.0f, 148.0f / 255.0f, 1.0f
					//playerHitParticle->SetParticleColor(Phoenix::Math::Color(33.0f / 255.0f, 245.0f / 255.0f, 148.0f / 255.0f, 1.0f)); // particleMainColor Phoenix::Math::Color(245.0f / 255.0f, 69.0f / 255.0f, 33.0f / 255.0f, 1.0f) // 40.0f / 255.0f, 40.0f / 255.0f, 148.0f / 255.0f, 1.0f

					//for (Phoenix::s32 i = 0; i < 3; ++i)
					//{
					//	dusterParticle[i]->Burst(1);
					//	dusterParticle[i]->SetParticleLife(1.0f);
					//	dusterParticle[i]->SetParticleSize(0.5f);
					//	dusterParticle[i]->SetParticleScale(0.5f);
					//	dusterParticle[i]->SetParticleNormal(Phoenix::Math::Vector4(0.0f, 0.0f, 0.0f, 0.0f));
					//	dusterParticle[i]->SetParticleColor(Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f)); // Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f)
					//}
					//dusterParticle[0]->SetParticleRotate(0.025f);
					//dusterParticle[1]->SetParticleRotate(-0.015f);
					//dusterParticle[2]->SetParticleRotate(0.005f);

					particlePos = (enemyPos + playerDatas.at(player->GetAttackCollisionIndex()).pos) * 0.5f;
					particlePos.y = playerDatas.at(player->GetAttackCollisionIndex()).pos.y;
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
					hitStopCnt = 0.0f;
				}

				// Set Camera Shake
				{
					if (!isCameraShake)
					{
						if (player->GetAttackPower() == 0)
						{
							isCameraShake = true;
							shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
							cameraShakeCnt = 0.0f;

							shakeWidth = 0.0f;
							shakeHeight = 0.05f;

							cameraShakeMaxCnt = 7.0f;

							SetXInputVibration(1.0f, 0.0f, 5.0f);
						}
						else if (player->GetAttackPower() == 1)
						{
							isCameraShake = true;
							shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
							cameraShakeCnt = 0.0f;

							shakeWidth = 0.0f;
							shakeHeight = 0.25f;

							cameraShakeMaxCnt = 10.0f;

							SetXInputVibration(1.0f, 1.0f, 10.0f);
						}
					}
				}
			}
		}

		if (enemy->IsAttackJudgment() && !player->Invincible() && !isHitStop && !player->IsJustDedge())
		{
			const std::vector<Phoenix::FrameWork::CollisionData> playerDatas = player->GetCollisionDatas();
			const std::vector<Phoenix::FrameWork::CollisionData>* enemyDatas = enemy->GetCollisionDatas();

			Phoenix::Math::Vector3 cp1 = Phoenix::Math::Vector3(playerDatas.at(0).pos.x, playerDatas.at(0).pos.y - 1.0f, playerDatas.at(0).pos.z);
			Phoenix::Math::Vector3 cp2 = Phoenix::Math::Vector3(playerDatas.at(0).pos.x, playerDatas.at(0).pos.y + 1.0f, playerDatas.at(0).pos.z);

			//if (SphereVsSphere(playerDatas.at(player->GetAttackCollisionIndex()).pos, enemyDatas->at(0).pos, playerDatas.at(player->GetAttackCollisionIndex()).radius, enemyDatas->at(0).radius))
			if (SphereVsCapsule(enemyDatas->at(enemy->GetAttackCollisionIndex()).pos, cp1, cp2, enemyDatas->at(enemy->GetAttackCollisionIndex()).radius, playerDatas.at(0).radius))
			{
				Phoenix::Math::Vector3 normal;
				normal = Phoenix::Math::Vector3Normalize(enemyPos - playerPos);

				// ジャスト回避成功
				if (player->OnJustDedge())
				{
					isSlow = true;
					player->SetIsJustDedge(true);
					//player->ChangeJustDedge();
				}
				// ジャスト回避失敗
				else
				{
					// 回避失敗
					if (!player->Damage(10, enemy->GetAttackPower()))
					{
						enemy->SetIsHit(true);

						// SE再生
						if (enemy->GetAttackCollisionIndex() == 1 || enemy->GetAttackCollisionIndex() == 2)
						{
							if (enemy->GetAttackPower() == 0)
							{
								soundSystem->Play(SoundType::SE_Player_Attack_Punch_Hit_Right, false, true);
							}
							else if (enemy->GetAttackPower() == 1)
							{
								soundSystem->Play(SoundType::SE_Player_Attack_Punch_Hit_Heavy, false, true);
							}
						}
						else if (enemy->GetAttackCollisionIndex() == 3 || enemy->GetAttackCollisionIndex() == 4)
						{
							if (enemy->GetAttackPower() == 0)
							{
								soundSystem->Play(SoundType::SE_Player_Attack_Kick_Hit_Right, false, true);
							}
							else if (enemy->GetAttackPower() == 1)
							{
								soundSystem->Play(SoundType::SE_Player_Attack_Kick_Hit_Heavy, false, true);
							}
						}

						// Burst Particle.
						{
							bossHitParticle->Burst(150);
							bossHitParticle->SetParticleLife(1.5f);
							bossHitParticle->SetParticleSize(0.004f);
							bossHitParticle->SetParticleScale(1.0f);
							bossHitParticle->SetParticleMotionBlurAmount(50.0f);
							bossHitParticle->SetParticleNormal(Phoenix::Math::Vector4(normal, 0.0f));
							//bossHitParticle->SetParticleColor(Phoenix::Math::Color(245.0f / 255.0f, 69.0f / 255.0f, 33.0f / 255.0f, 1.0f)); // particleMainColor Phoenix::Math::Color(245.0f / 255.0f, 69.0f / 255.0f, 33.0f / 255.0f, 1.0f) // 40.0f / 255.0f, 40.0f / 255.0f, 148.0f / 255.0f, 1.0f
							bossHitParticle->SetParticleColor(Phoenix::Math::Color(100.0f / 255.0f, 69.0f / 255.0f, 33.0f / 255.0f, 1.0f)); // particleMainColor Phoenix::Math::Color(245.0f / 255.0f, 69.0f / 255.0f, 33.0f / 255.0f, 1.0f) // 40.0f / 255.0f, 40.0f / 255.0f, 148.0f / 255.0f, 1.0f
							//bossHitParticle->SetParticleColor(Phoenix::Math::Color(40.0f / 255.0f, 40.0f / 255.0f, 148.0f / 255.0f, 1.0f)); // particleMainColor Phoenix::Math::Color(245.0f / 255.0f, 69.0f / 255.0f, 33.0f / 255.0f, 1.0f) // 40.0f / 255.0f, 40.0f / 255.0f, 148.0f / 255.0f, 1.0f
							//bossHitParticle->SetParticleColor(Phoenix::Math::Color(33.0f / 255.0f, 245.0f / 255.0f, 148.0f / 255.0f, 1.0f)); // particleMainColor Phoenix::Math::Color(245.0f / 255.0f, 69.0f / 255.0f, 33.0f / 255.0f, 1.0f) // 40.0f / 255.0f, 40.0f / 255.0f, 148.0f / 255.0f, 1.0f

							bossHitParticlePos = enemyPos + ((playerPos - enemyPos) * 0.5f);
							bossHitParticlePos.y = enemyDatas->at(enemy->GetAttackCollisionIndex()).pos.y;
						}

						// Set Camera Shake
						{
							if (!isCameraShake)
							{
								if (enemy->GetAttackPower() == 0)
								{
									isCameraShake = true;
									shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
									cameraShakeCnt = 0;

									shakeWidth = 0.0f;
									shakeHeight = 0.075f;

									cameraShakeMaxCnt = 7;

									SetXInputVibration(1.0f, 0.0f, 5.0f);
								}
								else if (enemy->GetAttackPower() == 1)
								{
									isCameraShake = true;
									shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
									cameraShakeCnt = 0;

									shakeWidth = 0.0f;
									shakeHeight = 0.35f;

									cameraShakeMaxCnt = 10;

									SetXInputVibration(1.0f, 1.0f, 10.0f);
								}
							}
						}
					}
				}
			}
		}
	}
}

void SceneGame::StartUpGUI()
{
	if (GetKeyState(VK_SHIFT) < 0)
	{
		if (GetKeyState('1') < 0)
		{
			onPlayerEditor = true;
		}
	}
}

void SceneGame::SwitchShader()
{
	if (isPBR)
	{
		currentShader = pbrShader;
	}
	else
	{
		currentShader = basicSkinShader;
	}
}

void SceneGame::NoticeMetaAI()
{
	playerBehaviorScore = player->GetScore();

#if 0
	if (oldPlayerBehaviorScore < playerBehaviorScore)
	{
		metaAI->Sensor(static_cast<Phoenix::s32>(MetaType::Battle), playerBehaviorScore);

		// 過去のスコア保存
		oldPlayerBehaviorScore = playerBehaviorScore;
	}
	else if (playerBehaviorScore - oldPlayerBehaviorScore <= -10)
	{
		metaAI->Sensor(static_cast<Phoenix::s32>(MetaType::Battle), playerBehaviorScore);

		// 過去のスコア保存
		oldPlayerBehaviorScore = playerBehaviorScore;
	}
#else
	metaAI->Sensor(static_cast<Phoenix::s32>(MetaType::Battle), playerBehaviorScore);

	// 過去のスコア保存
	oldPlayerBehaviorScore = playerBehaviorScore;
#endif
}

void SceneGame::Draw(Phoenix::f32 elapsedTime)
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

//	ibl->Clear(graphicsDevice);
//	ibl->Activate(graphicsDevice);
//	{
//		// Draw skymap.
//		{
//			Phoenix::FrameWork::LightState* light = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetLight();
//			Phoenix::Math::Color color = { 1.0f, 1.0f, 1.0f, 1.0f };
//			float skyDimension = 50000;
//			// ワールド行列を作成
//			Phoenix::Math::Matrix skyWorldM;
//			{
//				Phoenix::Math::Vector3 scale = { skyDimension, skyDimension, skyDimension };
//				Phoenix::Math::Vector3 rotate = { 0.0f, 90.0f * 0.01745f, 0.0f };
//				Phoenix::Math::Vector3 translate = { 0.0f, 0.0f, 0.0f };
//
//				Phoenix::Math::Matrix S, R, T;
//				S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
//				R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
//				T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);
//
//				skyWorldM = S * R * T;
//			}
//			skyMap->Draw(graphicsDevice, skyWorldM, *camera, light->direction, color);
//		}
//
//		// Draw stage.
//		{
//			// ワールド行列を作成
//			Phoenix::Math::Matrix W;
//			{
//				Phoenix::Math::Vector3 scale = { 40.0f, 40.0f, 40.0f };
//				Phoenix::Math::Vector3 rotate = { 0.0f, 0.0f, 0.0f };
//				Phoenix::Math::Vector3 translate = { 0.0f, 0.0f, 0.0f };
//
//				Phoenix::Math::Matrix S, R, T;
//				S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
//				R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
//				T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);
//
//				W = S * R * T;
//			}
//#if 1
//			basicShader->Begin(graphicsDevice, *camera);
//			basicShader->Draw(graphicsDevice, W, stageModel);
//			basicShader->End(graphicsDevice);
//#else
//			standardShader->Begin(graphicsDevice, camera);
//			standardShader->Draw(graphicsDevice, W, stageModel);
//			standardShader->End(graphicsDevice);
//#endif
//		}
//	}
//	ibl->Deactivate(graphicsDevice);

	Phoenix::Graphics::Viewport* v = new Phoenix::Graphics::Viewport();
	context->GetViewports(1, &v);
	float aspectRatio = v->width / v->height;
	Phoenix::FND::SafeDelete(v);

	// Generate IBL
	/*{
		ibl->Clear(graphicsDevice);
		ibl->Activate(graphicsDevice);
		{

		}
		ibl->Deactivate(graphicsDevice);
	}*/

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
						//currentShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
						for (const auto& enemy : enemyManager->GetEnemies())
						{
							if (enemy->GetEnable())
							{
								currentShader->Draw(graphicsDevice, enemy->GetWorldMatrix(), enemy->GetModel());
							}
						}
						currentShader->Draw(graphicsDevice, player->GetWorldMatrix(), player->GetModel());
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
					basicShader->Draw(graphicsDevice, W, bossStageModel);
				}
				voidPS->DeactivatePS(graphicsDevice->GetDevice());
				basicShader->End(graphicsDevice);
			}
		}
		shadowMap->Deactivate(graphicsDevice);
	}

	// Generate VelocityMap
	{
		motionBlur->ActivateVelocity(graphicsDevice);
		{
			// Draw player and enemies.
			//{
			//	if (currentShader)
			//	{
			//		currentShader->Begin(graphicsDevice, *camera);
			//		motionBlur->ActivateVelocityPS(graphicsDevice);
			//		{
			//			//currentShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
			//			for (const auto& enemy : enemyManager->GetEnemies())
			//			{
			//				if (enemy->GetEnable())
			//				{
			//					currentShader->Draw(graphicsDevice, enemy->GetWorldMatrix(), enemy->GetModel());
			//				}
			//			}
			//			currentShader->Draw(graphicsDevice, player->GetWorldMatrix(), player->GetModel());
			//		}
			//		motionBlur->DeactivateVelocityPS(graphicsDevice);
			//		currentShader->End(graphicsDevice);
			//	}
			//}

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

				basicShader->Begin(graphicsDevice, *camera);
				motionBlur->ActivateVelocityPS(graphicsDevice);
				{
					basicShader->Draw(graphicsDevice, W, bossStageModel);
				}
				motionBlur->DeactivateVelocityPS(graphicsDevice);
				basicShader->End(graphicsDevice);
			}
		}
		motionBlur->DeactivateVelocity(graphicsDevice);
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
					//Phoenix::Math::Vector3 scale = { 175.0f, 175.0f, 175.0f };
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
				basicShader->Draw(graphicsDevice, W, bossStageModel);
				basicShader->End(graphicsDevice);
#elif 0
				standardShader->Begin(graphicsDevice, camera);
				standardShader->Draw(graphicsDevice, W, bossStageModel);
				standardShader->End(graphicsDevice);
#else
				pbrSkinShader->Begin(graphicsDevice, *camera);
				pbrSkinShader->Draw(graphicsDevice, W, bossStageModel);
				pbrSkinShader->End(graphicsDevice);
#endif
			}

			// Draw player and boss.
			{
#if 0
				// メッシュ描画
#if 1
				basicSkinShader->Begin(graphicsDevice, *camera);
				//basicSkinShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
				// エフェクト描画
				{
					//commonData->renderer->BeginRendering();
					//commonData->manager->Draw();
					//commonData->renderer->EndRendering();
				}
				//basicSkinShader->Draw(graphicsDevice, player->GetWorldMatrix(), player->GetModel());
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

				pbrShader->Begin(graphicsDevice, *camera);
				pbrShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
				pbrShader->Draw(graphicsDevice, player->GetWorldMatrix(), player->GetModel());
				pbrShader->End(graphicsDevice);
#else
				if (currentShader)
				{
					/*
					currentShader->Begin(graphicsDevice, *camera);
					{
						pbrDissolvePixelShader->Activate(graphicsDevice->GetDevice());
						{
							// ピクセルシェーダー用バッファ更新
							{
								DissolveCB cb = {};
								{
									cb.dissolveThreshold = dissolveThreshold;
									cb.dissolveEmissiveWidth = dissolveEmissiveWidth;
									cb.dummy[0] = 0.0f;
									cb.dummy[1] = 0.0f;
								}
								Phoenix::Graphics::IBuffer* psCBuffer[] =
								{
									dissolveCB.get()
								};
								context->UpdateSubresource(dissolveCB.get(), 0, 0, &cb, 0, 0);
								context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Pixel, 2, Phoenix::FND::ArraySize(psCBuffer), psCBuffer);
							}

							// ピクセルシェーダー用テクスチャ更新
							{
								Phoenix::Graphics::ITexture* texture[] =
								{
									bossRedTexture.get(),
									dissolveTexture.get(),
									emissiveTexture.get()
								};
								context->SetShaderResources(Phoenix::Graphics::ShaderType::Pixel, 6, 3, texture);
							}

							// Draw.
							{
								currentShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
							}

							// ピクセルシェーダー用テクスチャ更新
							{
								Phoenix::Graphics::ITexture* texture[] =
								{
									nullptr,
									nullptr,
									nullptr
								};
								context->SetShaderResources(Phoenix::Graphics::ShaderType::Pixel, 6, 3, texture);
							}

							// ピクセルシェーダー用バッファ更新
							{
								Phoenix::Graphics::IBuffer* psCBuffer[] =
								{
									nullptr
								};
								context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Pixel, 2, Phoenix::FND::ArraySize(psCBuffer), psCBuffer);
							}
						}
						pbrDissolvePixelShader->Deactivate(graphicsDevice->GetDevice());
					}
					currentShader->End(graphicsDevice);
					*/

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
							/*for (Phoenix::s32 i = 0; i < 3; ++i)
							{
								dusterParticle[i]->Draw(graphicsDevice, *camera);
							}*/
						}
						context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
					}

					currentShader->Begin(graphicsDevice, *camera);
					for (const auto& enemy : enemyManager->GetEnemies())
					{
						if (enemy->GetEnable())
						{
							currentShader->Draw(graphicsDevice, enemy->GetWorldMatrix(), enemy->GetModel());
						}
					}
					currentShader->Draw(graphicsDevice, player->GetWorldMatrix(), player->GetModel());
					currentShader->End(graphicsDevice);
				}
#endif
			}

			// Draw collision primitive.
			if (isHitCollision)
			{
				Phoenix::Graphics::DeviceDX11* device = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());

				Phoenix::Graphics::ContextDX11* contextDX11 = static_cast<Phoenix::Graphics::ContextDX11*>(context);
				context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::Opaque), 0, 0xFFFFFFFF);

				Phoenix::s32 cnt = 0;
				for(const auto& data : player->GetCollisionDatas())
				{
					if (cnt++ == 0)
					{
						CylinderPrimitiveRender
						(
							device,
							Phoenix::Math::Vector3(data.pos.x, data.pos.y - 1.0f, data.pos.z),
							Phoenix::Math::Vector3(data.pos.x, data.pos.y + 1.0f, data.pos.z),
							Phoenix::Math::Vector3(data.pos.x, data.pos.y, data.pos.z),
							Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f),
							Phoenix::Math::Vector3(data.radius, data.radius, data.radius),
							Phoenix::Math::Vector3(data.radius, data.radius + 0.5f, data.radius)
						);
					}
					else
					{
						PrimitiveRender(device, data.pos, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), Phoenix::Math::Vector3(data.radius, data.radius, data.radius));
					}
				}

				for (const auto& enemy : enemyManager->GetEnemies())
				{
					Phoenix::s32 cnt = 0;
					for (const auto& data : *enemy->GetCollisionDatas())
					{
						if (enemy->GetEnable())
						{
							if (cnt++ == 0)
							{
								CylinderPrimitiveRender
								(
									device, 
									Phoenix::Math::Vector3(data.pos.x, data.pos.y - 1.0f, data.pos.z),
									Phoenix::Math::Vector3(data.pos.x, data.pos.y + 1.0f, data.pos.z),
									Phoenix::Math::Vector3(data.pos.x, data.pos.y, data.pos.z),
									Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f),
									Phoenix::Math::Vector3(data.radius, data.radius, data.radius),
									Phoenix::Math::Vector3(data.radius, data.radius + 0.5f, data.radius)
								);
							}
							else
							{
								PrimitiveRender(device, data.pos, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), Phoenix::Math::Vector3(data.radius, data.radius, data.radius));
							}
						}
					}
				}

				PrimitiveRender(device, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), Phoenix::Math::Vector3(stageRadius, stageRadius, stageRadius));

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

	frameBuffer[3]->Clear(graphicsDevice, 0, 0.5f, 0.5f, 0.5f, 1.0f);
	frameBuffer[3]->Activate(graphicsDevice);
	{
		// Blend Bloom.
		if (bloomBlend)
		{
			resolvedFramebuffer = 2;
			frameBuffer[resolvedFramebuffer]->Activate(graphicsDevice);
			{
				bloom->Blend(graphicsDevice, frameBuffer[0]->GetRenderTargetSurface()->GetTexture(), frameBuffer[1]->GetRenderTargetSurface()->GetTexture());
			}
			frameBuffer[resolvedFramebuffer]->Deactivate(graphicsDevice);

			quad->Draw(graphicsDevice, frameBuffer[resolvedFramebuffer]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f, static_cast<Phoenix::f32>(display->GetWidth()), static_cast<Phoenix::f32>(display->GetHeight()));

			//toneMap->Draw(graphicsDevice, frameBuffer[resolvedFramebuffer]->renderTargerSurface[0]->GetTexture(), elapsedTime);
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

			quad->Draw(graphicsDevice, commonData->operatorUI.get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f), 0.0f, 1.0f, 1.0f, 1.0f,1.0f, true, true, true , true, true, true);

			/*float w = 480.0f * 0.5f;
			float h = 270.0f * 0.5f;
			quad->Draw(graphicsDevice, commonData->avoidUI.get(), Phoenix::Math::Vector2(w * 0.0f, 1080.0f - h), Phoenix::Math::Vector2(w, h), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(480.0f, 270.0f));
			quad->Draw(graphicsDevice, commonData->attackUI.get(), Phoenix::Math::Vector2(w * 1.0f, 1080.0f - h), Phoenix::Math::Vector2(w, h), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(480.0f, 270.0f));
			quad->Draw(graphicsDevice, commonData->runUI.get(), Phoenix::Math::Vector2(w * 2.0f, 1080.0f - h), Phoenix::Math::Vector2(w, h), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(480.0f, 270.0f));
			quad->Draw(graphicsDevice, commonData->targetUI.get(), Phoenix::Math::Vector2(w * 3.0f, 1080.0f - h), Phoenix::Math::Vector2(w, h), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(480.0f, 270.0f));*/
#endif
		}
	}
	frameBuffer[3]->Deactivate(graphicsDevice);

	// Screen Filter
	frameBuffer[4]->Clear(graphicsDevice, 0, 0.5f, 0.5f, 0.5f, 1.0f);
	frameBuffer[4]->Activate(graphicsDevice);
	{
		quad->SetBright(bright);
		quad->SetContrast(contrast);
		quad->SetSaturate(saturate);
		quad->SetScreenColor(screenColor);

		quad->Draw(graphicsDevice, frameBuffer[3]->renderTargerSurface[0]->GetTexture(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, true, true, true, true, true, true);
	}
	frameBuffer[4]->Deactivate(graphicsDevice);

	// Motion Blur
	resolvedFramebuffer = 4;
	if (isMotionBlur)
	{
		resolvedFramebuffer = 5;
		frameBuffer[resolvedFramebuffer]->Clear(graphicsDevice, 0, 0.5f, 0.5f, 0.5f, 1.0f);
		frameBuffer[resolvedFramebuffer]->Activate(graphicsDevice);
		{
			motionBlur->Draw(graphicsDevice, frameBuffer[4]->renderTargerSurface[0]->GetTexture(), *camera, true);
		}
		frameBuffer[resolvedFramebuffer]->Deactivate(graphicsDevice);
	}

	// Final Draw
	{
		quad->Draw(graphicsDevice, frameBuffer[resolvedFramebuffer]->renderTargerSurface[0]->GetTexture(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f));
	}

	// Draw frameBuffer Texture.
	{
		if (active[0]) quad->Draw(graphicsDevice, shadowMap->GetDepthStencilSurface()->GetTexture(), texSize.x * 0, 0, texSize.x, texSize.y);
		//if (active[0]) quad->Draw(graphicsDevice, frameBuffer[0]->renderTargerSurface[0]->GetTexture(), texSize.x * 0, 0, texSize.x, texSize.y);
		if (active[1]) quad->Draw(graphicsDevice, frameBuffer[1]->renderTargerSurface[0]->GetTexture(), texSize.x * 1, 0, texSize.x, texSize.y);
		if (active[2]) quad->Draw(graphicsDevice, frameBuffer[1]->depthStencilSurface->GetTexture(), texSize.x * 2, 0, texSize.x, texSize.y);
		if (active[3]) quad->Draw(graphicsDevice, frameBuffer[0]->renderTargerSurface[0]->GetTexture(), texSize.x * 3, 0, texSize.x, texSize.y);
		if (active[4]) quad->Draw(graphicsDevice, motionBlur->GetVelocityFrameBuffer()->renderTargerSurface[0]->GetTexture(), texSize.x * 4, 0, texSize.x, texSize.y);
		/*if (active[3]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[0]->GetTexture(), texSize.x * 3, 0, texSize.x, texSize.y);
		if (active[4]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[1]->GetTexture(), texSize.x * 4, 0, texSize.x, texSize.y);
		if (active[5]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[2]->GetTexture(), texSize.x * 5, 0, texSize.x, texSize.y);
		if (active[6]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[3]->GetTexture(), texSize.x * 6, 0, texSize.x, texSize.y);
		if (active[7]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[4]->GetTexture(), texSize.x * 7, 0, texSize.x, texSize.y);
		if (active[8]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[5]->GetTexture(), texSize.x * 8, 0, texSize.x, texSize.y);*/
	}

	// 前フレームとしてカメラ情報を保存
	{
		context->UpdateConstantBufferPrevScene(camera->GetView(), camera->GetProjection());
	}
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

void SceneGame::CylinderPrimitiveRender(Phoenix::Graphics::DeviceDX11* device, Phoenix::Math::Vector3 cp1Translate, Phoenix::Math::Vector3 cp2Translate, Phoenix::Math::Vector3 cyilinderTranslate, Phoenix::Math::Vector3 rotate, Phoenix::Math::Vector3 scale, Phoenix::Math::Vector3 cyilinderScale)
{
	// Sphere1
	{
		PrimitiveRender(device, cp1Translate, rotate, scale);
	}

	// Sphere2
	{
		PrimitiveRender(device, cp2Translate, rotate, scale);
	}

	// Cylinder
	{
		// ワールド行列を作成
		Phoenix::Math::Matrix W;
		{
			Phoenix::Math::Matrix S, R, T;
			S = Phoenix::Math::MatrixScaling(cyilinderScale.x, cyilinderScale.y, cyilinderScale.z);
			R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
			T = Phoenix::Math::MatrixTranslation(cyilinderTranslate.x, cyilinderTranslate.y, cyilinderTranslate.z);

			W = S * R * T;
		}

		cylinderPrimitive->Render
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

Phoenix::Math::Vector3 SceneGame::WorldToScreen(const Phoenix::Math::Vector3& worldPosition)
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

void SceneGame::GUI()
{
#if	defined(PHOENIX_TARGET_DEBUG)
	ImGui::Begin("Game");
	{
		if (ImGui::Button("Open Player Editor"))
		{
			Phoenix::OS::StartUp(L"..\\Tool\\PlayerEditor.exe");
			isPlayerUpdate = false;
			isBossUpdate = false;
		}
		ImGui::Checkbox("Update", &isUpdate);
		ImGui::Checkbox("PlayerUpdate", &isPlayerUpdate);
		ImGui::Checkbox("BossUpdate", &isBossUpdate);
		ImGui::Checkbox("inTerritory", &inTerritory);
		ImGui::Checkbox("isSlow", &isSlow);
		{
			player->GUI();
		}
		{
			//boss->GUI();
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
		if (ImGui::TreeNode("Dissolve"))
		{
			ImGui::DragFloat("Threshold", &dissolveThreshold, 0.01f);
			ImGui::DragFloat("EmissiveWidth", &dissolveEmissiveWidth, 0.01f);
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
		if (ImGui::TreeNode("ScreenFilter"))
		{
			ImGui::DragFloat("bright", &bright, 0.1f);
			ImGui::DragFloat("contrast", &contrast, 0.1f);
			ImGui::DragFloat("saturate", &saturate, 0.1f);
			ImGui::ColorEdit4("screenColor", screenColor);

			quad->SetBright(bright);
			quad->SetContrast(contrast);
			quad->SetSaturate(saturate);
			quad->SetScreenColor(screenColor);

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
			ImGui::DragFloat("shakeMaxCnt", &vibrationMaxCnt);

			ImGui::TreePop();
		}
	}
	ImGui::End();
#else
	if (onPlayerEditor)
	{
		ImGui::Begin("Game", &onPlayerEditor);
		{
			if (ImGui::Button("Open Player Editor"))
			{
				Phoenix::OS::StartUp(L"..\\Tool\\PlayerEditor.exe");
				isPlayerUpdate = false;
				isBossUpdate = false;
			}
			ImGui::Checkbox("Update", &isUpdate);
			ImGui::Checkbox("PlayerUpdate", &isPlayerUpdate);
			ImGui::Checkbox("BossUpdate", &isBossUpdate);
		}
		ImGui::End();
	}
#endif
}