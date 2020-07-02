#include "Main.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FND/STD.h"
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

	Phoenix::Math::Vector3 vector3;
	vector3.x++;

	int i = 0;
	i++;
	
	renderer.emplace_back(std::make_unique<Phoenix::FrameWork::ModelRenderer>());

	model = std::make_unique<Phoenix::FrameWork::ModelObject>();
	model->Initialize(graphicsDevice.get());
	//model->Load(graphicsDevice.get(), "C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Data\\Assets\\Model\\Player\\MDL_Player_Attack.fbx");
	//model->Load(graphicsDevice.get(), "C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Data\\Assets\\Model\\danbo_fbx\\danbo_atk.fbx");
	model->Load(graphicsDevice.get(), "C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Data\\Assets\\Model\\Hip_Hop_Dancing\\Hip_Hop_Dancing.fbx");
	//model->Load(graphicsDevice.get(), "C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Data\\Assets\\Model\\Catwalk_Walk_Turn_180_Tight_60\\Catwalk_Walk_Turn_180_Tight.fbx");
	model->PlayAnimation(0, 0);

	basicShader = Phoenix::FrameWork::BasicShader::Create();
	basicShader->Initialize(graphicsDevice.get());

	basicSkinShader = Phoenix::FrameWork::BasicSkinShader::Create();
	basicSkinShader->Initialize(graphicsDevice.get());

	pos = { 0,0,0 };
	rotate = { 0,0,0 };
	scale = { 1,1,1 };

	return true;
}

void Main::Finalize()
{
	Super::Finalize();
}

void Main::Update()
{
	static Phoenix::Math::Vector3 c = { 0, 0, 0 };
	static Phoenix::f32 r = 365.0f;
	static Phoenix::s32 animClip = 0;
	//camera.ZoomOnSphere(c, r);
	camera.FreeCamera();
	camera.Update();

	ImGui::Begin("test");
	ImGui::Text("Now test.");
	ImGui::DragFloat3("pos", &pos.x);
	ImGui::DragFloat3("rotate", &rotate.x);
	ImGui::DragFloat3("scale", &scale.x);
	//ImGui::DragFloat3("center", &c.x);
	//ImGui::DragFloat("zoom", &r);
	ImGui::InputInt("AnimClip", &animClip);
	if (ImGui::Button("Play"))
	{
		model->PlayAnimation(0, animClip);
	}
	if (ImGui::Button("LoopPlay"))
	{
		model->SetLoopAnimation(true);
	}
	ImGui::End();
}

void Main::Render()
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	// ワールド行列を作成
	Phoenix::Math::Matrix W;
	{
		Phoenix::Math::Vector3 scale = this->scale;
		Phoenix::Math::Vector3 rotate = this->rotate;
		Phoenix::Math::Vector3 translate = pos;

		Phoenix::Math::Matrix S, R, T;
		S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
		R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
		T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

		W = S * R * T;
	}

	model->UpdateTransform(1 / 60.0f);

#if 1
	// メッシュ描画
	//basicShader->Begin(graphicsDevice.get());
	basicSkinShader->Begin(graphicsDevice.get());
	renderer[0]->Begin(graphicsDevice.get(), W, camera);
	renderer[0]->Draw(graphicsDevice.get(), model.get(), basicSkinShader.get());
	renderer[0]->End(graphicsDevice.get());
	basicSkinShader->End(graphicsDevice.get());
	//basicShader->End(graphicsDevice.get());
#endif
}