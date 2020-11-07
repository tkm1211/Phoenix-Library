#pragma once

#include "StateMachine.h"
#include "../../Enemy/EnemyState.h"


namespace AI
{
	namespace BattleEnemy
	{
#pragma region Idle
		class Idle : public State<BattleEnenyState>
		{
		private:


		public:
			Idle() : State<BattleEnenyState>(BattleEnenyState::Idle) {}
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
			BattleEnenyState Update() override;
		};
#pragma endregion

#pragma region Attack
		class Attack : public State<BattleEnenyState>
		{
		private:


		public:
			Attack() : State<BattleEnenyState>(BattleEnenyState::Attack) {}
			~Attack() {}

		public:
			// ����
			static std::shared_ptr<Attack> Create();

			// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
			void SetUp() override;

			// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
			void CleanUp() override;

			/// <summary>
			/// �X�V
			/// </summary>
			/// <returns> ���̈ڍs����X�e�[�gID </returns>
			BattleEnenyState Update() override;
		};
#pragma endregion

#pragma region Dedge
		class Dedge : public State<BattleEnenyState>
		{
		private:


		public:
			Dedge() : State<BattleEnenyState>(BattleEnenyState::Dedge) {}
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
			BattleEnenyState Update() override;
		};
#pragma endregion
	}
}