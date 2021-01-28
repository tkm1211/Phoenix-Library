#pragma once


// エネミー全体の状態
enum class EnemyMode
{
	Ordinary, // 通常
	Battle,   // バトル
};

// 戦闘エネミーの状態
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
	KnockBack,   // のけぞり
	GettingUp,   // 立ち上がり
	Guard,       // ガード
	Death,       // 死亡
};

// エネミー（雑魚敵）の攻撃状態
enum class EnemyAttackState
{
	NoneState,

	WeakRight,
	WeakLeft,
	StrongRight,
	StrongLeft,
};

// エネミー（ボス）の攻撃状態
enum class BossAttackState
{
	NoneState,

	RightHook,
	LeftHook,
	LeftTurn,
};