#pragma once

#include "Phoenix/FrameWork/StateMachine/StateMachine.h"
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
			const Phoenix::s32 MaxCount = 100;

		private:
			Phoenix::s32 timeCounter = 0;

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

#pragma region Walk
		class Walk : public State<BattleEnemyState>
		{
		private:
			static constexpr Phoenix::f32 Speed = 0.045f;

		private:
			std::shared_ptr<Enemy> owner;
			Phoenix::f32 moveX = 0.0f;

		public:
			Walk(std::shared_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::Walk), owner(owner) {}
			~Walk() {}

		public:
			// ����
			static std::shared_ptr<Walk> Create(std::shared_ptr<Enemy> owner);

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

#pragma region Run
		class Run : public State<BattleEnemyState>
		{
		private:
			static constexpr Phoenix::f32 Speed = 0.1f;

		private:
			std::shared_ptr<Enemy> owner;

		public:
			Run(std::shared_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::Run), owner(owner) {}
			~Run() {}

		public:
			// ����
			static std::shared_ptr<Run> Create(std::shared_ptr<Enemy> owner);

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
			static constexpr Phoenix::f32 Speed = 0.1f;

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
			static constexpr Phoenix::f32 Speed = 0.05f;

		private:
			std::shared_ptr<Enemy> owner;

		public:
			Dedge(std::shared_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::Dedge), owner(owner) {}
			~Dedge() {}

		public:
			// ����
			static std::shared_ptr<Dedge> Create(std::shared_ptr<Enemy> owner);

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

#pragma region DamageSmall
		class DamageSmall : public State<BattleEnemyState>
		{
		private:
			static constexpr Phoenix::f32 Speed = 0.1f;

		private:
			std::shared_ptr<Enemy> owner;

		public:
			DamageSmall(std::shared_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::DamageSmall), owner(owner) {}
			~DamageSmall() {}

		public:
			// ����
			static std::shared_ptr<DamageSmall> Create(std::shared_ptr<Enemy> owner);

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

#pragma region DamageBig
		class DamageBig : public State<BattleEnemyState>
		{
		private:
			static constexpr Phoenix::f32 Speed = 0.1f;

		private:
			std::shared_ptr<Enemy> owner;

		public:
			DamageBig(std::shared_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::DamageBig), owner(owner) {}
			~DamageBig() {}

		public:
			// ����
			static std::shared_ptr<DamageBig> Create(std::shared_ptr<Enemy> owner);

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

#pragma region Death
		class Death : public State<BattleEnemyState>
		{
		private:


		public:
			Death() : State<BattleEnemyState>(BattleEnemyState::Death) {}
			~Death() {}

		public:
			// ����
			static std::shared_ptr<Death> Create();

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