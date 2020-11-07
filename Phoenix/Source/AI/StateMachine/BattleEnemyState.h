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
			// 生成
			static std::shared_ptr<Attack> Create();

			// 状態に入ったときに呼ばれる関数
			void SetUp() override;

			// 次の状態に移る前に呼ばれる関数
			void CleanUp() override;

			/// <summary>
			/// 更新
			/// </summary>
			/// <returns> 次の移行するステートID </returns>
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
			BattleEnenyState Update() override;
		};
#pragma endregion
	}
}