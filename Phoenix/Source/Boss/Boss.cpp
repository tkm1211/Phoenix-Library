#include "Boss.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../Enemy/EnemyManager.h"


//std::unique_ptr<Boss> Boss::Create()
//{
//	return std::make_unique<Boss>();
//}
//
//void Boss::Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Player* player)
//{
//	// モデル読み込み
//	{
//		model = std::make_unique<Phoenix::FrameWork::ModelObject>();
//		model->Initialize(graphicsDevice);
//		model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Boss\\Mutant\\Idle\\Mutant_Roaring.fbx"); // Mutant_Idle02 Mutant_Breathing_Idle Mutant_Roaring
//
//		effectModel = std::make_unique<Phoenix::FrameWork::ModelObject>();
//		effectModel->Initialize(graphicsDevice);
//		effectModel->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Effect\\BossEffect\\JumpAttackEffect01.fbx");
//	}
//
//	// アニメーション読み込み
//	{
//		model->LoadAnimation("..\\Data\\Assets\\Model\\Boss\\Mutant\\Run\\Mutant_Run.fbx", -1);
//		model->LoadAnimation("..\\Data\\Assets\\Model\\Boss\\Mutant\\Avoid\\Sprinting_Forward_Roll.fbx", -1);
//		model->LoadAnimation("..\\Data\\Assets\\Model\\Boss\\Mutant\\Attack\\Right\\Mutant_Swiping.fbx", -1);
//		model->LoadAnimation("..\\Data\\Assets\\Model\\Boss\\Mutant\\Attack\\Left\\Mutant_Punch.fbx", -1);
//		model->LoadAnimation("..\\Data\\Assets\\Model\\Boss\\Mutant\\Attack\\Rotate\\Right_Rotate.fbx", -1);
//		model->LoadAnimation("..\\Data\\Assets\\Model\\Boss\\Mutant\\Attack\\Jump\\Jump_Attack02.fbx", -1);
//		model->LoadAnimation("..\\Data\\Assets\\Model\\Boss\\Mutant\\Damage\\Head_Hit.fbx", -1);
//
//		model->AddAnimationLayer(0);
//		model->AddAnimationLayer(1);
//		model->AddAnimationLayer(2);
//		model->AddAnimationLayer(3);
//		model->AddAnimationLayer(4);
//		model->AddAnimationLayer(5);
//		model->AddAnimationLayer(6);
//		model->AddAnimationLayer(7);
//
//		effectModel->AddAnimationLayer(0);
//	}
//
//	// コリジョン初期化
//	{
//		collisionDatas.resize(4);
//
//		collisionDatas.at(0).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
//		collisionDatas.at(0).radius = 1.5f;
//		collisionDatas.at(0).boneIndex = model->GetBoneIndex("Mutant:Hips");
//
//		collisionDatas.at(1).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
//		collisionDatas.at(1).radius = 0.5f;
//		collisionDatas.at(1).boneIndex = model->GetBoneIndex("Mutant:RightHandIndex1");
//
//		collisionDatas.at(2).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
//		collisionDatas.at(2).radius = 1.3f;
//		collisionDatas.at(2).boneIndex = model->GetBoneIndex("Mutant:LeftHand");
//
//		collisionDatas.at(3).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
//		collisionDatas.at(3).radius = 2.65f;
//		collisionDatas.at(3).boneIndex = -1;
//	}
//
//	// プレイヤーアドレス取得
//	{
//		this->player = player;
//	}
//
//	// UI生成
//	{
//		ui = BossUI::Create();
//	}
//
//	// AI初期化
//	{
//		ai = BossAI::Create();
//		ai->Construct();
//
//		bossAI = static_cast<BossAI*>(ai.get());
//		bossAI->SharedBossPtr(this);
//		bossAI->SharedPlayerPtr(player);
//	}
//}
//
//void Boss::Initialize()
//{
//	// 待機モーション開始
//	{
//		model->PlayAnimation(0, 0, 1);
//		model->UpdateTransform(1 / 60.0f);
//		model->SetLoopAnimation(false);
//		model->SetSpeed(2.0f);
//		//model->PauseAnimation(true);
//	}
//
//	// トランスフォームの初期化
//	{
//		worldMatrix = Phoenix::Math::MatrixIdentity();
//		pos = { 0,0,-12.0f }; // test : 100.0f
//		//rotate = { 0,0,0 };
//		rotate = { 0,0,0,1 };
//		scale = { 2.0f,2.0f,2.0f };
//		//scale = { 1.0f,1.0f,1.0f };
//		radius = 0.75f;
//		life = MaxLife;
//		accumulationDamege = 0;
//		accumulationTimeCnt = 0;
//		isChangeAccumulationDamege = false;
//	}
//
//	// AI初期化
//	{
//		ai->Initialize();
//		currentType = bossAI->GetCurrentStateType();
//	}
//
//	// インデックス取得
//	{
//		boneIndex = model->GetBoneIndex("Mutant:Hips");
//	}
//
//	isDissolve = false;
//	isJumpAttackStart = false;
//	onJumpAttackStart = false;
//}
//
//void Boss::Update(bool onControl)
//{
//	// 蓄積ダメージの確認
//	{
//		AccumulationDamege();
//	}
//
//	// ステートタイプ変更 (アニメーション変更)
//	if (nextType != AIStateType::None)
//	{
//		currentType = nextType;
//		nextType = AIStateType::None;
//		ChangeAnimation(currentType);
//	}
//
//	// AI更新
//	{
//		if (onControl) ai->Update();
//	}
//
//	// 次のステートタイプを取得
//	{
//		nextType = bossAI->GetNextStateType();
//	}
//
//	// アニメーション更新
//	{
//		model->UpdateTransform(1 / 60.0f);
//
//#if 0
//		// TODO : リファクタリング (アニメーション時間の取得)
//		if (!isDissolve) effectModel->UpdateTransform(1 / 60.0f);
//		if (IsJumpAttack())
//		{
//			if (animCnt <= 150.0f / 60.0f)
//			{
//				isDissolve = false;
//			}
//			else
//			{
//				isDissolve = true;
//			}
//			animCnt += 1.0f / 60.0f;
//		}
//		else
//		{
//			animCnt = 0.0f;
//			isDissolve = false;
//		}
//#else
//		if (IsJumpAttack()) effectModel->UpdateTransform(1 / 60.0f);
//#endif
//	}
//
//	// ワールド行列を作成
//	{
//		//pos.y -= 9.8f * 0.01f;
//		//if (pos.y <= 0.0f) pos.y = 0.0f;
//
//		Phoenix::Math::Matrix S, R, T;
//		S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
//		//R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
//		R = Phoenix::Math::MatrixRotationQuaternion(&rotate);
//		T = Phoenix::Math::MatrixTranslation(pos.x, pos.y, pos.z);
//		worldMatrix = S * R * T;
//	}
//
//	// コリジョン更新
//	{
//		Phoenix::Math::Matrix systemUnitTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
//		systemUnitTransform._11 = systemUnitTransform._22 = systemUnitTransform._33 = 0.01f;
//
//		auto nodes = model->GetNodes();
//		for (auto& data : collisionDatas)
//		{
//			if (data.boneIndex == -1)
//			{
//				data.pos = pos;
//				continue;
//			}
//			Phoenix::Math::Matrix bone = nodes->at(data.boneIndex).worldTransform;
//			bone *= systemUnitTransform * worldMatrix;
//			data.pos = Phoenix::Math::Vector3(bone._41, bone._42, bone._43);
//		}
//	}
//
//	// アタック判定中
//	{
//		AttackJudgment();
//	}
//}
//
//void Boss::AccumulationDamege()
//{
//	if (accumulationDamege == 0) return;
//
//	if (AccumulationMaxDamege <= accumulationDamege)
//	{
//		isChangeAccumulationDamege = true;
//		nextType = AIStateType::Damage;
//
//		accumulationDamege = 0;
//		accumulationTimeCnt = 0;
//	}
//	else if (AccumulationTime <= accumulationTimeCnt++)
//	{
//		accumulationDamege = 0;
//		accumulationTimeCnt = 0;
//	}
//}
//
//void Boss::UpdateUI()
//{
//	Phoenix::f32 hp = static_cast<Phoenix::f32>(life);
//	hp = hp <= 0 ? 0 : hp;
//
//	ui->Update((hp / MaxLife) * 100.0f);
//}
//
//void Boss::ChangeAnimation(AIStateType type)
//{
//	switch (type)
//	{
//	case AIStateType::Wait:
//		model->PlayAnimation(0, 1, 0.2f);
//		model->UpdateTransform(1 / 60.0f);
//		model->SetLoopAnimation(false);
//		model->SetSpeed(2.0f);
//		break;
//
//	case AIStateType::Move:
//		model->PlayAnimation(1, 1, 0.2f);
//		model->UpdateTransform(1 / 60.0f);
//		model->SetLoopAnimation(true);
//		break;
//
//	case AIStateType::Avoid:
//		model->PlayAnimation(2, 1, 0.2f);
//		model->UpdateTransform(1 / 60.0f);
//		model->SetLoopAnimation(false);
//		model->SetSpeed(1.5f);
//		break;
//
//	case AIStateType::SwingAttack01:
//		model->PlayAnimation(3, 1, 0.2f);
//		model->UpdateTransform(1 / 60.0f);
//		model->SetLoopAnimation(false);
//		break;
//
//	case AIStateType::SwingAttack02:
//		model->PlayAnimation(4, 1, 0.2f);
//		model->UpdateTransform(1 / 60.0f);
//		model->SetLoopAnimation(false);
//		break;
//
//	case AIStateType::RotationAttack:
//		model->PlayAnimation(5, 1, 0.2f);
//		model->UpdateTransform(1 / 60.0f);
//		model->SetLoopAnimation(false);
//		break;
//
//	case AIStateType::JumpAttack:
//		model->PlayAnimation(6, 0, 0.2f);
//		model->UpdateTransform(1 / 60.0f);
//		model->SetLoopAnimation(false);
//
//		effectModel->PlayAnimation(0, 0, 0.2f);
//		effectModel->UpdateTransform(1 / 60.0f);
//		effectModel->SetLoopAnimation(false);
//		//effectModel->SetEndTime(160.0f / 60.0f);
//		break;
//
//	case AIStateType::Damage:
//		model->PlayAnimation(7, 1, 0.2f);
//		model->UpdateTransform(1 / 60.0f);
//		model->SetLoopAnimation(false);
//		break;
//
//	case AIStateType::None:
//		break;
//
//	default: break;
//	}
//}
//
//void Boss::AttackJudgment()
//{
//	auto Judgment = [&](Phoenix::s32 index)
//	{
//		if (isHit)
//		{
//			isAttackJudgment = false;
//			return;
//		}
//
//		isAttackJudgment = true;
//		attackCollisionIndex = index;
//	};
//	auto NoJudgment = [&]()
//	{
//		isAttackJudgment = false;
//		isHit = false;
//		attackCollisionIndex = -1;
//	};
//
//	if (currentType == AIStateType::SwingAttack01)
//	{
//		BossAI* bossAI = static_cast<BossAI*>(ai.get());
//		float time = static_cast<SwingAttackState01*>(bossAI->GetCurrentState())->GetAnimationCnt() * 60.0f;
//
//		if (76.0f <= time && time <= 90.0f)
//		{
//			Judgment(2);
//		}
//		else
//		{
//			NoJudgment();
//		}
//	}
//	else if (currentType == AIStateType::SwingAttack02)
//	{
//		BossAI* bossAI = static_cast<BossAI*>(ai.get());
//		float time = static_cast<SwingAttackState02*>(bossAI->GetCurrentState())->GetAnimationCnt() * 60.0f;
//
//		if (14.0f <= time && time <= 23.0f)
//		{
//			Judgment(1);
//		}
//		else
//		{
//			NoJudgment();
//		}
//	}
//	else if (currentType == AIStateType::RotationAttack)
//	{
//		BossAI* bossAI = static_cast<BossAI*>(ai.get());
//		float time = static_cast<RotationAttackState*>(bossAI->GetCurrentState())->GetAnimationCnt() * 60.0f;
//
//		if (37.0f <= time && time <= 77.0f)
//		{
//			Judgment(2);
//		}
//		else
//		{
//			NoJudgment();
//		}
//	}
//	else if (currentType == AIStateType::JumpAttack)
//	{
//		BossAI* bossAI = static_cast<BossAI*>(ai.get());
//		float time = static_cast<JumpAttackState*>(bossAI->GetCurrentState())->GetAnimationCnt() * 60.0f;
//
//		//if (37.0f <= time && time <= 50.0f)
//		if (102.0f <= time && time <= 122.0f)
//		{
//			Judgment(3);
//
//			isJumpAttackStart = false;
//			if (!onJumpAttackStart)
//			{
//				isJumpAttackStart = true;
//				onJumpAttackStart = true;
//			}
//		}
//		else
//		{
//			NoJudgment();
//			onJumpAttackStart = false;
//		}
//	}
//}
//
//void Boss::GUI()
//{
//	if (ImGui::TreeNode("Boss"))
//	{
//		if (ImGui::TreeNode("Prameter"))
//		{
//			ImGui::DragFloat3("pos", &pos.x);
//			ImGui::Text("HP : %d", life);
//			ImGui::TreePop();
//		}
//		ai->GUI();
//		if (ImGui::TreeNode("Player data as seen by the boss"))
//		{
//			Phoenix::Math::Vector3 pos = player->GetPosition();
//			ImGui::Text("Pos : %f, %f, %f", pos.x, pos.y, pos.z);
//			ImGui::TreePop();
//		}
//		if (ImGui::TreeNode("Bone"))
//		{
//			/*for (auto& node : *model->GetNodes())
//			{
//				if (node.parent == nullptr)
//				{
//					ImGui::Text(node.name);
//					ImGui::Text("translate : %f, %f, %f", node.translate.x, node.translate.y, node.translate.z);
//				}
//			}*/
//
//			if (ImGui::TreeNode("BoneName"))
//			{
//				//ImGui::ListBox("BoneListBox\n(single select)", &boneIndex, model->GetBoneNode()->at(0).name.data(), model->GetBoneNode()->at(0).name.size(), 4);
//				//ImGui::ListBox("NodeListBox\n(single select)", &nodeIndex, model->GetNodes()->data(), model->GetNodes()->size(), 4);
//				Phoenix::s32 index = 0;
//				ImGui::ListBox("ListBox\n(single select)", &index, model->GetBoneNames().data(), static_cast<Phoenix::s32>(model->GetBoneNames().size()), 4);
//				ImGui::TreePop();
//			}
//
//			Phoenix::Math::Matrix boneM = model->GetBoneTransforms(0, boneIndex);
//			Phoenix::Math::Matrix mat = boneM * worldMatrix;
//			Phoenix::Math::Matrix matI = boneM * Phoenix::Math::MatrixInverse(worldMatrix);
//
//			ImGui::Text("wolrd translate : %f, %f, %f", worldMatrix._41, worldMatrix._42, worldMatrix._43);
//			ImGui::Text("bone translate : %f, %f, %f", boneM._41, boneM._42, boneM._43);
//			ImGui::Text("bone translate * worldMatrix : %f, %f, %f", mat._41, mat._42, mat._43);
//			ImGui::Text("bone translate * Inverse(worldMatrix) : %f, %f, %f", matI._41, matI._42, matI._43);
//
//			ImGui::TreePop();
//		}
//		ImGui::TreePop();
//	}
//}

