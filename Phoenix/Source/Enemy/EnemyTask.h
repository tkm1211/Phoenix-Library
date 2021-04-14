#pragma once

#include <vector>
#include "EnemyState.h"


// HTN用Enemy状態データ
struct EnemyWorldState
{
	BattleEnemyState enemyState = BattleEnemyState::NoneState; // 現在のステート状況
	bool playing = false; // アニメーションが再生中か

	bool inTerritory = false; // 索敵範囲内に敵(プレイヤー)が入っているか
	bool inDistanceHit = false; // 攻撃がヒットする範囲内に敵(プレイヤー)が入っているか
	bool hasAttackRight = false; // 攻撃権を持っているか
	bool playerAttacking = false; // プレイヤーが攻撃中か
};

namespace EnemyTask
{
	namespace Primitive
	{
		// 待機する
		bool Idle(EnemyWorldState* state);

		// 敵(プレイヤー)まで歩く
		bool Walk(EnemyWorldState* state);

		// 敵(プレイヤー)まで走る
		bool Run(EnemyWorldState* state);

		// 攻撃を回避
		bool Dedge(EnemyWorldState* state);

		// 敵(プレイヤー)を攻撃
		bool Attack(EnemyWorldState* state);
	}

	namespace Compound
	{
		// 敵(プレイヤー)まで歩く
		bool Walk(EnemyWorldState* state, std::vector<BattleEnemyState>* name);

		// 敵(プレイヤー)まで走る
		bool Run(EnemyWorldState* state, std::vector<BattleEnemyState>* name);

		// 攻撃を回避
		bool Dedge(EnemyWorldState* state, std::vector<BattleEnemyState>* name);

		// 敵(プレイヤー)を攻撃
		bool Attack(EnemyWorldState* state, std::vector<BattleEnemyState>* name);
	}

	/*namespace SetUp
	{
		// 待機する
		void Idle(std::shared_ptr<Enemy> owner);

		// 敵(プレイヤー)まで歩く
		void Walk(std::shared_ptr<Enemy> owner);

		// 敵(プレイヤー)まで走る
		void Run(std::shared_ptr<Enemy> owner);

		// 攻撃を回避
		void Dedge(std::shared_ptr<Enemy> owner);

		// 敵(プレイヤー)を攻撃
		void Attack(std::shared_ptr<Enemy> owner);
	}

	namespace Action
	{
		// 待機する
		bool Idle(std::shared_ptr<Enemy> owner);

		// 敵(プレイヤー)まで歩く
		bool Walk(std::shared_ptr<Enemy> owner);

		// 敵(プレイヤー)まで走る
		bool Run(std::shared_ptr<Enemy> owner);

		// 攻撃を回避
		bool Dedge(std::shared_ptr<Enemy> owner);

		// 敵(プレイヤー)を攻撃
		bool Attack(std::shared_ptr<Enemy> owner);
	}*/
}