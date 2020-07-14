#include "Scene.h"
#include "SceneSystem.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_win32.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_dx11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_internal.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"


void SceneTitle::Init(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	graphicsDevice = sceneSystem->GetGraphicsDevice();
	commonData = sceneSystem->GetSceneCommonData();

	Phoenix::Graphics::DeviceDX11* deviceDX11 = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());

	// �`��Ǘ��C���X�^���X�̐���
	renderer = ::EffekseerRendererDX11::Renderer::Create(deviceDX11->GetD3DDevice(), deviceDX11->GetD3DContext(), 8000);
	
	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	manager = ::Effekseer::Manager::Create(8000);
	
	// �`�惂�W���[���̐ݒ�
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());
	
	// �e�N�X�`���A���f���A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());
	manager->SetMaterialLoader(renderer->CreateMaterialLoader());
	
	// �T�E���h�Ǘ��C���X�^���X�̐���
	//IXAudio2* xAudio2 = GetIXAudio2();
	//::EffekseerSound::Sound* sound = ::EffekseerSound::Sound::Create(IXAudio2*, ���m�����Đ��p�{�C�X��, �X�e���I�Đ��p�{�C�X��);

	// ���Đ��p�C���X�^���X����Đ��@�\���w��
	//manager->SetSoundPlayer(sound->CreateSoundPlayer());

	// �T�E���h�f�[�^�̓Ǎ��@�\��ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	//manager->SetSoundLoader(sound->CreateSoundLoader());

	// �e�N�X�`���摜�̓Ǎ����@�̎w��(�p�b�P�[�W������ǂݍ��ޏꍇ�g������K�v������܂��B)
	manager->SetTextureLoader(renderer->CreateTextureLoader());

	// ���W�n�̎w��(RH�ŉE��n�ALH�ō���n)
	manager->SetCoordinateSystem(Effekseer::CoordinateSystem::RH);

	// ���_�ʒu���m��
	auto g_position = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	// ���e�s���ݒ�
	renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovRH(
		90.0f / 180.0f * 3.14f, (float)1280 / (float)720, 1.0f, 500.0f));

	// �J�����s���ݒ�
	renderer->SetCameraMatrix(::Effekseer::Matrix44().LookAtRH(g_position, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

	// �G�t�F�N�g�̓Ǎ�
	//auto effect = Effekseer::Effect::Create(manager, EFK_EXAMPLE_ASSETS_DIR_U16 "Laser01.efk");
	effect = Effekseer::Effect::Create(manager, u"D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Effect\\Examples\\Resources\\Laser01.efk");

	// �G�t�F�N�g�̍Đ�
	handle = manager->Play(effect, 0, 0, 0);
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
	manager->AddLocation(handle, ::Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

	// �S�ẴG�t�F�N�g�̍X�V
	manager->Update();
}

void SceneTitle::Draw()
{
	renderer->BeginRendering();
	manager->Draw();
	renderer->EndRendering();
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
			handle = manager->Play(effect, 0, 0, 0);
		}
	}
	ImGui::End();
}