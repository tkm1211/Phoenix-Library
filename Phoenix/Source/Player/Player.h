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
	// �A�j���[�V�����X�e�[�g
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

	// �A�j���[�V�������C���[
	enum class LayerType
	{
		Base,
		LowerBody,
	};

	// �X�e�[�g���
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

	// �U�����̓L�[
	enum class AttackKey
	{
		WeakAttack,
		StrongAttack,
		None
	};

public:
	// �U���f�[�^
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

	// �U���f�[�^�W
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

	// �U���f�[�^�W���X�g
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
	// ���f��
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;

	// UI
	std::shared_ptr<PlayerUI> ui;

	// ���[���h�s��
	Phoenix::Math::Matrix worldMatrix;

	// ���W
	Phoenix::Math::Vector3 pos;

	// ��]
	Phoenix::Math::Quaternion rotate;

	// �X�P�[��
	Phoenix::Math::Vector3 scale;

	// ���a
	Phoenix::f32 radius = 0.0f;

	// �ړ����x
	Phoenix::f32 speed = 0.0f;

	// �V���ȉ�]
	Phoenix::Math::Quaternion newRotate;

	// Y����]
	Phoenix::f32 rotateY = 0.0f;

	// �A�j���[�V�����X�e�[�g
	AnimationState animationState;

	// �U���X�e�[�g
	Phoenix::s32 attackState = 0;

	// �U���R���{�X�e�[�g
	Phoenix::s32 attackComboState = 0;

	// ���݂̍U���A�j���[�V�����ԍ�
	Phoenix::s32 currentAttackAnimIndex = -1;

	// �U���f�[�^���X�g
	AttackDataList attackDatasList;

	// �A�j���[�V�����̐؂�ւ�
	bool isChangeAnimation = false;

	// �U����
	bool isAttack = false;

	// ���ݒ�
	bool alive = false;

	// ���S��
	bool death = false;

	// �U�����͔���p�J�E���g
	Phoenix::f32 attackReceptionTimeCnt;

	// �W���X�g������ԃJ�E���g
	Phoenix::f32 justDedgeTimeCnt;

	// �A�j���[�V�������x
	Phoenix::f32 animationSpeed;

	// �����蔻��f�[�^
	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;

	// HP
	Phoenix::s32 life = 0;

	// �A�^�b�N�������������H
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
	Player() {}
	~Player() {}

public:
	// ����
	static std::unique_ptr<Player> Create();

	// �R���X�g���N�^
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);

	// ������
	void Initialize();

	// �I����
	void Finalize();

	// �X�V
	void Update(Phoenix::Graphics::Camera& camera, bool onControl, Phoenix::f32 elapsedTime, bool attackLoad = true);

	// �g�����X�t�H�[���X�V
	void UpdateTrasform();

	// UI�X�V
	void UpdateUI();

	// ����X�V
	void Control(Phoenix::Graphics::Camera& camera, Phoenix::f32 elapsedTime, bool control);

	// �A�j���[�V�����ڍs
	void ChangeAnimation();

	// �U���A�j���[�V�����ڍs
	void ChangeAttackAnimation(Phoenix::s32 layerIndex);

	// �U������
	void AttackJudgment();

	// GUI
	void GUI();

	// �W���X�g��𔻒�
	bool OnJustDedge();

	// �_���[�W�X�V
	bool Damage(int damage, Phoenix::u32 damagePower);

	// �~�σ_���[�W
	bool AccumulationDamege();

	// �^�[�Q�b�g�����ɉ�]
	void Rotation(Phoenix::Math::Vector3 targetPos);

	// �G�l�~�[�̍��G�͈͓��ł̍s��
	void InEnemyTerritory(bool inTerritory);

	// �A�j���[�V�����ύX
	void ChangeAnimationState(AnimationState state, Phoenix::f32 moveSpeed = 0.0f);

	// �U���A�j���[�V�����ύX
	void ChangeAttackAnimationState(Phoenix::s32 state, Phoenix::s32 attackAnimIndex, Phoenix::f32 speed);

	// �W���X�g���ύX
	void ChangeJustDedge();

	// ���͔���
	bool CheckHitKey(AttackKey key);

public:
	// ���W��ݒ�
	void SetPosition(Phoenix::Math::Vector3 pos) { this->pos = pos; }

	// �U�����q�b�g
	void SetIsHit(Phoenix::s32 index) { isHit.at(index) = true; }

	// �W���X�g�����ݒ�
	void SetIsJustDedge(bool isJustDedge) { this->isJustDedge = isJustDedge; }

	// ���b�N�I����ݒ�
	void SetBattleMode(bool isBattleMode) { this->isBattleMode = isBattleMode; }

	// ���b�N�I������^�[�Q�b�g���W��ݒ�
	void SetTargetPos(Phoenix::Math::Vector3 targetPos) { this->targetPos = targetPos; }

	// �U���f�[�^���X�g��ݒ�
	void SetAttackDatasList(AttackDataList data) { attackDatasList = data; }

	// �T�E���h�V�X�e����ݒ�
	void SetSoundSystem(std::shared_ptr<SoundSystem<SoundType>> soundSystem) { this->soundSystem = soundSystem; }

public:
	// ���f���擾
	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }

	// ���[���h�s��擾
	Phoenix::Math::Matrix GetWorldMatrix() { return worldMatrix; }

	// ���W�擾
	Phoenix::Math::Vector3 GetPosition() { return pos; }

	// ��]�擾
	Phoenix::Math::Quaternion GetRotate() { return rotate; }

	// ���a�擾
	Phoenix::f32 GetRadius() { return radius; }

	// HP�擾
	Phoenix::s32 GetHP() { return life; }

	// �U����ގ擾
	Phoenix::u32 GetAttackPower() { return attackPower; }

	// �s���X�R�A�擾
	Phoenix::s32 GetScore() { return behaviorScore; }

	// �U���͎擾
	Phoenix::s32 GetAttackDamage() { return attackDamage; }

	// �U���X�e�[�g�擾
	Phoenix::s32 GetAttackState() { return attackState; }

	// �U�������蔻��̔ԍ��擾
	Phoenix::s32 GetAttackCollisionIndex() { return attackCollisionIndex; }

	// �A�j���[�V�����X�e�[�g�擾
	AnimationState GetAnimationState() { return animationState; }

	// �U���f�[�^���X�g�擾
	AttackDataList& GetAttackDatasList() { return attackDatasList; }

	// UI�擾
	std::shared_ptr<PlayerUI> GetUI() { return ui; }

	// �����蔻��f�[�^�擾
	const std::vector<Phoenix::FrameWork::CollisionData> GetCollisionDatas() { return collisionDatas; }

	// �U�������蔻����q�b�g�ς݂��擾
	const std::vector<bool> IsAttackJudgment() { return isAttackJudgment; }
	
	// ���G��
	bool Invincible() { return invincible; }

	// �U����
	bool IsAttack() { return isAttack; }

	// �_���[�W��
	bool IsDamage() { return animationState == AnimationState::Damage; }

	// �W���X�g���
	bool IsJustDedge() { return isJustDedge; }

	// ���G��
	bool IsInvincible() { return isInvincible; }
	
	// �G�l�~�[�̍��G�͈͓��ɂ��邩����
	bool OnEnemyTerritory() { return inTerritory; }

	// ���ݒ�
	bool GetAlive() { return alive; }

	// ���S��
	bool GetDeath() { return death; }

	// ���
	bool GetDodging() { return (animationState == AnimationState::Dedge); }

	// �\����
	bool GetLockOn() { return isBattleMode; }
};