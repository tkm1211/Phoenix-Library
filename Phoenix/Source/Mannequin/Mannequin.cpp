#include "Mannequin.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"


std::unique_ptr<Mannequin> Mannequin::Create()
{
	return std::make_unique<Mannequin>();
}

void Mannequin::Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	// モデル読み込み
	{
		model = std::make_unique<Phoenix::FrameWork::ModelObject>();
		model->Initialize(graphicsDevice);
		model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Enemy\\Idol\\Mutant_Breathing_Idle.fbx");
		//model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Mixamo\\Sword_And_Shield_Attack\\Sword_And_Shield_Attack.fbx");

		model->AddAnimationLayer(0);
	}

	// コリジョン初期化
	{
		collisionDatas.resize(1);

		collisionDatas.at(0).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(0).radius = 1.25f;
		collisionDatas.at(0).boneIndex = model->GetBoneIndex("Hips");
	}
}

void Mannequin::Initialize()
{
	// 待機モーション開始
	{
		model->PlayAnimation(0, 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
	}

	// トランスフォームの初期化
	{
		worldMatrix = Phoenix::Math::MatrixIdentity();
		pos = { 0,0,55.0f };
		rotate = { 0,0,0,1 };
		scale = { 1.25f,1.25f,1.25f };
		radius = 1.25f;
	}
}

void Mannequin::Update(Phoenix::Graphics::Camera& camera, bool onControl)
{
	// アニメーション更新
	{
		model->UpdateTransform(1 / 60.0f);
	}

	// ワールド行列を作成
	{
		UpdateTrasform();
	}

	// コリジョン更新
	{
		Phoenix::Math::Matrix systemUnitTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		systemUnitTransform._11 = systemUnitTransform._22 = systemUnitTransform._33 = 0.01f;

		auto nodes = model->GetNodes();
		for (auto& data : collisionDatas)
		{
			if (data.boneIndex == -1)
			{
				data.pos = pos;
				continue;
			}
			Phoenix::Math::Matrix bone = nodes->at(data.boneIndex).worldTransform;
			bone *= systemUnitTransform * worldMatrix;
			data.pos = Phoenix::Math::Vector3(bone._41, bone._42, bone._43);
		}
	}
}

void Mannequin::UpdateTrasform()
{
	Phoenix::Math::Vector3 scale = this->scale;
	Phoenix::Math::Quaternion rotate = this->rotate;
	Phoenix::Math::Vector3 translate = pos;

	Phoenix::Math::Matrix S, R, T;
	S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
	R = Phoenix::Math::MatrixRotationQuaternion(&rotate);
	T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

	worldMatrix = S * R * T;
}

void Mannequin::GUI()
{
	if (ImGui::TreeNode("Boss"))
	{
		if (ImGui::TreeNode("Prameter"))
		{
			ImGui::DragFloat3("pos", &pos.x);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}