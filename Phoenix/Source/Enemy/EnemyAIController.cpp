#include "EnemyAIController.h"
#include "EnemyTask.h"


// 生成
std::shared_ptr<EnemyAIController> EnemyAIController::Create()
{
	return std::make_shared<EnemyAIController>();
}

// コンストラクタ
void EnemyAIController::Construct(std::weak_ptr<Enemy> owner)
{
	// エネミー設定
	{
		this->owner = owner;
	}

	// AI用データ
	{
		aiData = std::make_shared<BattleEnemy::AIData>();
	}

	// ステート
	{
		// 生成
		{
			status = FSM<BattleEnemyState, std::shared_ptr<Enemy>, std::shared_ptr<BattleEnemy::AIData>>::Create();
		}

		// 条件追加
		{
			status->AddTransition(BattleEnemyState::Idle, BattleEnemyState::Walk, BattleEnemy::Walk::Transition);
			status->AddTransition(BattleEnemyState::Idle, BattleEnemyState::Run, BattleEnemy::Run::Transition);
			status->AddTransition(BattleEnemyState::Idle, BattleEnemyState::Attack, BattleEnemy::Attack::Transition);

			status->AddTransition(BattleEnemyState::Walk, BattleEnemyState::Idle, BattleEnemy::Idle::Transition);
			status->AddTransition(BattleEnemyState::Walk, BattleEnemyState::Run, BattleEnemy::Run::Transition);
			status->AddTransition(BattleEnemyState::Walk, BattleEnemyState::Attack, BattleEnemy::Attack::Transition);

			status->AddTransition(BattleEnemyState::Run, BattleEnemyState::Idle, BattleEnemy::Idle::Transition);
			status->AddTransition(BattleEnemyState::Run, BattleEnemyState::Walk, BattleEnemy::Walk::Transition);
			status->AddTransition(BattleEnemyState::Run, BattleEnemyState::Attack, BattleEnemy::Attack::Transition);

			status->AddTransition(BattleEnemyState::KnockBack, BattleEnemyState::GettingUp, BattleEnemy::GettingUp::Transition);

			status->AddTransition(BattleEnemyState::Attack, BattleEnemyState::Idle, BattleEnemy::Idle::Transition);
			status->AddTransition(BattleEnemyState::DamageSmall, BattleEnemyState::Idle, BattleEnemy::Idle::Transition);
			status->AddTransition(BattleEnemyState::DamageBig, BattleEnemyState::Idle, BattleEnemy::Idle::Transition);
			status->AddTransition(BattleEnemyState::Dedge, BattleEnemyState::Idle, BattleEnemy::Idle::Transition);
			status->AddTransition(BattleEnemyState::GettingUp, BattleEnemyState::Idle, BattleEnemy::Idle::Transition);

			status->AddTransitionOfAnyState(BattleEnemyState::DamageSmall, BattleEnemy::DamageSmall::Transition);
			status->AddTransitionOfAnyState(BattleEnemyState::DamageBig, BattleEnemy::DamageBig::Transition);
			status->AddTransitionOfAnyState(BattleEnemyState::Dedge, BattleEnemy::Dedge::Transition);
			status->AddTransitionOfAnyState(BattleEnemyState::KnockBack, BattleEnemy::KnockBack::Transition);
			status->AddTransitionOfAnyState(BattleEnemyState::Death, BattleEnemy::Death::Transition);
		}

		// 初期化追加
		{
			status->AddEnter(BattleEnemyState::Idle, BattleEnemy::Idle::Enter);
			status->AddEnter(BattleEnemyState::Walk, BattleEnemy::Walk::Enter);
			status->AddEnter(BattleEnemyState::Run, BattleEnemy::Run::Enter);
			status->AddEnter(BattleEnemyState::Attack, BattleEnemy::Attack::Enter);
			status->AddEnter(BattleEnemyState::DamageSmall, BattleEnemy::DamageSmall::Enter);
			status->AddEnter(BattleEnemyState::DamageBig, BattleEnemy::DamageBig::Enter);
			status->AddEnter(BattleEnemyState::Dedge, BattleEnemy::Dedge::Enter);
			status->AddEnter(BattleEnemyState::KnockBack, BattleEnemy::KnockBack::Enter);
			status->AddEnter(BattleEnemyState::GettingUp, BattleEnemy::GettingUp::Enter);
			status->AddEnter(BattleEnemyState::Death, BattleEnemy::Death::Enter);
		}

		// 更新追加
		{
			status->AddUpdate(BattleEnemyState::Idle, BattleEnemy::Idle::Update);
			status->AddUpdate(BattleEnemyState::Walk, BattleEnemy::Walk::Update);
			status->AddUpdate(BattleEnemyState::Run, BattleEnemy::Run::Update);
			status->AddUpdate(BattleEnemyState::Attack, BattleEnemy::Attack::Update);
			status->AddUpdate(BattleEnemyState::DamageSmall, BattleEnemy::DamageSmall::Update);
			status->AddUpdate(BattleEnemyState::DamageBig, BattleEnemy::DamageBig::Update);
			status->AddUpdate(BattleEnemyState::Dedge, BattleEnemy::Dedge::Update);
			status->AddUpdate(BattleEnemyState::KnockBack, BattleEnemy::KnockBack::Update);
			status->AddUpdate(BattleEnemyState::GettingUp, BattleEnemy::GettingUp::Update);
			status->AddUpdate(BattleEnemyState::Death, BattleEnemy::Death::Update);
		}

		// 終了化追加
		{
			status->AddExit(BattleEnemyState::Idle, BattleEnemy::Idle::Exit);
			status->AddExit(BattleEnemyState::Walk, BattleEnemy::Walk::Exit);
			status->AddExit(BattleEnemyState::Run, BattleEnemy::Run::Exit);
			status->AddExit(BattleEnemyState::Attack, BattleEnemy::Attack::Exit);
			status->AddExit(BattleEnemyState::DamageSmall, BattleEnemy::DamageSmall::Exit);
			status->AddExit(BattleEnemyState::DamageBig, BattleEnemy::DamageBig::Exit);
			status->AddExit(BattleEnemyState::Dedge, BattleEnemy::Dedge::Exit);
			status->AddExit(BattleEnemyState::KnockBack, BattleEnemy::KnockBack::Exit);
			status->AddExit(BattleEnemyState::GettingUp, BattleEnemy::GettingUp::Exit);
			status->AddExit(BattleEnemyState::Death, BattleEnemy::Death::Exit);
		}
	}

	// HTN
	{
		// 生成
		{
			htn = HTN<BattleEnemyState, EnemyWorldState, std::shared_ptr<Enemy>>::Create();
			htn->Construct();
		}

		// プリミティブタスクを追加
		{
			htn->AddPrimitiveTask(BattleEnemyState::Walk, EnemyTask::Primitive::Walk);
			htn->AddPrimitiveTask(BattleEnemyState::Run, EnemyTask::Primitive::Run);
			htn->AddPrimitiveTask(BattleEnemyState::Dedge, EnemyTask::Primitive::Dedge);
			htn->AddPrimitiveTask(BattleEnemyState::Attack, EnemyTask::Primitive::Attack);
		}

		// 複合タスクを追加
		{
			htn->AddCompoundTask(BattleEnemyState::Walk, EnemyTask::Compound::Walk);
			htn->AddCompoundTask(BattleEnemyState::Run, EnemyTask::Compound::Run);
			htn->AddCompoundTask(BattleEnemyState::Dedge, EnemyTask::Compound::Dedge);
			htn->AddCompoundTask(BattleEnemyState::Attack, EnemyTask::Compound::Attack);
		}
	}
}

