#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Component/Transform.h"
#include "Phoenix/FrameWork/SharedFromThis/SharedFromThis.h"
#include "EnemyState.h"
#include "../AI/StateMachine/BattleEnemyAI.h"
#include "../AI/StateMachine/BattleEnemyState.h"
#include "../UI/EnemyUI.h"


class EnemyManager;
class Player;
class Enemy : public inheritable_enable_shared_from_this<Enemy>
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

	template <class T>
	struct AttackData
	{
		T animState; // �A�j���[�V�����X�e�[�g
		Phoenix::s32 animIndex; // �A�j���[�V�������C���[�̔ԍ�

		Phoenix::f32 playSpeed; // �Đ��X�s�[�h
		Phoenix::f32 playBeginTime; // �Đ��J�n���ԁi�Đ��t���[���j
		Phoenix::f32 playEndTime; // �Đ��I�����ԁi�Đ��t���[���j

		Phoenix::s32 collisionNum; // �����蔻��̔ԍ�
		Phoenix::f32 collisionBeginTime; // �����蔻��J�n���ԁi�Đ��t���[���j
		Phoenix::f32 collisionEndTime; // �����蔻��I�����ԁi�Đ��t���[���j
	};

	template <class T>
	struct AttackDatas
	{
		std::vector<AttackData<T>> datas;

		void AddData(AttackData<T> data)
		{
			datas.emplace_back(data);
		}
	};

protected:
	// ���f��
	std::shared_ptr<Phoenix::FrameWork::ModelObject> model;

	// �g�����X�t�H�[��
	std::unique_ptr<Phoenix::FrameWork::Transform> transform;

	// �����蔻��f�[�^
	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;

	// HP�Q�[�WUI
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
	Phoenix::f32 playBeginTime = 0.0f;
	Phoenix::f32 playEndTime = 0.0f;

	// ��ރ^�O
	TypeTag typeTag = TypeTag::Small;

	// �p�����[�^�[
	Phoenix::s32 life = 0;
	Phoenix::s32 lifeMax = 0;
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

private:
	// �U���X�e�[�g
	std::shared_ptr<BattleEnemy::Attack<EnemyAttackState, Enemy>> attackState;
	EnemyAttackState changeAttackState = EnemyAttackState::NoneState;
	EnemyAttackState currentAttackState = EnemyAttackState::NoneState;

	// �U���f�[�^���X�g
	std::vector<AttackDatas<EnemyAttackState>> attackDatasList;

public:
	Enemy() {}
	virtual ~Enemy() {}

public:
	// ����
	static std::shared_ptr<Enemy> Create();

	// �R���X�g���N�^
	virtual void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);

	// ������
	virtual void Initialize();

	// �I����
	virtual void Finalize();

	// �X�V
	virtual void Update(bool onControl, Phoenix::f32 elapsedTime);

	// �g�����X�t�H�[���X�V
	virtual void UpdateTransform();

	// ���W�X�V
	virtual void UpdateTranslate(Phoenix::f32 elapsedTime);

	// ��]�X�V
	virtual void UpdateRotate(Phoenix::f32 elapsedTime);

	// �A�j���[�V�����X�V
	virtual void UpdateAnimation(Phoenix::f32 elapsedTime);

	// AI�X�V
	virtual void UpdateAI(Phoenix::f32 elapsedTime);

	// �����蔻��X�V
	virtual void UpdateCollision();

	// UI�X�V
	virtual void UpdateUI(Phoenix::Math::Vector2 pos);

	// �`��
	virtual void Draw();

	// GUI
	virtual void GUI(Phoenix::s32 index);

	// �A�j���[�V�������ڍs
	virtual void ChangeAnimation();

	// �U���X�e�[�g���ڍs
	virtual void ChangeAttackAnimation();

	// �U������
	virtual void AttackJudgment();

	// �v���C���[�Ƃ̋����v��
	virtual void DistanceMeasurement();

	// �v���C���[�̍U������ɂ��邩����
	virtual bool JudgePlayerAttackRange();

	// �V���ȉ�]�l�̍X�V
	virtual void UpdateNewRotate();

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

	// ���f�����擾
	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }

	// �����蔻��f�[�^���擾
	const std::vector<Phoenix::FrameWork::CollisionData>* GetCollisionDatas() { return &collisionDatas; }

	// ���[���h�s����擾
	Phoenix::Math::Matrix GetWorldMatrix() { return transform->GetWorldTransform(); }

	// ���W���擾
	Phoenix::Math::Vector3 GetPosition() { return transform->GetTranslate(); }

	// ���a���擾
	Phoenix::f32 GetRadius() { return radius; }

	// �U�����蒆
	bool IsAttackJudgment() { return isAttackJudgment; }

	// �U���̓����蔻��ԍ����擾
	Phoenix::u32 GetAttackCollisionIndex() { return attackCollisionIndex; }

	// HP�Q�[�WUI���擾
	std::shared_ptr<EnemyUI> GetUI() { return ui; }

	// HP���擾
	Phoenix::s32 GetLife() { return life; }

	// �ő�HP���擾
	Phoenix::s32 GetLifeMax() { return lifeMax; }

	// ��ރ^�O
	TypeTag GetTypeTag() { return typeTag; }

	// �U�����̔���
	bool UnderAttack() { return battleAI->GetCurrentStateName() == BattleEnemyState::Attack; }
};