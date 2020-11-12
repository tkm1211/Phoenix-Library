#pragma once


enum class EnemyMode
{
	Ordinary, // �ʏ�
	Battle,   // �o�g��
};

enum class BattleEnemyState
{
	NoneState, // �Ȃ�
	Idle,      // �ҋ@
	Attack,    // �U��
	Dedge,     // ���
	Guard,     // �K�[�h
};

enum class EnemyAttackState
{
	NoneState,

	WeakRight,
	WeakLeft,
	StrongRight,
	StrongLeft,
};