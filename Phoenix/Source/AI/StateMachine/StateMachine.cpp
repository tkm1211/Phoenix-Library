#include "StateMachine.h"


namespace AI
{
	namespace StateMachine
	{
		// 生成
		std::shared_ptr<StateMachine> StateMachine::Create()
		{
			return std::make_shared<StateMachine>();
		}

		// コンストラクタ
		void StateMachine::Construct()
		{

		}

		// 初期化
		void StateMachine::Initialize()
		{
			
		}

		// 終了化
		void StateMachine::Finalize()
		{

		}

		// 更新
		void StateMachine::Update()
		{
			Phoenix::s32 index = currentState->Update();

			if (index != -1)
			{

			}
		}
	}
}