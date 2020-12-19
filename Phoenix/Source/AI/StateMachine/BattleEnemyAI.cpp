#include "BattleEnemyAI.h"
#include "BattleEnemyState.h"
#include "../../Enemy/Enemy.h"


// ����
std::shared_ptr<BattleEnemyAI> BattleEnemyAI::Create()
{
	return std::make_shared<BattleEnemyAI>();
}

// �X�e�[�g�}�V���̃Z�b�g�A�b�v
void BattleEnemyAI::SetUp()
{
	
}

// �X�e�[�g�}�V���̃N���[���A�b�v
void BattleEnemyAI::CleanUp()
{
	Super::CleanUp();
	owner.reset();
}

// �X�V
BattleEnemyState BattleEnemyAI::Update(Phoenix::f32 elapsedTime)
{
	if (currentState == nullptr) return BattleEnemyState::NoneState;

	BattleEnemyState nextState = currentState->Update(elapsedTime);
	if (nextState != BattleEnemyState::NoneState)
	{
		owner->SetState(nextState);
		return nextState;
	}

	return BattleEnemyState::NoneState;
}

// �G�l�~�[�ݒ�
void BattleEnemyAI::SetOwner(std::shared_ptr<Enemy> owner)
{
	this->owner = owner;
}