#include "Enemy.h"
#include "EnemyManager.h"
#include "../Player/Player.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"


// 生成
std::shared_ptr<Enemy> Enemy::Create()
{
	return std::make_shared<Enemy>();
}

// コンストラクタ
void Enemy::Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	// モデル読み込み
	{
		model = std::make_unique<Phoenix::FrameWork::ModelObject>();
		model->Initialize(graphicsDevice);
		model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Enemy\\Enemy\\Idle\\Ready_Idle.fbx"); // "..\\Data\\Assets\\Model\\Enemy\\Idle\\Idle.fbx"  // "..\\Data\\Assets\\Model\\Boss\\Mutant\\Idle\\Mutant_Roaring.fbx"
	}

	// アニメーション読み込み
	{
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Forward\\Walk_Forward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Back\\Walk_Backward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Right\\Walk_Right.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Left\\Walk_Left.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Run\\Running.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Dedge\\Back_Step.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Attack\\Cross_Punch.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Attack\\Punching.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Attack\\Hook_Punch.fbx", -1);

		model->AddAnimationLayer(0); // 0
		model->AddAnimationLayer(0, 56, 65); // 1

		model->AddAnimationLayer(5); // 2
		model->AddAnimationLayer(6); // 3

		model->AddAnimationLayer(7); // 4
		model->AddAnimationLayer(8); // 5
		model->AddAnimationLayer(9); // 6

		model->AddBlendAnimationToLayer(1, 1, Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f));
		model->AddBlendAnimationToLayer(2, 1, Phoenix::Math::Vector3(0.0f, -1.0f, 0.0f));
		model->AddBlendAnimationToLayer(3, 1, Phoenix::Math::Vector3(1.0f, 0.0f, 0.0f));
		model->AddBlendAnimationToLayer(4, 1, Phoenix::Math::Vector3(-1.0f, 0.0f, 0.0f));
	}

	// コリジョン初期化
	{
		collisionDatas.resize(4);

		collisionDatas.at(0).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(0).radius = 0.5f;
		collisionDatas.at(0).boneIndex = model->GetBoneIndex("Hips");

		collisionDatas.at(1).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(1).radius = 0.25f;
		collisionDatas.at(1).boneIndex = model->GetBoneIndex("RightHandIndex1");

		collisionDatas.at(2).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(2).radius = 0.25f;
		collisionDatas.at(2).boneIndex = model->GetBoneIndex("LeftHandIndex1");

		collisionDatas.at(3).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(3).radius = 0.25f;
		collisionDatas.at(3).boneIndex = model->GetBoneIndex("RightFoot");
	}

	// トランスフォームの初期化
	{
		transform = std::make_unique<Phoenix::FrameWork::Transform>();
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
			battleAI->SetOwner(shared_from_this());
			battleAI->SetUp();
		}
	}

	// その他のパラメータ初期化
	{
		Initialize();
	}
}

// 初期化
void Enemy::Initialize()
{
	// 待機モーション開始
	{
		model->PlayAnimation(0, 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
	}

	// AI
	{
		battleAI->GoToState(BattleEnemyState::Idle);
	}

	// パラメーター
	{
		enable = false;
		alive = false;
		inBattle = false;

		life = LifeRange;
		radius = 0.5f;
	}
}

// 終了化
void Enemy::Finalize()
{
	battleAI->CleanUp();
	battleAI.reset();

	collisionDatas.clear();
	transform.reset();
	model.reset();
}

// 更新
void Enemy::Update()
{
	if (!enable) return;
	if (!alive) return;

	// AI更新
	{
		BattleEnemyState nextBattleState = BattleEnemyState::NoneState;

		switch (currentMode)
		{
		case EnemyMode::Ordinary:
			break;

		case EnemyMode::Battle:
			nextBattleState = battleAI->Update();
			if (nextBattleState != BattleEnemyState::NoneState)
			{
				changeAnimation = true;
				changeState = nextBattleState;
				battleAI->GoToState(nextBattleState);
			}

			break;

		default: break;
		}
	}

	// 回転
	{
		Phoenix::Math::Vector3 dir = Phoenix::Math::Vector3Normalize(player->GetPosition() - GetPosition());
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

		Phoenix::Math::Quaternion newRotate;
		newRotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angleY);

		Phoenix::Math::Quaternion rotate = transform->GetRotate();
		rotate = Phoenix::Math::QuaternionSlerp(rotate, newRotate, 0.17f);

		transform->SetRotate(rotate);
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
		model->UpdateTransform(1 / 60.0f);
	}

	// トランスフォーム更新
	{
		transform->Update();
	}

	// コリジョン更新
	{
		Phoenix::Math::Matrix systemUnitTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		systemUnitTransform._11 = systemUnitTransform._22 = systemUnitTransform._33 = 0.01f;

		auto nodes = model->GetNodes();
		for (auto& data : collisionDatas)
		{
			Phoenix::Math::Matrix bone = nodes->at(data.boneIndex).worldTransform;
			bone *= systemUnitTransform * transform->GetWorldTransform();
			data.pos = Phoenix::Math::Vector3(bone._41, bone._42, bone._43);
		}
	}

	// ライフが０ならマネージャーの生存エネミーカウントを下げる
	if (life <= 0)
	{
		enable = false;
		alive = false;
		if (std::shared_ptr<EnemyManager> manager = owner.lock())
		{
			manager->SubAliveEnemyCount(1);
		}
	}
}

