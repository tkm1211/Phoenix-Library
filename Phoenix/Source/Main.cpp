#include "Main.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FND/STD.h"
#include "Phoenix/OS/Path.h"
#include "Phoenix/OS/ResourceManager.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/FrameWork/Renderer/ModelRenderer.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"


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

	const char* filename = "..\\Data\\Assets\\Model\\stage\\stage01.fbx";
	stageModel = std::make_unique<Phoenix::FrameWork::ModelObject>();
	stageModel->Initialize(graphicsDevice.get());
	stageModel->Load(graphicsDevice.get(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), filename));

	renderer.emplace_back(std::make_unique<Phoenix::FrameWork::ModelRenderer>());

	basicShader = Phoenix::FrameWork::BasicShader::Create();
	basicShader->Initialize(graphicsDevice.get());

	basicSkinShader = Phoenix::FrameWork::BasicSkinShader::Create();
	basicSkinShader->Initialize(graphicsDevice.get());

	cameraFlg = false;

	return true;
}

void Main::Finalize()
{
	Super::Finalize();
}

void Main::Update()
{
	GUI();

	player->Update(camera);
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
	}
	ImGui::End();
}

void Main::Render()
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	// メッシュ描画
	basicSkinShader->Begin(graphicsDevice.get());
	renderer[0]->Begin(graphicsDevice.get(), camera);
	renderer[0]->Draw(graphicsDevice.get(), player->GetWorldMatrix(), player->GetModel(), basicSkinShader.get());
	renderer[0]->End(graphicsDevice.get());
	basicSkinShader->End(graphicsDevice.get());

	// ワールド行列を作成
	Phoenix::Math::Matrix W;
	{
		Phoenix::Math::Vector3 scale = { 20.0f, 20.0f, 20.0f };
		Phoenix::Math::Vector3 rotate = { 0.0f, 0.0f, 0.0f };
		Phoenix::Math::Vector3 translate = { 0.0f, 0.0f, 0.0f };

		Phoenix::Math::Matrix S, R, T;
		S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
		R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
		T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

		W = S * R * T;
	}

	basicShader->Begin(graphicsDevice.get());
	renderer[0]->Begin(graphicsDevice.get(), camera);
	renderer[0]->Draw(graphicsDevice.get(), W, stageModel.get(), basicShader.get());
	renderer[0]->End(graphicsDevice.get());
	basicShader->End(graphicsDevice.get());
}