#include "EnemyAIController.h"
#include "EnemyTask.h"


// ����
std::shared_ptr<EnemyAIController> EnemyAIController::Create()
{
	return std::make_shared<EnemyAIController>();
}

// �R���X�g���N�^
void EnemyAIController::Construct(std::weak_ptr<Enemy> owner)
{
	// �G�l�~�[�ݒ�
	{
		this->owner = owner;
	}

	// AI�p�f�[�^
	{
		aiData = std::make_shared<BattleEnemy::AIData>();
	}

	// �X�e�[�g
	{
		// ����
		{
			status = FSM<BattleEnemyState, std::shared_ptr<Enemy>, std::shared_ptr<BattleEnemy::AIData>>::Create();
		}

		// �����ǉ�
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

		// �������ǉ�
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

		// �X�V�ǉ�
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

		// �I�����ǉ�
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
		// ����
		{
			htn = HTN<BattleEnemyState, EnemyWorldState, std::shared_ptr<Enemy>>::Create();
			htn->Construct();
		}

		// �v���~�e�B�u�^�X�N��ǉ�
		{
			htn->AddPrimitiveTask(BattleEnemyState::Walk, EnemyTask::Primitive::Walk);
			htn->AddPrimitiveTask(BattleEnemyState::Run, EnemyTask::Primitive::Run);
			htn->AddPrimitiveTask(BattleEnemyState::Dedge, EnemyTask::Primitive::Dedge);
			htn->AddPrimitiveTask(BattleEnemyState::Attack, EnemyTask::Primitive::Attack);
		}

		// �����^�X�N��ǉ�
		{
			htn->AddCompoundTask(BattleEnemyState::Walk, EnemyTask::Compound::Walk);
			htn->AddCompoundTask(BattleEnemyState::Run, EnemyTask::Compound::Run);
			htn->AddCompoundTask(BattleEnemyState::Dedge, EnemyTask::Compound::Dedge);
			htn->AddCompoundTask(BattleEnemyState::Attack, EnemyTask::Compound::Attack);
		}
	}
}

// ������
void EnemyAIController::Initialize()
{
	// �f�[�^������
	{
		aiData->Initialize();
	}

	// �X�e�[�g�}�V���J�n
	{
		status->Start(BattleEnemyState::Idle, owner.lock(), aiData);
	}

	// HTN������
	{
		htn->Initialize();
	}
}

// �I����
void EnemyAIController::Finalize()
{

}

// �X�V
void EnemyAIController::Update(Phoenix::f32 elapsedTime)
{
	// �X�e�[�g�J��
	{
		ChangeState(elapsedTime);
	}

	// �X�e�[�g�X�V
	{
		status->Update(owner.lock(), aiData);
	}
}

// �X�e�[�g�J��
void EnemyAIController::ChangeState(Phoenix::f32 elapsedTime)
{
	// ���ɑJ�ڂ������X�e�[�g������ꍇ
	if (nextState != BattleEnemyState::NoneState)
	{
		// �v�����j���O
		htn->ClearPlan();
		bool result = htn->Planning(*worldState.get(), nextState);

		// �f�[�^�X�V
		auto plan = htn->GetPlan();
		plan.emplace_back(BattleEnemyState::Idle);

		if (result) // �v�����j���O����
		{
			aiData->changeState = true;
			aiData->nextState = plan.at(0);
			aiData->plan = plan;
			aiData->planIndex = 0;
			aiData->elapsedTime = elapsedTime;
		}
		else // �v�����j���O���s
		{
			aiData->changeState = true;
			aiData->nextState = nextState;
			aiData->plan = plan;
			aiData->planIndex = -1;
			aiData->elapsedTime = elapsedTime;
		}

		// �X�e�[�g�}�V���ĊJ�n
		//status->Start(BattleEnemyState::Idle, owner.lock(), aiData);

		// ���̃t���[���ŃX�e�[�g�J�ڂ��Ȃ����߂ɏ�����
		nextState = BattleEnemyState::NoneState;
	}
}

// HTN�̃��[���h�X�e�[�g��ݒ�
void EnemyAIController::SetWorldState(std::shared_ptr<EnemyWorldState> worldState)
{
	this->worldState = worldState;
}

// ���Ɏ��s�������X�e�[�g��ݒ�
void EnemyAIController::SetNextState(BattleEnemyState nextState)
{
	this->nextState = nextState;
}

// ���݂̃X�e�[�g���擾
BattleEnemyState EnemyAIController::GetState()
{
	return status->GetCurrenState();
}