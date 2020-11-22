#include "Player.h"
#include <fstream>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/polymorphic.hpp>
#include "Phoenix/FrameWork/Renderer/ModelRenderer.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "../Enemy/EnemyManager.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"

CEREAL_CLASS_VERSION(Player::AttackDataList, 1)
CEREAL_CLASS_VERSION(Player::AttackDatas, 1)
CEREAL_CLASS_VERSION(Player::AttackData, 1)


std::unique_ptr<Player> Player::Create()
{
	return std::make_unique<Player>();
}

void Player::Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	// モデル読み込み
	{
		model = std::make_unique<Phoenix::FrameWork::ModelObject>();
		model->Initialize(graphicsDevice);
		model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Idle\\Breathing_Idle.fbx");
		//model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Mixamo\\Sword_And_Shield_Attack\\Sword_And_Shield_Attack.fbx");
	}

	// アニメーション読み込み
	{
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardWalk\\Walking_With_Shopping_Bag.fbx", -1); // 1
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\Running.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\SlowRunning.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Roll\\Sprinting_Forward_Roll.fbx", -1);
		
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Idle\\Ready_Idle.fbx", -1); // 5
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Forward\\Walk_Forward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Back\\Walk_Backward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Right\\Walk_Right.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Left\\Walk_Left.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Forward\\Forward_Step.fbx", -1); // 10
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Back\\Back_Step.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Right\\Right_Step.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Left\\Left_Step.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\RightPunch\\Righ_Punch_Begin.fbx", -1); // 14
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\RightPunch\\Righ_Punch_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\LeftPunch\\Left_Punch_Begin.fbx", -1); // 16
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\LeftPunch\\Left_Punch_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\RightHook\\Hook_Punch_Right_Begin.fbx", -1); // 18
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\RightHook\\Hook_Punch_Right_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\LeftHook\\Hook_Punch_Left_Begin.fbx", -1); // 20
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\LeftHook\\Hook_Punch_Left_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\RightKick\\Roundhouse_Kick_Right_Begin.fbx", -1); // 22
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\RightKick\\Roundhouse_Kick_Right_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\LeftKick\\Roundhouse_Kick_Left_Begin.fbx", -1); // 24
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\LeftKick\\Roundhouse_Kick_Left_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Punch\\Cross_Punch_Right.fbx", -1); // 26
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Punch\\Cross_Punch_Left.fbx", -1); // 27

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Hook\\Hook_Punch_Right.fbx", -1); // 28
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Hook\\Hook_Punch_Left.fbx", -1); // 29

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_Begin.fbx", -1); // 30
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_01.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Mma_Kick.fbx", -1); // 33

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Damage\\Head_Hit_Small.fbx", -1); // 34
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Damage\\Head_Hit.fbx", -1); // 35

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Guard\\Ready_Idle.fbx", -1); // 36

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Death\\Dying.fbx", -1); // 37


		model->AddAnimationLayer(0); // idle

		model->AddAnimationLayer(1); // walk
		model->AddAnimationLayer(2); // walk
		model->AddAnimationLayer(3); // walk
		model->AddAnimationLayer(4); // walk

		model->AddAnimationLayer(5); // battle idle up
		model->AddAnimationLayer(5, 56, 65); // battle idle down // anim num : 6

		model->AddAnimationLayer(10); // dodge // layer num : 7
		model->AddAnimationLayer(11); // dodge
		model->AddAnimationLayer(12); // dodge
		model->AddAnimationLayer(13); // dodge

		model->AddAnimationLayer(14); // right punch begin // 11
		model->AddAnimationLayer(15); // right punch end

		model->AddAnimationLayer(16); // left punch begin // 13
		model->AddAnimationLayer(17); // left punch end

		model->AddAnimationLayer(18); // right hook begin // 15
		model->AddAnimationLayer(19); // right hook end

		model->AddAnimationLayer(20); // left hook begin // 17
		model->AddAnimationLayer(21); // left hook end

		model->AddAnimationLayer(22); // right kick begin // 19
		model->AddAnimationLayer(23); // right kick end

		model->AddAnimationLayer(24); // left kick begin // 21
		model->AddAnimationLayer(25); // left kick end

		model->AddAnimationLayer(26); // strong right punch // 23
		model->AddAnimationLayer(27); // strong left punch

		model->AddAnimationLayer(28); // strong right hook // 25
		model->AddAnimationLayer(29); // strong left hook

		model->AddAnimationLayer(30); // turn right kick begin // 27
		model->AddAnimationLayer(31); // turn right kick 
		model->AddAnimationLayer(32); // turn right kick end

		model->AddAnimationLayer(33); // turn left kick // 30

		model->AddAnimationLayer(34); // damage small // 31
		model->AddAnimationLayer(35); // damage big

		model->AddAnimationLayer(36); // guard // 33

		model->AddAnimationLayer(37); // dying // 34

		model->AddBlendAnimationToLayer(6, 6, Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f));
		model->AddBlendAnimationToLayer(7, 6, Phoenix::Math::Vector3(0.0f, -1.0f, 0.0f));
		model->AddBlendAnimationToLayer(8, 6, Phoenix::Math::Vector3(1.0f, 0.0f, 0.0f));
		model->AddBlendAnimationToLayer(9, 6, Phoenix::Math::Vector3(-1.0f, 0.0f, 0.0f));

		model->AddBlendAnimationToLayer(static_cast<Phoenix::u32>(AnimationState::SlowRun), static_cast<Phoenix::u32>(AnimationState::Walk), Phoenix::Math::Vector3(1.0f, 0.0f, 0.0f));
	}

	// コリジョン初期化
	{
		collisionDatas.resize(5);

		collisionDatas.at(0).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(0).radius = 0.5f;
		collisionDatas.at(0).boneIndex = model->GetBoneIndex("Hips");

		collisionDatas.at(1).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(1).radius = WeakAttackCollisionRadius;
		collisionDatas.at(1).boneIndex = model->GetBoneIndex("RightHandIndex1");

		collisionDatas.at(2).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(2).radius = WeakAttackCollisionRadius;
		collisionDatas.at(2).boneIndex = model->GetBoneIndex("LeftHandIndex1");

		collisionDatas.at(3).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(3).radius = WeakAttackCollisionRadius;
		collisionDatas.at(3).boneIndex = model->GetBoneIndex("RightFoot");

		collisionDatas.at(4).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(4).radius = WeakAttackCollisionRadius;
		collisionDatas.at(4).boneIndex = model->GetBoneIndex("LeftFoot");
	}

	// UI生成
	{
		ui = PlayerUI::Create();
	}

	// アタックデータ生成
	{
		/*//auto SetAttackData = [&]
		//(
		//	Phoenix::s32 animState,
		//	Phoenix::s32 animIndex,

		//	Phoenix::f32 playSpeed,
		//	Phoenix::f32 playBeginTime,
		//	Phoenix::f32 playEndTime,

		//	Phoenix::s32 collisionNum,
		//	Phoenix::f32 collisionBeginTime,
		//	Phoenix::f32 collisionEndTime,

		//	bool receptionStack,
		//	Phoenix::f32 receptionBeginTime,
		//	Phoenix::f32 receptionEndTime,

		//	Phoenix::f32 dedgeReceptionBeginTime,
		//	Phoenix::f32 dedgeReceptionEndTime,

		//	Phoenix::s32 weakDerivedAttackState,
		//	Phoenix::s32 strongDerivedAttackState
		//)
		//{
		//	AttackData data;

		//	data.animState = animState;
		//	data.animIndex = animIndex;

		//	data.playSpeed = playSpeed;
		//	data.playBeginTime = playBeginTime == -1.0f ? -1.0f : playBeginTime / 60.0f;
		//	data.playEndTime = playEndTime == -1.0f ? -1.0f : playEndTime / 60.0f;

		//	data.collisionNum = collisionNum;
		//	data.collisionBeginTime = collisionBeginTime == -1.0f ? -1.0f : collisionBeginTime / 60.0f;
		//	data.collisionEndTime = collisionEndTime == -1.0f ? -1.0f : collisionEndTime / 60.0f;

		//	data.receptionStack = receptionStack;
		//	data.receptionBeginTime = receptionBeginTime == -1.0f ? -1.0f : receptionBeginTime / 60.0f;
		//	data.receptionEndTime = receptionEndTime == -1.0f ? -1.0f : receptionEndTime / 60.0f;

		//	data.dedgeReceptionBeginTime = dedgeReceptionBeginTime == -1.0f ? -1.0f : dedgeReceptionBeginTime / 60.0f;
		//	data.dedgeReceptionEndTime = dedgeReceptionEndTime == -1.0f ? -1.0f : dedgeReceptionEndTime / 60.0f;

		//	data.weakDerivedAttackState = weakDerivedAttackState;
		//	data.strongDerivedAttackState = strongDerivedAttackState;

		//	return data;
		//};

		//// 弱攻撃
		//{
		//	// 右ストレート
		//	{
		//		AttackDatas datas;

		//		// 入力キー設定
		//		datas.SetKey(AttackKey::WeakAttack);

		//		datas.AddData(SetAttackData(0, 11, 2.0f, -1.0f, -1.0f, 1, 13.0f, 23.0f, true,  13.0f, 23.0f, 13.0f, 23.0f, 1, 7));
		//		datas.AddData(SetAttackData(0, 12, 1.0f, 24.0f, 47.0f, 0, -1.0f, -1.0f, false, 24.0f, 47.0f, 24.0f, 47.0f, 1, 7));

		//		attackDatasList.attackDatas.emplace_back(datas);
		//	}

		//	// 左ストレート
		//	{
		//		AttackDatas datas;

		//		// 入力キー設定
		//		datas.SetKey(AttackKey::WeakAttack);

		//		datas.AddData(SetAttackData(1, 13, 2.0f, -1.0f, -1.0f, 2, 10.0f, 20.0f, true,  15.0f, 20.0f, 15.0f, 20.0f, 2, 6));
		//		datas.AddData(SetAttackData(1, 14, 1.0f, 21.0f, 47.0f, 0, -1.0f, -1.0f, false, 21.0f, 47.0f, 21.0f, 47.0f, 2, 6));

		//		attackDatasList.attackDatas.emplace_back(datas);
		//	}

		//	// 右フック
		//	{
		//		AttackDatas datas;

		//		// 入力キー設定
		//		datas.SetKey(AttackKey::WeakAttack);

		//		datas.AddData(SetAttackData(2, 15, 1.5f, 40.0f, -1.0f,  1, 46.0f, 72.0f, true,  46.0f, 72.0f,  46.0f, 72.0f,  3, 9));
		//		datas.AddData(SetAttackData(2, 16, 1.0f, 73.0f, 130.0f, 0, -1.0f, -1.0f, false, 73.0f, 130.0f, 73.0f, 130.0f, 3, 9));

		//		attackDatasList.attackDatas.emplace_back(datas);
		//	}

		//	// 左フック
		//	{
		//		AttackDatas datas;

		//		// 入力キー設定
		//		datas.SetKey(AttackKey::WeakAttack);

		//		datas.AddData(SetAttackData(3, 17, 1.5f, 40.0f, -1.0f,  2, 46.0f, 72.0f, true,  60.0f, 72.0f,  60.0f, 72.0f,  4, 8));
		//		datas.AddData(SetAttackData(3, 18, 1.0f, 73.0f, 130.0f, 0, -1.0f, -1.0f, false, 73.0f, 130.0f, 73.0f, 130.0f, 4, 8));

		//		attackDatasList.attackDatas.emplace_back(datas);
		//	}

		//	// 右キック
		//	{
		//		AttackDatas datas;

		//		// 入力キー設定
		//		datas.SetKey(AttackKey::WeakAttack);

		//		datas.AddData(SetAttackData(4, 19, 1.5f, 25.0f, -1.0f,  3, 50.0f, 60.0f, true,  50.0f, 60.0f,  50.0f, 60.0f,  5, 11));
		//		datas.AddData(SetAttackData(4, 20, 1.0f, 61.0f, 150.0f, 0, -1.0f, -1.0f, false, 61.0f, 150.0f, 61.0f, 150.0f, 5, 11));

		//		attackDatasList.attackDatas.emplace_back(datas);
		//	}

		//	// 左キック
		//	{
		//		AttackDatas datas;

		//		// 入力キー設定
		//		datas.SetKey(AttackKey::WeakAttack);

		//		datas.AddData(SetAttackData(5, 21, 1.5f, 25.0f, -1.0f,  4, 50.0f, 60.0f, true,  50.0f, 60.0f,  50.0f, 60.0f,  -1, 10));
		//		datas.AddData(SetAttackData(5, 22, 1.0f, 61.0f, 150.0f, 0, -1.0f, -1.0f, false, 61.0f, 150.0f, 61.0f, 150.0f, -1, 10));

		//		attackDatasList.attackDatas.emplace_back(datas);
		//	}
		//}

		//// 強攻撃
		//{
		//	// 右強ストレート
		//	{
		//		AttackDatas datas;

		//		// 入力キー設定
		//		datas.SetKey(AttackKey::StrongAttack);

		//		datas.AddData(SetAttackData(6, 23, 1.75f, 0.0f, 45.0f, 0, -1.0f, -1.0f, false, -1.0f, -1.0f, -1.0f,  -1.0f,  -1, -1));
		//		datas.AddData(SetAttackData(6, 23, 2.5f, 46.0f, 60.0f, 1, 50.0f, 60.0f, false, -1.0f, -1.0f, -1.0f,  -1.0f,  -1, -1));
		//		datas.AddData(SetAttackData(6, 23, 1.5f, 61.0f, -1.0f, 0, -1.0f, -1.0f, false, -1.0f, -1.0f, 115.0f, 120.0f, -1, -1));

		//		attackDatasList.attackDatas.emplace_back(datas);
		//	}

		//	// 左強ストレート
		//	{
		//		AttackDatas datas;

		//		// 入力キー設定
		//		datas.SetKey(AttackKey::StrongAttack);

		//		datas.AddData(SetAttackData(7, 24, 1.75f, 0.0f, 45.0f, 0, -1.0f, -1.0f, false, -1.0f, -1.0f, -1.0f,  -1.0f,  -1, -1));
		//		datas.AddData(SetAttackData(7, 24, 2.5f, 46.0f, 60.0f, 2, 50.0f, 60.0f, false, -1.0f, -1.0f, -1.0f,  -1.0f,  -1, -1));
		//		datas.AddData(SetAttackData(7, 24, 1.5f, 61.0f, -1.0f, 0, -1.0f, -1.0f, false, -1.0f, -1.0f, 115.0f, 120.0f, -1, -1));

		//		attackDatasList.attackDatas.emplace_back(datas);
		//	}

		//	// 右強フック
		//	{
		//		AttackDatas datas;

		//		// 入力キー設定
		//		datas.SetKey(AttackKey::StrongAttack);

		//		datas.AddData(SetAttackData(8, 25, 1.5f, 0.0f,  50.0f, 0, -1.0f, -1.0f, false, -1.0f, -1.0f, -1.0f,  -1.0f,  -1, -1));
		//		datas.AddData(SetAttackData(8, 25, 2.5f, 51.0f, 70.0f, 1, 51.0f, 70.0f, false, -1.0f, -1.0f, -1.0f,  -1.0f,  -1, -1));
		//		datas.AddData(SetAttackData(8, 25, 1.5f, 71.0f, -1.0f, 0, -1.0f, -1.0f, false, -1.0f, -1.0f, 125.0f, 130.0f, -1, -1));

		//		attackDatasList.attackDatas.emplace_back(datas);
		//	}

		//	// 左強フック
		//	{
		//		AttackDatas datas;

		//		// 入力キー設定
		//		datas.SetKey(AttackKey::StrongAttack);

		//		datas.AddData(SetAttackData(9, 26, 1.5f, 0.0f,  50.0f, 0, -1.0f, -1.0f, false, -1.0f, -1.0f, -1.0f,  -1.0f,  -1, -1));
		//		datas.AddData(SetAttackData(9, 26, 2.5f, 51.0f, 70.0f, 2, 51.0f, 70.0f, false, -1.0f, -1.0f, -1.0f,  -1.0f,  -1, -1));
		//		datas.AddData(SetAttackData(9, 26, 1.5f, 71.0f, -1.0f, 0, -1.0f, -1.0f, false, -1.0f, -1.0f, 125.0f, 130.0f, -1, -1));

		//		attackDatasList.attackDatas.emplace_back(datas);
		//	}

		//	// 右回転キック
		//	{
		//		AttackDatas datas;

		//		// 入力キー設定
		//		datas.SetKey(AttackKey::StrongAttack);

		//		datas.AddData(SetAttackData(10, 27, 1.05f, -1.0f, 24.0f, 0, -1.0f, -1.0f, false, -1.0f, -1.0f, -1.0f, -1.0f,  -1, -1));
		//		datas.AddData(SetAttackData(10, 28, 2.0f, 25.0f,  46.0f, 3, 25.0f, 46.0f, false, -1.0f, -1.0f, -1.0f, -1.0f,  -1, -1));
		//		datas.AddData(SetAttackData(10, 29, 1.0f, 47.0f,  -1.0f, 0, -1.0f, -1.0f, false, -1.0f, -1.0f, 95.0f, 100.0f, -1, -1));

		//		attackDatasList.attackDatas.emplace_back(datas);
		//	}

		//	// 左回転キック
		//	{
		//		AttackDatas datas;

		//		// 入力キー設定
		//		datas.SetKey(AttackKey::StrongAttack);

		//		datas.AddData(SetAttackData(11, 30, 1.05f, -1.0f, 24.0f, 0, -1.0f, -1.0f, false, -1.0f, -1.0f, -1.0f, -1.0f,  -1, -1));
		//		datas.AddData(SetAttackData(11, 30, 2.0f,  25.0f, 46.0f, 4, 25.0f, 46.0f, false, -1.0f, -1.0f, -1.0f, -1.0f,  -1, -1));
		//		datas.AddData(SetAttackData(11, 30, 1.0f,  47.0f, -1.0f, 0, -1.0f, -1.0f, false, -1.0f, -1.0f, 95.0f, 100.0f, -1, -1));

		//		attackDatasList.attackDatas.emplace_back(datas);
		//	}
		//}*/

		const char* fullPass = Phoenix::OS::Path::GetFullPath("..\\Data\\Document\\Player\\AttackDatas.atk");

		Player::AttackDataList data;
		Player::AttackDataList::Deserialize(data, fullPass);

		SetAttackDatasList(data);
	}

	// エネミーヒット
	{
		isHit.resize(EnemyManager::EnemyRange);
		for (auto hit : isHit)
		{
			hit = false;
		}

		isAttackJudgment.resize(EnemyManager::EnemyRange);
		for (auto judge : isAttackJudgment)
		{
			judge = false;
		}
	}
}

