#pragma once

#include "StateMachine.h"
#include "../../Enemy/EnemyState.h"


class Enemy;
class BattleEnemyAI : public StateMachine<BattleEnenyState>, public AIBase
{
private:
	using Super = StateMachine<BattleEnenyState>;

private:
	std::weak_ptr<Enemy> owner;

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
	void Update() override;

public:
	// エネミー設定
	void SetOwner(std::shared_ptr<Enemy> owner);
};