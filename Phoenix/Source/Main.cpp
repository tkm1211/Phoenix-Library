#include "Main.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/OS/ResourceManager.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/FrameWork/Component/ModelRenderer.h"


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

	//std::string filename = "C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Data\\Assets\\Model\\danbo_fbx\\danbo_atk.fbx";
	//if (Phoenix::OS::Path::CheckFileExtension(filename.c_str(), "fbx"))
	//{
	//	std::unique_ptr<Phoenix::Loader::ILoader> loader = Phoenix::Loader::ILoader::Create();
	//	if (!loader->Initialize(filename.c_str()))
	//	{
	//		return false;
	//	}
	//	if (!loader->Load(data))
	//	{
	//		return false;
	//	}
	//	//Phoenix::Graphics::ModelData::Serialize(data, m_model_filename.c_str());
	//}

	Phoenix::Graphics::PhoenixInputElementDesc inputElementDesc[] =
	{
		{ "POSITION", 0, Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT, 0, PHOENIX_APPEND_ALIGNED_ELEMENT, Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT, 0, PHOENIX_APPEND_ALIGNED_ELEMENT, Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32A32_FLOAT, 0, PHOENIX_APPEND_ALIGNED_ELEMENT, Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, Phoenix::Graphics::PHOENIX_FORMAT_R32G32_FLOAT, 0, PHOENIX_APPEND_ALIGNED_ELEMENT, Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32A32_FLOAT, 0, PHOENIX_APPEND_ALIGNED_ELEMENT, Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES", 0, Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32A32_UINT, 0, PHOENIX_APPEND_ALIGNED_ELEMENT, Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA, 0 },
	};

	shader = Phoenix::Graphics::IShader::Create();
	shader->LoadVS(graphicsDevice->GetDevice(), "C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Data\\Shader\\SkinnedMeshVS.cso", inputElementDesc, Phoenix::FND::ArraySize(inputElementDesc));

	renderer.emplace_back(std::make_unique<Phoenix::FrameWork::ModelRenderer>());

	model = std::make_unique<Phoenix::FrameWork::ModelObject>();
	model->Initialize(graphicsDevice.get());
	model->Load("C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Data\\Assets\\Model\\danbo_fbx\\danbo_atk.fbx");

	return true;
}

void Main::Finalize()
{
	Super::Finalize();
}

void Main::Update()
{
	ImGui::Begin("test");
	ImGui::Text("Now test.");
	ImGui::End();

	//camera.Update();
}

void Main::Begin()
{
	
}

void Main::Render()
{
	Begin();

	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	// メッシュ定数バッファ更新
	{
		//Phoenix::Math::Matrix worldTransform = Phoenix::Math::MatrixIdentity();
		context->UpdateConstantBufferMesh(Phoenix::Math::MatrixIdentity());
	}

	// ワールド行列を作成
	Phoenix::Math::Matrix W;
	{
		static float rx = 0;
		static float ry = 0;
		//rx += DirectX::XMConvertToRadians(0.25f);	// 角度をラジアン(θ)に変換
		//ry += DirectX::XMConvertToRadians(0.5f);	// 角度をラジアン(θ)に変換

		Phoenix::Math::Vector3 scale(1, 1, 1);
		Phoenix::Math::Vector3 rotate(rx, ry, 0);
		Phoenix::Math::Vector3 translate(0, 0, 0);

		Phoenix::Math::Matrix S, R, T;
		S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
		//R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);	// ZXY回転
		R = Phoenix::Math::MatrixRotationYawPitchRoll(rotate.x, rotate.y, rotate.z);	// ZXY回転
		T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

		W = S * R * T;
	}
	model->CalculateLocalTransform();
	model->CalculateWorldTransform(W);

	// メッシュ描画
	renderer[0]->Begin(graphicsDevice.get(), camera);
	//renderer[0]->Draw(graphicsDevice.get(), model.get());
	renderer[0]->End(graphicsDevice.get());

	End();
}

void Main::End()
{

}