void Player::Initialize()
{
	// アニメーションパラメーターの設定
	{
		animationState = AnimationState::Idle;
		attackState = -1;
		isChangeAnimation = false;
		isAttack = false;
		speed = 0.0f;
		animationSpeed = AnimationSpeed60;
		attackReceptionTimeCnt = 0.0f;
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
		pos = { 0,0,12.0f }; // tutorial : 135.0f , main : 12.0f
		//rotate = { 0,180.0f * 0.01745f,0 };
		rotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), 180.0f * 0.01745f);
		scale = { 1,1,1 };
		radius = 0.35f;
	}

	// パラメーターの初期化
	{
		life = MaxLife;
		//isHit = false;
		invincible = false;
		//isAttackJudgment = false;
		alive = true;
		death = false;
		attackCollisionIndex = -1;
		accumulationDamege = 0;
		accumulationTimeCnt = 0;
		blendRate = { 0.0f, 0.0f, 0.0f };
		newRotate = rotate;
		rotateY = 180.0f * 0.01745f;
		inTerritory = false;
		isBattleMode = false;
		receptionStack = false;
		stackKey = AttackKey::None;
		behaviorScore = 0;
		attackDamage = 0;

		for (auto judge : isAttackJudgment)
		{
			judge = false;
		}
	}

	{
		HANDLE hFile;
		FILETIME ftFileTime, ftLocalFileTime;

		const wchar_t* fullPass = Phoenix::OS::Path::GetFullPathW(L"..\\Data\\Document\\Player\\AttackDatas.atk");
		hFile = CreateFile(fullPass, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			GetFileTime(hFile, NULL, NULL, &ftFileTime);
			FileTimeToLocalFileTime(&ftFileTime, &ftLocalFileTime);
			FileTimeToSystemTime(&ftLocalFileTime, &stFileTime);

			CloseHandle(hFile);
		}
	}
}

