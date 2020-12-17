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
			canChangeState = false;
			timeCounter = 0.0f;
		}

		// 次の状態に移る前に呼ばれる関数
		void Idle::CleanUp()
		{

		}

		// 更新
		BattleEnemyState Idle::Update(Phoenix::f32 elapsedTime)
		{
			if ((MaxCount * elapsedTime) <= timeCounter)
			{
				canChangeState = true;
				timeCounter = 0;
			}
			else
			{
				timeCounter += 1.0f * elapsedTime;
			}

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
		BattleEnemyState Walk::Update(Phoenix::f32 elapsedTime)
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
		BattleEnemyState Run::Update(Phoenix::f32 elapsedTime)
		{
			if (owner->InBattleTerritory())
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
		// No Data.
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
		BattleEnemyState Dedge::Update(Phoenix::f32 elapsedTime)
		{
			if (!owner->GetModel()->IsPlaying())
			{
				owner->SetMoveInput(0.0f, 0.0f);
				owner->SetMoveSpeed(0.0f);

				return BattleEnemyState::Idle;
			}

#if 0
			Phoenix::s32 input = rand() % 2;
			if (input) owner->SetMoveInput(1.0f, 0.0f);
			else if (!input) owner->SetMoveInput(-1.0f, 0.0f);
#else
			owner->SetMoveInput(0.0f, 1.0f);
#endif

			Phoenix::f32 speed = owner->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * elapsedTime);
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
			canChangeState = false;
			owner->SetMoveSpeed(Speed);
		}

		// 次の状態に移る前に呼ばれる関数
		void DamageSmall::CleanUp()
		{

		}

		// 更新
		BattleEnemyState DamageSmall::Update(Phoenix::f32 elapsedTime)
		{
			if (!owner->GetModel()->IsPlaying())
			{
				canChangeState = true;
				return BattleEnemyState::Idle;
			}

			Phoenix::f32 speed = owner->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * elapsedTime);
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
			canChangeState = false;
			owner->SetMoveSpeed(Speed);
		}

		// 次の状態に移る前に呼ばれる関数
		void DamageBig::CleanUp()
		{

		}

		// 更新
		BattleEnemyState DamageBig::Update(Phoenix::f32 elapsedTime)
		{
			if (!owner->GetModel()->IsPlaying())
			{
				canChangeState = true;
				return BattleEnemyState::Idle;
			}

			Phoenix::f32 speed = owner->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * elapsedTime);
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
		BattleEnemyState Guard::Update(Phoenix::f32 elapsedTime)
		{
			return BattleEnemyState::NoneState;
		}
#pragma endregion

#pragma region Death
		// 生成
		std::shared_ptr<Death> Death::Create()
		{
			return std::make_shared<Death>();
		}

		// 状態に入ったときに呼ばれる関数
		void Death::SetUp()
		{

		}

		// 次の状態に移る前に呼ばれる関数
		void Death::CleanUp()
		{

		}

		// 更新
		BattleEnemyState Death::Update(Phoenix::f32 elapsedTime)
		{
			return BattleEnemyState::NoneState;
		}
#pragma endregion
	}
}