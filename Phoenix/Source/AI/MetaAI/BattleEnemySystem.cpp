#include "BattleEnemySystem.h"


namespace Meta
{
	// 生成
	std::shared_ptr<BattleEnemySystem> BattleEnemySystem::Create()
	{
		return std::make_shared<BattleEnemySystem>();
	}

	// コンストラクタ
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

	// 初期化
	void BattleEnemySystem::Initialize()
	{
		playerSkillAnalyzer->Initialize();
		dynamicDifficultyAdjuster->Initialize();
		battleEnemyController->Initialize();
	}

	// 終了化
	void BattleEnemySystem::Finalize()
	{
		battleEnemyController->Initialize();
		dynamicDifficultyAdjuster->Initialize();
		playerSkillAnalyzer->Initialize();
	}

	// 更新
	void BattleEnemySystem::Update(Phoenix::s32 score)
	{
		playerSkillAnalyzer->Update(score);
		dynamicDifficultyAdjuster->Update(playerSkillAnalyzer->GetSkillLevel());
		battleEnemyController->Update(dynamicDifficultyAdjuster->GetBattleEnemyState());
	}

	// 管理クラスの設定
	void BattleEnemySystem::SetManager(std::shared_ptr<EnemyManager> manager)
	{
		battleEnemyController->SetEnemyManager(manager);
	};
}