#pragma once

#include "Phoenix/Math/PhoenixMath.h"
#include "../../Enemy/EnemyState.h"


class DynamicDifficultyAdjuster
{
private:
	BattleEnemyState ideaState = BattleEnemyState::NoneState;
	Phoenix::f32 notUpdatedTime = 0.0f;

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
	/// <param name="downScore"> : プレイヤー行動の合計スコアが下がっているか </param>
	/// <param name="elapsedTime"> : 経過時間 </param>
	void Update(Phoenix::s32 skillLevel, bool downScore, Phoenix::f32 elapsedTime);

	// エネミーステートの取得
	BattleEnemyState GetBattleEnemyState();
};