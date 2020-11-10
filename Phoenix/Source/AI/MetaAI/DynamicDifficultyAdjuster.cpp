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
	if (skillLevel <= 10)
	{
		ideaState = BattleEnemyState::Attack;
	}
	/*else if (skillLevel <= 30)
	{
		
	}*/
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