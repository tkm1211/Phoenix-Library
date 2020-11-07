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
	ideaState = BattleEnenyState::NoneState;
}

// �I����
void DynamicDifficultyAdjuster::Finalize()
{
	ideaState = BattleEnenyState::NoneState;
}

// �X�V
void DynamicDifficultyAdjuster::Update(Phoenix::s32 skillLevel)
{
	if (skillLevel <= 10)
	{
		ideaState = BattleEnenyState::Idle;
	}
	else if (skillLevel <= 50)
	{
		ideaState = BattleEnenyState::Attack;
	}
}

// �G�l�~�[�X�e�[�g�̎擾
BattleEnenyState DynamicDifficultyAdjuster::GetBattleEnenyState()
{
	return ideaState;
}