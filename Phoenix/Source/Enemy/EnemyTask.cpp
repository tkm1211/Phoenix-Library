#include "EnemyTask.h"


namespace EnemyTask
{
	namespace Primitive
	{
		// 待機する
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

		// 敵(プレイヤー)まで歩く
		bool Walk(EnemyWorldState* state)
		{
			// 索敵範囲内に敵(プレイヤー)がいて、攻撃がヒットする範囲内に敵(プレイヤー)が入っているとき
			if (state->inTerritory && !state->inDistanceHit)
			{
				state->inDistanceHit = true;
				state->enemyState = BattleEnemyState::Walk;

				return true;
			}

			return false;
		}

		// 敵(プレイヤー)まで走る
		bool Run(EnemyWorldState* state)
		{
			// 索敵範囲内に敵(プレイヤー)がいないとき
			if (!state->inTerritory)
			{
				state->inTerritory = true;
				state->enemyState = BattleEnemyState::Run;

				return true;
			}

			return false;
		}

		// 攻撃を回避
		bool Dedge(EnemyWorldState* state)
		{
			// 索敵範囲内に敵(プレイヤー)がいて、敵(プレイヤー)が攻撃しているとき
			if (state->inTerritory && state->playerAttacking)
			{
				state->enemyState = BattleEnemyState::Dedge;

				return true;
			}

			return false;
		}

		// 敵(プレイヤー)を攻撃
		bool Attack(EnemyWorldState* state)
		{
			// 攻撃中ではなく、攻撃権を持っているとき
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
		// 敵(プレイヤー)まで歩く
		bool Walk(EnemyWorldState* state, std::vector<BattleEnemyState>* name)
		{
			if (state->inTerritory && !state->inDistanceHit) name->emplace_back(BattleEnemyState::Walk);

			return true;
		}

		// 敵(プレイヤー)まで走る
		bool Run(EnemyWorldState* state, std::vector<BattleEnemyState>* name)
		{
			if (!state->inTerritory) name->emplace_back(BattleEnemyState::Run);

			return true;
		}

		// 攻撃を回避
		bool Dedge(EnemyWorldState* state, std::vector<BattleEnemyState>* name)
		{
			if (state->inTerritory && state->playerAttacking) name->emplace_back(BattleEnemyState::Dedge);

			return true;
		}

		// 敵(プレイヤー)を攻撃
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
		// 待機する
		void Idle(std::shared_ptr<Enemy> owner)
		{
			std::shared_ptr<Enemy::HTNDatas> data = owner->GetHTNDatas();
			data->idleCount = 0.0f;
		}

		// 敵(プレイヤー)まで歩く
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

		// 敵(プレイヤー)まで走る
		void Run(std::shared_ptr<Enemy> owner)
		{
		}

		// 攻撃を回避
		void Dedge(std::shared_ptr<Enemy> owner)
		{
			owner->SetMoveSpeed(Enemy::DedgeSpeed);
		}

		// 敵(プレイヤー)を攻撃
		void Attack(std::shared_ptr<Enemy> owner)
		{
			std::shared_ptr<Enemy::HTNDatas> data = owner->GetHTNDatas();
			data->attackIndex = 0;
		}
	}

	namespace Action
	{
		// 待機する
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

		// 敵(プレイヤー)まで歩く
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

		// 敵(プレイヤー)まで走る
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

		// 攻撃を回避
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

		// 敵(プレイヤー)を攻撃
		bool Attack(std::shared_ptr<Enemy> owner)
		{
			return false;
		}
	}*/
}