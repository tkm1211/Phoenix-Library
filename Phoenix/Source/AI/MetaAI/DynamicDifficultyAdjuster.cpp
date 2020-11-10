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
	if (skillLevel <= 10)
	{
		ideaState = BattleEnemyState::Attack;
	}
	/*else if (skillLevel <= 30)
	{
		
	}*/
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