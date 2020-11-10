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

			attackList.emplace_back(EnemyAttackState::WeakLeft);
			attackList.emplace_back(EnemyAttackState::WeakRight);
			attackList.emplace_back(EnemyAttackState::StrongLeft);
			attackList.emplace_back(EnemyAttackState::StrongRight);
		}

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void Attack::CleanUp()
		{
			attackList.clear();
		}

		// �X�V
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
		// ����
		std::shared_ptr<Dedge> Dedge::Create()
		{
			return std::make_shared<Dedge>();
		}

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void Dedge::SetUp()
		{

		}

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void Dedge::CleanUp()
		{

		}

		// �X�V
		BattleEnemyState Dedge::Update()
		{
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