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


void SceneGame::Init(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	display = sceneSystem->GetDisplay();
	graphicsDevice = sceneSystem->GetGraphicsDevice();
	commonData = sceneSystem->GetSceneCommonData();

	// デバック用
	{
		Phoenix::Graphics::DeviceDX11* device = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());
		primitive = std::make_shared<GeometricPrimitive>(device->GetD3DDevice(), 1);

		texSize = Phoenix::Math::Vector2(256.0f, 256.0f);

		cameraFlg = false;
		lockOnCamera = false;
		isHitCollision = false;
		isUpdate = false;
		enableMSAA = true;
		shadowBlend = false;
		bloomBlend = true;
		isPBR = true;

		for (int i = 0; i < 10; ++i)
		{
			active[i] = false;
		}
	}

	// 共通データのアドレス取得
	{
		player = commonData->player.get();
		boss = commonData->boss.get();
		uiSystem = commonData->uiSystem.get();
		stageModel = commonData->stageModel.get();
		basicShader = commonData->basicShader.get();
		basicSkinShader = commonData->basicSkinShader.get();
		standardShader = commonData->standardShader.get();
		pbrShader = commonData->pbrShader.get();
		pbrSkinShader = commonData->pbrSkinShader.get();
		camera = commonData->camera.get();
		targetMark = commonData->targetMark.get();
	}

	// 共通データの初期化
	{
		camera->SetEye(Phoenix::Math::Vector3(0.0f, 0.0f, 10.0f));
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
		quad->Initialize(graphicsDevice);

		msaaResolve = Phoenix::FrameWork::MSAAResolve::Create();
		msaaResolve->Initialize(graphicsDevice);

		bloom = Phoenix::FrameWork::Bloom::Create();
		bloom->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight());
	}

	// スカイマップ
	{
		skyMap = Phoenix::FrameWork::SkyMap::Create();
		skyMap->Initialize(graphicsDevice, "..\\Data\\Assets\\Texture\\SkyMap\\AllSkyFree\\Epic_BlueSunset\\Epic_BlueSunset03.dds");
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

	{
		// エフェクトの読込
		//auto effect = Effekseer::Effect::Create(manager, EFK_EXAMPLE_ASSETS_DIR_U16 "Laser01.efk");
		//effect = Effekseer::Effect::Create(commonData->manager, u"D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Effect\\Examples\\Resources\\Laser01.efk");
		//hitEffect = Effekseer::Effect::Create(commonData->manager, u"D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Effect\\Examples\\MAGICALxSPIRAL\\HitEffect06.efk");

		// エフェクトの再生
		//hitEffectHandle = commonData->manager->Play(hitEffect, 0, 0, 0);
		//commonData->manager->SetPaused(hitEffectHandle, false);

		//Phoenix::Graphics::TextureDesc desc = {};

		//skyMap = Phoenix::Graphics::ITexture::Create();
		//skyMap->Initialize(graphicsDevice->GetDevice(), "D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Texture\\SkyMap\\skybox1.dds", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color::White);
		//skyMap->Initialize(graphicsDevice->GetDevice(), "D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Texture\\SkyMap\\AllSkyFree\\Cold Sunset\\Cold Sunset Equirect.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color::White);

		//DirectX::XMFLOAT3 s = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		//skyBox = std::make_shared<GeometricPrimitive>(device->GetD3DDevice(), 0, false, &s);

		//skyMapShader = Phoenix::FrameWork::SkyMapShader::Create();
		//skyMapShader->Initialize(graphicsDevice);
	}
}

