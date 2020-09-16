#include "Scene.h"
#include "SceneSystem.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_win32.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_dx11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_internal.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"
#include "Phoenix/FND/Util.h"


void SceneTitle::Init(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	display = sceneSystem->GetDisplay();
	graphicsDevice = sceneSystem->GetGraphicsDevice();
	commonData = sceneSystem->GetSceneCommonData();

	//{
	//	//const char* filename = "..\\Data\\Assets\\Model\\TestModel\\BlackSword03.fbx";
	//	const char* filename = "..\\Data\\Assets\\Model\\TestModel\\danbo_fbx\\danbo_atk.fbx";
	//	//const char* filename = "..\\Data\\Assets\\Model\\TestModel\\Sphere01\\Sphere01.fbx";
	//	//const char* filename = "..\\Data\\Assets\\Model\\TestModel\\mari\\mari.fbx";
	//	model = std::make_unique<Phoenix::FrameWork::ModelObject>();
	//	model->Initialize(graphicsDevice);
	//	model->Load(graphicsDevice, Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), filename));

	//	// 待機モーション開始
	//	{
	//		model->PlayAnimation(0, 0);
	//		model->UpdateTransform(1 / 60.0f);
	//		model->SetLoopAnimation(true);
	//		//model->PauseAnimation(true);
	//	}
	//}

	// 共通データのアドレス取得
	{
		stageModel = commonData->stageModel.get();
		pbrShader = commonData->pbrShader.get();
		pbrSkinShader = commonData->pbrSkinShader.get();
		camera = commonData->camera.get();
	}

	// 共通データの初期化
	{
		camera->SetEye(Phoenix::Math::Vector3(0.0f, 0.0f, 10.0f));
		camera->SetRotateX(0.1f);
	}

	// フレームバッファ
	{
		frameBuffer[0] = Phoenix::FrameWork::FrameBuffer::Create();
		frameBuffer[1] = Phoenix::FrameWork::FrameBuffer::Create();
		frameBuffer[2] = Phoenix::FrameWork::FrameBuffer::Create();

		frameBuffer[0]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 8, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
		frameBuffer[1]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
		frameBuffer[2]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::UNKNOWN);
	}

	// ポストプロセス
	{
		postProcessingEffects = Phoenix::FrameWork::PostProcessingEffects::Create();
		postProcessingEffects->Initialize(graphicsDevice);
	}

	// ブルーム
	{
		quad = Phoenix::FrameWork::Quad::Create();
		quad->Initialize(graphicsDevice);
		quad->LoadDissolveTexture(graphicsDevice, "..\\Data\\Assets\\Texture\\Mask\\Dissolve\\dissolve_animation2.png");
		quad->LoadDissolveTexture02(graphicsDevice, "..\\Data\\Assets\\Texture\\Mask\\Dissolve\\dissolve_animation1.png");
		quad->LoadEmissiveTexture(graphicsDevice, "..\\Data\\Assets\\Texture\\Mask\\Dissolve\\dissolve_edgecolor.png");

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

	// タイトル
	{
		logo = Phoenix::Graphics::ITexture::Create();
		icon = Phoenix::Graphics::ITexture::Create();
		button = Phoenix::Graphics::ITexture::Create();

		logo->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\Title\\TitleLogo.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
		icon->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\Title\\TitleIcon.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
		button->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\Title\\TitleButton.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));

		dissolveThreshold = 1.05f;
		dissolveEmissiveWidth = 0.027f;
	}

	// フラグ
	{
		isChangeScene = false;
	}
}

void SceneTitle::Update(Phoenix::f32 elapsedTime)
{
	camera->SurveyCamera(0.0f, -0.005f, 300.0f, Phoenix::Math::Vector3(0.0f, 25.0f, 0.0f));
	//camera->FreeCamera();
	camera->Update();

	if (isChangeScene)
	{
		if (dissolveThreshold <= 1.2f) dissolveThreshold += dissolveSpeed;
		else sceneSystem->ChangeScene(SceneType::Game, false, true);
		return;
	}

	if (sceneSystem->GetOnFade()) return;

	if (xInput[0].bAt || xInput[0].bBt || xInput[0].bXt || xInput[0].bYt || xInput[0].bRBt || xInput[0].bLBt || xInput[0].bRTt || xInput[0].bLTt || xInput[0].bSTARTt || xInput[0].bBACKt)
	{
		isChangeScene = true;
		dissolveThreshold = 0.5f;
	}

	if (-0.1f < dissolveThreshold) dissolveThreshold -= dissolveSpeed;
}

void SceneTitle::Draw(Phoenix::f32 elapsedTime)
{
	/*commonData->renderer->BeginRendering();
	commonData->manager->Draw();
	commonData->renderer->EndRendering();*/

	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	Phoenix::Graphics::Viewport* v = new Phoenix::Graphics::Viewport();
	context->GetViewports(1, &v);

	Phoenix::f32 aspectRatio = v->width / v->height;
	Phoenix::f32 width = v->width;
	Phoenix::f32 height = v->height;

	Phoenix::FND::SafeDelete(v);

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
					Phoenix::Math::Vector3 translate = { 0.0f, -22.05f, 0.0f };

					Phoenix::Math::Matrix S, R, T;
					S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
					R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
					T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

					W = S * R * T;
				}
#if 0
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

			// Draw Title Logo.
			{
				quad->SetDissolveThreshold(dissolveThreshold);
				quad->SetDissolveEmissiveWidth(dissolveEmissiveWidth);
				quad->Draw(graphicsDevice, icon.get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f));
				quad->Draw(graphicsDevice, logo.get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, true, true, true, true, true, true, true);
				quad->Draw(graphicsDevice, button.get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f));
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

void SceneTitle::GUI()
{
	//ImGui::Begin("Title");
	//{
	//	ImGui::Text("test");
	//	if (ImGui::Button("Chage Scene Game"))
	//	{
	//		sceneSystem->ChangeScene(SceneType::Game, false);
	//	}
	//	//if (ImGui::Button("play"))
	//	//{
	//	//	// エフェクトの再生
	//	//	//handle = commonData->manager->Play(effect, 0, 0, 0);
	//	//}
	//}
	//ImGui::End();

	//ImGui::Begin("Game");
	//{
	//	/*if (ImGui::TreeNode("Shader"))
	//	{
	//		Phoenix::FrameWork::LightState* light = static_cast<Phoenix::FrameWork::PBRShader*>(commonData->pbrShader.get())->GetLight();
	//		Phoenix::FrameWork::MaterialState* material = static_cast<Phoenix::FrameWork::PBRShader*>(commonData->pbrShader.get())->GetMaterial();

	//		ImGui::DragFloat4("dir", &light->direction.x, 0.01f, -1.0f, 1.0f);
	//		ImGui::DragFloat4("color", &light->color.x);
	//		ImGui::DragFloat4("albedo", &material->albedo.x);
	//		ImGui::DragFloat("metallic", &material->metallic, 0.01f, 0.0f, 1.0f);
	//		ImGui::DragFloat("roughness", &material->roughness, 0.01f, 0.0f, 1.0f);
	//		ImGui::TreePop();
	//	}*/
	//	if (ImGui::TreeNode("Dissolve"))
	//	{
	//		ImGui::DragFloat("Threshold", &dissolveThreshold, 0.01f);
	//		ImGui::DragFloat("EmissiveWidth", &dissolveEmissiveWidth, 0.01f);
	//		ImGui::TreePop();
	//	}
	//}
	//ImGui::End();
}