// UI更新
void Enemy::UpdateUI()
{

}

// 描画
void Enemy::Draw() {}

// GUI
void Enemy::GUI(Phoenix::s32 index)
{
	std::string name = std::string("Enemy") + std::to_string(index);
	if (ImGui::TreeNode(name.c_str()))
	{
		transform->GUI();
		ImGui::TreePop();
	}
}

// 有効化設定
void Enemy::SetEnable(bool enable)
{
	this->enable = enable;
}

// 存在フラグ設定
void Enemy::SetAlive(bool alive)
{
	this->alive = alive;
}

// 戦闘中フラグ設定
void Enemy::SetInBattle(bool inBattle)
{
	this->inBattle = inBattle;
	currentMode = inBattle ? EnemyMode::Battle : EnemyMode::Ordinary;
}

// トランスフォームの設定
void Enemy::SetTransform(Phoenix::FrameWork::Transform transform)
{
	this->transform->SetTranslate(transform.GetTranslate());
	this->transform->SetRotate(transform.GetRotate());
	this->transform->SetScale(transform.GetScale());
}

// トランスレートの設定
void Enemy::SetTranslate(Phoenix::Math::Vector3 translate)
{
	this->transform->SetTranslate(translate);
}

// エネミーマネージャーの設定
void Enemy::SetOwner(std::shared_ptr<EnemyManager> owner)
{
	this->owner = owner;
}

// 攻撃権を発行
void Enemy::SetAttackRight()
{
	battleAI->GoToState(BattleEnemyState::Attack);
}

// 攻撃ステートを変更
void Enemy::SetAttackState(EnemyAttackState state)
{
	changeAttackAnimation = true;
	changeAttackState = state;
}

// プレイヤーを設定
void Enemy::SetPlayer(std::shared_ptr<Player> player)
{
	this->player = player;
}

// アニメーションを移行
void Enemy::ChangeAnimation()
{
	if (!changeAnimation) return;

	switch (changeState)
	{
	case BattleEnemyState::Idle:
		model->PlayAnimation(0, 1, 0.2f);
		//model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		break;

	case BattleEnemyState::Attack: break;

	case BattleEnemyState::Dedge:
		model->PlayAnimation(3, 1, 0.2f);
		//model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		break;

	case BattleEnemyState::Guard:
		model->PlayAnimation(0, 1, 0.2f);
		//model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		break;

	case BattleEnemyState::NoneState: break;

	default: break;
	}

	changeAnimation = false;
	changeState = BattleEnemyState::NoneState;
}

// 攻撃ステートを移行
void Enemy::ChangeAttackAnimation()
{
	if (!changeAttackAnimation) return;

	switch (changeAttackState)
	{
	case EnemyAttackState::WeakRight:
		model->PlayAnimation(4, 1, 0.2f);
		//model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		model->SetSpeed(2.0f);

		break;

	case EnemyAttackState::WeakLeft:
		model->PlayAnimation(5, 1, 0.2f);
		//model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		model->SetSpeed(2.0f);

		break;

	case EnemyAttackState::StrongRight:
		model->PlayAnimation(6, 1, 0.2f);
		//model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		model->SetSpeed(2.0f);

		break;

	case EnemyAttackState::StrongLeft:
		model->PlayAnimation(7, 1, 0.2f);
		//model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		model->SetSpeed(2.0f);

		break;

	case EnemyAttackState::NoneState: break;

	default: break;
	}

	changeAttackAnimation = false;
	changeAttackState = EnemyAttackState::NoneState;
}

void Enemy::Damage(int damage)
{
	//life -= damage;
}

// 有効フラグ取得
bool Enemy::GetEnable()
{
	return enable;
}

// 存在フラグ取得
bool Enemy::GetAlive()
{
	return alive;
}

// 戦闘中フラグ取得
bool Enemy::GetInBattle()
{
	return inBattle;
}

// トランスフォームの取得
Phoenix::FrameWork::Transform Enemy::GetTransform()
{
	return *transform.get();
}

// バトルモードのステート取得
BattleEnemyState Enemy::GetBattleState()
{
	return battleAI->GetCurrentStateName();
}