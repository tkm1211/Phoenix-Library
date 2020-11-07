#pragma once

#include <memory>
#include "DynamicDifficultyAdjuster.h"


class EnemyManager;
class BattleEnemyController
{
private:
	std::shared_ptr<EnemyManager> enemyManager;

public:
	BattleEnemyController() {}
	~BattleEnemyController() {}

public:
	// 生成
	static std::unique_ptr<BattleEnemyController> Create();

	// コンストラクタ
	void Construct();

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="battleEnenyState"> : 分析したプレイヤーレベルから最適なエネミー行動 </param>
	void Update(BattleEnenyState battleEnenyState);

	// エネミーマネージャー設定
	void SetEnemyManager(std::shared_ptr<EnemyManager> enemyManager);
};