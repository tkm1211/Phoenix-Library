#include "BattleEnemyState.h"


namespace AI
{
	namespace BattleEnemy
	{
#pragma region Idle
		// 生成
		std::shared_ptr<Idle> Idle::Create()
		{
			return std::make_shared<Idle>();
		}

		// 状態に入ったときに呼ばれる関数
		void Idle::SetUp()
		{

		}

		// 次の状態に移る前に呼ばれる関数
		void Idle::CleanUp()
		{

		}

		// 更新
		BattleEnenyState Idle::Update()
		{
			return BattleEnenyState::NoneState;
		}
#pragma endregion

#pragma region Attack
		// 生成
		std::shared_ptr<Attack> Attack::Create()
		{
			return std::make_shared<Attack>();
		}

		// 状態に入ったときに呼ばれる関数
		void Attack::SetUp()
		{

		}

		// 次の状態に移る前に呼ばれる関数
		void Attack::CleanUp()
		{

		}

		// 更新
		BattleEnenyState Attack::Update()
		{
			return BattleEnenyState::NoneState;
		}
#pragma endregion

#pragma region Dedge
		// 生成
		std::shared_ptr<Dedge> Dedge::Create()
		{
			return std::make_shared<Dedge>();
		}

		// 状態に入ったときに呼ばれる関数
		void Dedge::SetUp()
		{

		}

		// 次の状態に移る前に呼ばれる関数
		void Dedge::CleanUp()
		{

		}

		// 更新
		BattleEnenyState Dedge::Update()
		{
			return BattleEnenyState::NoneState;
		}
#pragma endregion
	}
}