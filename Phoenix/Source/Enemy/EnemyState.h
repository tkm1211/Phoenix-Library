#pragma once


// �G�l�~�[�S�̂̏��
enum class EnemyMode
{
	Ordinary, // �ʏ�
	Battle,   // �o�g��
};

// �퓬�G�l�~�[�̏��
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
	KnockBack,   // �̂�����
	GettingUp,   // �����オ��
	Guard,       // �K�[�h
	Death,       // ���S
};

// �G�l�~�[�i�G���G�j�̍U�����
enum class EnemyAttackState
{
	NoneState,

	WeakRight,
	WeakLeft,
	StrongRight,
	StrongLeft,
};

// �G�l�~�[�i�{�X�j�̍U�����
enum class BossAttackState
{
	NoneState,

	RightHook,
	LeftHook,
	LeftTurn,
};