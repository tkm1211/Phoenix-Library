#include "BattleEnemyState.h"
#include "../../Enemy/Enemy.h"


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
		if (MaxCount <= timeCounter)
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
	std::shared_ptr<Walk> Walk::Create(std::weak_ptr<Enemy> owner)
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
		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			if (obj->InDistanceHitByAttack())
			{
				obj->SetMoveInput(0.0f, 0.0f);
				obj->SetMoveSpeed(0.0f);
				//return BattleEnemyState::Attack;
				return BattleEnemyState::Idle;
			}
			else if (!obj->InBattleTerritory())
			{
				obj->SetMoveInput(0.0f, 0.0f);
				obj->SetMoveSpeed(0.0f);
				return BattleEnemyState::Run;
			}

			obj->SetMoveInput(moveX, -1.0f);
			obj->SetMoveSpeed(Speed);
		}

		return BattleEnemyState::NoneState;
	}
#pragma endregion

#pragma region Run
	// 生成
	std::shared_ptr<Run> Run::Create(std::weak_ptr<Enemy> owner)
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
		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			if (obj->InBattleTerritory())
			{
				obj->SetMoveInput(0.0f, 0.0f);
				obj->SetMoveSpeed(0.0f);
				return BattleEnemyState::Idle;
			}

			obj->SetMoveInput(0.0f, -1.0f);
			obj->SetMoveSpeed(Speed);
		}

		return BattleEnemyState::NoneState;
	}
#pragma endregion

#pragma region Attack
	// No Data.
#pragma endregion

#pragma region Dedge
		// 生成
	std::shared_ptr<Dedge> Dedge::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<Dedge>(owner);
	}

	// 状態に入ったときに呼ばれる関数
	void Dedge::SetUp()
	{
		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			obj->SetMoveSpeed(Speed);
		}
	}

	// 次の状態に移る前に呼ばれる関数
	void Dedge::CleanUp()
	{

	}

	// 更新
	BattleEnemyState Dedge::Update(Phoenix::f32 elapsedTime)
	{
		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			if (!obj->GetModel()->IsPlaying())
			{
				obj->SetMoveInput(0.0f, 0.0f);
				obj->SetMoveSpeed(0.0f);

				return BattleEnemyState::Idle;
			}

#if 0
			Phoenix::s32 input = rand() % 2;
			if (input) obj->SetMoveInput(1.0f, 0.0f);
			else if (!input) obj->SetMoveInput(-1.0f, 0.0f);
#else
			obj->SetMoveInput(0.0f, 1.0f);
#endif

			Phoenix::f32 speed = obj->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * elapsedTime);
			obj->SetMoveSpeed(speed);
		}

		return BattleEnemyState::NoneState;
	}
#pragma endregion

#pragma region DamageSmall
	// 生成
	std::shared_ptr<DamageSmall> DamageSmall::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<DamageSmall>(owner);
	}

	// 状態に入ったときに呼ばれる関数
	void DamageSmall::SetUp()
	{
		canChangeState = false;

		std::shared_ptr<Enemy> obj = owner.lock();
		if (obj)
		{
			obj->SetMoveSpeed(Speed);
		}
	}

	// 次の状態に移る前に呼ばれる関数
	void DamageSmall::CleanUp()
	{

	}

	// 更新
	BattleEnemyState DamageSmall::Update(Phoenix::f32 elapsedTime)
	{
		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			if (!obj->GetModel()->IsPlaying())
			{
				canChangeState = true;
				return BattleEnemyState::Idle;
			}

			Phoenix::f32 speed = obj->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * elapsedTime);
			obj->SetMoveSpeed(speed);
			obj->SetMoveInput(0.0f, 1.0f);
		}

		return BattleEnemyState::NoneState;
	}
#pragma endregion