void SceneGame::Update()
{
	// プレイヤー更新
	if (isUpdate)
	{
		player->Update(*camera);
	}

	// ボス更新
	if (isUpdate)
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
			const std::vector<Phoenix::FrameWork::CollisionData>* playerDatas = player->GetCollisionDatas();
			const std::vector<Phoenix::FrameWork::CollisionData>* bossDatas = boss->GetCollisionDatas();
			if (SphereVsSphere(playerDatas->at(player->GetAttackCollisionIndex()).pos, bossDatas->at(0).pos, playerDatas->at(player->GetAttackCollisionIndex()).radius, bossDatas->at(0).radius))
			{
				Phoenix::Math::Vector3 pos;
				Phoenix::Math::Vector3 dir = Phoenix::Math::Vector3Normalize(playerDatas->at(player->GetAttackCollisionIndex()).pos - bossDatas->at(0).pos);
				pos = bossDatas->at(0).pos + dir * bossDatas->at(0).radius / 2.0f;

				//hitEffectHandle = commonData->manager->Play(hitEffect, 0,0,0);
				//hitEffectHandle = commonData->manager->Play(hitEffect, pos.x, pos.y, pos.z);
				//hitEffectHandle = commonData->manager->Play(hitEffect, { pos.x, pos.y, pos.z }, 20);
				//commonData->manager->SetScale(hitEffectHandle, 50.0f, 50.0f, 50.0f);
				player->SetIsHit(true);
				boss->Damage(10);
			}
		}

		if (boss->IsAttackJudgment() && !player->Invincible())
		{
			const std::vector<Phoenix::FrameWork::CollisionData>* playerDatas = player->GetCollisionDatas();
			const std::vector<Phoenix::FrameWork::CollisionData>* bossDatas = boss->GetCollisionDatas();
			if (SphereVsSphere(playerDatas->at(0).pos, bossDatas->at(boss->GetAttackCollisionIndex()).pos, playerDatas->at(0).radius, bossDatas->at(boss->GetAttackCollisionIndex()).radius))
			{
				boss->SetIsHit(true);
				player->Damage(10);
			}
		}
	}

	// UI更新
	{
		player->UpdateUI();
		boss->UpdateUI();
	}

	// カメラ更新
	{
		if (xInput[0].bLBt)
		{
			lockOnCamera = !lockOnCamera;
		}

		if (cameraFlg)
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
			//camera.ControllerCamera(player->GetPosition(), Phoenix::Math::Vector3(0.0f, 100.0f, 0.0f));

			Phoenix::Math::Vector3 bossPos = boss->GetPosition();
			Phoenix::Math::Vector3 playerPos = player->GetPosition();

			if (lockOnCamera) camera->LockOnCamera(playerPos, bossPos, Phoenix::Math::Vector3(0.0f, 125.0f, 0.0f), Phoenix::Math::Vector3(0.0f, 185.0f, 0.0f));
			else camera->ControllerCamera(playerPos, Phoenix::Math::Vector3(0.0f, 125.0f, 0.0f));
		}
		//camera->Update();
	}

	Phoenix::FrameWork::LightState* light = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetLight();
	light->direction = Phoenix::Math::Vector4(-camera->GetFront(), 1.0f);

	if (isPBR)
	{
		currentShader = pbrShader;
	}
	else
	{
		currentShader = basicSkinShader;
	}

	// エフェクト更新
	/*{
	//	// 投影行列の更新 + カメラ行列の更新
	//	Phoenix::Math::Matrix projection = camera->GetProjection();
	//	Phoenix::Math::Matrix view = camera->GetView();

	//	::Effekseer::Matrix44 projectionMat;
	//	::Effekseer::Matrix44 viewMat;

	//	for (Phoenix::u32 i = 0; i < 4; ++i)
	//	{
	//		for (Phoenix::u32 j = 0; j < 4; ++j)
	//		{
	//			projectionMat.Values[i][j] = projection.m[i][j];
	//			viewMat.Values[i][j] = view.m[i][j];
	//		}
	//	}
	//	commonData->renderer->SetProjectionMatrix(projectionMat);
	//	commonData->renderer->SetCameraMatrix(viewMat);

	//	// 3Dサウンド用リスナー設定の更新
	//	//sound->SetListener(リスナー位置, 注目点, 上方向ベクトル);

	//	// 再生中のエフェクトの移動等(::Effekseer::Manager経由で様々なパラメーターが設定できます。)
	//	//commonData->manager->AddLocation(handle, ::Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

	//	// 全てのエフェクトの更新
	//	commonData->manager->Flip();
	//	commonData->manager->Update();
	//}*/
}

void SceneGame::Draw()
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
						currentShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
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
					Phoenix::Math::Vector3 scale = { 40.0f, 40.0f, 40.0f };
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
#elif 0
				standardShader->Begin(graphicsDevice, camera);
				standardShader->Draw(graphicsDevice, W, stageModel);
				standardShader->End(graphicsDevice);
