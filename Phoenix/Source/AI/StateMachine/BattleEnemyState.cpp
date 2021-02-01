#include "BattleEnemyState.h"
#include "../../Enemy/Enemy.h"


namespace BattleEnemy
{
#pragma region Idle
	// ����
	std::shared_ptr<Idle> Idle::Create()
	{
		return std::make_shared<Idle>();
	}

	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
	void Idle::SetUp()
	{
		canChangeState = false;
		timeCounter = 0.0f;
	}

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	void Idle::CleanUp()
	{

	}

	// �X�V
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
	// ����
	std::shared_ptr<Walk> Walk::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<Walk>(owner);
	}

	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
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

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	void Walk::CleanUp()
	{

	}

	// �X�V
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
	// ����
	std::shared_ptr<Run> Run::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<Run>(owner);
	}

	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
	void Run::SetUp()
	{

	}

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	void Run::CleanUp()
	{

	}

	// �X�V
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
		// ����
	std::shared_ptr<Dedge> Dedge::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<Dedge>(owner);
	}

	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
	void Dedge::SetUp()
	{
		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			obj->SetMoveSpeed(Speed);
		}
	}

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	void Dedge::CleanUp()
	{

	}

	// �X�V
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
	// ����
	std::shared_ptr<DamageSmall> DamageSmall::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<DamageSmall>(owner);
	}

	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
	void DamageSmall::SetUp()
	{
		canChangeState = false;

		std::shared_ptr<Enemy> obj = owner.lock();
		if (obj)
		{
			obj->SetMoveSpeed(Speed);
		}
	}

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	void DamageSmall::CleanUp()
	{

	}

	// �X�V
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
	// ����
	std::shared_ptr<DamageBig> DamageBig::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<DamageBig>(owner);
	}

	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
	void DamageBig::SetUp()
	{
		canChangeState = false;

		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			obj->SetMoveSpeed(Speed);
		}
	}

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	void DamageBig::CleanUp()
	{

	}

	// �X�V
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
	// ����
	std::shared_ptr<KnockBack> KnockBack::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<KnockBack>(owner);
	}

	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
	void KnockBack::SetUp()
	{
		canChangeState = false;

		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			obj->SetMoveSpeed(Speed);
		}
	}

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	void KnockBack::CleanUp()
	{

	}

	// �X�V
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
	// ����
	std::shared_ptr<GettingUp> GettingUp::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<GettingUp>(owner);
	}

	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
	void GettingUp::SetUp()
	{
		canChangeState = false;
	}

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	void GettingUp::CleanUp()
	{

	}

	// �X�V
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
	// ����
	std::shared_ptr<Guard> Guard::Create()
	{
		return std::make_shared<Guard>();
	}

	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
	void Guard::SetUp()
	{

	}

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	void Guard::CleanUp()
	{

	}

	// �X�V
	BattleEnemyState Guard::Update(Phoenix::f32 elapsedTime)
	{
		return BattleEnemyState::NoneState;
	}
#pragma endregion

#pragma region Death
	// ����
	std::shared_ptr<Death> Death::Create()
	{
		return std::make_shared<Death>();
	}

	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
	void Death::SetUp()
	{

	}

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	void Death::CleanUp()
	{

	}

	// �X�V
	BattleEnemyState Death::Update(Phoenix::f32 elapsedTime)
	{
		return BattleEnemyState::NoneState;
	}
#pragma endregion
}

namespace BattleBoss
{
#pragma region Walk
	// ����
	std::shared_ptr<Walk> Walk::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<Walk>(owner);
	}

	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
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

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	void Walk::CleanUp()
	{

	}

	// �X�V
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
	// ����
	std::shared_ptr<Run> Run::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<Run>(owner);
	}

	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
	void Run::SetUp()
	{

	}

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	void Run::CleanUp()
	{

	}

	// �X�V
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