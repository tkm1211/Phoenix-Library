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
	ideaState = BattleEnenyState::NoneState;
}

// 終了化
void DynamicDifficultyAdjuster::Finalize()
{
	ideaState = BattleEnenyState::NoneState;
}

// 更新
void DynamicDifficultyAdjuster::Update(Phoenix::s32 skillLevel)
{
	if (skillLevel <= 10)
	{
		ideaState = BattleEnenyState::Idle;
	}
	else if (skillLevel <= 50)
	{
		ideaState = BattleEnenyState::Attack;
	}
}

// エネミーステートの取得
BattleEnenyState DynamicDifficultyAdjuster::GetBattleEnenyState()
{
	return ideaState;
}