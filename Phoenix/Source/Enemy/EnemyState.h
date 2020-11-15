#pragma once


enum class EnemyMode
{
	Ordinary, // 通常
	Battle,   // バトル
};

enum class BattleEnemyState
{
	NoneState,   // なし
	Idle,        // 待機
	Walk,        // 歩き
	Run,         // 走り
	Attack,      // 攻撃
	Dedge,       // 回避
	DamageSmall, // 小ダメージ
	DamageBig,   // 大ダメージ
	Guard,       // ガード
	Death,       // 死亡
};

enum class EnemyAttackState
{
	NoneState,

	WeakRight,
	WeakLeft,
	StrongRight,
	StrongLeft,
};