void Player::Finalize()
{
	attackDatasList.attackDatas.clear();
}

void Player::Update(Phoenix::Graphics::Camera& camera, bool onControl, bool attackLoad)
{
	if (attackLoad)
	{
		HANDLE hFile;

		const wchar_t* fullPass = Phoenix::OS::Path::GetFullPathW(L"..\\Data\\Document\\Player\\AttackDatas.atk");
		hFile = CreateFile(fullPass, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			FILETIME ftFileTime, ftLocalFileTime;
			SYSTEMTIME newSTFileTime;

			newSTFileTime.wYear = 0;
			newSTFileTime.wMonth = 0;
			newSTFileTime.wDay = 0;
			newSTFileTime.wHour = 0;
			newSTFileTime.wMinute = 0;
			newSTFileTime.wSecond = 0;

			bool check = GetFileTime(hFile, NULL, NULL, &ftFileTime);
			CloseHandle(hFile);

			if (check)
			{
				FileTimeToLocalFileTime(&ftFileTime, &ftLocalFileTime);
				FileTimeToSystemTime(&ftLocalFileTime, &newSTFileTime);

				if (stFileTime.wYear != newSTFileTime.wYear
					|| stFileTime.wMonth != newSTFileTime.wMonth
					|| stFileTime.wDay != newSTFileTime.wDay
					|| stFileTime.wHour != newSTFileTime.wHour
					|| stFileTime.wMinute != newSTFileTime.wMinute
					|| stFileTime.wSecond != newSTFileTime.wSecond)
				{
					const char* fullPass = Phoenix::OS::Path::GetFullPath("..\\Data\\Document\\Player\\AttackDatas.atk");

					Player::AttackDataList data;
					Player::AttackDataList::Deserialize(data, fullPass);

					SetAttackDatasList(data);

					stFileTime = newSTFileTime;
				}
			}
		}
	}

	// ライフが０ならマネージャーの生存エネミーカウントを下げる
	if (life <= 0 && alive)
	{
		alive = false;
		ChangeAnimationState(AnimationState::Death);
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
			model->UpdateTransform(1 / 60.0f);
		}

		return;
	}

	bool isAccumulationDamege = false;

	// スコア計算
	{
		Phoenix::f32 score = static_cast<Phoenix::f32>(behaviorScore);
		{
			score = Phoenix::Math::f32Lerp(score, 0.0f, 0.05f);
		}
		behaviorScore = static_cast<Phoenix::s32>(score);
	}

	// 蓄積ダメージの確認
	{
		isAccumulationDamege = AccumulationDamege();
	}

	// コントローラー操作(位置更新)
	{
		if (onControl && !isAccumulationDamege) Control(camera);
	}

	// アニメーション変更
	{
		ChangeAnimation();
	}

	// アニメーション更新
	{
		if (isBattleMode)
		{
			model->SetBlendRate(Phoenix::Math::Vector3(-blendRate.x, blendRate.y, 0.0f));
		}
		else
		{
			model->SetBlendRate(blendRate.z);
		}

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
			Phoenix::Math::Matrix bone = nodes->at(data.boneIndex).worldTransform;
			bone *= systemUnitTransform * worldMatrix;
			data.pos = Phoenix::Math::Vector3(bone._41, bone._42, bone._43);
		}
	}

	// アタック判定中
	{
		AttackJudgment();
	}
}

