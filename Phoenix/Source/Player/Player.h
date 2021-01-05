#pragma once

#include <memory>
#include <functional>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"
#include "../UI/PlayerUI.h"
#include "../Sound/SoundSystem.h"
#include "../Collision/Collision.h"


class Player
{
public:
	enum class AnimationState
	{
		Idle,
		Walk,
		Run,
		SlowRun,
		Roll,
		Attack,
		Damage,
		Dedge,
		Guard,
		Death,
	};

	enum class LayerType
	{
		Base,
		LowerBody,
	};

	enum class StateType
	{
		Idle,
		BattleIdle,
		DamageSmall,
		DamageBig,
		ForwardDedge,
		BackDedge,
		RightDedge,
		LeftDedge,
		Death,
	};

	enum class AttackKey
	{
		WeakAttack,
		StrongAttack,
		None
	};

public:
	struct AttackData
	{
		Phoenix::s32 animState = 0; // �A�j���[�V�����X�e�[�g
		Phoenix::s32 animIndex = 0; // �A�j���[�V�������C���[�̔ԍ�

		Phoenix::f32 playSpeed = 1.0f; // �Đ��X�s�[�h
		Phoenix::f32 playBeginTime = 0.0f; // �Đ��J�n���ԁi�Đ��t���[���j
		Phoenix::f32 playEndTime = 0.0f; // �Đ��I�����ԁi�Đ��t���[���j

		Phoenix::s32 collisionNum = 0; // �����蔻��̔ԍ�
		Phoenix::f32 collisionBeginTime = 0.0f; // �����蔻��J�n���ԁi�Đ��t���[���j
		Phoenix::f32 collisionEndTime = 0.0f; // �����蔻��I�����ԁi�Đ��t���[���j

		bool receptionStack = false; // ���̃A�j���[�V�����֑J�ڂ��邽�߂̓��͎�t���X�^�b�N���邩�ǂ����H
		Phoenix::f32 receptionBeginTime = 0.0f; // ���̃A�j���[�V�����֑J�ڂ��邽�߂̓��͎�t�J�n����
		Phoenix::f32 receptionEndTime = 0.0f; // ���̃A�j���[�V�����֑J�ڂ��邽�߂̓��͎�t�I������

		Phoenix::f32 dedgeReceptionBeginTime = 0.0f; // ����֑J�ڂ��邽�߂̓��͎�t�J�n����
		Phoenix::f32 dedgeReceptionEndTime = 0.0f; // ����֑J�ڂ��邽�߂̓��͎�t�I������

		Phoenix::s32 weakDerivedAttackState = -1; // ���̔h����U���̃X�e�[�g
		Phoenix::s32 strongDerivedAttackState = -1; // ���̔h�����U���̃X�e�[�g

		template<class Archive>
		void serialize(Archive& archive, Phoenix::u32 version);
	};

	struct AttackDatas
	{
		AttackKey receptionKey = AttackKey::WeakAttack; // ���̓L�[
		std::vector<AttackData> datas;

		void SetKey(AttackKey key)
		{
			receptionKey = key;
		}
		void AddData(AttackData data)
		{
			datas.emplace_back(data);
		}

		template<class Archive>
		void serialize(Archive& archive, Phoenix::u32 version);
	};

	struct AttackDataList
	{
		std::vector<AttackDatas> attackDatas;

		AttackDataList() {}

		template<class Archive>
		void serialize(Archive& archive, Phoenix::u32 version);

		// �V���A���C�Y
		static void Serialize(const AttackDataList& data, const char* filename);

