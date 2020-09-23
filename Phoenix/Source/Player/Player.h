#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "../UI/PlayerUI.h"


//******************************************************
//球体　vs　球体
//******************************************************
static bool SphereVsSphere(Phoenix::Math::Vector3 pos1, Phoenix::Math::Vector3 pos2, float r1, float r2)
{
	if ((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) + (pos2.z - pos1.z) * (pos2.z - pos1.z) <= (r1 + r2) * (r1 + r2))
	{
		return true;
	}
	else return false;
}

static bool CircleVsCircle(Phoenix::Math::Vector2 pos1, Phoenix::Math::Vector2 pos2, float r1, float r2)
{
	if ((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) <= (r1 + r2) * (r1 + r2))
	{
		return true;
	}
	else return false;
}

class Player
{
public:
#if 0
	enum class AnimationState
	{
		Idle,
		Walk,
		Run,
		SlowRun,
		Roll,
		Attack01,
		Attack02,
		Attack03,
		Damage
	};
#else
	enum class AnimationState
	{
		Idle,
		Walk,
		Run,
		SlowRun,
		Roll,
		Attack01,
		Attack02,
		Attack03,
		Attack04,
		Attack05,
		Attack06,
		Damage
	};
#endif

#if 0
	enum class AttackAnimationState
	{
		Attack01,
		Attack02,
		Attack03,
		End
	};
#elif 0
	enum class AttackAnimationState
	{
		Attack01_1,
		Attack01_2,
		Attack02_1,
		Attack02_2,
		Attack02_3,
		Attack02_4,
		Attack02_5,
		Attack02_6,
		Attack03_1,
		End
	};
#else
	enum class AttackAnimationState
	{
		Attack01,
		Attack02,
		Attack03,
		Attack04,
		Attack05,
		Attack06,
		End
	};
#endif

private:
	struct AttackData
	{
		AnimationState animationState;
		AttackAnimationState attackState;

		AnimationState nextAnimationState;
		AttackAnimationState nextAttackState;

		Phoenix::f32 playBeginTime;
		Phoenix::f32 playEndTime;

		Phoenix::f32 collisionBeginTime;
		Phoenix::f32 collisionEndTime;

		Phoenix::s32 collisionNum;

		Phoenix::f32 receptionBeginTime;
		Phoenix::f32 receptionEndTime;

		Phoenix::f32 nextMoveSpeed;
		Phoenix::f32 animationSpeed;
	};

private:
	/*static constexpr*/ Phoenix::f32 WalkSpeed = 0.021f;
	/*static constexpr*/ Phoenix::f32 RunSpeed = 0.18f;
	/*static constexpr*/ Phoenix::f32 SlowRunSpeed = 0.09f;
	/*static constexpr*/ Phoenix::f32 RollSpeed = 0.15f;
	/*static constexpr*/ Phoenix::f32 KnockBackSpeed = -0.03f;
	/*static constexpr*/ Phoenix::f32 KnockBackDownSpeed = 0.0003f;
	/*static constexpr*/ Phoenix::f32 Attack03Speed = 0.1f;
	/*static constexpr*/ Phoenix::f32 AnimationSpeed30 = 30.0f; // 0.03333333f
	/*static constexpr*/ Phoenix::f32 AnimationSpeed45 = 45.0f; // 0.02222222f
	/*static constexpr*/ Phoenix::f32 AnimationSpeed60 = 60.0f; // 0.01666667f
	/*static constexpr*/ Phoenix::f32 Attack01AnimationSpeed = 1.75f;
	/*static constexpr*/ Phoenix::f32 Attack02AnimationSpeed = 2.0f;
	/*static constexpr*/ Phoenix::f32 Attack03AnimationSpeed = 1.75f;
	/*static constexpr*/ Phoenix::f32 Attack04AnimationSpeed = 1.15f;
	/*static constexpr*/ Phoenix::f32 Attack05AnimationSpeed = 1.0f;
	/*static constexpr*/ Phoenix::f32 Attack06AnimationSpeed = 1.0f;
	/*static constexpr*/ Phoenix::f32 Attack01ReceptionStartTime = 1.3333332f; // Goalは、Animationの時間の長さから取得 // 20 * 0.0166666667f;
	/*static constexpr*/ Phoenix::f32 Attack02ReceptionStartTime = 2.2f; // Goalは、Animationの時間の長さから取得 // 20 * 0.0166666667f;
	/*static constexpr*/ Phoenix::s32 MaxLife = 100; // TODO : 調整必須
	/*static constexpr*/ Phoenix::s32 AccumulationMaxDamege = 10; // TODO : 調整必須
	/*static constexpr*/ Phoenix::s32 AccumulationTime = 60;

private:
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;
	std::shared_ptr<PlayerUI> ui;