void Player::UpdateTrasform()
{
	Phoenix::Math::Vector3 scale = this->scale;
	//Phoenix::Math::Vector3 rotate = this->rotate;
	Phoenix::Math::Quaternion rotate = this->rotate;
	Phoenix::Math::Vector3 translate = pos;

	Phoenix::Math::Matrix S, R, T;
	S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
	//R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
	R = Phoenix::Math::MatrixRotationQuaternion(&rotate);
	T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

	worldMatrix = S * R * T;
}

void Player::UpdateUI()
{
	Phoenix::f32 hp = static_cast<Phoenix::f32>(life);
	hp = hp <= 0 ? 0 : hp;

	ui->Update((hp / MaxLife) * 100.0f);
}

void Player::Control(Phoenix::Graphics::Camera& camera) // TODO : re -> player control
{
	if (animationState == AnimationState::Damage && model->IsPlaying()) return;

	Phoenix::f32 sX = 0.0f;
	Phoenix::f32 sY = 0.0f;

	sX = xInput[0].sX / 1000.0f;
	sY = xInput[0].sY / 1000.0f;

	sY = GetKeyState('W') < 0 ? -1.0f : sY;
	sY = GetKeyState('S') < 0 ? 1.0f : sY;
	sX = GetKeyState('A') < 0 ? -1.0f : sX;
	sX = GetKeyState('D') < 0 ? 1.0f : sX;

	if (xInput[0].bRBs || GetKeyState('L') < 0)
	{
		InEnemyTerritory(true);
	}
	else
	{
		InEnemyTerritory(false);
	}

	// ブレンドレート計算
	{
		if (isBattleMode)
		{
			blendRate.x = sX;
			blendRate.y = sY;
		}

		blendRate.z = Phoenix::Math::Vector2Length(Phoenix::Math::Vector2(sX, sY));
		blendRate.z = 1.0f <= blendRate.z ? 1.0f : blendRate.z;
	}

	// プレイヤーの最終方向を決定する角度を計算
	auto UpdateRotateY = [&](Phoenix::f32 sX, Phoenix::f32 sY, Phoenix::f32 cameraRotateY)
	{
		float len = sqrtf(sX * sX + sY * sY);

		if (len <= 0)
		{
			sX = 0;
			sY = 0;
		}

		float mag = 1 / len;

		sX *= mag;
		sY *= mag;

		rotateY = cameraRotateY + atan2f(sX, sY);
	};

	// プレイヤー回転
	auto RotatePlayer = [&](Phoenix::f32 angle, bool isBattleMode)
	{
		if (isBattleMode)
		{
			Phoenix::Math::Vector3 dir = Phoenix::Math::Vector3Normalize(targetPos - GetPosition());
			float len = sqrtf(dir.x * dir.x + dir.z * dir.z);

			if (len <= 0)
			{
				dir.x = 0;
				dir.z = 0;
			}

			float mag = 1 / len;

			dir.x *= mag;
			dir.z *= mag;

			Phoenix::f32 angleY = atan2f(dir.x, dir.z);

			newRotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angleY);
		}
		else
		{
			newRotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angle);
		}
	};

	auto RotatePlayerToAttack = [&]()
	{
		Phoenix::Math::Vector3 dir = targetPos - pos;
		dir = Phoenix::Math::Vector3Normalize(dir);
		dir.y = 0.0f;

		Phoenix::Math::Matrix m = Phoenix::Math::MatrixRotationQuaternion(&rotate);
		Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(m._31, m._32, m._33);
		forward.y = 0.0f;

		/*{
			Phoenix::Math::Matrix m = Phoenix::Math::MatrixRotationQuaternion(&newRotate);
			Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(m._31, m._32, m._33);
			forward.y = 0.0f;

			Phoenix::f32 angle;
			angle = acosf(Phoenix::Math::Vector3Dot(dir, forward));

			if (1e-8f < fabs(angle))
			{
				angle /= 0.01745f;
			}
		}*/

		Phoenix::f32 angle;
		angle = acosf(Phoenix::Math::Vector3Dot(dir, forward));

		if (1e-8f < fabs(angle))
		{
			angle /= 0.01745f;

			if (angle <= 45.0f)
			{
				float len = sqrtf(dir.x * dir.x + dir.z * dir.z);

				if (len <= 0)
				{
					dir.x = 0;
					dir.z = 0;
				}

				float mag = 1 / len;

				dir.x *= mag;
				dir.z *= mag;

				Phoenix::f32 angleY = atan2f(dir.x, dir.z);

				newRotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angleY);
			}
		}
	};

	auto ChangeAnimation = [&](Phoenix::u32 index, Phoenix::u32 nextIndex)
	{
		if (attackDatasList.attackDatas.at(index).datas.size() - 1 <= attackComboState)
		{
			attackComboState = -1;
		}

		if (attackComboState == -1)
		{
			attackComboState = 0;
			attackReceptionTimeCnt = attackDatasList.attackDatas.at(nextIndex).datas.at(attackComboState).playBeginTime != -1 ? attackDatasList.attackDatas.at(nextIndex).datas.at(attackComboState).playBeginTime : 0.0f;

			receptionStack = false;
			stackKey = AttackKey::None;

			ChangeAnimationState(AnimationState::Attack, 0.0f);
			ChangeAttackAnimationState(attackDatasList.attackDatas.at(nextIndex).datas.at(attackComboState).animState, attackDatasList.attackDatas.at(nextIndex).datas.at(attackComboState).animIndex, attackDatasList.attackDatas.at(nextIndex).datas.at(attackComboState).playSpeed);
		
			speed = Attack01MoveSpeed;

			if (sX != 0.0f || sY != 0.0f)
			{
				UpdateRotateY(sX, sY, camera.GetRotateY());
				RotatePlayer(rotateY, isBattleMode);
			}

			RotatePlayerToAttack();
		}
	};

	auto JudgeDedgeIndex = [&]()
	{
		if (sX != 0.0f || sY != 0.0f)
		{
			UpdateRotateY(sX, sY, camera.GetRotateY());
			RotatePlayer(rotateY, true);

			if (fabsf(sX) <= fabsf(sY))
			{
				if (sY < 0.0f)
				{
					dedgeLayerIndex = 7;
				}
				if (sY > 0.0f)
				{
					dedgeLayerIndex = 8;
				}
			}
			else if (fabsf(sY) <= fabsf(sX))
			{
				if (sX < 0.0f)
				{
					dedgeLayerIndex = 9;
				}
				if (sX > 0.0f)
				{
					dedgeLayerIndex = 10;
				}
			}

			/*
			//Phoenix::Math::Vector3 dir = targetPos - GetPosition();
			//dir = Phoenix::Math::Vector3Normalize(dir);
			//dir.y = 0.0f;

			//Phoenix::f32 newRotateY = atan2f(-dir.x, -dir.z) + atan2f(sX, sY);

			//Phoenix::Math::Vector3 f = { sinf(newRotateY),  0.0f, cosf(newRotateY) };

			//Phoenix::Math::Quaternion newQ = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), newRotateY);
			//Phoenix::Math::Matrix newM = Phoenix::Math::MatrixRotationQuaternion(&newQ);
			//Phoenix::Math::Vector3 newForward = Phoenix::Math::Vector3(newM._31, newM._32, newM._33);
			//newForward.y = 0.0f;

			//Phoenix::Math::Quaternion q = rotate;
			//Phoenix::Math::Quaternion q = rotate // * newQ;
			//Phoenix::Math::Matrix matrix = Phoenix::Math::MatrixRotationQuaternion(&q);
			//Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(matrix._31, matrix._32, matrix._33);

			//Phoenix::Math::Vector3 cameraForword = camera.GetFront();
			//cameraForword.y = 0.0f;

			//Phoenix::f32 angle = acosf(Phoenix::Math::Vector3Dot(dir, cameraForword)) / 0.01745f;
			////if (45.0f <= angle && angle <= 135.0f) f *= -1;

			//if (fabsf(f.x) < fabsf(f.z))
			//{
			//	if (f.z < 0.0f)
			//	{
			//		dedgeLayerIndex = 15;
			//		rotateY = atan2f(-dir.x, -dir.z) + atan2f(0.0f, -1.0f);
			//	}
			//	if (f.z > 0.0f)
			//	{
			//		dedgeLayerIndex = 16;
			//		rotateY = atan2f(-dir.x, -dir.z) + atan2f(0.0f, 1.0f);
			//	}
			//}
			//if (fabsf(f.z) < fabsf(f.x))
			//{
			//	if (f.x < 0.0f)
			//	{
			//		dedgeLayerIndex = 17;
			//		rotateY = atan2f(-dir.x, -dir.z) + atan2f(-1.0f, 0.0f);
			//	}
			//	if (f.x > 0.0f)
			//	{
			//		dedgeLayerIndex = 18;
			//		rotateY = atan2f(-dir.x, -dir.z) + atan2f(1.0f, 0.0f);
			//	}
			//}

			//rotateY = newRotateY;

			Phoenix::Math::Quaternion q = rotate * newRotateY;
			Phoenix::Math::Matrix matrix = Phoenix::Math::MatrixRotationQuaternion(&q);
			Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(matrix._31, matrix._32, matrix._33);
			Phoenix::Math::Vector3 up = Phoenix::Math::Vector3(matrix._21, matrix._22, matrix._23);
			Phoenix::Math::Vector3 right = Phoenix::Math::Vector3(matrix._11, matrix._12, matrix._13);

			forward.y = 0.0f;

			Phoenix::f32 angleSX, angleSY;
			angleSX = acosf(Phoenix::Math::Vector3Dot(dir, forward));
			angleSY = acosf(Phoenix::Math::Vector3Dot(dir, right));

			if (1e-8f < fabs(angleSX))
			{
				Phoenix::f32 angleR;
				{
					angleR = acosf(Phoenix::Math::Vector3Dot(dir, right));
					angleR -= (90.0f * 0.01745f);
					if (0.0f < angleR) angleSX *= -1;
				}
				{
					angleR = acosf(Phoenix::Math::Vector3Dot(dir, forward));
					angleR -= (90.0f * 0.01745f);
					if (0.0f < angleR) angleSY *= -1;
				}

				if (fabsf(forward.z) < fabsf(forward.x))
				{
					if (angleSX < 0.0f)
					{
						dedgeLayerIndex = 17;
						rotateY = atan2f(dir.x, dir.z);
						rotateY += 270.0f * 0.01745f;

					}
					if (angleSX > 0.0f)
					{
						dedgeLayerIndex = 18;
						rotateY = atan2f(dir.x, dir.z);
						rotateY += 90.0f * 0.01745f;
					}
				}
				else if (fabsf(forward.x) < fabsf(forward.z))
				{
					if (angleSY < 0.0f)
					{
						dedgeLayerIndex = 15;
						rotateY = atan2f(dir.x, dir.z);
						rotateY += 0.0f * 0.01745f;
					}
					if (angleSY > 0.0f)
					{
						dedgeLayerIndex = 16;
						rotateY = atan2f(dir.x, dir.z);
						rotateY += 180.0f * 0.01745f;
					}
				}
			}
			*/
		}
		else
		{
			Phoenix::Math::Vector3 dir = targetPos - GetPosition();
			dir = Phoenix::Math::Vector3Normalize(dir);
			dir.y = 0.0f;

			Phoenix::f32 fictitiousCameraRotateY = atan2f(-dir.x, -dir.z);

			UpdateRotateY(0.0f, 1.0f, fictitiousCameraRotateY);
			RotatePlayer(rotateY, true);
			dedgeLayerIndex = 8;
		}
	};

	auto JudgeInput01 = [&](Phoenix::s32 index, Phoenix::s32 nextIndex)
	{
		if (!attackDatasList.attackDatas.at(index).datas.at(attackComboState).receptionStack)
		{
			if (attackDatasList.attackDatas.at(nextIndex).receptionKey == stackKey)
			{
				ChangeAnimation(index, nextIndex);
			}
		}
	};

	auto JudgeInput02 = [&](Phoenix::s32 index, Phoenix::s32 nextIndex, AttackKey key)
	{
		if (attackDatasList.attackDatas.at(nextIndex).receptionKey == key)
		{
			if (attackDatasList.attackDatas.at(index).datas.at(attackComboState).receptionStack)
			{
				receptionStack = true;
				stackKey = key;
			}
			else
			{
				ChangeAnimation(index, nextIndex);
			}
		}
	};

	// 攻撃キーの入力確認
	AttackKey key = AttackKey::None;
	{
		if (xInput[0].bXt || GetAsyncKeyState('J') & 1)
		{
			key = AttackKey::WeakAttack;
		}
		if (xInput[0].bYt || GetAsyncKeyState('K') & 1)
		{
			key = AttackKey::StrongAttack;
		}
	}

	// 攻撃ステートへ
	if ((key != AttackKey::None) && 0 < attackDatasList.attackDatas.size() && ((animationState == AnimationState::Attack) || (animationState == AnimationState::Idle) || (animationState == AnimationState::Walk) || (animationState == AnimationState::Run)))
	{
		if (attackState == -1)
		{
			// 弱攻撃からスタートするため
			if (attackDatasList.attackDatas.at(0).receptionKey == key && 0 < attackDatasList.attackDatas.at(0).datas.size())
			{
				attackComboState = 0;
				attackReceptionTimeCnt = attackDatasList.attackDatas.at(0).datas.at(0).playBeginTime != -1 ? attackDatasList.attackDatas.at(0).datas.at(0).playBeginTime : 0.0f;

				ChangeAnimationState(AnimationState::Attack, 0.0f);
				ChangeAttackAnimationState(attackDatasList.attackDatas.at(0).datas.at(0).animState, attackDatasList.attackDatas.at(0).datas.at(0).animIndex, attackDatasList.attackDatas.at(0).datas.at(0).playSpeed);

				speed = Attack01MoveSpeed;

				if (sX != 0.0f || sY != 0.0f)
				{
					UpdateRotateY(sX, sY, camera.GetRotateY());
					RotatePlayer(rotateY, isBattleMode);
				}

				RotatePlayerToAttack();
			}
		}
		else if (0 < attackDatasList.attackDatas.at(attackState).datas.size())
		{
			Phoenix::s32 index = attackState;
			Phoenix::s32 wearNextIndex = attackDatasList.attackDatas.at(index).datas.at(attackComboState).weakDerivedAttackState;
			Phoenix::s32 strongNextIndex = attackDatasList.attackDatas.at(index).datas.at(attackComboState).strongDerivedAttackState;
			Phoenix::s32 endIndex = static_cast<Phoenix::s32>(attackDatasList.attackDatas.size());

			// 次の攻撃が発動するボタンの受付
			if (receptionStack)
			{
				if (wearNextIndex < endIndex && 0 <= wearNextIndex)
				{
					JudgeInput01(index, wearNextIndex);
				}
				if (strongNextIndex < endIndex && 0 <= strongNextIndex)
				{
					JudgeInput01(index, strongNextIndex);
				}
			}
			else if (attackDatasList.attackDatas.at(index).datas.at(attackComboState).receptionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= attackDatasList.attackDatas.at(index).datas.at(attackComboState).receptionEndTime)
			{
				if (wearNextIndex < endIndex && 0 <= wearNextIndex)
				{
					JudgeInput02(index, wearNextIndex, key);
				}
				if (strongNextIndex < endIndex && 0 <= strongNextIndex)
				{
					JudgeInput02(index, strongNextIndex, key);
				}
			}
		}
	}
	else if ((animationState == AnimationState::Attack) && !model->IsPlaying())
	{
		Phoenix::s32 index = attackState;

		if (0 <= attackComboState && attackComboState < attackDatasList.attackDatas.at(index).datas.size() - 1)
		{
			++attackComboState;

			ChangeAnimationState(AnimationState::Attack, 0.0f);
			ChangeAttackAnimationState(attackDatasList.attackDatas.at(index).datas.at(attackComboState).animState, attackDatasList.attackDatas.at(index).datas.at(attackComboState).animIndex, attackDatasList.attackDatas.at(index).datas.at(attackComboState).playSpeed);
		}
		else
		{
			ChangeAnimationState(AnimationState::Idle, 0.0f);
			ChangeAttackAnimationState(-1, -1, 0.0f);

			isAttack = false;
			attackReceptionTimeCnt = 0.0f;
			attackComboState = -1;

			receptionStack = false;
			stackKey = AttackKey::None;
		}
	}

	if (isChangeAnimation && isAttack)
	{
		if (key == AttackKey::WeakAttack)
		{
			behaviorScore += WeakAttackScore;
		}
		else if (key == AttackKey::StrongAttack)
		{
			behaviorScore += StrongAttackScore;
		}
	}

	// 攻撃ステート以外
	if (!isAttack)
	{
		// 回避ステート
		//if (isBattleMode)
		{
			// 回避ステートへ
			if ((xInput[0].bAt || GetAsyncKeyState(VK_SPACE) & 1) && animationState != AnimationState::Dedge)
			{
				ChangeAnimationState(AnimationState::Dedge, DedgeSpeed);
				JudgeDedgeIndex();
			}
			// 回避ステート中
			else if (animationState == AnimationState::Dedge)
			{
				speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.025f);
				// 待機ステートへ
				if (!model->IsPlaying())
				{
					ChangeAnimationState(AnimationState::Idle, 0.0f);
				}
			}
		}

		// 回避ステート以外
		if (animationState != AnimationState::Roll && animationState != AnimationState::Dedge)
		{
			// 移動ステート
			if (sX != 0.0f || sY != 0.0f)
			{
				UpdateRotateY(sX, sY, camera.GetRotateY());
				RotatePlayer(rotateY, isBattleMode);

				if (isBattleMode)
				{
					if (animationState != AnimationState::Walk)
					{
						ChangeAnimationState(AnimationState::Walk, BattleWalkSpeed);
					}
				}
				else
				{
					if ((!xInput[0].bRBs && !(GetKeyState(VK_SHIFT) < 0)) && animationState != AnimationState::Walk)
					{
						ChangeAnimationState(AnimationState::Walk, WalkSpeed);
					}
					/*if ((xInput[0].bRBs || GetKeyState(VK_SHIFT) < 0) && animationState != AnimationState::Run)
					{
						ChangeAnimationState(AnimationState::Run, RunSpeed);
					}*/
				}
			}
			// 待機ステートへ
			else if (animationState != AnimationState::Idle)
			{
				ChangeAnimationState(AnimationState::Idle, 0.0f);
			}
		}
	}
	// 攻撃中
	else
	{
		//if (isBattleMode)
		{
			// 回避ステートへ
			if ((xInput[0].bAt || GetAsyncKeyState(VK_SPACE) & 1) && animationState != AnimationState::Dedge)
			{
				Phoenix::s32 index = attackState;

				// 次の攻撃が発動するボタンの受付
				if (attackDatasList.attackDatas.at(index).datas.at(attackComboState).dedgeReceptionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= attackDatasList.attackDatas.at(index).datas.at(attackComboState).dedgeReceptionEndTime)
				{
					ChangeAnimationState(AnimationState::Dedge, DedgeSpeed);
					ChangeAttackAnimationState(-1, -1, 0.0f);

					isAttack = false;
					attackReceptionTimeCnt = 0.0f;

					JudgeDedgeIndex();
				}
			}
		}

		// アタックアニメーションスピード計測
		attackReceptionTimeCnt += animationSpeed / 60.0f;
	}

	// 座標更新
	if (!isChangeAnimation)
	{
		rotate = Phoenix::Math::QuaternionSlerp(rotate, newRotate, 0.17f);
		{
			if (animationState == AnimationState::Walk/* && !isBattleMode*/)
			{
				if (isBattleMode)
				{
					pos.x += sinf(rotateY) * (speed + (BattleSlowRunSpeed * blendRate.z));
					pos.z += cosf(rotateY) * (speed + (BattleSlowRunSpeed * blendRate.z));
				}
				else
				{
					pos.x += sinf(rotateY) * (speed + (SlowRunSpeed * blendRate.z));
					pos.z += cosf(rotateY) * (speed + (SlowRunSpeed * blendRate.z));
				}
			}
			else if (animationState != AnimationState::Attack)
			{
				pos.x += sinf(rotateY) * speed;
				pos.z += cosf(rotateY) * speed;
			}
		}

		if (animationState == AnimationState::Attack)
		{
			if (isAttack && 0.0f < speed)
			{
				Phoenix::Math::Matrix matrix = Phoenix::Math::MatrixRotationQuaternion(&rotate);
				Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(matrix._31, matrix._32, matrix._33);

				pos.x += forward.x * speed;
				pos.z += forward.z * speed;

				speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f);
			}
		}
	}
}

