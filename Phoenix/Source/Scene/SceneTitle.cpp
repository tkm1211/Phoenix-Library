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

	//	// �ҋ@���[�V�����J�n
	//	{
	//		model->PlayAnimation(0, 0);
	//		model->UpdateTransform(1 / 60.0f);
	//		model->SetLoopAnimation(true);
	//		//model->PauseAnimation(true);
	//	}
	//}

	// ���ʃf�[�^�̃A�h���X�擾
	{
		stageModel = commonData->stageModel.get();
		pbrShader = commonData->pbrShader.get();
		pbrSkinShader = commonData->pbrSkinShader.get();
		camera = commonData->camera.get();
	}

	// ���ʃf�[�^�̏�����
	{
		camera->SetEye(Phoenix::Math::Vector3(0.0f, 0.0f, 10.0f));
		camera->SetRotateX(0.1f);
	}

	// �t���[���o�b�t�@
	{
		frameBuffer[0] = Phoenix::FrameWork::FrameBuffer::Create();
		frameBuffer[1] = Phoenix::FrameWork::FrameBuffer::Create();
		frameBuffer[2] = Phoenix::FrameWork::FrameBuffer::Create();

		frameBuffer[0]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 8, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
		frameBuffer[1]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
		frameBuffer[2]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::UNKNOWN);
	}

	// �|�X�g�v���Z�X
	{
		postProcessingEffects = Phoenix::FrameWork::PostProcessingEffects::Create();
		postProcessingEffects->Initialize(graphicsDevice);
	}

	// �u���[��
	{
		quad = Phoenix::FrameWork::Quad::Create();
		quad->Initialize(graphicsDevice);

		msaaResolve = Phoenix::FrameWork::MSAAResolve::Create();
		msaaResolve->Initialize(graphicsDevice);

		bloom = Phoenix::FrameWork::Bloom::Create();
		bloom->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight());
	}

	// �X�J�C�}�b�v
	{
		skyMap = Phoenix::FrameWork::SkyMap::Create();
		skyMap->Initialize(graphicsDevice, "..\\Data\\Assets\\Texture\\SkyMap\\AllSkyFree\\Night MoonBurst\\NightMoonBurst.dds"); //Epic_BlueSunset\\Epic_BlueSunset03 //Night MoonBurst\\NightMoonBurst
	}

	// �^�C�g��
	{
		logo = Phoenix::Graphics::ITexture::Create();
		button = Phoenix::Graphics::ITexture::Create();

		logo->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\Title\\TitleLogo.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
		button->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\Title\\TitleButton.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// �G�t�F�N�g�̓Ǎ�
	//auto effect = Effekseer::Effect::Create(manager, EFK_EXAMPLE_ASSETS_DIR_U16 "Laser01.efk");
	//effect = Effekseer::Effect::Create(commonData->manager, u"D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Effect\\Examples\\Resources\\Laser01.efk");
	//effect = Effekseer::Effect::Create(commonData->manager, u"D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Effect\\Examples\\MAGICALxSPIRAL\\HitEffect.efk");

	// �G�t�F�N�g�̍Đ�
	//handle = commonData->manager->Play(effect, 0, 0, 0);
}

void SceneTitle::Update()
{
	if (xInput[0].bAt || xInput[0].bBt || xInput[0].bXt || xInput[0].bYt || xInput[0].bRBt || xInput[0].bLBt || xInput[0].bRTt || xInput[0].bLTt || xInput[0].bSTARTt || xInput[0].bBACKt)
	{
		sceneSystem->ChangeScene(SceneType::Game, false);
	}

	// ���e�s��̍X�V
	//renderer->SetProjectionMatrix(::Effekseer::Matrix44);

	// �J�����s��̍X�V
	//renderer->SetCameraMatrix(::Effekseer::Matrix44);

	// 3D�T�E���h�p���X�i�[�ݒ�̍X�V
	//sound->SetListener(���X�i�[�ʒu, ���ړ_, ������x�N�g��);

	// �Đ����̃G�t�F�N�g�̈ړ���(::Effekseer::Manager�o�R�ŗl�X�ȃp�����[�^�[���ݒ�ł��܂��B)
	//commonData->manager->AddLocation(handle, ::Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

	// �S�ẴG�t�F�N�g�̍X�V
	//commonData->manager->Update();

	camera->SurveyCamera(0.0f, -0.005f, 300.0f, Phoenix::Math::Vector3(0.0f, 25.0f, 0.0f));
	//camera->FreeCamera();
	camera->Update();
}

void SceneTitle::Draw()
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
				// ���[���h�s����쐬
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
				// ���[���h�s����쐬
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

	// Draw Title Logo.
	{
		quad->Draw(graphicsDevice, logo.get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f));
		quad->Draw(graphicsDevice, button.get(), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(width, height), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(1920.0f, 1080.0f));
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
	//	//	// �G�t�F�N�g�̍Đ�
	//	//	//handle = commonData->manager->Play(effect, 0, 0, 0);
	//	//}
	//}
	//ImGui::End();

	//ImGui::Begin("Game");
	//{
	//	if (ImGui::TreeNode("Shader"))
	//	{
	//		Phoenix::FrameWork::LightState* light = static_cast<Phoenix::FrameWork::PBRShader*>(commonData->pbrShader.get())->GetLight();
	//		Phoenix::FrameWork::MaterialState* material = static_cast<Phoenix::FrameWork::PBRShader*>(commonData->pbrShader.get())->GetMaterial();

	//		ImGui::DragFloat4("dir", &light->direction.x, 0.01f, -1.0f, 1.0f);
	//		ImGui::DragFloat4("color", &light->color.x);
	//		ImGui::DragFloat4("albedo", &material->albedo.x);
	//		ImGui::DragFloat("metallic", &material->metallic, 0.01f, 0.0f, 1.0f);
	//		ImGui::DragFloat("roughness", &material->roughness, 0.01f, 0.0f, 1.0f);
	//		ImGui::TreePop();
	//	}
	//}
	//ImGui::End();
}