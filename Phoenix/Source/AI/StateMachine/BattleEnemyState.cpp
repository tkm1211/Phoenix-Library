#include "BattleEnemyState.h"


namespace AI
{
	namespace BattleEnemy
	{
#pragma region Idle
		// 生成
		std::shared_ptr<Idle> Idle::Create()
		{
			return std::make_shared<Idle>();
		}

		// 状態に入ったときに呼ばれる関数
		void Idle::SetUp()
		{

		}

		// 次の状態に移る前に呼ばれる関数
		void Idle::CleanUp()
		{

		}

		// 更新
		BattleEnemyState Idle::Update()
		{
			return BattleEnemyState::NoneState;
		}
#pragma endregion

#pragma region Attack
		// 生成
		std::shared_ptr<Attack> Attack::Create(std::shared_ptr<Enemy> owner)
		{
			return std::make_shared<Attack>(owner);
		}

		// 状態に入ったときに呼ばれる関数
		void Attack::SetUp()
		{
			index = 0;

			attackList.emplace_back(EnemyAttackState::WeakLeft);
			attackList.emplace_back(EnemyAttackState::WeakRight);
			attackList.emplace_back(EnemyAttackState::StrongLeft);
			attackList.emplace_back(EnemyAttackState::StrongRight);
		}

		// 次の状態に移る前に呼ばれる関数
		void Attack::CleanUp()
		{
			attackList.clear();
		}

		// 更新
		BattleEnemyState Attack::Update()
		{
			if (owner->GetModel()->IsPlaying() || index == 0)
			{
				currentAttack = attackList.at(index);
				++index;

				if (attackList.size() <= index)
				{
					return BattleEnemyState::Idle;
				}
				else
				{
					owner->SetAttackState(currentAttack);
				}
			}

			return BattleEnemyState::NoneState;
		}
#pragma endregion

#pragma region Dedge
		// 生成
		std::shared_ptr<Dedge> Dedge::Create()
		{
			return std::make_shared<Dedge>();
		}

		// 状態に入ったときに呼ばれる関数
		void Dedge::SetUp()
		{

		}

		// 次の状態に移る前に呼ばれる関数
		void Dedge::CleanUp()
		{

		}

		// 更新
		BattleEnemyState Dedge::Update()
		{
			return BattleEnemyState::NoneState;
		}
#pragma endregion

#pragma region Guard
		// 生成
		std::shared_ptr<Guard> Guard::Create()
		{
			return std::make_shared<Guard>();
		}

		// 状態に入ったときに呼ばれる関数
		void Guard::SetUp()
		{

		}

		// 次の状態に移る前に呼ばれる関数
		void Guard::CleanUp()
		{

		}

		// 更新
		BattleEnemyState Guard::Update()
		{
			return BattleEnemyState::NoneState;
		}
#pragma endregion
	}
}