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
	notAttack = false;
	notAttackTime = 0;
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

	//bool inBattle = false;
	bool attackRight = true;
	bool dedgeRight = true;

	Phoenix::s32 index = -1;
	Phoenix::s32 nearEnemyIndex = -1;
	std::vector<Phoenix::s32> indices;

	Phoenix::f32 len = 0.0f;

	const auto& enemies = enemyManager->GetEnemies();

	switch (battleEnemyState)
	{
	case BattleEnemyState::Idle:
		for (const auto& enemy : enemies)
		{
			if (!enemy->GetAlive()) continue;
			if (!enemy->GetInBattle()) continue;

			if (enemy->GetBattleState() == BattleEnemyState::Idle)
			{
				if (enemy->InBattleTerritory())
				{
					enemy->SetState(BattleEnemyState::Walk);
				}
				else
				{
					enemy->SetState(BattleEnemyState::Run);
				}
			}
		}
		break;

	case BattleEnemyState::Run:
		for (const auto& enemy : enemies)
		{
			if (!enemy->GetAlive()) continue;
			if (!enemy->GetInBattle()) continue;

			if (enemy->GetBattleState() == BattleEnemyState::Idle)
			{
				if (enemy->InBattleTerritory())
				{
					enemy->SetState(BattleEnemyState::Walk);
				}
				else
				{
					enemy->SetState(BattleEnemyState::Run);
				}
			}
		}
		break;

	case BattleEnemyState::Attack:
		for (const auto& enemy : enemies)
		{
			++index;

			if (!enemy->GetAlive()) continue;
			if (!enemy->GetInBattle()) continue;
			if (enemy->GetBattleState() == BattleEnemyState::DamageSmall) continue;
			if (enemy->GetBattleState() == BattleEnemyState::DamageBig) continue;

			if (enemy->GetBattleState() == BattleEnemyState::Idle)
			{
				if (!enemy->InDistanceHitByAttack())
				{
					if (enemy->InBattleTerritory())
					{
						enemy->SetState(BattleEnemyState::Walk);
						continue;
					}
					else
					{
						enemy->SetState(BattleEnemyState::Run);
						continue;
					}
				}
			}
			else if (enemy->GetBattleState() == BattleEnemyState::Attack)
			{
				attackRight = false;
			}

			if (!enemy->InBattleTerritory()) continue;

			if (enemy->GetBattleState() == BattleEnemyState::Idle || enemy->GetBattleState() == BattleEnemyState::Walk || enemy->GetBattleState() == BattleEnemyState::Run)
			{
				indices.emplace_back(index);
			}
		}

		if (/*!notAttack && */attackRight /*&& inBattle*/ && 0 < indices.size())
		{
			// TODO : Idleステートの時に再抽選
			Phoenix::s32 r = rand() % static_cast<Phoenix::s32>(indices.size());
			/*if (enemies.at(indices.at(r))->GetBattleState() == BattleEnemyState::DamageSmall || enemies.at(indices.at(r))->GetBattleState() == BattleEnemyState::DamageBig)
			{
				attackRight = attackRight;
			}*/
			if (enemies.at(indices.at(r))->InBattleTerritory())
			{
				notAttack = true;
				enemyManager->SetAttackRight(indices.at(r), (enemyManager->GetAliveEnemyCount() == 1));
			}
		}
		else if (notAttack)
		{
			if (10 <= notAttackTime++)
			{
				notAttack = false;
				notAttackTime = 0;
			}
		}

		break;

	case BattleEnemyState::Dedge:
		for (const auto& enemy : enemies)
		{
			++index;

			if (!enemy->GetAlive()) continue;
			if (!enemy->GetInBattle()) continue;
			if (!enemy->InBattleTerritory()) continue;
			if (!enemy->JudgePlayerAttackRange()) continue;

			if (enemy->GetBattleState() == BattleEnemyState::Dedge)
			{
				dedgeRight = false;
			}

			if (enemy->GetBattleState() != BattleEnemyState::Dedge
			 && enemy->GetBattleState() != BattleEnemyState::DamageSmall
			 && enemy->GetBattleState() != BattleEnemyState::DamageBig
			 && enemy->GetBattleState() != BattleEnemyState::Attack)
			{
				Phoenix::f32 dis = enemy->GetDistanceToPlayer();

				if (dis < len || len == 0.0f)
				{
					nearEnemyIndex = index;
					len = dis;
				}
			}
		}

		if (dedgeRight && 0 <= nearEnemyIndex && nearEnemyIndex < enemies.size())
		{
			enemies.at(nearEnemyIndex)->SetState(BattleEnemyState::Dedge);
		}
		break;

	default: break;
	}
}

// エネミーマネージャー設定
void BattleEnemyController::SetEnemyManager(std::shared_ptr<EnemyManager> enemyManager)
{
	this->enemyManager = enemyManager;
}