#pragma region DamageBig
	// 生成
	std::shared_ptr<DamageBig> DamageBig::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<DamageBig>(owner);
	}

	// 状態に入ったときに呼ばれる関数
	void DamageBig::SetUp()
	{
		canChangeState = false;

		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			obj->SetMoveSpeed(Speed);
		}
	}

	// 次の状態に移る前に呼ばれる関数
	void DamageBig::CleanUp()
	{

	}

	// 更新
	BattleEnemyState DamageBig::Update(Phoenix::f32 elapsedTime)
	{
		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			if (!obj->GetModel()->IsPlaying())
			{
				canChangeState = true;
				return BattleEnemyState::Idle;
			}

			Phoenix::f32 speed = obj->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * elapsedTime);
			obj->SetMoveSpeed(speed);
			obj->SetMoveInput(0.0f, 1.0f);
		}

		return BattleEnemyState::NoneState;
	}
#pragma endregion

#pragma region KnockBack
	// 生成
	std::shared_ptr<KnockBack> KnockBack::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<KnockBack>(owner);
	}

	// 状態に入ったときに呼ばれる関数
	void KnockBack::SetUp()
	{
		canChangeState = false;

		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			obj->SetMoveSpeed(Speed);
		}
	}

	// 次の状態に移る前に呼ばれる関数
	void KnockBack::CleanUp()
	{

	}

	// 更新
	BattleEnemyState KnockBack::Update(Phoenix::f32 elapsedTime)
	{
		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			Phoenix::f32 speed = obj->GetMoveSpeed();

			if (!obj->GetModel()->IsPlaying())
			{
				if (0.0006f < speed)
				{
					canChangeState = true;
					return BattleEnemyState::KnockBack;
				}
				canChangeState = true;
				return BattleEnemyState::GettingUp;
			}

			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.05f * elapsedTime);
			obj->SetMoveSpeed(speed);
			obj->SetMoveInput(0.0f, 1.0f);
		}

		return BattleEnemyState::NoneState;
	}
#pragma endregion

#pragma region GettingUp
	// 生成
	std::shared_ptr<GettingUp> GettingUp::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<GettingUp>(owner);
	}

	// 状態に入ったときに呼ばれる関数
	void GettingUp::SetUp()
	{
		canChangeState = false;
	}

	// 次の状態に移る前に呼ばれる関数
	void GettingUp::CleanUp()
	{

	}

	// 更新
	BattleEnemyState GettingUp::Update(Phoenix::f32 elapsedTime)
	{
		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			if (!obj->GetModel()->IsPlaying())
			{
				canChangeState = true;
				return BattleEnemyState::Idle;
			}
		}

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

namespace BattleBoss
{
#pragma region Walk
	// 生成
	std::shared_ptr<Walk> Walk::Create(std::weak_ptr<Enemy> owner)
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
		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			if (obj->InDistanceHitByAttack())
			{
				obj->SetMoveInput(0.0f, 0.0f);
				obj->SetMoveSpeed(0.0f);
				//return BattleEnemyState::Attack;
				return BattleEnemyState::Idle;
			}
			else if (!obj->InBattleTerritory())
			{
				obj->SetMoveInput(0.0f, 0.0f);
				obj->SetMoveSpeed(0.0f);
				return BattleEnemyState::Run;
			}

			obj->SetMoveInput(0.0f, -1.0f);
			obj->SetMoveSpeed(Speed);
		}

		return BattleEnemyState::NoneState;
	}
#pragma endregion

#pragma region Run
	// 生成
	std::shared_ptr<Run> Run::Create(std::weak_ptr<Enemy> owner)
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
		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			if (obj->InDistanceHitByAttack())
			{
				obj->SetMoveInput(0.0f, 0.0f);
				obj->SetMoveSpeed(0.0f);
				return BattleEnemyState::Idle;
			}

			obj->SetMoveInput(0.0f, -1.0f);
			obj->SetMoveSpeed(Speed);
		}

		return BattleEnemyState::NoneState;
	}
#pragma endregion
}