#else
				pbrSkinShader->Begin(graphicsDevice, *camera);
				pbrSkinShader->Draw(graphicsDevice, W, stageModel);
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
					currentShader->Begin(graphicsDevice, *camera);
					currentShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
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

				for(const auto data : *player->GetCollisionDatas())
				{
					PrimitiveRender(device, data.pos, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), Phoenix::Math::Vector3(data.radius, data.radius, data.radius));
				}

				for(const auto data : *boss->GetCollisionDatas())
				{
					PrimitiveRender(device, data.pos, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), Phoenix::Math::Vector3(data.radius, data.radius, data.radius));
				}

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
		}

		bloom->Generate(graphicsDevice, frameBuffer[resolvedFramebuffer]->GetRenderTargetSurface()->GetTexture(), false);

		frameBuffer[resolvedFramebuffer]->Activate(graphicsDevice);
		{
			bloom->Draw(graphicsDevice);
		}
		frameBuffer[resolvedFramebuffer]->Deactivate(graphicsDevice);
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

		quad->Draw(graphicsDevice, frameBuffer[resolvedFramebuffer]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f, static_cast<Phoenix::f32>(display->GetWidth()), static_cast<Phoenix::f32>(display->GetHeight()));
	}
	else
	{
		quad->Draw(graphicsDevice, frameBuffer[0]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f, static_cast<Phoenix::f32>(display->GetWidth()), static_cast<Phoenix::f32>(display->GetHeight()));
	}

	// Draw UI and Effect.
	{
		if (lockOnCamera)
		{
			/*Phoenix::Graphics::ContextDX11* contextDX11 = static_cast<Phoenix::Graphics::ContextDX11*>(context);
			context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);*/

			Phoenix::f32 size = 128.0f / 4.0f;

			Phoenix::Math::Vector3 bossPos = boss->GetPosition();
			bossPos.y += 150.0f;

			Phoenix::Math::Vector3 screenPos = WorldToScreen(bossPos);
			screenPos.x -= size / 2.0f;

			quad->Draw(graphicsDevice, targetMark, screenPos.x, screenPos.y, size, size);
		}

		uiSystem->Draw(graphicsDevice);
	}
	
	// Draw frameBuffer Texture.
	{
		if (active[0]) quad->Draw(graphicsDevice, shadowMap->GetDepthStencilSurface()->GetTexture(), texSize.x * 0, 0, texSize.x, texSize.y);
		//if (active[0]) quad->Draw(graphicsDevice, frameBuffer[0]->renderTargerSurface[0]->GetTexture(), texSize.x * 0, 0, texSize.x, texSize.y);
		if (active[1]) quad->Draw(graphicsDevice, frameBuffer[1]->renderTargerSurface[0]->GetTexture(), texSize.x * 1, 0, texSize.x, texSize.y);
		if (active[2]) quad->Draw(graphicsDevice, frameBuffer[1]->depthStencilSurface->GetTexture(), texSize.x * 2, 0, texSize.x, texSize.y);
		/*if (active[3]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[0]->GetTexture(), texSize.x * 3, 0, texSize.x, texSize.y);
		if (active[4]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[1]->GetTexture(), texSize.x * 4, 0, texSize.x, texSize.y);
		if (active[5]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[2]->GetTexture(), texSize.x * 5, 0, texSize.x, texSize.y);
		if (active[6]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[3]->GetTexture(), texSize.x * 6, 0, texSize.x, texSize.y);
		if (active[7]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[4]->GetTexture(), texSize.x * 7, 0, texSize.x, texSize.y);
		if (active[8]) quad->Draw(graphicsDevice, ibl->GetFrameBuffer()->renderTargerSurface[5]->GetTexture(), texSize.x * 8, 0, texSize.x, texSize.y);*/
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
	ImGui::Begin("Game");
	{
		ImGui::Checkbox("Update", &isUpdate);
		player->GUI();
		boss->GUI();
		if (ImGui::TreeNode("Camera"))
		{
			ImGui::Checkbox("FreeCamera", &cameraFlg);
			ImGui::DragFloat("MoveSpeed", &camera->speed, 0.1f, 0.1f, 50.0f);
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
			ImGuiColorEditFlags flag = ImGuiColorEditFlags_Float; // 0 ~ 255表記ではなく、0.0 ~ 1.0表記にします。

			ImGui::Checkbox("OnPBR", &isPBR);
			ImGui::DragFloat4("dir", &light->direction.x);
			ImGui::DragFloat4("sunDir", &sunLight->direction.x, 0.01f);
			ImGui::DragFloat4("color", &light->color.x);
			ImGui::ColorEdit4("albedo", material->albedo, flag);
			ImGui::DragFloat("metallic", &material->metallic, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("roughness", &material->roughness, 0.01f, 0.0f, 1.0f);
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
	}
	ImGui::End();
}