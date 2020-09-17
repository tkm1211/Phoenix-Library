#include "SceneCommonData.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"


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

	uiSystem = UISystem::Create();
	{
		uiSystem->AddUI(player->GetUI());
		uiSystem->AddUI(boss->GetUI());
	}
	uiSystem->Initialize(graphicsDevice);

	const char* filename = "..\\Data\\Assets\\Model\\Stage\\MDL_StartStage.fbx"; // stage01 Floor01 JOL\\MDL_StartStage
	stageModel = std::make_unique<Phoenix::FrameWork::ModelObject>();
	stageModel->Initialize(graphicsDevice);
	stageModel->Load(graphicsDevice, Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), filename));

	filename = "..\\Data\\Assets\\Model\\Stage\\MDL_BossStage03.fbx";
	bossStageModel = std::make_unique<Phoenix::FrameWork::ModelObject>();
	bossStageModel->Initialize(graphicsDevice);
	bossStageModel->Load(graphicsDevice, Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), filename));

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
	targetMark->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\TargetMark\\target.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));
}