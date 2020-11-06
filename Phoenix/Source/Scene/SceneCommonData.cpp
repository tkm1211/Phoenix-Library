#include "SceneCommonData.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../AI/MetaAI/BattleEnemySystem.h"
#include "../AI/MetaAI/MetaType.h"


std::shared_ptr<SceneCommonData> SceneCommonData::Create()
{
	return std::make_shared<SceneCommonData>();
}

void SceneCommonData::Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	player = Player::Create();
	player->Construct(graphicsDevice);

	boss = Boss::Create();
	boss->Construct(graphicsDevice, player.get());

	enemyManager = EnemyManager::Create();
	enemyManager->Construct(graphicsDevice);

	mannequin = Mannequin::Create();
	mannequin->Construct(graphicsDevice);

	std::shared_ptr<Meta::BattleEnemySystem> battleEnemySystem = Meta::BattleEnemySystem::Create();
	battleEnemySystem->Construct();
	battleEnemySystem->SetManager(enemyManager);

	metaAI = MetaAI::Create();
	metaAI->AddSystem<Meta::BattleEnemySystem>(static_cast<Phoenix::s32>(MetaType::Battle), battleEnemySystem);
	metaAI->Construct();

	targetMarkUI = TargetMarkUI::Create();

	uiSystem = UISystem::Create();
	{
		uiSystem->AddUI(player->GetUI());
		uiSystem->AddUI(boss->GetUI());
		uiSystem->AddUI(targetMarkUI.get());
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

	targetMark = Phoenix::Graphics::ITexture::Create();
	targetMark->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\UI\\TargetMark\\target.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));

	bgm = Phoenix::FrameWork::CXAudio2::Create();
	bgm->Load(L"..\\Data\\Assets\\Audio\\BGM\\GameMain.wav", 1);

	se = Phoenix::FrameWork::CXAudio2::Create();
	se->Load(L"..\\Data\\Assets\\Audio\\SE\\Player\\Walk\\walk.wav", 0);

	avoidUI = Phoenix::Graphics::ITexture::Create();
	attackUI = Phoenix::Graphics::ITexture::Create();
	runUI = Phoenix::Graphics::ITexture::Create();
	targetUI = Phoenix::Graphics::ITexture::Create();

	avoidUI->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\Avoid.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	attackUI->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\Attack.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	runUI->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\Run.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
	targetUI->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\UI\\Tutorial\\Target.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
}