// 生成
std::shared_ptr<Boss> Boss::Create()
{
	return std::make_shared<Boss>();
}

// コンストラクタ
void Boss::Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	// モデル読み込み
	{
		model = std::make_unique<Phoenix::FrameWork::ModelObject>();
		model->Initialize(graphicsDevice);
		model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Enemy\\Boss02\\Idle\\Idle.fbx");
	}

	// アニメーション読み込み
	Phoenix::s32 beginIndex, endIndex;
	{
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Walk\\Walk_Forward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Walk\\Walk_Backward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Walk\\Walk_Right.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Walk\\Walk_Left.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Run\\Mutant_Run.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Avoid\\Back_Step.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Damage\\Head_Hit.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Damage\\Head_Hit_Big.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Death\\Mutant_Dying.fbx", -1);

		beginIndex = model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Attack\\Right\\Mutant_Swiping.fbx", -1);
					 model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Attack\\Left\\Hook_Punch.fbx", -1);
		endIndex   = model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Attack\\Rotate\\Right_Rotate.fbx", -1);

		Phoenix::s32 layerNum = 0;
		{
			layerNum = model->AddAnimationLayer();
			layerIndexList.insert(std::make_pair(LayerType::Base, layerNum));

			layerNum = model->AddAnimationLayer(29, 38);
			layerIndexList.insert(std::make_pair(LayerType::LowerBody, layerNum));
		}

		// ステート追加
		Phoenix::s32 stateNum = 0;
		{
			auto AddState = [&](StateType type, Phoenix::u32 animationIndex, Phoenix::u32 layerIndex)
			{
				stateNum = model->AddAnimationStateToLayer(animationIndex, layerIndex);
				stateIndexList.insert(std::make_pair(type, stateNum));
			};

			// ベースレイヤーにステート追加
			layerNum = layerIndexList.at(LayerType::Base);
			{
				for (Phoenix::s32 i = beginIndex; i <= endIndex; ++i)
				{
					model->AddAnimationStateToLayer(i, layerNum);
				}

				AddState(StateType::Idle, 0, layerNum);
				AddState(StateType::Run, 5, layerNum);
				AddState(StateType::DamageSmall, 7, layerNum);
				AddState(StateType::DamageBig, 8, layerNum);
				AddState(StateType::Dedge, 6, layerNum);
				AddState(StateType::Death, 9, layerNum);
			}

			// 下半身レイヤーにブレンドツリー追加
			layerNum = layerIndexList.at(LayerType::LowerBody);
			{
				Phoenix::s32 blendTreeIndex = model->AddBlendTreeToLayer(layerNum);
				model->AddBlendAnimationStateToBlendTree(0, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(1, Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(2, Phoenix::Math::Vector3(0.0f, -1.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(3, Phoenix::Math::Vector3(1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(4, Phoenix::Math::Vector3(-1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
			}
		}
	}

	// コリジョン初期化
	{
		collisionDatas.resize(3);

		collisionDatas.at(0).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(0).radius = 1.0f;
		collisionDatas.at(0).boneIndex = model->GetBoneIndex("Mutant:Hips");

		collisionDatas.at(1).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(1).radius = 0.5f;
		collisionDatas.at(1).boneIndex = model->GetBoneIndex("Mutant:RightHandIndex1");

		collisionDatas.at(2).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(2).radius = 1.3f;
		collisionDatas.at(2).boneIndex = model->GetBoneIndex("Mutant:LeftHand");
	}

	// アタックデータ生成
	{
		auto SetAttackData = [&]
		(
			BossAttackState animState,
			Phoenix::s32 animIndex,

			Phoenix::f32 playSpeed,
			Phoenix::f32 playBeginTime,
			Phoenix::f32 playEndTime,

			Phoenix::s32 collisionNum,
			Phoenix::f32 collisionBeginTime,
			Phoenix::f32 collisionEndTime
		)
		{
			AttackData<BossAttackState> data;

			data.animState = animState;
			data.animIndex = animIndex;

			data.playSpeed = playSpeed;
			data.playBeginTime = playBeginTime == -1.0f ? -1.0f : playBeginTime / 60.0f;
			data.playEndTime = playEndTime == -1.0f ? -1.0f : playEndTime / 60.0f;

			data.collisionNum = collisionNum;
			data.collisionBeginTime = collisionBeginTime == -1.0f ? -1.0f : collisionBeginTime / 60.0f;
			data.collisionEndTime = collisionEndTime == -1.0f ? -1.0f : collisionEndTime / 60.0f;

			return data;
		};

		// 弱攻撃
		{ // TODO : Add attack data.
			// 右フック
			{
				AttackDatas<BossAttackState> datas;

				datas.AddData(SetAttackData(BossAttackState::RightHook, 7, 1.0f, -1.0f, -1.0f, 2, 75.0f, 85.0f));

				attackDatasList.emplace_back(datas);
			}

			// 左フック
			{
				AttackDatas<BossAttackState> datas;

				datas.AddData(SetAttackData(BossAttackState::LeftHook, 8, 1.0f, -1.0f, -1.0f, 1, 55.0f, 70.0f));

				attackDatasList.emplace_back(datas);
			}

			// 左回転
			{
				AttackDatas<BossAttackState> datas;

				datas.AddData(SetAttackData(BossAttackState::LeftTurn, 8, 1.0f, -1.0f, -1.0f, 2, 55.0f, 65.0f));

				attackDatasList.emplace_back(datas);
			}
		}
	}

	// トランスフォームの初期化
	{
		transform = std::make_unique<Phoenix::FrameWork::Transform>();
	}

	// UI生成
	{
		ui = EnemyUI::Create();
	}

	// AIの初期化
	{
		currentMode = EnemyMode::Battle;

		// 通常モードAI
		//ordinaryAI = OrdinaryAIEnemyAI::Create();
		{
			//ordinaryAI->SetOwner(shared_from_this());
		}

		// バトルモードAI
		battleAI = BattleEnemyAI::Create();
		{
			std::shared_ptr<Boss> owner = downcasted_shared_from_this<Boss>();
			attackState = BattleEnemy::Attack<BossAttackState, Boss>::Create(owner);

			battleAI->SetOwner(owner);
			battleAI->SetUp();

			battleAI->AddState(BattleEnemy::Idle::Create());
			battleAI->AddState(BattleEnemy::Walk::Create(owner));
			battleAI->AddState(BattleEnemy::Run::Create(owner));
			battleAI->AddState(BattleEnemy::Dedge::Create(owner));
			battleAI->AddState(BattleEnemy::DamageSmall::Create(owner));
			battleAI->AddState(BattleEnemy::DamageBig::Create(owner));
			battleAI->AddState(BattleEnemy::Guard::Create());
			battleAI->AddState(BattleEnemy::Death::Create());
			battleAI->AddState(attackState);

			attackState->AddAttack(BossAttackState::RightHook);
			attackState->AddAttack(BossAttackState::LeftHook);
			attackState->AddAttack(BossAttackState::LeftTurn);
		}
	}

	// その他のパラメータ初期化
	{
		Initialize();
	}
}

// 初期化
void Boss::Initialize()
{
	// 待機モーション開始
	{
		model->PlayAnimation(layerIndexList.at(LayerType::Base), stateIndexList.at(StateType::Idle), 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
	}

	// AI
	{
		battleAI->GoToState(BattleEnemyState::Idle);
	}

	// パラメーター
	{
		typeTag = TypeTag::Large;

		enable = false;
		alive = false;
		death = false;
		inBattle = false;

		newRotate = Phoenix::Math::Quaternion::Zero;

		life = LifeRange;
		lifeMax = LifeRange;
		radius = 1.0f;

		attackReceptionTimeCnt = 0.0f;
		attackAnimationSpeed = 0.0f;

		distanceToPlayer = 0.0f;

		moveSpeed = 0.0f;

		moveX = 0.0f;
		moveY = 0.0f;

		currentAttackState = BossAttackState::NoneState;
	}
}

// 終了化
void Boss::Finalize()
{
	battleAI->CleanUp();
	battleAI.reset();

	collisionDatas.clear();
	transform.reset();
	model.reset();
}

// 更新
void Boss::Update(bool onControl, Phoenix::f32 elapsedTime)
{
	// ライフが０ならマネージャーの生存エネミーカウントを下げる
	if (life <= 0 && alive)
	{
		alive = false;
		if (std::shared_ptr<EnemyManager> manager = owner.lock())
		{
			manager->SubAliveEnemyCount(1);
		}

		SetState(BattleEnemyState::Death, true);
		ChangeAnimation();
	}

	// 死亡アニメーション終了時に存在を消す
	if (!alive)
	{
		if (!model->IsPlaying())
		{
			death = true;
		}
		else
		{
			model->UpdateTransform(elapsedTime / 60.0f);
		}
	}

	if (!enable) return;
	if (!alive) return;
	if (death) return;
	if (!onControl)
	{
		if (!model->IsPlaying())
		{
			SetState(BattleEnemyState::Idle);
		}
		model->UpdateTransform(elapsedTime / 60.0f);

		UpdateRotate(elapsedTime);
		UpdateTranslate(elapsedTime);
		UpdateTransform();
		UpdateCollision();

		return;
	}

	// AI更新
	{
		UpdateAI(elapsedTime);
	}

	// 回転
	{
		UpdateRotate(elapsedTime);
	}

	// 移動
	{
		UpdateTranslate(elapsedTime);
	}

	// アニメーションの切り替え
	{
		ChangeAnimation();
	}

	// 攻撃アニメーションの切り替え
	{
		ChangeAttackAnimation();
	}

	// アニメーション更新
	{
		UpdateAnimation(elapsedTime);
	}

	// トランスフォーム更新
	{
		UpdateTransform();
	}

	// コリジョン更新
	{
		UpdateCollision();
	}

	// プレイヤーとの距離計測
	{
		DistanceMeasurement();
	}

	// アタック判定中
	{
		AttackJudgment();
	}
}

// 描画
void Boss::Draw() {}

// GUI
void Boss::GUI(Phoenix::s32 index)
{

}

// アニメーションを移行
//void Boss::ChangeAnimation()
//{
//
//}

// 攻撃ステートを移行
void Boss::ChangeAttackAnimation()
{
	if (!changeAttackAnimation) return;

	Phoenix::s32 baseLayerIndex = layerIndexList.at(LayerType::Base);

	switch (changeAttackState)
	{
	case BossAttackState::RightHook:
		model->PlayAnimation(baseLayerIndex, 0, 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(attackAnimationSpeed);

		break;

	case BossAttackState::LeftHook:
		model->PlayAnimation(baseLayerIndex, 1, 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(attackAnimationSpeed);

		break;

	case BossAttackState::LeftTurn:
		model->PlayAnimation(baseLayerIndex, 2, 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(attackAnimationSpeed);

		break;

	case BossAttackState::NoneState: break;

	default: break;
	}

	changeAttackAnimation = false;
	changeAttackState = BossAttackState::NoneState;
}

// 攻撃ステートを変更
void Boss::SetAttackState(BossAttackState state)
{
	changeAttackAnimation = true;
	changeAttackState = state;
	currentAttackState = state;

	for (const auto& data : attackDatasList)
	{
		if (state == data.datas.at(0).animState)
		{
			attackAnimationSpeed = data.datas.at(0).playSpeed;
		}
	}
	attackReceptionTimeCnt = 0.0f;
}

// 攻撃判定
void Boss::AttackJudgment()
{
	if (battleAI->GetCurrentStateName() == BattleEnemyState::Attack)
	{
		auto Judgment = [&](Phoenix::s32 index)
		{
			if (isHit)
			{
				isAttackJudgment = false;
				return;
			}

			isAttackJudgment = true;
			attackCollisionIndex = index;
		};
		auto NoJudgment = [&]()
		{
			isAttackJudgment = false;
			isHit = false;
			attackCollisionIndex = -1;
		};

		//Phoenix::u32 index = static_cast<Phoenix::u32>(attackState);

		//// 当たり判定
		//if (attackDatasList.at(index).datas.at(attackComboState).collisionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= attackDatasList.at(index).datas.at(attackComboState).collisionEndTime)
		//{
		//	Judgment(attackDatasList.at(index).datas.at(attackComboState).collisionNum);
		//	attackPower = attackDatasList.at(index).receptionKey == AttackKey::WeakAttack ? 0 : 1;
		//}
		//else
		//{
		//	NoJudgment();
		//}

		bool hit = false;
		for (const auto& data : attackDatasList)
		{
			if (currentAttackState == data.datas.at(0).animState)
			{
				if (data.datas.at(0).collisionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= data.datas.at(0).collisionEndTime)
				{
					hit = true;
					Judgment(data.datas.at(0).collisionNum);
					if (currentAttackState == BossAttackState::RightHook)
					{
						attackPower = 0;
					}
					else if (currentAttackState == BossAttackState::LeftHook || currentAttackState == BossAttackState::LeftTurn)
					{
						attackPower = 1;
					}
				}
			}
		}
		if (!hit)
		{
			NoJudgment();
		}
	}
	else
	{
		isAttackJudgment = false;
	}
}