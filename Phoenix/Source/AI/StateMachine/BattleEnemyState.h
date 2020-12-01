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
			// 生成
			static std::shared_ptr<Walk> Create(std::shared_ptr<Enemy> owner);

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
			// 生成
			static std::shared_ptr<Run> Create(std::shared_ptr<Enemy> owner);

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
			static constexpr Phoenix::f32 Speed = 0.05f;

		private:
			std::shared_ptr<Enemy> owner;

		public:
			Dedge(std::shared_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::Dedge), owner(owner) {}
			~Dedge() {}

		public:
			// 生成
			static std::shared_ptr<Dedge> Create(std::shared_ptr<Enemy> owner);

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
			// 生成
			static std::shared_ptr<DamageSmall> Create(std::shared_ptr<Enemy> owner);

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
			// 生成
			static std::shared_ptr<DamageBig> Create(std::shared_ptr<Enemy> owner);

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

#pragma region Death
		class Death : public State<BattleEnemyState>
		{
		private:


		public:
			Death() : State<BattleEnemyState>(BattleEnemyState::Death) {}
			~Death() {}

		public:
			// 生成
			static std::shared_ptr<Death> Create();

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