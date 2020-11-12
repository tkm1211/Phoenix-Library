#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Component/Transform.h"
#include "EnemyState.h"
#include "../AI/StateMachine/BattleEnemyAI.h"


class EnemyManager;
class Player;
class Enemy : public std::enable_shared_from_this<Enemy>
{
private:
	static constexpr Phoenix::s32 LifeRange = 50;

public:
	enum class TypeTag
	{
		Small,  // �G��
		Medium, // ���{�X
		Large   // �{�X
	};

private:
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;
	std::unique_ptr<Phoenix::FrameWork::Transform> transform;
	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;

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

	// �p�����[�^�[
	Phoenix::s32 life = 0;
	Phoenix::f32 radius = 0.0f;

	// �R���W�����f�[�^�̗v�f��
	Phoenix::u32 attackCollisionIndex = 0;

	// �t���O
	bool enable = false;
	bool alive = false;
	bool inBattle = false;

	// �A�^�b�N�������������H
	bool isHit = false;

	// �A�^�b�N�̔��蒆���H
	bool isAttackJudgment = false;

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
	void Update();

	// UI�X�V
	void UpdateUI();

	// �`��
	void Draw();

	// GUI
	void GUI(Phoenix::s32 index);

public:
	// �L���t���O�ݒ�
	void SetEnable(bool enable);

	// ���݃t���O�ݒ�
	void SetAlive(bool alive);

	// �퓬���t���O�ݒ�
	void SetInBattle(bool inBattle);

	// �U���q�b�g�ݒ�
	void SetIsHit(bool isHit) { this->isHit = isHit; }

	// �g�����X�t�H�[���̐ݒ�
	void SetTransform(Phoenix::FrameWork::Transform transform);

	// �g�����X���[�g�̐ݒ�
	void SetTranslate(Phoenix::Math::Vector3 translate);

	// �G�l�~�[�}�l�[�W���[�̐ݒ�
	void SetOwner(std::shared_ptr<EnemyManager> owner);

	// �U�����𔭍s
	void SetAttackRight();

	// �U���X�e�[�g��ύX
	void SetAttackState(EnemyAttackState state);

	// �v���C���[��ݒ�
	void SetPlayer(std::shared_ptr<Player> player);

	// �A�j���[�V�������ڍs
	void ChangeAnimation();

	// �U���X�e�[�g���ڍs
	void ChangeAttackAnimation();

	// �_���[�W
	void Damage(int damage);

public:
	// �L���t���O�擾
	bool GetEnable();

	// ���݃t���O�擾
	bool GetAlive();

	// �퓬���t���O�擾
	bool GetInBattle();

	// �g�����X�t�H�[���̎擾
	Phoenix::FrameWork::Transform GetTransform();

	// �o�g�����[�h�̃X�e�[�g�擾
	BattleEnemyState GetBattleState();


	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }

	const std::vector<Phoenix::FrameWork::CollisionData>* GetCollisionDatas() { return &collisionDatas; }

	Phoenix::Math::Matrix GetWorldMatrix() { return transform->GetWorldTransform(); }

	Phoenix::Math::Vector3 GetPosition() { return transform->GetTranslate(); }

	Phoenix::f32 GetRadius() { return radius; }

	bool IsAttackJudgment() { return isAttackJudgment; }

	Phoenix::u32 GetAttackCollisionIndex() { return attackCollisionIndex; }
};