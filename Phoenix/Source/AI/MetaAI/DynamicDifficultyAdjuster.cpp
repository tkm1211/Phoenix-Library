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
	notUpdatedTime = 0.0f;
}

// 終了化
void DynamicDifficultyAdjuster::Finalize()
{
	ideaState = BattleEnemyState::NoneState;
}

// 更新
void DynamicDifficultyAdjuster::Update(Phoenix::s32 skillLevel, bool downScore, Phoenix::f32 elapsedTime)
{
	if (skillLevel <= 0)
	{
		if (downScore)
		{
			ideaState = BattleEnemyState::Attack;
		}
		if (100.0f <= notUpdatedTime)
		{
			//ideaState = BattleEnemyState::Run;
			ideaState = BattleEnemyState::Attack;
			notUpdatedTime = 0.0f;
		}
		else
		{
			notUpdatedTime += 1.0f * elapsedTime;
		}
	}
	else if (10 < skillLevel)
	{
		ideaState = BattleEnemyState::Attack;
	}
	else if (0 < skillLevel && skillLevel <= 10)
	{
		Phoenix::s32 percent = rand() % 100;
		if (percent < 95)
		{
			ideaState = BattleEnemyState::Idle;
		}
		else if (percent < 100)
		{
			ideaState = BattleEnemyState::Dedge;
		}
	}
}

// エネミーステートの取得
BattleEnemyState DynamicDifficultyAdjuster::GetBattleEnemyState()
{
	return ideaState;
}