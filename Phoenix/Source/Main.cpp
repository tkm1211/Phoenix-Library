#include "Main.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/OS/ResourceManager.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/FrameWork/Renderer/ModelRenderer.h"
#include "Phoenix/FND/STD.h"

#define USE_ANIM

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

	// 定数バッファ
	{
		cbMatrial = Phoenix::Graphics::IBuffer::Create();

		Phoenix::Graphics::PhoenixBufferDesc desc = {};
		Phoenix::FND::MemSet(&desc, 0, sizeof(desc));
		desc.usage = Phoenix::Graphics::PhoenixUsage::Default;
		desc.bindFlags = static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixBindFlag::ConstantBuffer);
		desc.cpuAccessFlags = 0;
		desc.miscFlags = 0;
		desc.byteWidth = sizeof(CbMaterial);
		desc.structureByteStride = 0;
		if (!cbMatrial->Initialize(graphicsDevice->GetDevice(), desc))
		{
			return false;
		}
	}

#ifdef USE_ANIM
	Phoenix::Graphics::PhoenixInputElementDesc inputElementDesc[] =
	{
		// SemanticName	 SemanticIndex	Format													InputSlot	AlignedByteOffset	InputSlotClass										InstanceDataStepRate
		{"POSITION",	 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,		0,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
		{"TEXCOORD",	 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32_FLOAT,			1,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
		{"BLENDWEIGHT",	 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32A32_FLOAT,	2,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
		{"BLENDWEIGHT",	 1,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32A32_FLOAT,	3,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
		{"BLENDINDICES", 0,				Phoenix::Graphics::PHOENIX_FORMAT_R8G8B8A8_UINT,		4,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
		{"BLENDINDICES", 1,				Phoenix::Graphics::PHOENIX_FORMAT_R8G8B8A8_UINT,		5,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
	};
#else
	Phoenix::Graphics::PhoenixInputElementDesc inputElementDesc[] =
	{
		// SemanticName	 SemanticIndex	Format													InputSlot	AlignedByteOffset	InputSlotClass										InstanceDataStepRate
		{"POSITION",	 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,		0,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
		{"TEXCOORD",	 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32_FLOAT,			1,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
		//{"BLENDWEIGHT",	 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32A32_FLOAT,	2,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
		//{"BLENDWEIGHT",	 1,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32A32_FLOAT,	3,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
		//{"BLENDINDICES", 0,				Phoenix::Graphics::PHOENIX_FORMAT_R8G8B8A8_UINT,		4,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
		//{"BLENDINDICES", 1,				Phoenix::Graphics::PHOENIX_FORMAT_R8G8B8A8_UINT,		5,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
	};
#endif

	shader = Phoenix::Graphics::IShader::Create();
#ifdef USE_ANIM
	shader->LoadVS(graphicsDevice->GetDevice(), "C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Library\\PhoenixLibrary\\Build\\vs2019\\obj\\PhoenixLib_HLSL\\x86\\Debug\\BasicVSSkin.cso", inputElementDesc, Phoenix::FND::ArraySize(inputElementDesc));
#else
	shader->LoadVS(graphicsDevice->GetDevice(), "C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Library\\PhoenixLibrary\\Build\\vs2019\\obj\\PhoenixLib_HLSL\\x86\\Debug\\BasicVS.cso", inputElementDesc, Phoenix::FND::ArraySize(inputElementDesc));
#endif
	shader->LoadPS(graphicsDevice->GetDevice(), "C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Library\\PhoenixLibrary\\Build\\vs2019\\obj\\PhoenixLib_HLSL\\x86\\Debug\\BasicPS.cso");

	renderer.emplace_back(std::make_unique<Phoenix::FrameWork::ModelRenderer>());

	model = std::make_unique<Phoenix::FrameWork::ModelObject>();
	model->Initialize(graphicsDevice.get());
	//model->Load(graphicsDevice.get(), "C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Data\\Assets\\Model\\Player\\MDL_Player_Attack.fbx");
	//model->Load(graphicsDevice.get(), "C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Data\\Assets\\Model\\danbo_fbx\\danbo_atk.fbx");
	model->Load(graphicsDevice.get(), "C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Data\\Assets\\Model\\Hip_Hop_Dancing\\Hip_Hop_Dancing.fbx");
	//model->Load(graphicsDevice.get(), "C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Data\\Assets\\Model\\Catwalk_Walk_Turn_180_Tight_60\\Catwalk_Walk_Turn_180_Tight.fbx");
	model->PlayAnimation(0, 0);

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
	camera.ZoomOnSphere(c, r);
	camera.Update();

	ImGui::Begin("test");
	ImGui::Text("Now test.");
	ImGui::DragFloat3("pos", &pos.x);
	ImGui::DragFloat3("rotate", &rotate.x);
	ImGui::DragFloat3("scale", &scale.x);
	ImGui::DragFloat3("center", &c.x);
	ImGui::DragFloat("zoom", &r);
	ImGui::InputInt("AnimClip", &animClip);
	if (ImGui::Button("play"))
	{
		model->PlayAnimation(0, animClip);
	}
	ImGui::End();
}

void Main::Begin()
{
	
}

void Main::Render()
{
	Begin();

	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	// ワールド行列を作成
	Phoenix::Math::Matrix W;
	{
		static float rx = 0;
		static float ry = 0;
		//rx += DirectX::XMConvertToRadians(0.25f);	// 角度をラジアン(θ)に変換
		//ry += DirectX::XMConvertToRadians(0.5f);	// 角度をラジアン(θ)に変換

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

	// メッシュ描画
	Phoenix::Graphics::IBuffer* buffers[] =
	{
		cbMatrial.get()
	};
	renderer[0]->Begin(graphicsDevice.get(), camera, buffers, Phoenix::FND::ArraySize(buffers));

	// メッシュ定数バッファ更新
	CbMaterial cb = {};
	cb.color = { 1,1,1,1 };
	context->UpdateSubresource(cbMatrial.get(), 0, 0, &cb, 0, 0);
	context->UpdateConstantBufferMesh(W);

	shader->Activate(graphicsDevice->GetDevice());
#ifdef USE_ANIM
	renderer[0]->Draw(graphicsDevice.get(), model.get(), true);
#else
	renderer[0]->Draw(graphicsDevice.get(), model.get(), false);
#endif
	shader->Deactivate(graphicsDevice->GetDevice());

	renderer[0]->End(graphicsDevice.get());

	End();
}

void Main::End()
{

}