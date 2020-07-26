#include "Scene.h"
#include "SceneSystem.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_win32.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_dx11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_internal.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/FrameWork/Shader/SkyMapShader.h"
#include "../Source/Graphics/Texture/Win/DirectX11/TextureDX11.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"


void SceneGame::Init(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	display = sceneSystem->GetDisplay();
	graphicsDevice = sceneSystem->GetGraphicsDevice();
	commonData = sceneSystem->GetSceneCommonData();

	player = commonData->player.get();
	boss = commonData->boss.get();
	stageModel = commonData->stageModel.get();
	basicShader = commonData->basicShader.get();
	basicSkinShader = commonData->basicSkinShader.get();
	standardShader = commonData->standardShader.get();
	pbrShader = commonData->pbrShader.get();
	camera = commonData->camera.get();

	cameraFlg = false;
	isHitCollision = false;
	isUpdate = false;

	Phoenix::Graphics::DeviceDX11* device = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());
	primitive = std::make_shared<GeometricPrimitive>(device->GetD3DDevice(), 1);

	// �G�t�F�N�g�̓Ǎ�
	//auto effect = Effekseer::Effect::Create(manager, EFK_EXAMPLE_ASSETS_DIR_U16 "Laser01.efk");
	//effect = Effekseer::Effect::Create(commonData->manager, u"D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Effect\\Examples\\Resources\\Laser01.efk");
	//hitEffect = Effekseer::Effect::Create(commonData->manager, u"D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Effect\\Examples\\MAGICALxSPIRAL\\HitEffect06.efk");

	// �G�t�F�N�g�̍Đ�
	//hitEffectHandle = commonData->manager->Play(hitEffect, 0, 0, 0);
	//commonData->manager->SetPaused(hitEffectHandle, false);

	Phoenix::Graphics::TextureDesc desc = {};

	skyMap = Phoenix::Graphics::ITexture::Create();
	//skyMap->Initialize(graphicsDevice->GetDevice(), "D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Texture\\SkyMap\\skybox1.dds", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color::White);
	skyMap->Initialize(graphicsDevice->GetDevice(), "D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Texture\\SkyMap\\AllSkyFree\\Cold Sunset\\Cold Sunset Equirect.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color::White);

	DirectX::XMFLOAT3 s = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	skyBox = std::make_shared<GeometricPrimitive>(device->GetD3DDevice(), 0, false, &s);

	skyMapShader = Phoenix::FrameWork::SkyMapShader::Create();
	skyMapShader->Initialize(graphicsDevice);

	enableMSAA = true;

	frameBuffer[0] = Phoenix::FrameWork::FrameBuffer::Create();
	frameBuffer[1] = Phoenix::FrameWork::FrameBuffer::Create();
	frameBuffer[2] = Phoenix::FrameWork::FrameBuffer::Create();

	frameBuffer[0]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), enableMSAA, 8, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
	frameBuffer[1]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
	frameBuffer[2]->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight(), false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::UNKNOWN);

	quad = Phoenix::FrameWork::Quad::Create();
	quad->Initialize(graphicsDevice);

	msaaResolve = Phoenix::FrameWork::MSAAResolve::Create();
	msaaResolve->Initialize(graphicsDevice);

	bloom = Phoenix::FrameWork::Bloom::Create();
	bloom->Initialize(graphicsDevice, display->GetWidth(), display->GetHeight());
}

void SceneGame::Update()
{
	// �v���C���[�X�V
	if (isUpdate)
	{
		player->Update(*camera);
	}

	// �{�X�X�V
	if (isUpdate)
	{
		boss->Update();
	}

	// �����蔻��
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
				//hitEffectHandle = commonData->manager->Play(hitEffect, { pos.x, pos.y, pos.z }, 20);
				//commonData->manager->SetScale(hitEffectHandle, 50.0f, 50.0f, 50.0f);
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

	// �J�����X�V
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

	// �G�t�F�N�g�X�V
	//{
	//	// ���e�s��̍X�V + �J�����s��̍X�V
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

	//	// 3D�T�E���h�p���X�i�[�ݒ�̍X�V
	//	//sound->SetListener(���X�i�[�ʒu, ���ړ_, ������x�N�g��);

	//	// �Đ����̃G�t�F�N�g�̈ړ���(::Effekseer::Manager�o�R�ŗl�X�ȃp�����[�^�[���ݒ�ł��܂��B)
	//	//commonData->manager->AddLocation(handle, ::Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

	//	// �S�ẴG�t�F�N�g�̍X�V
	//	commonData->manager->Flip();
	//	commonData->manager->Update();
	//}
}

void SceneGame::Draw()
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	frameBuffer[0]->Clear(graphicsDevice, 0.0f, 0.0f, 0.0f, 1.0f);
	frameBuffer[0]->Activate(graphicsDevice);

	// ���[���h�s����쐬
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

	// ���b�V���`��
