#pragma once

#include <memory>
#include "Phoenix/Math/PhoenixMath.h"
#include "../AI/HTN/HTN.h"
#include "EnemyState.h"
#include "EnemyTask.h"
#include "../AI/StateMachine/BattleEnemyAI.h"
#include "../AI/StateMachine/BattleEnemyState.h"
#include "Phoenix/FrameWork/StateMachine/StateMachine.h"


class Enemy;
class EnemyAIController
{
private:
	// ステート管理
	std::shared_ptr<FSM<BattleEnemyState, std::shared_ptr<Enemy>, std::shared_ptr<BattleEnemy::AIData>>> status;

	// ステートプランナー
	std::shared_ptr<HTN<BattleEnemyState, EnemyWorldState, std::shared_ptr<Enemy>>> htn;

	// HTN用ワールドステート
	std::shared_ptr<EnemyWorldState> worldState;

	// オーナー
	std::weak_ptr<Enemy> owner;

	// AI用データ
	std::shared_ptr<BattleEnemy::AIData> aiData;

	// 次に実行してほしいステート
	BattleEnemyState nextState;

public:
	EnemyAIController() {}
	~EnemyAIController() {}

public:
	// 生成
	static std::shared_ptr<EnemyAIController> Create();

	// コンストラクタ
	void Construct(std::weak_ptr<Enemy> owner);

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	// 更新
	void Update(Phoenix::f32 elapsedTime);

	// ステート遷移
	void ChangeState(Phoenix::f32 elapsedTime);

	// HTNのワールドステートを設定
	void SetWorldState(std::shared_ptr<EnemyWorldState> worldState);

	// 次に実行したいステートを設定
	void SetNextState(BattleEnemyState nextState);

	// 現在のステートを取得
	BattleEnemyState GetState();
};