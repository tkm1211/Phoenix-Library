#include "BossAI.h"
#include "AIState/WaitState.h"
#include "AIState/MoveState.h"
#include "AIState/AvoidState.h"
#include "AIState/SwingAttackState.h"
#include "AIState/JumpAttackState.h"
#include "AIState/DamageState.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "Boss.h"
#include "../Player/Player.h"


std::unique_ptr<AI> BossAI::Create()
{
	return std::make_unique<BossAI>();
}

void BossAI::Construct()
{
	// ステートの追加
	{
		AddState<WaitState>();
		AddState<MoveState>();
		AddState<AvoidState>();
		AddState<SwingAttackState01>();
		AddState<SwingAttackState02>();
		AddState<JumpAttackState>();
		AddState<DamageState>();
	}
}

void BossAI::Initialize()
{
	// 全ステートの初期化
	for (Phoenix::u32 i = 0; i < states.size(); ++i)
	{
		states[i]->Init();
	}

	// 初期ステートの取得
	{
		currentState = GetState(AIStateType::Wait);
	}

	isAI = true;
}

void BossAI::Update()
{
	if (!isAI) return;

	// 蓄積ダメージステートに変更していないか確認
	if (boss->IsChangeAccumulationDamege())
	{
		nextState = GetState(AIStateType::Damage);
	}

	// ステート移行
	if (nextState)
	{
		currentState = nextState;
		nextState = nullptr;
		currentState->Init();
	}

	// ステート更新
	{
		currentState->Update(boss, player);
	}

	// 次のステートを取得
	if (currentState->IsChangeState())
	{
		nextState = GetState(currentState->GetNextStateType());
	}
}

void BossAI::GUI()
{
	if (ImGui::TreeNode("AI"))
	{
		ImGui::Checkbox("On", &isAI);
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