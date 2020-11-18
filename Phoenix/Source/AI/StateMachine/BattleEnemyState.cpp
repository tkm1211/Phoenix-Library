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

		}

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void Idle::CleanUp()
		{

		}

		// �X�V
		BattleEnemyState Idle::Update()
		{
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
		// ����
		std::shared_ptr<Attack> Attack::Create(std::shared_ptr<Enemy> owner)
		{
			return std::make_shared<Attack>(owner);
		}

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void Attack::SetUp()
		{
			index = 0;

			attackList.emplace_back(EnemyAttackState::WeakRight);
			attackList.emplace_back(EnemyAttackState::WeakLeft);
			attackList.emplace_back(EnemyAttackState::StrongRight);
			//attackList.emplace_back(EnemyAttackState::StrongLeft);
		}

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void Attack::CleanUp()
		{
			attackList.clear();
		}

		// �X�V
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
		// ����
		std::shared_ptr<DamageSmall> DamageSmall::Create(std::shared_ptr<Enemy> owner)
		{
			return std::make_shared<DamageSmall>(owner);
		}

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void DamageSmall::SetUp()
		{
			owner->SetMoveSpeed(Speed);
		}

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void DamageSmall::CleanUp()
		{

		}

		// �X�V
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
		// ����
		std::shared_ptr<DamageBig> DamageBig::Create(std::shared_ptr<Enemy> owner)
		{
			return std::make_shared<DamageBig>(owner);
		}

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void DamageBig::SetUp()
		{
			owner->SetMoveSpeed(Speed);
		}

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void DamageBig::CleanUp()
		{

		}

		// �X�V
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
		BattleEnemyState Guard::Update()
		{
			return BattleEnemyState::NoneState;
		}
#pragma endregion
	}
}