	Phoenix::Math::Matrix worldMatrix;
	Phoenix::Math::Vector3 pos;
	Phoenix::Math::Vector3 rotate;
	//Phoenix::Math::Quaternion rotate;
	Phoenix::Math::Vector3 scale;
	Phoenix::f32 radius;
	Phoenix::f32 speed;

	AnimationState animationState;
	AttackAnimationState attackState;
	std::vector<AttackData> attackDatas;

	bool isChangeAnimation;
	bool isAttack;
	float attackReceptionTimeCnt;
	float animationSpeed;

	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;

	// HP
	Phoenix::s32 life = 0;

	// アタックが当たったか？
	bool isHit = false;

	// アタックの判定中か？
	bool isAttackJudgment = false;

	// 回避の無敵中か？
	bool invincible = false;

	// コリジョンデータの要素数
	Phoenix::u32 attackCollisionIndex = 0;

	// 蓄積ダメージ
	Phoenix::s32 accumulationDamege = 0;
	Phoenix::s32 accumulationTimeCnt = 0;

	// ブレンド
	Phoenix::f32 blendRate = 0.0f;

public:
	Player() :
		worldMatrix(Phoenix::Math::MatrixIdentity()), 
		speed(0.0f), 
		animationState(AnimationState::Idle),
		attackState(AttackAnimationState::End), 
		isChangeAnimation(false), 
		isAttack(false),
		attackReceptionTimeCnt(0.0f), 
		animationSpeed(0.0f),
		radius(0.0f)
	{}
	~Player() {}

public:
	static std::unique_ptr<Player> Create();
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
	void Initialize();
	void Update(Phoenix::Graphics::Camera& camera, bool onControl);
	void UpdateTrasform();
	void UpdateUI();
	void Control(Phoenix::Graphics::Camera& camera);
	void ChangeAnimation();
	void ChangeAttackAnimation(Phoenix::u32 animationNum);
	void AttackJudgment();
	void GUI();
	void Damage(int damage);
	bool AccumulationDamege();

	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }
	Phoenix::Math::Matrix GetWorldMatrix() { return worldMatrix; }
	Phoenix::Math::Vector3 GetPosition() { return pos; }
	Phoenix::Math::Vector3 GetRotate() { return rotate; }
	Phoenix::f32 GetRadius() { return radius; }
	Phoenix::s32 GetHP() { return life; }
	AnimationState GetAnimationState() { return animationState; }
	const std::vector<Phoenix::FrameWork::CollisionData>* GetCollisionDatas() { return &collisionDatas; }
	bool IsAttackJudgment() { return isAttackJudgment; }
	bool Invincible() { return invincible; }
	Phoenix::u32 GetAttackCollisionIndex() { return attackCollisionIndex; }
	PlayerUI* GetUI() { return ui.get(); }
	bool IsAttack() { return isAttack; }

	void SetPosition(Phoenix::Math::Vector3 pos) { this->pos = pos; }
	void SetIsHit(bool isHit) { this->isHit = isHit; }
};