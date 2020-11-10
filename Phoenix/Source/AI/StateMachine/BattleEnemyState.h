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
			// 生成
			static std::shared_ptr<Idle> Create();

			// 状態に入ったときに呼ばれる関数
			void SetUp() override;

			// 次の状態に移る前に呼ばれる関数
			void CleanUp() override;

			/// <summary>
			/// 更新
			/// </summary>
			/// <returns> 次の移行するステートID </returns>
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
			// 生成
			static std::shared_ptr<Attack> Create(std::shared_ptr<Enemy> owner);

			// 状態に入ったときに呼ばれる関数
			void SetUp() override;

			// 次の状態に移る前に呼ばれる関数
			void CleanUp() override;

			/// <summary>
			/// 更新
			/// </summary>
			/// <returns> 次の移行するステートID </returns>
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
			// 生成
			static std::shared_ptr<Dedge> Create();

			// 状態に入ったときに呼ばれる関数
			void SetUp() override;

			// 次の状態に移る前に呼ばれる関数
			void CleanUp() override;

			/// <summary>
			/// 更新
			/// </summary>
			/// <returns> 次の移行するステートID </returns>
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
			// 生成
			static std::shared_ptr<Guard> Create();

			// 状態に入ったときに呼ばれる関数
			void SetUp() override;

			// 次の状態に移る前に呼ばれる関数
			void CleanUp() override;

			/// <summary>
			/// 更新
			/// </summary>
			/// <returns> 次の移行するステートID </returns>
			BattleEnemyState Update() override;
		};
#pragma endregion
	}
}