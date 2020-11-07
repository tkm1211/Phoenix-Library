#pragma once

#include "StateMachine.h"
#include "../../Enemy/EnemyState.h"


class Enemy;
class BattleEnemyAI : public StateMachine<BattleEnenyState>, public AIBase
{
private:
	using Super = StateMachine<BattleEnenyState>;

private:
	std::weak_ptr<Enemy> owner;

public:
	BattleEnemyAI() {}
	~BattleEnemyAI() {}

public:
	// ����
	static std::shared_ptr<BattleEnemyAI> Create();

	// �X�e�[�g�}�V���̃Z�b�g�A�b�v
	void SetUp() override;

	// �X�e�[�g�}�V���̃N���[���A�b�v
	void CleanUp() override;

	// �X�V
	void Update() override;

public:
	// �G�l�~�[�ݒ�
	void SetOwner(std::shared_ptr<Enemy> owner);
};