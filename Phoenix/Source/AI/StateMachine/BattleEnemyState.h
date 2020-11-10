#pragma once

#include "StateMachine.h"
#include "../../Enemy/EnemyState.h"
#include "../../Enemy/Enemy.h"


namespace AI
{
	namespace BattleEnemy
	{
#pragma region Idle
		class Idle : public State<BattleEnemyState>
		{
		private:


		public:
			Idle() : State<BattleEnemyState>(BattleEnemyState::Idle) {}
			~Idle() {}

		public:
			// ����
			static std::shared_ptr<Idle> Create();

			// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
			void SetUp() override;

			// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
			void CleanUp() override;

			/// <summary>
			/// �X�V
			/// </summary>
			/// <returns> ���̈ڍs����X�e�[�gID </returns>
			BattleEnemyState Update() override;
		};
#pragma endregion

#pragma region Attack
		class Attack : public State<BattleEnemyState>
		{
		private:
			std::shared_ptr<Enemy> owner;

			Phoenix::s32 index = 0;
			EnemyAttackState currentAttack = EnemyAttackState::NoneState;
			std::vector<EnemyAttackState> attackList;

		public:
			Attack(std::shared_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::Attack), owner(owner) {}
			~Attack() {}

		public:
			// ����
			static std::shared_ptr<Attack> Create(std::shared_ptr<Enemy> owner);

			// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
			void SetUp() override;

			// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
			void CleanUp() override;

			/// <summary>
			/// �X�V
			/// </summary>
			/// <returns> ���̈ڍs����X�e�[�gID </returns>
			BattleEnemyState Update() override;
		};
#pragma endregion

#pragma region Dedge
		class Dedge : public State<BattleEnemyState>
		{
		private:


		public:
			Dedge() : State<BattleEnemyState>(BattleEnemyState::Dedge) {}
			~Dedge() {}

		public:
			// ����
			static std::shared_ptr<Dedge> Create();

			// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
			void SetUp() override;

			// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
			void CleanUp() override;

			/// <summary>
			/// �X�V
			/// </summary>
			/// <returns> ���̈ڍs����X�e�[�gID </returns>
			BattleEnemyState Update() override;
		};
#pragma endregion

#pragma region Guard
		class Guard : public State<BattleEnemyState>
		{
		private:


		public:
			Guard() : State<BattleEnemyState>(BattleEnemyState::Guard) {}
			~Guard() {}

		public:
			// ����
			static std::shared_ptr<Guard> Create();

			// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
			void SetUp() override;

			// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
			void CleanUp() override;

			/// <summary>
			/// �X�V
			/// </summary>
			/// <returns> ���̈ڍs����X�e�[�gID </returns>
			BattleEnemyState Update() override;
		};
#pragma endregion
	}
}