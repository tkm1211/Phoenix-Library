#include "BattleEnemyAI.h"
#include "BattleEnemyState.h"
#include "../../Enemy/Enemy.h"


// 生成
std::shared_ptr<BattleEnemyAI> BattleEnemyAI::Create()
{
	return std::make_shared<BattleEnemyAI>();
}

// ステートマシンのセットアップ
void BattleEnemyAI::SetUp()
{
	AddState(AI::BattleEnemy::Idle::Create());
	AddState(AI::BattleEnemy::Attack::Create(owner));
	AddState(AI::BattleEnemy::Dedge::Create());
	AddState(AI::BattleEnemy::Guard::Create());
}

// ステートマシンのクリーンアップ
void BattleEnemyAI::CleanUp()
{
	Super::CleanUp();
	owner.reset();
}

// 更新
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

// エネミー設定
void BattleEnemyAI::SetOwner(std::shared_ptr<Enemy> owner)
{
	this->owner = owner;
}