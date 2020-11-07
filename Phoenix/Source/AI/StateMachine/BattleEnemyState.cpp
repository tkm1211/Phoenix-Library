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
		BattleEnenyState Idle::Update()
		{
			return BattleEnenyState::NoneState;
		}
#pragma endregion

#pragma region Attack
		// ����
		std::shared_ptr<Attack> Attack::Create()
		{
			return std::make_shared<Attack>();
		}

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void Attack::SetUp()
		{

		}

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void Attack::CleanUp()
		{

		}

		// �X�V
		BattleEnenyState Attack::Update()
		{
			return BattleEnenyState::NoneState;
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
		BattleEnenyState Dedge::Update()
		{
			return BattleEnenyState::NoneState;
		}
#pragma endregion
	}
}