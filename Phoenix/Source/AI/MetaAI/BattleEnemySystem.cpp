#include "BattleEnemySystem.h"


namespace Meta
{
	// ����
	std::shared_ptr<BattleEnemySystem> BattleEnemySystem::Create()
	{
		return std::make_shared<BattleEnemySystem>();
	}

	// �R���X�g���N�^
	void BattleEnemySystem::Construct()
	{
		if (constructed) return;

		playerSkillAnalyzer = PlayerSkillAnalyzer::Create();
		dynamicDifficultyAdjuster = DynamicDifficultyAdjuster::Create();
		battleEnemyController = BattleEnemyController::Create();

		playerSkillAnalyzer->Construct();
		dynamicDifficultyAdjuster->Construct();
		battleEnemyController->Construct();

		constructed = true;
	}

	// ������
	void BattleEnemySystem::Initialize()
	{
		playerSkillAnalyzer->Initialize();
		dynamicDifficultyAdjuster->Initialize();
		battleEnemyController->Initialize();
	}

	// �I����
	void BattleEnemySystem::Finalize()
	{
		battleEnemyController->Initialize();
		dynamicDifficultyAdjuster->Initialize();
		playerSkillAnalyzer->Initialize();
	}

	// �X�V
	void BattleEnemySystem::Update(Phoenix::s32 score)
	{
		playerSkillAnalyzer->Update(score);
		dynamicDifficultyAdjuster->Update(playerSkillAnalyzer->GetSkillLevel());
		battleEnemyController->Update(dynamicDifficultyAdjuster->GetBattleEnemyState());
	}

	// �Ǘ��N���X�̐ݒ�
	void BattleEnemySystem::SetManager(std::shared_ptr<EnemyManager> manager)
	{
		battleEnemyController->SetEnemyManager(manager);
	};
}