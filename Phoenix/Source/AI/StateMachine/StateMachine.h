#pragma once

#include <memory>
#include <vector>
#include "Phoenix/Math/PhoenixMath.h"


namespace AI
{
	namespace StateMachine
	{
		class IState
		{
		public:
			virtual ~IState() {}

		public:
			// 更新
			virtual Phoenix::s32 Update() = 0;

			// 入り口関数（主にアニメーション切り替えなど）
			virtual void Enter() {}
		};

		class StateMachine
		{
		private:
			std::vector<std::shared_ptr<IState>> stateList;
			std::shared_ptr<IState> currentState;

		public:
			StateMachine() {}
			~StateMachine() {}

		public:
			// 生成
			static std::shared_ptr<StateMachine> Create();

			// コンストラクタ
			void Construct();

			// 初期化
			void Initialize();

			// 終了化
			void Finalize();

			// 更新
			void Update();

		public:
			template <class T>
			void AddState()
			{
				std::shared_ptr<T> obj = std::make_shared<T>();
				stateList.emplace_back(obj);
			}
		};
	}
}