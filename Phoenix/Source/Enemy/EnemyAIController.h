#pragma once

#include <memory>
#include "Phoenix/Math/PhoenixMath.h"
#include "../AI/HTN/HTN.h"
#include "EnemyState.h"
#include "EnemyTask.h"
#include "../AI/StateMachine/BattleEnemyAI.h"
#include "../AI/StateMachine/BattleEnemyState.h"
#include "Phoenix/FrameWork/StateMachine/StateMachine.h"


class Enemy;
class EnemyAIController
{
private:
	// �X�e�[�g�Ǘ�
	std::shared_ptr<FSM<BattleEnemyState, std::shared_ptr<Enemy>, std::shared_ptr<BattleEnemy::AIData>>> status;

	// �X�e�[�g�v�����i�[
	std::shared_ptr<HTN<BattleEnemyState, EnemyWorldState, std::shared_ptr<Enemy>>> htn;

	// HTN�p���[���h�X�e�[�g
	std::shared_ptr<EnemyWorldState> worldState;

	// �I�[�i�[
	std::weak_ptr<Enemy> owner;

	// AI�p�f�[�^
	std::shared_ptr<BattleEnemy::AIData> aiData;

	// ���Ɏ��s���Ăق����X�e�[�g
	BattleEnemyState nextState;

public:
	EnemyAIController() {}
	~EnemyAIController() {}

public:
	// ����
	static std::shared_ptr<EnemyAIController> Create();

	// �R���X�g���N�^
	void Construct(std::weak_ptr<Enemy> owner);

	// ������
	void Initialize();

	// �I����
	void Finalize();

	// �X�V
	void Update(Phoenix::f32 elapsedTime);

	// �X�e�[�g�J��
	void ChangeState(Phoenix::f32 elapsedTime);

	// HTN�̃��[���h�X�e�[�g��ݒ�
	void SetWorldState(std::shared_ptr<EnemyWorldState> worldState);

	// ���Ɏ��s�������X�e�[�g��ݒ�
	void SetNextState(BattleEnemyState nextState);

	// ���݂̃X�e�[�g���擾
	BattleEnemyState GetState();
};