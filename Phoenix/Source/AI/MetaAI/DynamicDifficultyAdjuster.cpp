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
	ideaState = EnenyBattleState::NoneState;
}

// 終了化
void DynamicDifficultyAdjuster::Finalize()
{
	ideaState = EnenyBattleState::NoneState;
}

// 更新
void DynamicDifficultyAdjuster::Update(Phoenix::s32 skillLevel)
{
	if (skillLevel <= 10)
	{
		ideaState = EnenyBattleState::Idle;
	}
	else if (skillLevel <= 50)
	{
		ideaState = EnenyBattleState::Attack;
	}
}

// エネミーステートの取得
EnenyBattleState DynamicDifficultyAdjuster::GetEnenyBattleState()
{
	return ideaState;
}