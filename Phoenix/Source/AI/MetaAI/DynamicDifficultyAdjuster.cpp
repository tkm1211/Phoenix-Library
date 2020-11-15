#include "DynamicDifficultyAdjuster.h"


// 生成
std::unique_ptr<DynamicDifficultyAdjuster> DynamicDifficultyAdjuster::Create()
{
	return std::make_unique<DynamicDifficultyAdjuster>();
}

// コンストラクタ
void DynamicDifficultyAdjuster::Construct()
{
	
}

// 初期化
void DynamicDifficultyAdjuster::Initialize()
{
	ideaState = BattleEnemyState::NoneState;
}

// 終了化
void DynamicDifficultyAdjuster::Finalize()
{
	ideaState = BattleEnemyState::NoneState;
}

// 更新
void DynamicDifficultyAdjuster::Update(Phoenix::s32 skillLevel)
{
	if (0 < skillLevel && skillLevel <= 10)
	{
		Phoenix::s32 percent = rand() % 100;
		if (percent < 65)
		{
			ideaState = BattleEnemyState::Idle;
		}
		else if (percent < 100)
		{
			ideaState = BattleEnemyState::Dedge;
		}
	}
	else if (skillLevel < 0)
	{
		ideaState = BattleEnemyState::Run;
	}
	else
	{
		ideaState = BattleEnemyState::Attack;
	}
}

// エネミーステートの取得
BattleEnemyState DynamicDifficultyAdjuster::GetBattleEnemyState()
{
	return ideaState;
}