#pragma once

#include "MetaAISystem.h"
#include "PlayerSkillAnalyzer.h"
#include "DynamicDifficultyAdjuster.h"
#include "BattleEnemyController.h"
#include "../../Enemy/EnemyManager.h"


namespace Meta
{
	class BattleEnemySystem : public IMetaAISystem
	{
	private:
		std::unique_ptr<PlayerSkillAnalyzer> playerSkillAnalyzer;
		std::unique_ptr<DynamicDifficultyAdjuster> dynamicDifficultyAdjuster;
		std::unique_ptr<BattleEnemyController> battleEnemyController;

		bool constructed = false;

	public:
		BattleEnemySystem() {}
		~BattleEnemySystem() override {}

	public:
		// 生成
		static std::shared_ptr<BattleEnemySystem> Create();

		// コンストラクタ
		void Construct() override;

		// 初期化
		void Initialize() override;

		// 終了化
		void Finalize() override;

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="score"> : プレイヤー行動の合計スコア </param>
		/// <param name="elapsedTime"> : 経過時間 </param>
		void Update(Phoenix::s32 score, Phoenix::f32 elapsedTime) override;

		/// <summary>
		/// 管理クラスの設定
		/// </summary>
		/// <param name="manager"> : 管理クラスの実体 </param>
		void SetManager(std::shared_ptr<EnemyManager> manager);
	};
}