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

	// 描画管理インスタンスの生成
	renderer = ::EffekseerRendererDX11::Renderer::Create(deviceDX11->GetD3DDevice(), deviceDX11->GetD3DContext(), 8000);
	
	// エフェクト管理用インスタンスの生成
	manager = ::Effekseer::Manager::Create(8000);
	
	// 描画モジュールの設定
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());
	
	// テクスチャ、モデル、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());
	manager->SetMaterialLoader(renderer->CreateMaterialLoader());
	
	// サウンド管理インスタンスの生成
	//IXAudio2* xAudio2 = GetIXAudio2();
	//::EffekseerSound::Sound* sound = ::EffekseerSound::Sound::Create(IXAudio2*, モノラル再生用ボイス数, ステレオ再生用ボイス数);

	// 音再生用インスタンスから再生機能を指定
	//manager->SetSoundPlayer(sound->CreateSoundPlayer());

	// サウンドデータの読込機能を設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	//manager->SetSoundLoader(sound->CreateSoundLoader());

	// テクスチャ画像の読込方法の指定(パッケージ等から読み込む場合拡張する必要があります。)
	manager->SetTextureLoader(renderer->CreateTextureLoader());

	// 座標系の指定(RHで右手系、LHで左手系)
	manager->SetCoordinateSystem(Effekseer::CoordinateSystem::RH);

	// 視点位置を確定
	auto g_position = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	// 投影行列を設定
	renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovRH(
		90.0f / 180.0f * 3.14f, (float)1280 / (float)720, 1.0f, 500.0f));

	// カメラ行列を設定
	renderer->SetCameraMatrix(::Effekseer::Matrix44().LookAtRH(g_position, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

	// エフェクトの読込
	//auto effect = Effekseer::Effect::Create(manager, EFK_EXAMPLE_ASSETS_DIR_U16 "Laser01.efk");
	effect = Effekseer::Effect::Create(manager, u"D:\\Phoenix Project\\Phoenix\\Data\\Assets\\Effect\\Examples\\Resources\\Laser01.efk");

	// エフェクトの再生
	handle = manager->Play(effect, 0, 0, 0);
}

void SceneTitle::Update()
{
	if (xInput[0].bBACKt)
	{
		sceneSystem->ChangeScene(SceneType::Game, false);
	}

	// 投影行列の更新
	//renderer->SetProjectionMatrix(::Effekseer::Matrix44);

	// カメラ行列の更新
	//renderer->SetCameraMatrix(::Effekseer::Matrix44);

	// 3Dサウンド用リスナー設定の更新
	//sound->SetListener(リスナー位置, 注目点, 上方向ベクトル);

	// 再生中のエフェクトの移動等(::Effekseer::Manager経由で様々なパラメーターが設定できます。)
	manager->AddLocation(handle, ::Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

	// 全てのエフェクトの更新
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
			// エフェクトの再生
			handle = manager->Play(effect, 0, 0, 0);
		}
	}
	ImGui::End();
}