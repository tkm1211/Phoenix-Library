#include "SceneCommonData.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../AI/MetaAI/BattleEnemySystem.h"
#include "../AI/MetaAI/MetaType.h"
#include "../Enemy/Enemy.h"


std::shared_ptr<SceneCommonData> SceneCommonData::Create()
{
	return std::make_shared<SceneCommonData>();
}

void SceneCommonData::Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	player = Player::Create();
	player->Construct(graphicsDevice);
	
	enemyManager = EnemyManager::Create();
	enemyManager->Construct(graphicsDevice);
	enemyManager->SetPlayer(player);

	//mannequin = Mannequin::Create();
	//mannequin->Construct(graphicsDevice);

	std::shared_ptr<Meta::BattleEnemySystem> battleEnemySystem = Meta::BattleEnemySystem::Create();
	battleEnemySystem->Construct();
	battleEnemySystem->SetManager(enemyManager);

	metaAI = MetaAI::Create();
	metaAI->AddSystem<Meta::BattleEnemySystem>(static_cast<Phoenix::s32>(MetaType::Battle), battleEnemySystem);
	metaAI->Construct();

	tutorialUI = TutorialUI::Create();

	uiSystem = UISystem::Create();
	{
		uiSystem->AddUI(player->GetUI());
		uiSystem->AddUI(enemyManager->GetEnemiesUI());
		uiSystem->AddUI(tutorialUI);
	}
	uiSystem->Initialize(graphicsDevice);

	const char* filename = "..\\Data\\Assets\\Model\\Stage\\MDL_StartStage.fbx"; // stage01 Floor01 JOL // MDL_StartStage  // Stage_Collision
	stageModel = std::make_unique<Phoenix::FrameWork::ModelObject>();
	stageModel->Initialize(graphicsDevice);
	stageModel->Load(graphicsDevice, filename);

	filename = "..\\Data\\Assets\\Model\\Stage\\MDL_BossStage03.fbx"; // MDL_BossStage03 // Stage_Collision
	bossStageModel = std::make_unique<Phoenix::FrameWork::ModelObject>();
	bossStageModel->Initialize(graphicsDevice);
	bossStageModel->Load(graphicsDevice, filename);

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
	
	bgm = Phoenix::FrameWork::CXAudio2::Create();
	bgm->Load(L"..\\Data\\Assets\\Audio\\BGM\\GameMain.wav", 1);

	se = Phoenix::FrameWork::CXAudio2::Create();
	se->Load(L"..\\Data\\Assets\\Audio\\SE\\Player\\Walk\\walk.wav", 0);

	soundSystem = SoundSystem<SoundType>::Create();

	// 読み込み
	{
		Phoenix::s32 loadTypeNum = 0; // コピペミスを防ぐため

		// BGM
		loadTypeNum = 0;
		{
		}

		// Loop BGM
		loadTypeNum = 1;
		{
			soundSystem->AddSound(SoundType::BGM_Title, L"..\\Data\\Assets\\Audio\\BGM\\Title.wav", loadTypeNum);
			soundSystem->AddSound(SoundType::BGM_Game, L"..\\Data\\Assets\\Audio\\BGM\\GameMain2.wav", loadTypeNum);
		}

		// SE
		loadTypeNum = 0;
		{
			soundSystem->AddSound(SoundType::SE_Player_Attack_Punch_Swing, L"..\\Data\\Assets\\Audio\\SE\\Player\\Attack\\Punch\\Swing\\punch_swing.wav", loadTypeNum);
			soundSystem->AddSound(SoundType::SE_Player_Attack_Punch_Hit_Right, L"..\\Data\\Assets\\Audio\\SE\\Player\\Attack\\Punch\\Hit\\punch_hit_right.wav", loadTypeNum);
			soundSystem->AddSound(SoundType::SE_Player_Attack_Punch_Hit_Heavy, L"..\\Data\\Assets\\Audio\\SE\\Player\\Attack\\Punch\\Hit\\punch_hit_heavy.wav", loadTypeNum);

			soundSystem->AddSound(SoundType::SE_Player_Attack_Kick_Swing, L"..\\Data\\Assets\\Audio\\SE\\Player\\Attack\\Kick\\Swing\\kick_swing.wav", loadTypeNum);
			soundSystem->AddSound(SoundType::SE_Player_Attack_Kick_Hit_Right, L"..\\Data\\Assets\\Audio\\SE\\Player\\Attack\\Kick\\Hit\\kick_hit_right.wav", loadTypeNum);
			soundSystem->AddSound(SoundType::SE_Player_Attack_Kick_Hit_Heavy, L"..\\Data\\Assets\\Audio\\SE\\Player\\Attack\\Kick\\Hit\\kick_hit_heavy.wav", loadTypeNum);

			soundSystem->AddSound(SoundType::SE_Player_Dedge, L"..\\Data\\Assets\\Audio\\SE\\Player\\Dedge\\dedge.wav", loadTypeNum);

			soundSystem->AddSound(SoundType::SE_Round_Move, L"..\\Data\\Assets\\Audio\\SE\\Fight\\move.wav", loadTypeNum);
			soundSystem->AddSound(SoundType::SE_Round_Fight, L"..\\Data\\Assets\\Audio\\SE\\Fight\\fight.wav", loadTypeNum);

			soundSystem->AddSound(SoundType::SE_KO, L"..\\Data\\Assets\\Audio\\SE\\KO\\ko.wav", loadTypeNum);
		}

		// Loop SE
		loadTypeNum = 1;
		{
			soundSystem->AddSound(SoundType::SE_Player_Walk, L"..\\Data\\Assets\\Audio\\SE\\Player\\Walk\\walk4.wav", loadTypeNum);
		}
	}

	// 音量調整
	{
		soundSystem->SetVolume(SoundType::BGM_Title, 0.25f);
		soundSystem->SetVolume(SoundType::BGM_Game, 0.25f);

		soundSystem->SetVolume(SoundType::SE_Player_Walk, 0.75f);
		soundSystem->SetVolume(SoundType::SE_Player_Attack_Punch_Swing, 0.75f);
		soundSystem->SetVolume(SoundType::SE_Player_Attack_Punch_Hit_Right, 0.75f);
		soundSystem->SetVolume(SoundType::SE_Player_Attack_Punch_Hit_Heavy, 0.75f);
		soundSystem->SetVolume(SoundType::SE_Player_Attack_Kick_Swing, 0.75f);
		soundSystem->SetVolume(SoundType::SE_Player_Attack_Kick_Hit_Right, 0.75f);
		soundSystem->SetVolume(SoundType::SE_Player_Attack_Kick_Hit_Heavy, 0.75f);
		soundSystem->SetVolume(SoundType::SE_Player_Dedge, 0.75f);

		soundSystem->SetVolume(SoundType::SE_Round_Move, 0.75f);
		soundSystem->SetVolume(SoundType::SE_Round_Fight, 0.75f);

		soundSystem->SetVolume(SoundType::SE_KO, 0.75f);
	}

	// 共有
	{
		player->SetSoundSystem(soundSystem);
	}

	operatorUI = Phoenix::Graphics::ITexture::Create();
	operatorUI->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\operator.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));

	/*avoidUI = Phoenix::Graphics::ITexture::Create();
	attackUI = Phoenix::Graphics::ITexture::Create();
	runUI = Phoenix::Graphics::ITexture::Create();
	targetUI = Phoenix::Graphics::ITexture::Create();

	avoidUI->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\Avoid.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	attackUI->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\Attack.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	runUI->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\Run.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	targetUI->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\Target.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));*/
}