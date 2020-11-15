#pragma once


enum class EnemyMode
{
	Ordinary, // �ʏ�
	Battle,   // �o�g��
};

enum class BattleEnemyState
{
	NoneState,   // �Ȃ�
	Idle,        // �ҋ@
	Walk,        // ����
	Run,         // ����
	Attack,      // �U��
	Dedge,       // ���
	DamageSmall, // ���_���[�W
	DamageBig,   // ��_���[�W
	Guard,       // �K�[�h
	Death,       // ���S
};

enum class EnemyAttackState
{
	NoneState,

	WeakRight,
	WeakLeft,
	StrongRight,
	StrongLeft,
};