		// �f�V���A���C�Y
		static bool Deserialize(AttackDataList& data, const char* filename);
	};

private:
	static constexpr Phoenix::f32 WalkSpeed = 0.021f;
	static constexpr Phoenix::f32 BattleWalkSpeed = 0.0105f;
	static constexpr Phoenix::f32 RunSpeed = 0.18f;
	static constexpr Phoenix::f32 SlowRunSpeed = 0.09f;
	static constexpr Phoenix::f32 BattleSlowRunSpeed = 0.045f;
	static constexpr Phoenix::f32 RollSpeed = 0.15f;
	static constexpr Phoenix::f32 DedgeSpeed = 0.15f;
	static constexpr Phoenix::f32 KnockBackSpeed = -0.03f;
	static constexpr Phoenix::f32 KnockBackDownSpeed = 0.0003f;
	static constexpr Phoenix::f32 Attack03Speed = 0.1f;
	static constexpr Phoenix::f32 AnimationSpeed30 = 30.0f; // 0.03333333f
	static constexpr Phoenix::f32 AnimationSpeed45 = 45.0f; // 0.02222222f
	static constexpr Phoenix::f32 AnimationSpeed60 = 60.0f; // 0.01666667f
	static constexpr Phoenix::f32 Attack01AnimationSpeed = 1.75f;
	static constexpr Phoenix::f32 Attack02AnimationSpeed = 2.0f;
	static constexpr Phoenix::f32 Attack03AnimationSpeed = 1.75f;
	static constexpr Phoenix::f32 Attack04AnimationSpeed = 1.15f;
	static constexpr Phoenix::f32 Attack05AnimationSpeed = 1.0f;
	static constexpr Phoenix::f32 Attack06AnimationSpeed = 1.0f;
	static constexpr Phoenix::f32 Attack01ReceptionStartTime = 1.3333332f; // Goal�́AAnimation�̎��Ԃ̒�������擾 // 20 * 0.0166666667f;
	static constexpr Phoenix::f32 Attack02ReceptionStartTime = 2.2f; // Goal�́AAnimation�̎��Ԃ̒�������擾 // 20 * 0.0166666667f;
	static constexpr Phoenix::f32 Attack01MoveSpeed = 0.1f;
	static constexpr Phoenix::f32 WeakAttackCollisionRadius = 0.25f;
	static constexpr Phoenix::f32 StrongAttackCollisionRadius = 0.75f;
	static constexpr Phoenix::s32 MaxLife = 100; // TODO : �����K�{
	static constexpr Phoenix::s32 AccumulationMaxDamege = 10; // TODO : �����K�{
	static constexpr Phoenix::s32 AccumulationTime = 60;

private:
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;
	std::shared_ptr<PlayerUI> ui;

	Phoenix::Math::Matrix worldMatrix;
	Phoenix::Math::Vector3 pos;
	Phoenix::Math::Quaternion rotate;
	Phoenix::Math::Vector3 scale;
	Phoenix::f32 radius = 0.0f;
	Phoenix::f32 speed = 0.0f;

	Phoenix::Math::Quaternion newRotate;
	Phoenix::f32 rotateY = 0.0f;

	AnimationState animationState;
	Phoenix::s32 attackState = 0;
	Phoenix::s32 attackComboState = 0;
	Phoenix::s32 currentAttackAnimIndex = -1;
	AttackDataList attackDatasList;

	bool isChangeAnimation;
	bool isAttack;
	bool alive = false;
	bool death = false;
	Phoenix::f32 attackReceptionTimeCnt;
	Phoenix::f32 justDedgeTimeCnt;
	Phoenix::f32 animationSpeed;

	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;

	// HP
	Phoenix::s32 life = 0;

	// �A�^�b�N�������������H
	//bool isHit = false;
	std::vector<bool> isHit;

	// �A�^�b�N�̔��蒆���H
	std::vector<bool> isAttackJudgment;

	// �A�^�b�N�_���[�W��
	Phoenix::s32 attackDamage = 0;

	// ����̖��G�����H
	bool invincible = false;

	// �R���W�����f�[�^�̗v�f��
	Phoenix::s32 attackCollisionIndex = 0;

	// �U���̈З�
	Phoenix::u32 attackPower = 0;

	// �~�σ_���[�W
	Phoenix::s32 accumulationDamege = 0;
	Phoenix::s32 accumulationTimeCnt = 0;

	// �_���[�W�̈З�
	Phoenix::u32 damagePower = 0;

