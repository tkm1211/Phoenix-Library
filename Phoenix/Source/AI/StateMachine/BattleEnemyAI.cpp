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
	
}

// ステートマシンのクリーンアップ
void BattleEnemyAI::CleanUp()
{
	Super::CleanUp();
	owner.reset();
}

// 更新
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

// エネミー設定
void BattleEnemyAI::SetOwner(std::shared_ptr<Enemy> owner)
{
	this->owner = owner;
}