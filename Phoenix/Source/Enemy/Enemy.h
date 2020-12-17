#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Component/Transform.h"
#include "EnemyState.h"
#include "../AI/StateMachine/BattleEnemyAI.h"
#include "../UI/EnemyUI.h"


class EnemyManager;
class Player;
class Enemy : public std::enable_shared_from_this<Enemy>
{
public:
	static constexpr Phoenix::s32 LifeRange = 150;

public:
	enum class TypeTag
	{
		Small,  // �G��
		Medium, // ���{�X
		Large   // �{�X
	};

	enum class LayerType
	{
		Base,
		LowerBody,
	};

	enum class StateType
	{
		Idle,
		Run,
		DamageSmall,
		DamageBig,
		Dedge,
		Death,
	};

	struct AttackData
	{
		EnemyAttackState animState; // �A�j���[�V�����X�e�[�g
		Phoenix::s32 animIndex; // �A�j���[�V�������C���[�̔ԍ�

		Phoenix::f32 playSpeed; // �Đ��X�s�[�h
		Phoenix::f32 playBeginTime; // �Đ��J�n���ԁi�Đ��t���[���j
		Phoenix::f32 playEndTime; // �Đ��I�����ԁi�Đ��t���[���j

		Phoenix::s32 collisionNum; // �����蔻��̔ԍ�
		Phoenix::f32 collisionBeginTime; // �����蔻��J�n���ԁi�Đ��t���[���j
		Phoenix::f32 collisionEndTime; // �����蔻��I�����ԁi�Đ��t���[���j
	};

	struct AttackDatas
	{
		std::vector<AttackData> datas;

		void AddData(AttackData data)
		{
			datas.emplace_back(data);
		}
	};

private:
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;
	std::unique_ptr<Phoenix::FrameWork::Transform> transform;
	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;
	std::vector<AttackDatas> attackDatasList;
	std::shared_ptr<EnemyUI> ui;

	// AI
	EnemyMode currentMode = EnemyMode::Ordinary;
	//std::shared_ptr<OrdinaryAIEnemyAI> ordinaryAI;
	std::shared_ptr<BattleEnemyAI> battleAI;

	// �}�l�[�W���[
	std::weak_ptr<EnemyManager> owner;

	// �v���C���[
	std::shared_ptr<Player> player;

	// �A�j���[�V����
	bool changeAnimation = false;
	bool changeAttackAnimation = false;
	BattleEnemyState changeState = BattleEnemyState::NoneState;
	EnemyAttackState changeAttackState = EnemyAttackState::NoneState;
	EnemyAttackState currentAttackState = EnemyAttackState::NoneState;

	// �p�����[�^�[
	Phoenix::s32 life = 0;
	Phoenix::f32 radius = 0.0f;

	// �V���ȉ�]�l
	Phoenix::Math::Quaternion newRotate;

	// �R���W�����f�[�^�̗v�f��
	Phoenix::u32 attackCollisionIndex = 0;

	// �U���̈З�
	Phoenix::u32 attackPower = 0;

	// �v���C���[�Ƃ̋���
	Phoenix::f32 distanceToPlayer = 0.0f;

	// �ړ��X�s�[�h
	Phoenix::f32 moveSpeed = 0.0f;

	// �ړ�����
	Phoenix::f32 moveX = 0.0f;
	Phoenix::f32 moveY = 0.0f;

	// �t���O
	bool enable = false;
	bool alive = false;
	bool death = false;
	bool inBattle = false;

	// �A�^�b�N�������������H
	bool isHit = false;

	// �A�^�b�N�̔��蒆���H
	bool isAttackJudgment = false;

	// �U���A�j���[�V��������
	float attackReceptionTimeCnt = 0.0f;
	float attackAnimationSpeed = 0.0f;

	// �U����
	bool stackAttackRight = false;

	// ���C���[�ԍ�
	std::map<LayerType, Phoenix::s32> layerIndexList;

	// ���C���[���̃X�e�[�g�ԍ�
	std::map<StateType, Phoenix::s32> stateIndexList;

public:
	Enemy() {}
	~Enemy()
	{
		//Finalize();
	}

public:
	// ����
	static std::shared_ptr<Enemy> Create();

