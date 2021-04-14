#pragma once

#include <vector>
#include "EnemyState.h"


// HTN�pEnemy��ԃf�[�^
struct EnemyWorldState
{
	BattleEnemyState enemyState = BattleEnemyState::NoneState; // ���݂̃X�e�[�g��
	bool playing = false; // �A�j���[�V�������Đ�����

	bool inTerritory = false; // ���G�͈͓��ɓG(�v���C���[)�������Ă��邩
	bool inDistanceHit = false; // �U�����q�b�g����͈͓��ɓG(�v���C���[)�������Ă��邩
	bool hasAttackRight = false; // �U�����������Ă��邩
	bool playerAttacking = false; // �v���C���[���U������
};

namespace EnemyTask
{
	namespace Primitive
	{
		// �ҋ@����
		bool Idle(EnemyWorldState* state);

		// �G(�v���C���[)�܂ŕ���
		bool Walk(EnemyWorldState* state);

		// �G(�v���C���[)�܂ő���
		bool Run(EnemyWorldState* state);

		// �U�������
		bool Dedge(EnemyWorldState* state);

		// �G(�v���C���[)���U��
		bool Attack(EnemyWorldState* state);
	}

	namespace Compound
	{
		// �G(�v���C���[)�܂ŕ���
		bool Walk(EnemyWorldState* state, std::vector<BattleEnemyState>* name);

		// �G(�v���C���[)�܂ő���
		bool Run(EnemyWorldState* state, std::vector<BattleEnemyState>* name);

		// �U�������
		bool Dedge(EnemyWorldState* state, std::vector<BattleEnemyState>* name);

		// �G(�v���C���[)���U��
		bool Attack(EnemyWorldState* state, std::vector<BattleEnemyState>* name);
	}

	/*namespace SetUp
	{
		// �ҋ@����
		void Idle(std::shared_ptr<Enemy> owner);

		// �G(�v���C���[)�܂ŕ���
		void Walk(std::shared_ptr<Enemy> owner);

		// �G(�v���C���[)�܂ő���
		void Run(std::shared_ptr<Enemy> owner);

		// �U�������
		void Dedge(std::shared_ptr<Enemy> owner);

		// �G(�v���C���[)���U��
		void Attack(std::shared_ptr<Enemy> owner);
	}

	namespace Action
	{
		// �ҋ@����
		bool Idle(std::shared_ptr<Enemy> owner);

		// �G(�v���C���[)�܂ŕ���
		bool Walk(std::shared_ptr<Enemy> owner);

		// �G(�v���C���[)�܂ő���
		bool Run(std::shared_ptr<Enemy> owner);

		// �U�������
		bool Dedge(std::shared_ptr<Enemy> owner);

		// �G(�v���C���[)���U��
		bool Attack(std::shared_ptr<Enemy> owner);
	}*/
}