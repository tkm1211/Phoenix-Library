#include "StateMachine.h"


namespace AI
{
	namespace StateMachine
	{
		// ����
		std::shared_ptr<StateMachine> StateMachine::Create()
		{
			return std::make_shared<StateMachine>();
		}

		// �R���X�g���N�^
		void StateMachine::Construct()
		{

		}

		// ������
		void StateMachine::Initialize()
		{
			
		}

		// �I����
		void StateMachine::Finalize()
		{

		}

		// �X�V
		void StateMachine::Update()
		{
			Phoenix::s32 index = currentState->Update();

			if (index != -1)
			{

			}
		}
	}
}