	// �R���X�g���N�^
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);

	// ������
	void Initialize();

	// �I����
	void Finalize();

	// �X�V
	void Update(bool onControl, Phoenix::f32 elapsedTime);

	// �g�����X�t�H�[���X�V
	void UpdateTrasform();

	// UI�X�V
	void UpdateUI(Phoenix::Math::Vector2 pos);

	// �`��
	void Draw();

	// GUI
	void GUI(Phoenix::s32 index);

	// �A�j���[�V�������ڍs
	void ChangeAnimation();

	// �U���X�e�[�g���ڍs
	void ChangeAttackAnimation();

	// �U������
	void AttackJudgment();

	// �v���C���[�Ƃ̋����v��
	void DistanceMeasurement();

	// �v���C���[�̍U������ɂ��邩����
	bool JudgePlayerAttackRange();

	// �V���ȉ�]�l�̍X�V
	void UpdateNewRotate();

public:
	// �L���t���O�ݒ�
	void SetEnable(bool enable);

	// ���݃t���O�ݒ�
	void SetAlive(bool alive);

	// ���S�t���O�ݒ�
	void SetDeath(bool death);

	// �퓬���t���O�ݒ�
	void SetInBattle(bool inBattle);

	// �U���q�b�g�ݒ�
	void SetIsHit(bool isHit) { this->isHit = isHit; }

	// �g�����X�t�H�[���̐ݒ�
	void SetTransform(Phoenix::FrameWork::Transform transform);

	// �g�����X���[�g�̐ݒ�
	void SetTranslate(Phoenix::Math::Vector3 translate);

	// �V���ȉ�]�l��ݒ�
	void SetNewRotate(Phoenix::Math::Quaternion newRotate);

	// �G�l�~�[�}�l�[�W���[�̐ݒ�
	void SetOwner(std::shared_ptr<EnemyManager> owner);

	// �X�e�[�g��ύX
	void SetState(BattleEnemyState state, bool forcedChange = false);

	// �U�����𔭍s
	bool SetAttackRight(bool stackAttackRight);

	// �U���X�e�[�g��ύX
	void SetAttackState(EnemyAttackState state);

	// �v���C���[��ݒ�
	void SetPlayer(std::shared_ptr<Player> player);

	// �ړ��X�s�[�h�ݒ�
	void SetMoveSpeed(Phoenix::f32 speed);

	// �ړ������̎w��
	void SetMoveInput(Phoenix::f32 moveX, Phoenix::f32 moveY);

	// �_���[�W
	void Damage(int damage);

public:
	// �L���t���O�擾
	bool GetEnable();

	// ���݃t���O�擾
	bool GetAlive();

	// ���S�t���O�擾
	bool GetDeath();

	// �퓬���t���O�擾
	bool GetInBattle();

	// �g�����X�t�H�[���̎擾
	Phoenix::FrameWork::Transform GetTransform();

	// �U���̈З͂̎擾
	Phoenix::u32 GetAttackPower() { return attackPower; }

	// �ړ��X�s�[�h�擾
	Phoenix::f32 GetMoveSpeed() { return moveSpeed; }

	// �v���C���[�Ƃ̋������擾
	Phoenix::f32 GetDistanceToPlayer() { return distanceToPlayer; }

	// �o�g�����[�h�̃X�e�[�g�擾
	BattleEnemyState GetBattleState();

	// �v���C���[�̐퓬�G���A�ɐN��������
	bool InBattleTerritory();

	// �v���C���[�ɍU���������鋗���ɓ����Ă��邩�H
	bool InDistanceHitByAttack();

	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }

	const std::vector<Phoenix::FrameWork::CollisionData>* GetCollisionDatas() { return &collisionDatas; }

	Phoenix::Math::Matrix GetWorldMatrix() { return transform->GetWorldTransform(); }

	Phoenix::Math::Vector3 GetPosition() { return transform->GetTranslate(); }

	Phoenix::f32 GetRadius() { return radius; }

	bool IsAttackJudgment() { return isAttackJudgment; }

	Phoenix::u32 GetAttackCollisionIndex() { return attackCollisionIndex; }

	std::shared_ptr<EnemyUI> GetUI() { return ui; }

	Phoenix::s32 GetLife() { return life; }

	bool UnderAttack() { return battleAI->GetCurrentStateName() == BattleEnemyState::Attack; }
};