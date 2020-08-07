#include "SceneCommonData.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"


std::shared_ptr<SceneCommonData> SceneCommonData::Create()
{
	return std::make_shared<SceneCommonData>();
}

void SceneCommonData::Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	player = Player::Create();
	player->Init(graphicsDevice);

	boss = Boss::Create();
	boss->Init(graphicsDevice, player.get());

	const char* filename = "..\\Data\\Assets\\Model\\stage\\stage01.fbx"; // stage01 Floor01
	stageModel = std::make_unique<Phoenix::FrameWork::ModelObject>();
	stageModel->Initialize(graphicsDevice);
	stageModel->Load(graphicsDevice, Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), filename));

	basicShader = Phoenix::FrameWork::BasicShader::Create();
	basicShader->Initialize(graphicsDevice);

	basicSkinShader = Phoenix::FrameWork::BasicSkinShader::Create();
	basicSkinShader->Initialize(graphicsDevice);

	standardShader = Phoenix::FrameWork::StandardShader::Create();
	//standardShader->Initialize(graphicsDevice);

	pbrShader = Phoenix::FrameWork::PBRShader::Create();
	pbrShader->Initialize(graphicsDevice);

	pbrSkinShader = Phoenix::FrameWork::PBRSkinShader::Create();
	pbrSkinShader->Initialize(graphicsDevice);

	camera = std::make_shared<Phoenix::Graphics::Camera>();

	Phoenix::Graphics::DeviceDX11* deviceDX11 = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());

	//// �`��Ǘ��C���X�^���X�̐���
	//renderer = ::EffekseerRendererDX11::Renderer::Create(deviceDX11->GetD3DDevice(), deviceDX11->GetD3DContext(), 8000);

	//// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	//manager = ::Effekseer::Manager::Create(8000);

	//// �`�惂�W���[���̐ݒ�
	//manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	//manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	//manager->SetRingRenderer(renderer->CreateRingRenderer());
	//manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	//manager->SetModelRenderer(renderer->CreateModelRenderer());

	//// �e�N�X�`���A���f���A�}�e���A�����[�_�[�̐ݒ肷��B
	//// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	//manager->SetTextureLoader(renderer->CreateTextureLoader());
	//manager->SetModelLoader(renderer->CreateModelLoader());
	//manager->SetMaterialLoader(renderer->CreateMaterialLoader());

	//// �T�E���h�Ǘ��C���X�^���X�̐���
	////IXAudio2* xAudio2 = GetIXAudio2();
	////::EffekseerSound::Sound* sound = ::EffekseerSound::Sound::Create(IXAudio2*, ���m�����Đ��p�{�C�X��, �X�e���I�Đ��p�{�C�X��);

	//// ���Đ��p�C���X�^���X����Đ��@�\���w��
	////manager->SetSoundPlayer(sound->CreateSoundPlayer());

	//// �T�E���h�f�[�^�̓Ǎ��@�\��ݒ肷��B
	//// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	////manager->SetSoundLoader(sound->CreateSoundLoader());

	//// �e�N�X�`���摜�̓Ǎ����@�̎w��(�p�b�P�[�W������ǂݍ��ޏꍇ�g������K�v������܂��B)
	//manager->SetTextureLoader(renderer->CreateTextureLoader());

	//// ���W�n�̎w��(RH�ŉE��n�ALH�ō���n)
	//manager->SetCoordinateSystem(Effekseer::CoordinateSystem::RH);

	//// ���_�ʒu���m��
	//auto g_position = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	//// ���e�s���ݒ�
	//renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovRH(
	//	90.0f / 180.0f * 3.14f, (float)1280 / (float)720, 1.0f, 500.0f));

	//// �J�����s���ݒ�
	//renderer->SetCameraMatrix(::Effekseer::Matrix44().LookAtRH(g_position, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
}