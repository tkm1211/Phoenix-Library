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
	AddState(AI::BattleEnemy::Idle::Create());
	AddState(AI::BattleEnemy::Walk::Create(owner));
	AddState(AI::BattleEnemy::Run::Create(owner));
	AddState(AI::BattleEnemy::Attack::Create(owner));
	AddState(AI::BattleEnemy::Dedge::Create(owner));
	AddState(AI::BattleEnemy::DamageSmall::Create(owner));
	AddState(AI::BattleEnemy::DamageBig::Create(owner));
	AddState(AI::BattleEnemy::Guard::Create());
	AddState(AI::BattleEnemy::Death::Create());
}

// �X�e�[�g�}�V���̃N���[���A�b�v
void BattleEnemyAI::CleanUp()
{
	Super::CleanUp();
	owner.reset();
}

// �X�V
BattleEnemyState BattleEnemyAI::Update()
{
	if (currentState == nullptr) return BattleEnemyState::NoneState;

	BattleEnemyState nextState = currentState->Update();
	if (nextState != BattleEnemyState::NoneState)
	{
		GoToState(nextState);
		return nextState;
	}

	return BattleEnemyState::NoneState;
}

// �G�l�~�[�ݒ�
void BattleEnemyAI::SetOwner(std::shared_ptr<Enemy> owner)
{
	this->owner = owner;
}