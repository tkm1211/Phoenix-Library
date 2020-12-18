#pragma once

#include "Phoenix/Math/PhoenixMath.h"


class PlayerSkillAnalyzer
{
private:
	Phoenix::s32 playerSkillLevel = 1;
	Phoenix::s32 playerSkillLevelRange = 100;
	Phoenix::s32 levelUpExperiencePoint = 10;

	Phoenix::s32 oldPlayerSkillLevel = 1;
	Phoenix::s32 oldPlayerScore = 0;

	bool downScore = false;

public:
	PlayerSkillAnalyzer() {}
	~PlayerSkillAnalyzer() {}

public:
	// 生成
	static std::unique_ptr<PlayerSkillAnalyzer> Create();

	// コンストラクタ
	void Construct();

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="score"> : プレイヤー行動の合計スコア </param>
	/// <param name="elapsedTime"> : 経過時間 </param>
	void Update(Phoenix::s32 score, Phoenix::f32 elapsedTime);

	// 分析したプレイヤーレベルの取得
	Phoenix::s32 GetSkillLevel();

	// 分析したプレイヤーレベルが下がっているか取得
	bool GetDownScore();
};