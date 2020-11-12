#pragma once


enum class EnemyMode
{
	Ordinary, // 通常
	Battle,   // バトル
};

enum class BattleEnemyState
{
	NoneState, // なし
	Idle,      // 待機
	Attack,    // 攻撃
	Dedge,     // 回避
	Guard,     // ガード
};

enum class EnemyAttackState
{
	NoneState,

	WeakRight,
	WeakLeft,
	StrongRight,
	StrongLeft,
};