#if 1
	basicSkinShader->Begin(graphicsDevice, *camera);
	basicSkinShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
	// �G�t�F�N�g�`��
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
	pbrShader->Draw(graphicsDevice, player->GetWorldMatrix(), player->GetModel());
	pbrShader->End(graphicsDevice);

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

	frameBuffer[0]->Deactivate(graphicsDevice);

	Phoenix::u32 resolvedFramebuffer = 0;
	if (enableMSAA)
	{
		resolvedFramebuffer = 1;
		msaaResolve->Resolve(graphicsDevice, frameBuffer[0].get(), frameBuffer[resolvedFramebuffer].get());
	}

	bloom->Generate(graphicsDevice, frameBuffer[resolvedFramebuffer]->GetRenderTargetSurface()->GetTexture(), false);

	frameBuffer[resolvedFramebuffer]->Activate(graphicsDevice);
	bloom->Draw(graphicsDevice);
	frameBuffer[resolvedFramebuffer]->Deactivate(graphicsDevice);

	quad->Draw(graphicsDevice, frameBuffer[resolvedFramebuffer]->renderTargerSurface->GetTexture(), 1280.0f * 0, 0.0f, 1280.0f, 720.0f);
	
	quad->Draw(graphicsDevice, frameBuffer[0]->renderTargerSurface->GetTexture(), 256 * 0, 0, 256, 256);
	quad->Draw(graphicsDevice, frameBuffer[1]->renderTargerSurface->GetTexture(), 256 * 1, 0, 256, 256);
	

	// �X�J�C�{�b�N�X�`��
	//{
	//	// ���[���h�s����쐬
	//	Phoenix::Math::Matrix W;
	//	{
	//		Phoenix::Math::Vector3 scale = { 1.0f, 1.0f, 1.0f };
	//		Phoenix::Math::Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	//		Phoenix::Math::Vector3 translate = { 0.0f, 0.0f, 0.0f };

	//		Phoenix::Math::Matrix S, R, T;
	//		S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
	//		R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
	//		T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

	//		W = S * R * T;
	//	}

	//	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();
	//	Phoenix::Graphics::DeviceDX11* device = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());
	//	Phoenix::Graphics::ContextDX11* contextDX11 = static_cast<Phoenix::Graphics::ContextDX11*>(context);

	//	// ���b�V���萔�o�b�t�@�X�V
	//	context->UpdateConstantBufferMesh(W);

	//	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	//	Phoenix::Graphics::ITexture* texture[] = { skyMap.get() };
	//	graphicsDevice->GetContext()->SetShaderResources(Phoenix::Graphics::ShaderType::Pixel, 0, 1, texture);

	//	// �`��
	//	context->SetRasterizer(contextDX11->GetRasterizerState(Phoenix::Graphics::RasterizerState::SolidCullFront));
	//	skyMapShader->Begin(graphicsDevice, *camera);
	//	skyBox->Render(device->GetD3DContext());
	//	skyMapShader->End(graphicsDevice);
	//	context->SetRasterizer(contextDX11->GetRasterizerState(Phoenix::Graphics::RasterizerState::SolidCullNone));
	//}

	//// ���[���h�s����쐬
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
	// ���[���h�s����쐬
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
			Phoenix::FrameWork::MaterialState* material = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetMaterial();
			ImGuiColorEditFlags flag = ImGuiColorEditFlags_Float; // 0 ~ 255�\�L�ł͂Ȃ��A0.0 ~ 1.0�\�L�ɂ��܂��B

			ImGui::DragFloat4("dir", &light->direction.x);
			ImGui::DragFloat4("color", &light->color.x);
			ImGui::ColorEdit4("albedo", material->albedo, flag);
			ImGui::DragFloat("metallic", &material->metallic, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("roughness", &material->roughness, 0.01f, 0.0f, 1.0f);
			ImGui::TreePop();

		}
		if (ImGui::TreeNode("Bloom"))
		{
			ImGui::DragFloat("glowExtractionThreshold", &bloom->shaderContants.glowExtractionThreshold);
			ImGui::DragFloat("blurConvolutionIntensity", &bloom->shaderContants.blurConvolutionIntensity);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("FrameBuffer"))
		{
			//graphicsDevice->GetSwapChain()->GetRenderTargerSurface()->GetTexture()->Handle();
			//ImGui::Image(graphicsDevice->GetSwapChain()->GetRenderTargerSurface()->GetTexture()->Handle(), ImVec2(256.0f, 256.0f));

			//ID3D11ShaderResourceView* srv = static_cast<Phoenix::Graphics::TextureDX11*>(graphicsDevice->GetSwapChain()->GetRenderTargerSurface()->GetTexture())->GetD3DShaderResourceView();
			//ImGui::Image(srv, ImVec2(256.0f, 256.0f));
			//frameBuffer[0]->DrawSRV(true, false);
			ID3D11ShaderResourceView* srv = static_cast<Phoenix::Graphics::TextureDX11*>(skyMap.get())->GetD3DShaderResourceView();
			ImGui::Image(srv, ImVec2(100.0f, 100.0f));

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