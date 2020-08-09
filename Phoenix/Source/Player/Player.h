#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"


//******************************************************
//���́@vs�@����
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
	enum class AnimationState
	{
		Idle,
		Walk,
		Run,
		Roll,
		Attack,
		Damage
		//BackWalk,
		//BackRun,
	};

	enum class AttackAnimationState
	{
		Attack01,
		Attack02,
		Attack03,
		End
	};

private:
	/*static constexpr*/ float WalkSpeed = 2.1f;
	/*static constexpr*/ float RunSpeed = 18.0f;
	/*static constexpr*/ float RollSpeed = 15.0f;
	/*static constexpr*/ float KnockBackSpeed = -1.5f;
	/*static constexpr*/ float Attack03Speed = 10.0f;
	/*static constexpr*/ float AnimationSpeed30 = 0.03333333f;
	/*static constexpr*/ float AnimationSpeed45 = 0.02222222f;
	/*static constexpr*/ float AnimationSpeed60 = 0.01666667f;
	/*static constexpr*/ float Attack01ReceptionStartTime = 1.3333332f; // Goal�́AAnimation�̎��Ԃ̒�������擾 // 20 * 0.0166666667f;
	/*static constexpr*/ float Attack02ReceptionStartTime = 2.2f; // Goal�́AAnimation�̎��Ԃ̒�������擾 // 20 * 0.0166666667f;
	/*static constexpr*/ Phoenix::s32 AccumulationMaxDamege = 20; // TODO : �����K�{
	/*static constexpr*/ Phoenix::s32 AccumulationTime = 60;

private:
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;

	Phoenix::Math::Matrix worldMatrix;
	Phoenix::Math::Vector3 pos;
	Phoenix::Math::Vector3 rotate;
	//Phoenix::Math::Quaternion rotate;
	Phoenix::Math::Vector3 scale;
	Phoenix::f32 radius;
	Phoenix::f32 speed;

	AnimationState animationState;
	AttackAnimationState attackState;

	bool isChangeAnimation;
	bool isAttack;
	float attackReceptionTimeCnt;
	float animationSpeed;

	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;

	// HP
	Phoenix::s32 life = 0;

	// �A�^�b�N�������������H
	bool isHit = false;

	// �A�^�b�N�̔��蒆���H
	bool isAttackJudgment = false;

	// �R���W�����f�[�^�̗v�f��
	Phoenix::u32 attackCollisionIndex = 0;

	// �~�σ_���[�W
	Phoenix::s32 accumulationDamege = 0;
	Phoenix::s32 accumulationTimeCnt = 0;

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
	void Init(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
	void Update(Phoenix::Graphics::Camera& camera);
	void UpdateTrasform();
	void Control(Phoenix::Graphics::Camera& camera);
	void ChangeAnimation();
	void ChangeAttackAnimation();
	void AttackJudgment();
	void GUI();
	void Damage(int damage);
	void AccumulationDamege();

	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }
	Phoenix::Math::Matrix GetWorldMatrix() { return worldMatrix; }
	Phoenix::Math::Vector3 GetPosition() { return pos; }
	Phoenix::Math::Vector3 GetRotate() { return rotate; }
	Phoenix::f32 GetRadius() { return radius; }
	AnimationState GetAnimationState() { return animationState; }
	std::vector<Phoenix::FrameWork::CollisionData>* GetCollisionDatas() { return &collisionDatas; }
	bool IsAttackJudgment() { return isAttackJudgment; }
	Phoenix::u32 GetAttackCollisionIndex() { return attackCollisionIndex; }

	void SetPosition(Phoenix::Math::Vector3 pos) { this->pos = pos; }
	void SetIsHit(bool isHit) { this->isHit = isHit; }
};