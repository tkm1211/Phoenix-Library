#include "BattleEnemyController.h"
#include "../../Enemy/EnemyManager.h"
#include "../../Enemy/Enemy.h"


// 生成
std::unique_ptr<BattleEnemyController> BattleEnemyController::Create()
{
	return std::make_unique<BattleEnemyController>();
}

// コンストラクタ
void BattleEnemyController::Construct()
{

}

// 初期化
void BattleEnemyController::Initialize()
{

}

// 終了化
void BattleEnemyController::Finalize()
{

}

// 更新
void BattleEnemyController::Update(BattleEnemyState battleEnemyState)
{
	if (enemyManager->GetAliveEnemyCount() <= 0) return;
	if (enemyManager->GetBattleEnemyCount() <= 0) return;

	bool inBattle = false;
	bool attackRight = true;
	Phoenix::s32 index = 0;
	std::vector<Phoenix::s32> indices;

	switch (battleEnemyState)
	{
	case BattleEnemyState::Idle:

		break;

	case BattleEnemyState::Attack:
		for (const auto& enemy : enemyManager->GetEnemies())
		{
			if (!enemy->GetAlive()) continue;
			if (!enemy->GetInBattle()) continue;

			if (enemy->GetBattleState() == BattleEnemyState::Attack)
			{
				attackRight = false;
			}

			inBattle = true;

			indices.emplace_back(index);
			++index;
		}

		if (attackRight && inBattle)
		{
			Phoenix::s32 r = rand() % static_cast<Phoenix::s32>(indices.size());
			enemyManager->SetAttackRight(indices.at(r));
		}

		break;

	case BattleEnemyState::Dedge:

		break;

	default: break;
	}
}

// エネミーマネージャー設定
void BattleEnemyController::SetEnemyManager(std::shared_ptr<EnemyManager> enemyManager)
{
	this->enemyManager = enemyManager;
}