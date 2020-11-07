#pragma once

#include "Phoenix/Math/PhoenixMath.h"
#include "../../Enemy/EnemyState.h"


class DynamicDifficultyAdjuster
{
private:
	BattleEnenyState ideaState = BattleEnenyState::NoneState;

public:
	DynamicDifficultyAdjuster() {}
	~DynamicDifficultyAdjuster() {}

public:
	// 生成
	static std::unique_ptr<DynamicDifficultyAdjuster> Create();

	// コンストラクタ
	void Construct();

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="skillLevel"> : プレイヤー行動の合計スコアから分析したプレイヤーレベル </param>
	void Update(Phoenix::s32 skillLevel);

	// エネミーステートの取得
	BattleEnenyState GetBattleEnenyState();
};