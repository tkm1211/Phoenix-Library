#include "BattleEnemyController.h"
#include "../../Enemy/EnemyManager.h"


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
void BattleEnemyController::Update(BattleEnenyState battleEnenyState)
{
	switch (battleEnenyState)
	{
	case BattleEnenyState::Idle:
		if (0 < enemyManager->GetAliveEnemyCount())
		{
			if (0 < enemyManager->GetBattleEnemyCount())
			{

			}
		}

		break;

	case BattleEnenyState::Attack:

		break;

	default: break;
	}
}

// エネミーマネージャー設定
void BattleEnemyController::SetEnemyManager(std::shared_ptr<EnemyManager> enemyManager)
{
	this->enemyManager = enemyManager;
}