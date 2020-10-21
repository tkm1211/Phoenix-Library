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


void SceneEvent::Construct(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	display = sceneSystem->GetDisplay();
	graphicsDevice = sceneSystem->GetGraphicsDevice();
	commonData = sceneSystem->GetSceneCommonData();

	// デバック用
	{
		Phoenix::Graphics::DeviceDX11* device = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());
		primitive = std::make_shared<GeometricPrimitive>(device->GetD3DDevice(), 1);
	}

	// 共通データのアドレス取得
	{
		player = commonData->player.get();
		boss = commonData->boss.get();
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
		skyMap->Initialize(graphicsDevice, "..\\Data\\Assets\\Texture\\SkyMap\\AllSkyFree\\Night MoonBurst\\NightMoonBurst.dds"); //Epic_BlueSunset\\Epic_BlueSunset03 //Night MoonBurst\\NightMoonBurst
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

	// Dissolve
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

	// Lua
	{
		luaSystem = Phoenix::FrameWork::LuaSystem::Create();
		luaSystem->Initialize(graphicsDevice);
	}
}

void SceneEvent::Initialize()
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
	}

	// 共通データの初期化
	{
		player->Initialize();
		boss->Initialize();

		boss->SetPosition(Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f));

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

	// Lua
	{
		luaSystem->LoadScript(std::string(Phoenix::OS::Path::GetFullPath("..\\Data\\Script\\EventScript\\BossViewEvent.lua")).c_str());

		luaSystem->BeginFunction("Start");
		{
			luaSystem->CallFunction(0);
		}
		luaSystem->EndFunction();
	}

	// Camera
	{
		Phoenix::Math::Vector3 focus =
		{
			luaSystem->GetGlobalValue<Phoenix::f32>("cameraTargetX"),
			luaSystem->GetGlobalValue<Phoenix::f32>("cameraTargetY"),
			luaSystem->GetGlobalValue<Phoenix::f32>("cameraTargetZ")
		};

		Phoenix::Math::Vector3 front =
		{
			0.0f,
			0.0f,
			0.0f
		};

		Phoenix::f32 rotateY = luaSystem->GetGlobalValue<Phoenix::f32>("rotateY");
		Phoenix::f32 len = luaSystem->GetGlobalValue<Phoenix::f32>("len");

		front.x = sinf(rotateY);
		front.z = cosf(rotateY);

		camera->InitEventCamera(focus, front, len);
	}
}

void SceneEvent::Update(Phoenix::f32 elapsedTime)
{
	static Phoenix::f32 targetY = 0.0f;

	bool onFade = sceneSystem->GetOnFade();
	if (runned && !onFade && luaSystem->GetGlobalValue<Phoenix::s32>("isEvent"))
	{
		luaSystem->BeginFunction("Run");
		{
			luaSystem->CallFunction(0);
		}
		luaSystem->EndFunction();

		Phoenix::Math::Vector3 focus =
		{
			luaSystem->GetGlobalValue<Phoenix::f32>("cameraTargetX"),
			luaSystem->GetGlobalValue<Phoenix::f32>("cameraTargetY"),
			luaSystem->GetGlobalValue<Phoenix::f32>("cameraTargetZ")
		};

		Phoenix::Math::Vector3 front =
		{
			0.0f,
			0.0f,
			0.0f
		};

		targetY = focus.y;

		Phoenix::f32 rotateY = luaSystem->GetGlobalValue<Phoenix::f32>("rotateY");
		Phoenix::f32 len = luaSystem->GetGlobalValue<Phoenix::f32>("len");

		front.x = sinf(rotateY);
		front.z = cosf(rotateY);

		camera->EventCamera(focus, front, len);
		camera->Update();

		boss->Update(false);
	}
}

void SceneEvent::Draw(Phoenix::f32 elapsedTime)
{
	/*commonData->renderer->BeginRendering();
	commonData->manager->Draw();
	commonData->renderer->EndRendering();*/

	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	Phoenix::f32 width = static_cast<Phoenix::f32>(display->GetWidth());
	Phoenix::f32 height = static_cast<Phoenix::f32>(display->GetHeight());

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

	// Work No_0 framebuffer.
	{
		frameBuffer[0]->Clear(graphicsDevice, 0, 0.5f, 0.5f, 0.5f, 1.0f);
		frameBuffer[0]->Activate(graphicsDevice);
		{
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
				if (currentShader)
				{
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
				}
			}
		}
		frameBuffer[0]->Deactivate(graphicsDevice);
	}

	// Generate Bloom Texture.
	Phoenix::u32 resolvedFramebuffer = 0;
	{
		bloom->Generate(graphicsDevice, frameBuffer[resolvedFramebuffer]->GetRenderTargetSurface()->GetTexture(), false);

		resolvedFramebuffer = 1;

		frameBuffer[resolvedFramebuffer]->Activate(graphicsDevice);
		{
			bloom->Draw(graphicsDevice);
		}
		frameBuffer[resolvedFramebuffer]->Deactivate(graphicsDevice);
	}

	// Blend Bloom.
	{
		resolvedFramebuffer = 2;
		frameBuffer[resolvedFramebuffer]->Activate(graphicsDevice);
		{
			bloom->Blend(graphicsDevice, frameBuffer[0]->GetRenderTargetSurface()->GetTexture(), frameBuffer[1]->GetRenderTargetSurface()->GetTexture());
		}
		frameBuffer[resolvedFramebuffer]->Deactivate(graphicsDevice);

		quad->Draw(graphicsDevice, frameBuffer[resolvedFramebuffer]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f, static_cast<Phoenix::f32>(display->GetWidth()), static_cast<Phoenix::f32>(display->GetHeight()));
	}
}

void SceneEvent::GUI()
{
	ImGui::Begin("EventScript");
	{
		ImGui::Checkbox("Run", &runned);

		static Phoenix::s8 valName[1024] = "";
		ImGui::InputText("Global Val", valName, 1024);
		if (valName[0] != NULL) ImGui::Text("%f", luaSystem->GetGlobalValue<Phoenix::f32>(valName));
	}
	ImGui::End();
}

void SceneEvent::PrimitiveRender(Phoenix::Graphics::DeviceDX11* device, Phoenix::Math::Vector3 translate, Phoenix::Math::Vector3 rotate, Phoenix::Math::Vector3 scale)
{

}

Phoenix::Math::Vector3 WorldToScreen(const Phoenix::Math::Vector3& worldPosition)
{
	return Phoenix::Math::Vector3();
}