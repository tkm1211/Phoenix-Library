#pragma once

#include "MetaAISystem.h"
#include "PlayerSkillAnalyzer.h"
#include "DynamicDifficultyAdjuster.h"
#include "BattleEnemyController.h"
#include "../../Enemy/EnemyManager.h"
#include "../../Player/Player.h"
#include "../../Enemy/EnemyState.h"


namespace Meta
{
	/*
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
	*/

	// ゲームシーンから必要最低限のデータ
	struct MetaData
	{
		Phoenix::Math::Vector3 playerPos;
		Phoenix::s32 playerState = -1;
		Phoenix::s32 playerAttackState = -1;
		bool playerIsInvincible = false;

		std::vector<Phoenix::Math::Vector3> enemiesPos;
		std::vector<Phoenix::s32> enemiesState;
		std::vector<Phoenix::s32> enemiesType;
	};

	class BattleEnemySystem : public IMetaAISystem
	{
	private:
		// ゲームシーンからのデータ
		MetaData metaData;

		// エネミー管理
		std::shared_ptr<EnemyManager> enemyManager;

		Phoenix::s32 prevPlayerAttackState = -1; // 前回のプレイヤー攻撃状態

		std::vector<bool> canDedge; // 各エネミーが回避判定
		std::vector<bool> attackWait; // 攻撃待機中
		std::vector<Phoenix::f32> attackWaitCnt; // 次の攻撃権が発行されるまでの待機カウント
		std::vector<Phoenix::f32> attackWaitMaxCnt; // 次の攻撃権が発行されるまでの待機最大カウント

	public:
		BattleEnemySystem() {}
		~BattleEnemySystem() {}

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
		/// <param name="elapsedTime"> : 経過時間 </param>
		void Update(Phoenix::f32 elapsedTime) override;

		/// <summary>
		/// ゲームからメタAIを起動する関数
		/// </summary>
		/// <param name="metaData"> : ゲームからメタAIで必要なデータ </param>
		void Sensor(MetaData metaData);

		/// <summary>
		/// 管理クラスの設定
		/// </summary>
		/// <param name="manager"> : 管理クラスの実体 </param>
		void SetManager(std::shared_ptr<EnemyManager> manager);

		// プレイヤーとエネミーの距離
		Phoenix::f32 DistancePlayerAndEnemy(Phoenix::Math::Vector3 playerPos, Phoenix::Math::Vector3 enemyPos);

		// プレイヤーの戦闘エリアに侵入したか
		bool InBattleTerritory(Phoenix::f32 distance);

		// プレイヤーに攻撃が当たる距離に入っているか？
		bool InDistanceHitByAttack(Phoenix::f32 distance, Phoenix::s32 enemyType);

		// エネミーの状態からおすすめのエネミーのステートを返す
		BattleEnemyState RecommendEnemyStateByEnemy(BattleEnemyState currentEnemyState, Phoenix::s32 enemyType, bool inBattleTerritory, bool inDistanceHitByAttack);

		// プレイヤーの状態からおすすめのエネミーのステートを返す
		BattleEnemyState RecommendEnemyStateByPlayer(Player::AnimationState currentPlayerState, BattleEnemyState currentEnemyState, Phoenix::s32 enemyType, bool& canDedge, bool inBattleTerritory, bool inDistanceHitByAttack);

		// 新たなエネミー達の状態を所得
		std::vector<Phoenix::s32>& GetNewEnemiesState();
	};
}