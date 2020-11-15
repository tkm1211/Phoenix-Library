#include "DynamicDifficultyAdjuster.h"


// ����
std::unique_ptr<DynamicDifficultyAdjuster> DynamicDifficultyAdjuster::Create()
{
	return std::make_unique<DynamicDifficultyAdjuster>();
}

// �R���X�g���N�^
void DynamicDifficultyAdjuster::Construct()
{
	
}

// ������
void DynamicDifficultyAdjuster::Initialize()
{
	ideaState = BattleEnemyState::NoneState;
}

// �I����
void DynamicDifficultyAdjuster::Finalize()
{
	ideaState = BattleEnemyState::NoneState;
}

// �X�V
void DynamicDifficultyAdjuster::Update(Phoenix::s32 skillLevel)
{
	if (0 < skillLevel && skillLevel <= 10)
	{
		Phoenix::s32 percent = rand() % 100;
		if (percent < 65)
		{
			ideaState = BattleEnemyState::Idle;
		}
		else if (percent < 100)
		{
			ideaState = BattleEnemyState::Dedge;
		}
	}
	else if (skillLevel < 0)
	{
		ideaState = BattleEnemyState::Run;
	}
	else
	{
		ideaState = BattleEnemyState::Attack;
	}
}

// �G�l�~�[�X�e�[�g�̎擾
BattleEnemyState DynamicDifficultyAdjuster::GetBattleEnemyState()
{
	return ideaState;
}