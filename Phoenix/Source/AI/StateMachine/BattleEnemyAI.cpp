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
	AddState(AI::BattleEnemy::Attack::Create());
	AddState(AI::BattleEnemy::Dedge::Create());
}

// �X�e�[�g�}�V���̃N���[���A�b�v
void BattleEnemyAI::CleanUp()
{
	Super::CleanUp();
}

// �X�V
void BattleEnemyAI::Update()
{
	if (currentState == nullptr) return;

	BattleEnenyState nextState = currentState->Update();
	if (nextState != BattleEnenyState::NoneState)
	{
		GoToState(nextState);
	}
}

// �G�l�~�[�ݒ�
void BattleEnemyAI::SetOwner(std::shared_ptr<Enemy> owner)
{
	this->owner = owner;
}