	// �u�����h
	Phoenix::Math::Vector3 blendRate = { 0.0f, 0.0f, 0.0f };

	// �G�l�~�[�e���g���[�t���O
	bool inTerritory = false;

	// �o�g�����[�h
	bool isBattleMode = false;
	Phoenix::u32 dedgeLayerIndex = 0;
	Phoenix::Math::Vector3 targetPos = { 0.0f, 0.0f, 0.0f };

	// ���̓X�^�b�N
	bool receptionStack = false;
	AttackKey stackKey = AttackKey::None;

	// �s���X�R�A
	Phoenix::s32 behaviorScore = 0;
	const Phoenix::s32 WeakAttackScore = 100;
	const Phoenix::s32 StrongAttackScore = 300;

	// �t�@�C���^�C���X�^���v
	SYSTEMTIME stFileTime{};

	// �T�E���h
	std::shared_ptr<SoundSystem<SoundType>> soundSystem;

	// ���C���[�ԍ�
	std::map<LayerType, Phoenix::s32> layerIndexList;

	// ���C���[���̃X�e�[�g�ԍ�
	std::map<StateType, Phoenix::s32> stateIndexList;

	// �W���X�g���
	bool isJustDedge = false;

	// ���G
	bool isInvincible = false;
	Phoenix::f32 invincibleTimeCnt = 0.0f;

public:
	Player() :
		worldMatrix(Phoenix::Math::MatrixIdentity()), 
		speed(0.0f),
		animationState(AnimationState::Idle),
		attackState(0),
		isChangeAnimation(false), 
		isAttack(false),
		attackReceptionTimeCnt(0.0f),
		animationSpeed(0.0f),
		radius(0.0f)
	{}
	~Player() { /*Finalize();*/ }

public:
	static std::unique_ptr<Player> Create();
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
	void Initialize();
	void Finalize();
	void Update(Phoenix::Graphics::Camera& camera, bool onControl, Phoenix::f32 elapsedTime, bool attackLoad = true);
	void UpdateTrasform();
	void UpdateUI();
	void Control(Phoenix::Graphics::Camera& camera, Phoenix::f32 elapsedTime, bool control);
	void ChangeAnimation();
	void ChangeAttackAnimation(Phoenix::s32 layerIndex);
	void AttackJudgment();
	void GUI();
	bool OnJustDedge();
	bool Damage(int damage, Phoenix::u32 damagePower);
	bool AccumulationDamege();
	void Rotation(Phoenix::Math::Vector3 targetPos)
	{
		Phoenix::Math::Vector3 dir = targetPos - pos;
		dir = Phoenix::Math::Vector3Normalize(dir);
		dir.y = 0.0f;

		//Phoenix::Math::Quaternion rotate = boss->GetRotate();
		Phoenix::Math::Matrix m = Phoenix::Math::MatrixRotationQuaternion(&rotate);
		Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(m._31, m._32, m._33);
		Phoenix::Math::Vector3 up = Phoenix::Math::Vector3(m._21, m._22, m._23);
		Phoenix::Math::Vector3 right = Phoenix::Math::Vector3(m._11, m._12, m._13);
		forward.y = 0.0f;

		Phoenix::f32 angle;
		angle = acosf(Phoenix::Math::Vector3Dot(dir, forward));

		// ��]
		if (1e-8f < fabs(angle))
		{
			Phoenix::f32 angleR;
			angleR = acosf(Phoenix::Math::Vector3Dot(dir, right));
			angleR -= (90.0f * 0.01745f);

			if (0.0f < angleR) angle *= -1;

			Phoenix::Math::Quaternion q;
			q = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angle);
			//rotate *= q;

			Phoenix::Math::Quaternion rotateT = rotate;
			rotateT *= q;
			//rotate = Phoenix::Math::QuaternionSlerp(rotate, rotateT, 0.17f);
			rotate = rotateT;
		}
	}
	void InEnemyTerritory(bool inTerritory)
	{
		if (this->inTerritory != inTerritory && animationState != AnimationState::Attack)
		{
			ChangeAnimationState(AnimationState::Idle, 0.0f);

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
		this->inTerritory = inTerritory;
		SetBattleMode(inTerritory);
	}

	// �A�j���[�V�����ύX
	void ChangeAnimationState(AnimationState state, Phoenix::f32 moveSpeed = 0.0f)
	{
		isChangeAnimation = true;
		animationState = state;

		speed = moveSpeed;
	};

	// �U���A�j���[�V�����ύX
	void ChangeAttackAnimationState(Phoenix::s32 state, Phoenix::s32 attackAnimIndex, Phoenix::f32 speed)
	{
		isAttack = true;

		attackState = state;
		currentAttackAnimIndex = attackAnimIndex;
		animationSpeed = speed;
	};

	// �W���X�g���ύX
	void ChangeJustDedge()
	{
		ChangeAnimationState(AnimationState::Dedge, DedgeSpeed);
		
		Phoenix::s32 x = rand() % 2;
		if (x)
		{
			dedgeLayerIndex = stateIndexList.at(StateType::RightDedge);
		}
		else
		{
			dedgeLayerIndex = stateIndexList.at(StateType::LeftDedge);
		}

		isJustDedge = false;
		justDedgeTimeCnt = 0.0f;
	}

	bool CheckHitKey(AttackKey key)
	{
		switch (key)
		{
		case AttackKey::WeakAttack:
			if ((xInput[0].bXt || GetAsyncKeyState('J') & 1))
			{
				return true;
			}
			break;

		case AttackKey::StrongAttack:
			if ((xInput[0].bYt || GetAsyncKeyState('K') & 1))
			{
				return true;
			}
			break;

		default: break;
		}

		return false;
	}

	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }
	Phoenix::Math::Matrix GetWorldMatrix() { return worldMatrix; }
	Phoenix::Math::Vector3 GetPosition() { return pos; }
	//Phoenix::Math::Vector3 GetRotate() { return rotate; }
	Phoenix::Math::Quaternion GetRotate() { return rotate; }
	Phoenix::f32 GetRadius() { return radius; }
	Phoenix::s32 GetHP() { return life; }
	Phoenix::u32 GetAttackPower() { return attackPower; }
	Phoenix::s32 GetScore() { return behaviorScore; }
	Phoenix::s32 GetAttackDamage() { return attackDamage; }
	AnimationState GetAnimationState() { return animationState; }
	AttackDataList& GetAttackDatasList() { return attackDatasList; }
	bool GetDodging() { return (animationState == AnimationState::Dedge); }
	const std::vector<Phoenix::FrameWork::CollisionData> GetCollisionDatas() { return collisionDatas; }
	//bool IsAttackJudgment() { return isAttackJudgment; }
	std::vector<bool> IsAttackJudgment() { return isAttackJudgment; }
	bool Invincible() { return invincible; }
	Phoenix::s32 GetAttackCollisionIndex() { return attackCollisionIndex; }
	std::shared_ptr<PlayerUI> GetUI() { return ui; }
	bool IsAttack() { return isAttack; }
	bool IsDamage() { return animationState == AnimationState::Damage; }
	bool IsJustDedge() { return isJustDedge; }

	bool GetAlive() { return alive; }
	bool GetDeath() { return death; }

	bool OnEnemyTerritory() { return inTerritory; }

	void SetPosition(Phoenix::Math::Vector3 pos) { this->pos = pos; }
	void SetIsHit(Phoenix::s32 index) { isHit.at(index) = true; }
	void SetIsJustDedge(bool isJustDedge) { this->isJustDedge = isJustDedge; }

	void SetBattleMode(bool isBattleMode) { this->isBattleMode = isBattleMode; }
	void SetTargetPos(Phoenix::Math::Vector3 targetPos) { this->targetPos = targetPos; }
	void SetAttackDatasList(AttackDataList data) { attackDatasList = data; }
	void SetSoundSystem(std::shared_ptr<SoundSystem<SoundType>> soundSystem) { this->soundSystem = soundSystem; }
};