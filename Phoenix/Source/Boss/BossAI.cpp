#include "BossAI.h"
#include "AIState/WaitState.h"
#include "AIState/MoveState.h"
#include "AIState/AvoidState.h"
#include "AIState/SwingAttackState.h"
#include "AIState/JumpAttackState.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "Boss.h"
#include "../Player/Player.h"


std::unique_ptr<AI> BossAI::Create()
{
	return std::make_unique<BossAI>();
}

void BossAI::Init()
{
	// �X�e�[�g�̒ǉ�
	{
		AddState<WaitState>();
		AddState<MoveState>();
		AddState<AvoidState>();
		AddState<SwingAttackState01>();
		AddState<SwingAttackState02>();
		AddState<JumpAttackState>();
	}

	// �S�X�e�[�g�̏�����
	for (Phoenix::u32 i = 0; i < states.size(); ++i)
	{
		states[i]->Init();
	}

	// �����X�e�[�g�̎擾
	{
		currentState = GetState(AIStateType::Wait);
	}
}

void BossAI::Update()
{
	// �X�e�[�g�ڍs
	if (nextState)
	{
		currentState = nextState;
		nextState = nullptr;
		currentState->Init();
	}

	// �X�e�[�g�X�V
	{
		currentState->Update(boss, player);
	}

	// ���̃X�e�[�g���擾
	if (currentState->IsChangeState())
	{
		nextState = GetState(currentState->GetNextStateType());
	}
}

void BossAI::GUI()
{
	if (ImGui::TreeNode("AI"))
	{
		switch (currentState->GetStateType())
		{
		case AIStateType::None:
			break;

		case AIStateType::Wait:
			ImGui::Text("State : 'Wait'");
			break;

		case AIStateType::Move:
			ImGui::Text("State : 'Move'");
			break;

		default: break;
		}
		ImGui::TreePop();
	}
}