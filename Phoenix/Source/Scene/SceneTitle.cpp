#include "Scene.h"
#include "SceneSystem.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_win32.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_dx11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_internal.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"


void SceneTitle::Init(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	graphicsDevice = sceneSystem->GetGraphicsDevice();
	commonData = sceneSystem->GetSceneCommonData();

	// �G�t�F�N�g�̓Ǎ�
	//auto effect = Effekseer::Effect::Create(manager, EFK_EXAMPLE_ASSETS_DIR_U16 "Laser01.efk");
	//effect = Effekseer::Effect::Create(commonData->manager, u"D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Effect\\Examples\\Resources\\Laser01.efk");
	effect = Effekseer::Effect::Create(commonData->manager, u"D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Effect\\Examples\\MAGICALxSPIRAL\\HitEffect.efk");

	// �G�t�F�N�g�̍Đ�
	handle = commonData->manager->Play(effect, 0, 0, 0);
}

void SceneTitle::Update()
{
	if (xInput[0].bBACKt)
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
	commonData->manager->Update();
}

void SceneTitle::Draw()
{
	commonData->renderer->BeginRendering();
	commonData->manager->Draw();
	commonData->renderer->EndRendering();
}

void SceneTitle::GUI()
{
	ImGui::Begin("Title");
	{
		ImGui::Text("test");
		if (ImGui::Button("Chage Scene Game"))
		{
			sceneSystem->ChangeScene(SceneType::Game, false);
		}
		if (ImGui::Button("play"))
		{
			// �G�t�F�N�g�̍Đ�
			handle = commonData->manager->Play(effect, 0, 0, 0);
		}
	}
	ImGui::End();
}