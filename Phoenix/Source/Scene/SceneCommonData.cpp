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

	//// 描画管理インスタンスの生成
	//renderer = ::EffekseerRendererDX11::Renderer::Create(deviceDX11->GetD3DDevice(), deviceDX11->GetD3DContext(), 8000);

	//// エフェクト管理用インスタンスの生成
	//manager = ::Effekseer::Manager::Create(8000);

	//// 描画モジュールの設定
	//manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	//manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	//manager->SetRingRenderer(renderer->CreateRingRenderer());
	//manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	//manager->SetModelRenderer(renderer->CreateModelRenderer());

	//// テクスチャ、モデル、マテリアルローダーの設定する。
	//// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	//manager->SetTextureLoader(renderer->CreateTextureLoader());
	//manager->SetModelLoader(renderer->CreateModelLoader());
	//manager->SetMaterialLoader(renderer->CreateMaterialLoader());

	//// サウンド管理インスタンスの生成
	////IXAudio2* xAudio2 = GetIXAudio2();
	////::EffekseerSound::Sound* sound = ::EffekseerSound::Sound::Create(IXAudio2*, モノラル再生用ボイス数, ステレオ再生用ボイス数);

	//// 音再生用インスタンスから再生機能を指定
	////manager->SetSoundPlayer(sound->CreateSoundPlayer());

	//// サウンドデータの読込機能を設定する。
	//// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	////manager->SetSoundLoader(sound->CreateSoundLoader());

	//// テクスチャ画像の読込方法の指定(パッケージ等から読み込む場合拡張する必要があります。)
	//manager->SetTextureLoader(renderer->CreateTextureLoader());

	//// 座標系の指定(RHで右手系、LHで左手系)
	//manager->SetCoordinateSystem(Effekseer::CoordinateSystem::RH);

	//// 視点位置を確定
	//auto g_position = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	//// 投影行列を設定
	//renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovRH(
	//	90.0f / 180.0f * 3.14f, (float)1280 / (float)720, 1.0f, 500.0f));

	//// カメラ行列を設定
	//renderer->SetCameraMatrix(::Effekseer::Matrix44().LookAtRH(g_position, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
}