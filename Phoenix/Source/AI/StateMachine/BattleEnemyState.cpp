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

#pragma region Walk
		// 生成
		std::shared_ptr<Walk> Walk::Create(std::shared_ptr<Enemy> owner)
		{
			return std::make_shared<Walk>(owner);
		}

		// 状態に入ったときに呼ばれる関数
		void Walk::SetUp()
		{
			moveX = 0.0f;

			Phoenix::s32 judge = rand() % 2;

			if (judge)
			{
				moveX = -1.0f;
			}
			else
			{
				moveX = 1.0f;
			}
		}

		// 次の状態に移る前に呼ばれる関数
		void Walk::CleanUp()
		{

		}

		// 更新
		BattleEnemyState Walk::Update()
		{
			if (owner->InDistanceHitByAttack())
			{
				owner->SetMoveInput(0.0f, 0.0f);
				owner->SetMoveSpeed(0.0f);
				//return BattleEnemyState::Attack;
				return BattleEnemyState::Idle;
			}
			else if (!owner->InBattleTerritory())
			{
				owner->SetMoveInput(0.0f, 0.0f);
				owner->SetMoveSpeed(0.0f);
				return BattleEnemyState::Run;
			}

			owner->SetMoveInput(moveX, -1.0f);
			owner->SetMoveSpeed(Speed);
			return BattleEnemyState::NoneState;
		}
#pragma endregion

#pragma region Run
		// 生成
		std::shared_ptr<Run> Run::Create(std::shared_ptr<Enemy> owner)
		{
			return std::make_shared<Run>(owner);
		}

		// 状態に入ったときに呼ばれる関数
		void Run::SetUp()
		{

		}

		// 次の状態に移る前に呼ばれる関数
		void Run::CleanUp()
		{

		}

		// 更新
		BattleEnemyState Run::Update()
		{
			if (owner->InDistanceHitByAttack())
			{
				owner->SetMoveInput(0.0f, 0.0f);
				owner->SetMoveSpeed(0.0f);
				return BattleEnemyState::Idle;
			}

			owner->SetMoveInput(0.0f, -1.0f);
			owner->SetMoveSpeed(Speed);
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

			attackList.emplace_back(EnemyAttackState::WeakRight);
			attackList.emplace_back(EnemyAttackState::WeakLeft);
			attackList.emplace_back(EnemyAttackState::StrongRight);
			//attackList.emplace_back(EnemyAttackState::StrongLeft);
		}

		// 次の状態に移る前に呼ばれる関数
		void Attack::CleanUp()
		{
			attackList.clear();
		}

		// 更新
		BattleEnemyState Attack::Update()
		{
			if (!owner->GetModel()->IsPlaying() || index == 0)
			{
				if (attackList.size() <= index)
				{
					return BattleEnemyState::Idle;
				}

				currentAttack = attackList.at(index);
				++index;

				owner->SetAttackState(currentAttack);
				owner->UpdateNewRotate();
				owner->SetMoveSpeed(Speed);
			}

			Phoenix::f32 speed = owner->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f);
			owner->SetMoveSpeed(speed);
			owner->SetMoveInput(0.0f, 1.0f);

			return BattleEnemyState::NoneState;
		}
#pragma endregion

#pragma region Dedge
		// 生成
		std::shared_ptr<Dedge> Dedge::Create(std::shared_ptr<Enemy> owner)
		{
			return std::make_shared<Dedge>(owner);
		}

		// 状態に入ったときに呼ばれる関数
		void Dedge::SetUp()
		{
			owner->SetMoveSpeed(Speed);
		}

		// 次の状態に移る前に呼ばれる関数
		void Dedge::CleanUp()
		{

		}

		// 更新
		BattleEnemyState Dedge::Update()
		{
			if (!owner->GetModel()->IsPlaying())
			{
				owner->SetMoveInput(0.0f, 0.0f);
				owner->SetMoveSpeed(0.0f);

				return BattleEnemyState::Idle;
			}

			Phoenix::s32 input = rand() % 2;
			if (input) owner->SetMoveInput(1.0f, 0.0f);
			else if (!input) owner->SetMoveInput(-1.0f, 0.0f);

			Phoenix::f32 speed = owner->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f);
			owner->SetMoveSpeed(speed);

			return BattleEnemyState::NoneState;
		}
#pragma endregion

#pragma region DamageSmall
		// 生成
		std::shared_ptr<DamageSmall> DamageSmall::Create(std::shared_ptr<Enemy> owner)
		{
			return std::make_shared<DamageSmall>(owner);
		}

		// 状態に入ったときに呼ばれる関数
		void DamageSmall::SetUp()
		{
			owner->SetMoveSpeed(Speed);
		}

		// 次の状態に移る前に呼ばれる関数
		void DamageSmall::CleanUp()
		{

		}

		// 更新
		BattleEnemyState DamageSmall::Update()
		{
			if (!owner->GetModel()->IsPlaying())
			{
				return BattleEnemyState::Idle;
			}

			Phoenix::f32 speed = owner->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f);
			owner->SetMoveSpeed(speed);
			owner->SetMoveInput(0.0f, 1.0f);

			return BattleEnemyState::NoneState;
		}
#pragma endregion

#pragma region DamageBig
		// 生成
		std::shared_ptr<DamageBig> DamageBig::Create(std::shared_ptr<Enemy> owner)
		{
			return std::make_shared<DamageBig>(owner);
		}

		// 状態に入ったときに呼ばれる関数
		void DamageBig::SetUp()
		{
			owner->SetMoveSpeed(Speed);
		}

		// 次の状態に移る前に呼ばれる関数
		void DamageBig::CleanUp()
		{

		}

		// 更新
		BattleEnemyState DamageBig::Update()
		{
			if (!owner->GetModel()->IsPlaying())
			{
				return BattleEnemyState::Idle;
			}

			Phoenix::f32 speed = owner->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f);
			owner->SetMoveSpeed(speed);
			owner->SetMoveInput(0.0f, 1.0f);

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