// 初期化
void EnemyAIController::Initialize()
{
	// データ初期化
	{
		aiData->Initialize();
	}

	// ステートマシン開始
	{
		status->Start(BattleEnemyState::Idle, owner.lock(), aiData);
	}

	// HTN初期化
	{
		htn->Initialize();
	}
}

// 終了化
void EnemyAIController::Finalize()
{

}

// 更新
void EnemyAIController::Update(Phoenix::f32 elapsedTime)
{
	// ステート遷移
	{
		ChangeState(elapsedTime);
	}

	// ステート更新
	{
		status->Update(owner.lock(), aiData);
	}
}

// ステート遷移
void EnemyAIController::ChangeState(Phoenix::f32 elapsedTime)
{
	// 次に遷移したいステートがある場合
	if (nextState != BattleEnemyState::NoneState)
	{
		// プランニング
		htn->ClearPlan();
		bool result = htn->Planning(*worldState.get(), nextState);

		// データ更新
		auto plan = htn->GetPlan();
		plan.emplace_back(BattleEnemyState::Idle);

		if (result) // プランニング成功
		{
			aiData->changeState = true;
			aiData->nextState = plan.at(0);
			aiData->plan = plan;
			aiData->planIndex = 0;
			aiData->elapsedTime = elapsedTime;
		}
		else // プランニング失敗
		{
			aiData->changeState = true;
			aiData->nextState = nextState;
			aiData->plan = plan;
			aiData->planIndex = -1;
			aiData->elapsedTime = elapsedTime;
		}

		// ステートマシン再開始
		//status->Start(BattleEnemyState::Idle, owner.lock(), aiData);

		// 次のフレームでステート遷移しないために初期化
		nextState = BattleEnemyState::NoneState;
	}
}

// HTNのワールドステートを設定
void EnemyAIController::SetWorldState(std::shared_ptr<EnemyWorldState> worldState)
{
	this->worldState = worldState;
}

// 次に実行したいステートを設定
void EnemyAIController::SetNextState(BattleEnemyState nextState)
{
	this->nextState = nextState;
}

// 現在のステートを取得
BattleEnemyState EnemyAIController::GetState()
{
	return status->GetCurrenState();
}