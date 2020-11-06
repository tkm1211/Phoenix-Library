#include "PlayerSkillAnalyzer.h"


// 生成
std::unique_ptr<PlayerSkillAnalyzer> PlayerSkillAnalyzer::Create()
{
	return std::make_unique<PlayerSkillAnalyzer>();
}

// コンストラクタ
void PlayerSkillAnalyzer::Construct()
{

}

// 初期化
void PlayerSkillAnalyzer::Initialize()
{
	playerSkillLevel = 1;
}

// 終了化
void PlayerSkillAnalyzer::Finalize()
{
	playerSkillLevel = 1;
}

// 更新
void PlayerSkillAnalyzer::Update(Phoenix::s32 score)
{
	playerSkillLevel = score / levelUpExperiencePoint;

	if (playerSkillLevelRange <= playerSkillLevel)
	{
		playerSkillLevel = playerSkillLevelRange;
	}
}

// 分析したプレイヤーレベルの取得
Phoenix::s32 PlayerSkillAnalyzer::GetSkillLevel()
{
	return playerSkillLevel;
}