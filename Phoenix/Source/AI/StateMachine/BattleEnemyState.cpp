#include "BattleEnemyState.h"


namespace AI
{
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
		// ����
		std::shared_ptr<Walk> Walk::Create(std::shared_ptr<Enemy> owner)
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
		// ����
		std::shared_ptr<Run> Run::Create(std::shared_ptr<Enemy> owner)
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
		// ����
		std::shared_ptr<Dedge> Dedge::Create(std::shared_ptr<Enemy> owner)
		{
			return std::make_shared<Dedge>(owner);
		}

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void Dedge::SetUp()
		{
			owner->SetMoveSpeed(Speed);
		}

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void Dedge::CleanUp()
		{

		}

		// �X�V
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
		// ����
		std::shared_ptr<DamageSmall> DamageSmall::Create(std::shared_ptr<Enemy> owner)
		{
			return std::make_shared<DamageSmall>(owner);
		}

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void DamageSmall::SetUp()
		{
			canChangeState = false;
			owner->SetMoveSpeed(Speed);
		}

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void DamageSmall::CleanUp()
		{

		}

		// �X�V
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
		// ����
		std::shared_ptr<DamageBig> DamageBig::Create(std::shared_ptr<Enemy> owner)
		{
			return std::make_shared<DamageBig>(owner);
		}

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void DamageBig::SetUp()
		{
			canChangeState = false;
			owner->SetMoveSpeed(Speed);
		}

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void DamageBig::CleanUp()
		{

		}

		// �X�V
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
}