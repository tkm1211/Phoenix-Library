#include "Boss.h"
#include "BossAI.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"


std::unique_ptr<Boss> Boss::Create()
{
	return std::make_unique<Boss>();
}

void Boss::Init(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Player* player)
{
	// モデル読み込み
	{
		model = std::make_unique<Phoenix::FrameWork::ModelObject>();
		model->Initialize(graphicsDevice);
		model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Boss\\Mutant\\Idle\\Mutant_Breathing_Idle.fbx");
	}

	// アニメーション読み込み
	{
		model->LoadAnimation("..\\Data\\Assets\\Model\\Boss\\Mutant\\Run\\Mutant_Run.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Boss\\Mutant\\Attack\\Right\\Mutant_Swiping.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Boss\\Mutant\\Attack\\Left\\Mutant_Punch.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Boss\\Mutant\\Attack\\Jump\\Jump_Attack.fbx", -1);
	}

	// 待機モーション開始
	{
		model->PlayAnimation(0, 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		//model->PauseAnimation(true);
	}

	// トランスフォームの初期化
	{
		worldMatrix = Phoenix::Math::MatrixIdentity();
		pos = { 0,0,-1000.0f };
		//rotate = { 0,0,0 };
		rotate = { 0,0,0,1 };
		scale = { 1.5f,1.5f,1.5f };
		radius = 75.0f;
	}

	// プレイヤーアドレス取得
	{
		this->player = player;
	}

	// AI初期化
	{
		ai = BossAI::Create();
		ai->Init();

		bossAI = static_cast<BossAI*>(ai.get());
		bossAI->SharedBossPtr(this);
		bossAI->SharedPlayerPtr(player);

		currentType = bossAI->GetCurrentStateType();
	}

	// インデックス取得
	{
		boneIndex = model->GetBoneIndex("Mutant:Hips");
	}
}

void Boss::Update()
{
	// ステートタイプ変更 (アニメーション変更)
	if (nextType != AIStateType::None)
	{
		currentType = nextType;
		nextType = AIStateType::None;
		ChangeAnimation(currentType);
	}

	// AI更新
	{
		ai->Update();
	}

	// 次のステートタイプを取得
	{
		nextType = bossAI->GetNextStateType();
	}

	// アニメーション更新
	{
		model->UpdateTransform(1 / 60.0f);
	}

	// ワールド行列を作成
	{
		Phoenix::Math::Matrix S, R, T;
		S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
		//R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
		R = Phoenix::Math::MatrixRotationQuaternion(&rotate);
		T = Phoenix::Math::MatrixTranslation(pos.x, pos.y, pos.z);
		worldMatrix = S * R * T;
	}
}

void Boss::ChangeAnimation(AIStateType type)
{
	switch (type)
	{
	case AIStateType::Wait:
		model->PlayAnimation(0, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		break;

	case AIStateType::Move:
		model->PlayAnimation(1, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		break;

	case AIStateType::SwingAttack01:
		model->PlayAnimation(2, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		break;

	case AIStateType::SwingAttack02:
		model->PlayAnimation(3, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		break;

	case AIStateType::JumpAttack:
		model->PlayAnimation(4, 0, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		break;

	case AIStateType::None:
		break;

	default: break;
	}
}

void Boss::GUI()
{
	if (ImGui::TreeNode("Boss"))
	{
		if (ImGui::TreeNode("Prameter"))
		{
			ImGui::DragFloat3("pos", &pos.x);
			ImGui::TreePop();
		}
		ai->GUI();
		if (ImGui::TreeNode("Player data as seen by the boss"))
		{
			Phoenix::Math::Vector3 pos = player->GetPosition();
			ImGui::Text("Pos : %f, %f, %f", pos.x, pos.y, pos.z);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Bone"))
		{
			/*for (auto& node : *model->GetNodes())
			{
				if (node.parent == nullptr)
				{
					ImGui::Text(node.name);
					ImGui::Text("translate : %f, %f, %f", node.translate.x, node.translate.y, node.translate.z);
				}
			}*/

			Phoenix::Math::Matrix boneM = model->GetBoneTransforms(0, boneIndex);
			Phoenix::Math::Matrix mat = boneM * worldMatrix;
			Phoenix::Math::Matrix matI = boneM * Phoenix::Math::MatrixInverse(worldMatrix);

			ImGui::Text("wolrd translate : %f, %f, %f", worldMatrix._41, worldMatrix._42, worldMatrix._43);
			ImGui::Text("bone translate : %f, %f, %f", boneM._41, boneM._42, boneM._43);
			ImGui::Text("bone translate * worldMatrix : %f, %f, %f", mat._41, mat._42, mat._43);
			ImGui::Text("bone translate * Inverse(worldMatrix) : %f, %f, %f", matI._41, matI._42, matI._43);

			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}