#include "EnemyTask.h"


namespace EnemyTask
{
	namespace Primitive
	{
		// �ҋ@����
		bool Idle(EnemyWorldState* state)
		{
			if ((state->enemyState != BattleEnemyState::Idle) && !state->playing)
			{
				state->enemyState = BattleEnemyState::Idle;
				state->playing = true;

				return true;
			}

			return false;
		}

		// �G(�v���C���[)�܂ŕ���
		bool Walk(EnemyWorldState* state)
		{
			// ���G�͈͓��ɓG(�v���C���[)�����āA�U�����q�b�g����͈͓��ɓG(�v���C���[)�������Ă���Ƃ�
			if (state->inTerritory && !state->inDistanceHit)
			{
				state->inDistanceHit = true;
				state->enemyState = BattleEnemyState::Walk;

				return true;
			}

			return false;
		}

		// �G(�v���C���[)�܂ő���
		bool Run(EnemyWorldState* state)
		{
			// ���G�͈͓��ɓG(�v���C���[)�����Ȃ��Ƃ�
			if (!state->inTerritory)
			{
				state->inTerritory = true;
				state->enemyState = BattleEnemyState::Run;

				return true;
			}

			return false;
		}

		// �U�������
		bool Dedge(EnemyWorldState* state)
		{
			// ���G�͈͓��ɓG(�v���C���[)�����āA�G(�v���C���[)���U�����Ă���Ƃ�
			if (state->inTerritory && state->playerAttacking)
			{
				state->enemyState = BattleEnemyState::Dedge;

				return true;
			}

			return false;
		}

		// �G(�v���C���[)���U��
		bool Attack(EnemyWorldState* state)
		{
			// �U�����ł͂Ȃ��A�U�����������Ă���Ƃ�
			if ((state->enemyState != BattleEnemyState::Attack) && state->hasAttackRight)
			{
				state->enemyState = BattleEnemyState::Attack;
				state->hasAttackRight = false;

				return true;
			}

			return false;
		}
	}

	namespace Compound
	{
		// �G(�v���C���[)�܂ŕ���
		bool Walk(EnemyWorldState* state, std::vector<BattleEnemyState>* name)
		{
			if (state->inTerritory && !state->inDistanceHit) name->emplace_back(BattleEnemyState::Walk);

			return true;
		}

		// �G(�v���C���[)�܂ő���
		bool Run(EnemyWorldState* state, std::vector<BattleEnemyState>* name)
		{
			if (!state->inTerritory) name->emplace_back(BattleEnemyState::Run);

			return true;
		}

		// �U�������
		bool Dedge(EnemyWorldState* state, std::vector<BattleEnemyState>* name)
		{
			if (state->inTerritory && state->playerAttacking) name->emplace_back(BattleEnemyState::Dedge);

			return true;
		}

		// �G(�v���C���[)���U��
		bool Attack(EnemyWorldState* state, std::vector<BattleEnemyState>* name)
		{
			if (!state->inTerritory) name->emplace_back(BattleEnemyState::Run);
			if (state->inTerritory && !state->inDistanceHit) name->emplace_back(BattleEnemyState::Walk);
			if ((state->enemyState != BattleEnemyState::Attack) && state->hasAttackRight) name->emplace_back(BattleEnemyState::Attack);

			return true;
		}
	}

	/*namespace SetUp
	{
		// �ҋ@����
		void Idle(std::shared_ptr<Enemy> owner)
		{
			std::shared_ptr<Enemy::HTNDatas> data = owner->GetHTNDatas();
			data->idleCount = 0.0f;
		}

		// �G(�v���C���[)�܂ŕ���
		void Walk(std::shared_ptr<Enemy> owner)
		{
			std::shared_ptr<Enemy::HTNDatas> data = owner->GetHTNDatas();
			data->moveX = 0.0f;

			Phoenix::s32 judge = rand() % 2;
			if (judge)
			{
				data->moveX = -1.0f;
			}
			else
			{
				data->moveX = 1.0f;
			}
		}

		// �G(�v���C���[)�܂ő���
		void Run(std::shared_ptr<Enemy> owner)
		{
		}

		// �U�������
		void Dedge(std::shared_ptr<Enemy> owner)
		{
			owner->SetMoveSpeed(Enemy::DedgeSpeed);
		}

		// �G(�v���C���[)���U��
		void Attack(std::shared_ptr<Enemy> owner)
		{
			std::shared_ptr<Enemy::HTNDatas> data = owner->GetHTNDatas();
			data->attackIndex = 0;
		}
	}

	namespace Action
	{
		// �ҋ@����
		bool Idle(std::shared_ptr<Enemy> owner)
		{
			std::shared_ptr<Enemy::HTNDatas> data = owner->GetHTNDatas();
			if (Enemy::IdleMaxCount <= data->idleCount)
			{
				data->idleCount = 0.0f;
				return true;
			}
			else
			{
				data->idleCount += 1.0f * data->elapsedTime;
			}

			return false;
		}

		// �G(�v���C���[)�܂ŕ���
		bool Walk(std::shared_ptr<Enemy> owner)
		{
			if (owner->InDistanceHitByAttack())
			{
				owner->SetMoveInput(0.0f, 0.0f);
				owner->SetMoveSpeed(0.0f);
				return true;
			}
			//else if (!owner->InBattleTerritory())
			//{
			//	owner->SetMoveInput(0.0f, 0.0f);
			//	owner->SetMoveSpeed(0.0f);
			//	return BattleEnemyState::Run;
			//}

			owner->SetMoveInput(0.0f, -1.0f); // TODO : moveX
			owner->SetMoveSpeed(Enemy::WalkSpeed);

			return false;
		}

		// �G(�v���C���[)�܂ő���
		bool Run(std::shared_ptr<Enemy> owner)
		{
			if (owner->InBattleTerritory())
			{
				owner->SetMoveInput(0.0f, 0.0f);
				owner->SetMoveSpeed(0.0f);
				return true;
			}

			owner->SetMoveInput(0.0f, -1.0f);
			owner->SetMoveSpeed(Enemy::RunSpeed);

			return false;
		}

		// �U�������
		bool Dedge(std::shared_ptr<Enemy> owner)
		{
			std::shared_ptr<Enemy::HTNDatas> data = owner->GetHTNDatas();

			if (!owner->GetModel()->IsPlaying())
			{
				owner->SetMoveInput(0.0f, 0.0f);
				owner->SetMoveSpeed(0.0f);

				return true;
			}

			owner->SetMoveInput(0.0f, 1.0f);

			Phoenix::f32 speed = owner->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * data->elapsedTime);
			owner->SetMoveSpeed(speed);

			return false;
		}

		// �G(�v���C���[)���U��
		bool Attack(std::shared_ptr<Enemy> owner)
		{
			return false;
		}
	}*/
}