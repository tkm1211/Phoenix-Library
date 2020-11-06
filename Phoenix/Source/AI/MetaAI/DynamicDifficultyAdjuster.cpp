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
	ideaState = EnenyBattleState::NoneState;
}

// �I����
void DynamicDifficultyAdjuster::Finalize()
{
	ideaState = EnenyBattleState::NoneState;
}

// �X�V
void DynamicDifficultyAdjuster::Update(Phoenix::s32 skillLevel)
{
	if (skillLevel <= 10)
	{
		ideaState = EnenyBattleState::Idle;
	}
	else if (skillLevel <= 50)
	{
		ideaState = EnenyBattleState::Attack;
	}
}

// �G�l�~�[�X�e�[�g�̎擾
EnenyBattleState DynamicDifficultyAdjuster::GetEnenyBattleState()
{
	return ideaState;
}