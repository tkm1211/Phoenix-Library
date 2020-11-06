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
			// �X�V
			virtual Phoenix::s32 Update() = 0;

			// ������֐��i��ɃA�j���[�V�����؂�ւ��Ȃǁj
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
			// ����
			static std::shared_ptr<StateMachine> Create();

			// �R���X�g���N�^
			void Construct();

			// ������
			void Initialize();

			// �I����
			void Finalize();

			// �X�V
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