void Player::ChangeAnimation()
{
	if (!isChangeAnimation) return;

	Phoenix::u32 animationNum = static_cast<Phoenix::u32>(animationState);
	switch (animationState)
	{
	case AnimationState::Idle:
		if (isBattleMode)
		{
			model->PlayAnimation(5, 1, 0.2f);
			model->SetLoopAnimation(true);
		}
		else
		{
			model->PlayAnimation(animationNum, 1, 0.1f);
			model->SetLoopAnimation(true);
		}
		break;

	case AnimationState::Walk:
		if (isBattleMode)
		{
			model->PlayAnimation(5, 1, 0.2f);
			model->PlayBlendAnimation(6, 1, 0.2f);
			model->SetLoopAnimation(true);
			model->SetBlendLoopAnimation(true);
		}
		else
		{
			model->PlayAnimation(animationNum, 1, 0.2f);
			model->SetLoopAnimation(true);
		}
		break;

	case AnimationState::Run:
		model->PlayAnimation(animationNum, 1, 0.2f);
		model->SetLoopAnimation(true);
		break;

	case AnimationState::Roll:
		model->PlayAnimation(animationNum, 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(1.5f);
		break;

	case AnimationState::Attack:
		ChangeAttackAnimation(animationNum);
		break;

	case AnimationState::Damage:
		if (damagePower == 0)
		{
			model->PlayAnimation(31, 1, 0.2f);
			model->SetEndTime(43.0f / 60.0f);
		}
		else if (damagePower == 1)
		{
			model->PlayAnimation(32, 1, 0.2f);
		}
		model->SetLoopAnimation(false);
		break;

	case AnimationState::Dedge:
		model->PlayAnimation(dedgeLayerIndex, 0, 0.2f);
		//model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		model->SetSpeed(2.25f);

		/*if (dedgeLayerIndex == 15)
		{
			model->SetSpeed(1.85f);
		}
		else if(dedgeLayerIndex == 16)
		{
			model->SetSpeed(1.75f);
		}
		else
		{
			model->SetSpeed(1.5f);
		}*/
		break;

	case AnimationState::Guard:
		model->PlayAnimation(33, 1, 0.2f);
		model->SetLoopAnimation(false);
		break;

	case AnimationState::Death:
		model->PlayAnimation(34, 1, 0.2f);
		model->SetLoopAnimation(false);
		break;
	
	default: break;
	}

	isChangeAnimation = false;
}

void Player::ChangeAttackAnimation(Phoenix::u32 animationNum)
{
	Phoenix::s32 index = attackState;
	Phoenix::u32 animIndex = static_cast<Phoenix::u32>(attackDatasList.attackDatas.at(index).datas.at(attackComboState).animIndex);
	Phoenix::f32 animationSpeed = attackDatasList.attackDatas.at(index).datas.at(attackComboState).playSpeed;
	Phoenix::f32 beginTime = attackDatasList.attackDatas.at(index).datas.at(attackComboState).playBeginTime;
	Phoenix::f32 endTime = attackDatasList.attackDatas.at(index).datas.at(attackComboState).playEndTime;

	if (index == 6 || index == 7 || index == 8 || index == 9 || index == 11)
	{
		model->PlayAnimation(animIndex, 1, 0.2f);
	}
	else model->PlayAnimation(animIndex, 0, 0.2f);
	model->SetLoopAnimation(false);
	model->SetSpeed(animationSpeed);

	if (beginTime != -1.0f)
	{
		model->SetBeginTime(beginTime);
		model->SetCurrentTime(beginTime);
	}
	if (endTime != -1.0f)
	{
		model->SetEndTime(endTime);
	}
}

void Player::AttackJudgment()
{
	if (isAttack)
	{
		auto Judgment = [&](Phoenix::s32 index)
		{
			for (Phoenix::s32 i = 0; i < isHit.size(); ++i)
			{
				if (isHit.at(i))
				{
					isAttackJudgment.at(i) = false;
					return;
				}

				isAttackJudgment.at(i) = true;
			}

			attackCollisionIndex = index;
		};
		auto NoJudgment = [&]()
		{
			for (Phoenix::s32 i = 0; i < isHit.size(); ++i)
			{
				isAttackJudgment.at(i) = false;
				isHit.at(i) = false;
			}
			attackCollisionIndex = -1;
		};

		Phoenix::s32 index = attackState;

		// 当たり判定
		if (attackDatasList.attackDatas.at(index).datas.at(attackComboState).collisionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= attackDatasList.attackDatas.at(index).datas.at(attackComboState).collisionEndTime)
		{
			Judgment(attackDatasList.attackDatas.at(index).datas.at(attackComboState).collisionNum);
			if (attackDatasList.attackDatas.at(index).receptionKey == AttackKey::WeakAttack)
			{
				attackPower = 0;
				attackDamage = 10;
				collisionDatas.at(attackCollisionIndex).radius = WeakAttackCollisionRadius;
			}
			else if (attackDatasList.attackDatas.at(index).receptionKey == AttackKey::StrongAttack)
			{
				attackPower = 1;
				attackDamage = 20;
				collisionDatas.at(attackCollisionIndex).radius = StrongAttackCollisionRadius;
			}
			else
			{
				attackPower = 0;
				attackDamage = 0;
				collisionDatas.at(attackCollisionIndex).radius = WeakAttackCollisionRadius;
			}
		}
		else
		{
			NoJudgment();
		}
	}
	else
	{
		for (Phoenix::s32 i = 0; i < isHit.size(); ++i)
		{
			isAttackJudgment.at(i) = false;
		}
	}
}

void Player::Damage(int damage, Phoenix::u32 damagePower)
{
	life -= damage;
	accumulationDamege += damage;
	this->damagePower = damagePower;
}

bool Player::AccumulationDamege()
{
	if (animationState == AnimationState::Damage)
	{
		if (!model->IsPlaying())
		{
			if (animationState != AnimationState::Idle)
			{
				attackReceptionTimeCnt = 0;
				isAttack = false;
				isChangeAnimation = true;
				speed = 0.0f;
				animationState = AnimationState::Idle;
				attackState = -1;
			}
		}
		else
		{
			//speed += KnockBackDownSpeed;
		}
	}

	if (accumulationDamege == 0) return false;

	if (AccumulationMaxDamege <= accumulationDamege)
	{
		isChangeAnimation = true;
		isAttack = false;
		speed = 0.0f;
		animationState = AnimationState::Damage;

		accumulationDamege = 0;
		accumulationTimeCnt = 0;

		return true;
	}
	else if (AccumulationTime <= accumulationTimeCnt++)
	{
		accumulationDamege = 0;
		accumulationTimeCnt = 0;
	}

	return false;
}

void Player::GUI()
{
	static Phoenix::s32 animClip = 0;

	if (ImGui::TreeNode("Player"))
	{
		/*if (ImGui::Button("Load Attack Data"))
		{
			const char* fullPass = Phoenix::OS::Path::GetFullPath("..\\Data\\Document\\Player\\AttackDatas.atk");

			Player::AttackDataList data;
			Player::AttackDataList::Deserialize(data, fullPass);

			SetAttackDatasList(data);
		}*/
		if (ImGui::TreeNode("Prameter"))
		{
			ImGui::Text("HP : %d", life);
			ImGui::Text("BehaviorScore : %d", behaviorScore);
			ImGui::Checkbox("BattleMode", &isBattleMode);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Transform"))
		{
			ImGui::DragFloat3("pos", &pos.x);
			ImGui::DragFloat3("rotate", &rotate.x);
			ImGui::DragFloat3("scale", &scale.x);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Speed"))
		{
			ImGui::Text("speed : %f", speed);
			/*ImGui::DragFloat("WalkSpeed", &WalkSpeed, 0.1f);
			ImGui::DragFloat("RunSpeed", &RunSpeed, 0.1f);
			ImGui::DragFloat("RollSpeed", &RollSpeed, 0.1f);
			ImGui::DragFloat("KnockBackSpeed", &KnockBackSpeed, 0.1f);*/
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Bone"))
		{
			//ImGui::ListBox("BoneListBox\n(single select)", &boneIndex, model->GetBoneNode()->at(0).name.data(), model->GetBoneNode()->at(0).name.size(), 4);
			//ImGui::ListBox("NodeListBox\n(single select)", &nodeIndex, model->GetNodes()->data(), model->GetNodes()->size(), 4);
			//ImGui::ListBox("ListBox\n(single select)", &boneIndex, model->GetBoneNames().data(), model->GetBoneNames().size(), 4);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Animation"))
		{
			ImGui::InputInt("AnimClip", &animClip);
			ImGui::Text("Len : %f", model->GetLength());
			ImGui::Text("RunLen : %f", attackReceptionTimeCnt);
			if (ImGui::Button("Play"))
			{
				model->PlayAnimation(0, animClip);
			}
			if (ImGui::Button("LoopPlay"))
			{
				model->SetLoopAnimation(true);
			}
			ImGui::TreePop();

			if (ImGui::TreeNode("Blend"))
			{
				ImGui::DragFloat("Rate", &blendRate.x, 0.01f, 0.0f, 1.0f);
				if (ImGui::Button("PlayBlendAnim"))
				{
					isChangeAnimation = true;
					//speed = WalkSpeed;
					animationState = AnimationState::Walk;
				}
				ImGui::TreePop();
			}
		}
		/*if (ImGui::TreeNode("Pass"))
		{
			HANDLE hFile;
			FILETIME ftFileTime, ftLocalFileTime;
			SYSTEMTIME newSTFileTime;

			const wchar_t* fullPass = Phoenix::OS::Path::GetFullPathW(L"..\\Data\\Document\\Player\\AttackDatas.atk");
			hFile = CreateFile(fullPass, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			GetFileTime(hFile, NULL, NULL, &ftFileTime);
			FileTimeToLocalFileTime(&ftFileTime, &ftLocalFileTime);
			FileTimeToSystemTime(&ftLocalFileTime, &newSTFileTime);
			CloseHandle(hFile);

			ImGui::Text("%d", stFileTime.wYear);
			ImGui::Text("%d", stFileTime.wMonth);
			ImGui::Text("%d", stFileTime.wDay);
			ImGui::Text("%d", stFileTime.wHour);
			ImGui::Text("%d", stFileTime.wMinute);
			ImGui::Text("%d", stFileTime.wSecond);

			ImGui::Text("%d", newSTFileTime.wYear);
			ImGui::Text("%d", newSTFileTime.wMonth);
			ImGui::Text("%d", newSTFileTime.wDay);
			ImGui::Text("%d", newSTFileTime.wHour);
			ImGui::Text("%d", newSTFileTime.wMinute);
			ImGui::Text("%d", newSTFileTime.wSecond);
			ImGui::TreePop();
		}*/
		ImGui::TreePop();
	}
}

template<class Archive>
void Player::AttackData::serialize(Archive& archive, Phoenix::u32 version)
{
	archive
	(
		CEREAL_NVP(animState),
		CEREAL_NVP(animIndex),

		CEREAL_NVP(playSpeed),
		CEREAL_NVP(playBeginTime),
		CEREAL_NVP(playEndTime),

		CEREAL_NVP(collisionNum),
		CEREAL_NVP(collisionBeginTime),
		CEREAL_NVP(collisionEndTime),

		CEREAL_NVP(receptionStack),
		CEREAL_NVP(receptionBeginTime),
		CEREAL_NVP(receptionEndTime),

		CEREAL_NVP(dedgeReceptionBeginTime),
		CEREAL_NVP(dedgeReceptionEndTime),

		CEREAL_NVP(weakDerivedAttackState),
		CEREAL_NVP(strongDerivedAttackState)
	);
}

template<class Archive>
void Player::AttackDatas::serialize(Archive& archive, Phoenix::u32 version)
{
	archive
	(
		CEREAL_NVP(receptionKey),
		CEREAL_NVP(datas)
	);
}

template<class Archive>
void Player::AttackDataList::serialize(Archive& archive, Phoenix::u32 version)
{
	archive
	(
		CEREAL_NVP(attackDatas)
	);
}

// シリアライズ
void Player::AttackDataList::Serialize(const Player::AttackDataList& data, const char* filename)
{
	// バイナリ
	{
		std::ofstream stream(filename, std::ios::binary);
		if (stream.is_open())
		{
			cereal::BinaryOutputArchive archive(stream);

			archive(CEREAL_NVP(data));
		}
	}
}

// デシリアライズ
bool Player::AttackDataList::Deserialize(Player::AttackDataList& data, const char* filename)
{
	std::ifstream stream(filename, std::ios::binary);
	if (stream.is_open())
	{
		cereal::BinaryInputArchive archive(stream);

		archive(CEREAL_NVP(data));
	}

	return true;
}