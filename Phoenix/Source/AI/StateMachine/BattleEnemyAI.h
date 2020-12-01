#pragma once

#include "Phoenix/FrameWork/StateMachine/StateMachine.h"
#include "../../Enemy/EnemyState.h"


class Enemy;
class BattleEnemyAI : public StateMachine<BattleEnemyState>
{
private:
	using Super = StateMachine<BattleEnemyState>;

private:
	std::shared_ptr<Enemy> owner;

public:
	BattleEnemyAI() {}
	~BattleEnemyAI() {}

public:
	// 生成
	static std::shared_ptr<BattleEnemyAI> Create();

	// ステートマシンのセットアップ
	void SetUp() override;

	// ステートマシンのクリーンアップ
	void CleanUp() override;

	// 更新
	BattleEnemyState Update() override;

public:
	// エネミー設定
	void SetOwner(std::shared_ptr<Enemy> owner);
};