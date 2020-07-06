#include "Main.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FND/STD.h"
#include "Phoenix/OS/Path.h"
#include "Phoenix/OS/ResourceManager.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/FrameWork/Renderer/ModelRenderer.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FrameWork/Shader/StandardShader.h"


namespace Phoenix
{
	namespace OS
	{
		std::unique_ptr<IMain> IMain::Create()
		{
			return std::make_unique<Main>();
		}
	}
}

bool Main::Initialize(Phoenix::uintPtr instance)
{
	Super::Initialize(instance);

	player = Player::Create();
	player->Init(graphicsDevice.get());

	boss = Boss::Create();
	boss->Init(graphicsDevice.get());

	const char* filename = "..\\Data\\Assets\\Model\\stage\\stage01.fbx";
	stageModel = std::make_unique<Phoenix::FrameWork::ModelObject>();
	stageModel->Initialize(graphicsDevice.get());
	stageModel->Load(graphicsDevice.get(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), filename));

	renderer.emplace_back(std::make_unique<Phoenix::FrameWork::ModelRenderer>());

	basicShader = Phoenix::FrameWork::BasicShader::Create();
	basicShader->Initialize(graphicsDevice.get());

	basicSkinShader = Phoenix::FrameWork::BasicSkinShader::Create();
	basicSkinShader->Initialize(graphicsDevice.get());

	standardShader = Phoenix::FrameWork::StandardShader::Create();
	standardShader->Initialize(graphicsDevice.get());

	cameraFlg = false;

	return true;
}

void Main::Finalize()
{
	Super::Finalize();
}

void Main::Update()
{
	// パラメーター調整用GUI
	{
		GUI();
	}

	// プレイヤー更新
	{
		player->Update(camera);
	}

	// ボス更新
	{
		boss->Update(camera);
	}

	// カメラ更新
	{
		if (cameraFlg)
		{
			camera.FreeCamera();
		}
		else
		{
			camera.ControllerCamera(player->GetPosition(), Phoenix::Math::Vector3(0.0f, 100.0f, 0.0f));
		}
		camera.Update();
	}
}

// GUI
void Main::GUI()
{
	ImGui::Begin("MainGUI");
	{
		player->GUI();
		if (ImGui::TreeNode("Camera"))
		{
			ImGui::Checkbox("FreeCamera", &cameraFlg);
			ImGui::TreePop();
		}
		Phoenix::Graphics::DirLight* dir = static_cast<Phoenix::FrameWork::StandardShader*>(standardShader.get())->GetLight()->GetDefaultDirLight();
		if (ImGui::TreeNode("Light"))
		{
			ImGui::DragFloat3("dir", &dir->direction.x, 0.01f, -1.0f, 1.0f);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void Main::Render()
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	// ワールド行列を作成
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

	// メッシュ描画
#if 1
	basicSkinShader->Begin(graphicsDevice.get(), camera);
	basicSkinShader->Draw(graphicsDevice.get(), player->GetWorldMatrix(), player->GetModel());
	basicSkinShader->Draw(graphicsDevice.get(), boss->GetWorldMatrix(), boss->GetModel());
	basicSkinShader->End(graphicsDevice.get());
#else
	standardShader->Begin(graphicsDevice.get(), camera);
	standardShader->Draw(graphicsDevice.get(), player->GetWorldMatrix(), player->GetModel());
	standardShader->End(graphicsDevice.get());
#endif

#if 1
	//basicSkinShader->Begin(graphicsDevice.get(), camera);
	//basicSkinShader->Draw(graphicsDevice.get(), boss->GetWorldMatrix(), boss->GetModel());
	//basicSkinShader->End(graphicsDevice.get());
#else
	standardShader->Begin(graphicsDevice.get(), camera);
	standardShader->Draw(graphicsDevice.get(), boss->GetWorldMatrix(), boss->GetModel());
	standardShader->End(graphicsDevice.get());
#endif

#if 1
	basicShader->Begin(graphicsDevice.get(), camera);
	basicShader->Draw(graphicsDevice.get(), W, stageModel.get());
	basicShader->End(graphicsDevice.get());
#else
	standardShader->Begin(graphicsDevice.get(), camera);
	standardShader->Draw(graphicsDevice.get(), W, stageModel.get());
	standardShader->End(graphicsDevice.get());
#endif
}