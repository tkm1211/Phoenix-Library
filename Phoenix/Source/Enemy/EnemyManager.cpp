#include "EnemyManager.h"
#include "Enemy.h"
#include "../Player/Player.h"


// 生成
std::shared_ptr<EnemyManager> EnemyManager::Create()
{
	return std::make_shared<EnemyManager>();
}

// コンストラクタ
void EnemyManager::Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	enemies.resize(EnemyRange);

	for (auto& enemy : enemies)
	{
		enemy = std::make_shared<Enemy>();
		enemy->Construct(graphicsDevice);
	}

	aliveEnemyCount = 0;
}

// 初期化
void EnemyManager::Initialize()
{
	//enemies.clear();
}

// 終了化
void EnemyManager::Finalize()
{
	for (auto& enemy : enemies)
	{
		enemy->Finalize();
		enemy.reset();
	}
	enemies.clear();
}

// 更新
void EnemyManager::Update()
{
	for (auto enemy : enemies)
	{
		enemy->Update();
	}
}

// 描画
void EnemyManager::Draw()
{

}

// エネミー追加
void EnemyManager::AddEnemy(Phoenix::FrameWork::Transform transform)
{
	for (auto enemy : enemies)
	{
		if (!enemy->GetEnable())
		{
			enemy->SetEnable(true);
			enemy->SetAlive(true);
			enemy->SetTransform(transform);
			enemy->SetOwner(shared_from_this());
			enemy->SetPlayer(player);

			++aliveEnemyCount;
			++battleEnemyCount; // TODO : delete.

			break;
		}
	}
}

// エネミー生存最大数からカウントダウン
void EnemyManager::SubAliveEnemyCount(Phoenix::s32 sub)
{
	aliveEnemyCount -= sub;
}

// 指定のエネミーに攻撃権を発行
void EnemyManager::SetAttackRight(Phoenix::s32 enemyIndex)
{
	enemies.at(enemyIndex)->SetAttackRight();
}

// 指定のエネミーをバトルモードに変更
void EnemyManager::SetBattleEnemy(Phoenix::s32 enemyIndex)
{
	enemies.at(enemyIndex)->SetInBattle(true);
}

// プレイヤーを設定
void EnemyManager::SetPlayer(std::shared_ptr<Player> player)
{
	this->player = player;
}

// GUI
void EnemyManager::GUI()
{
	if (ImGui::TreeNode("EnemyManager"))
	{
		Phoenix::s32 index = 0;

		for (auto enemy : enemies)
		{
			if (enemy->GetEnable())
			{
				enemy->GUI(index);
			}

			++index;
		}
		ImGui::TreePop();
	}
}

// エネミー達を取得
std::vector<std::shared_ptr<Enemy>>& EnemyManager::GetEnemies()
{
	return enemies;
}

// エネミー限界最大数
Phoenix::s32 EnemyManager::GetEnemyRange()
{
	return EnemyRange;
}

// エネミー生存最大数
Phoenix::s32 EnemyManager::GetAliveEnemyCount()
{
	return aliveEnemyCount;
}

// バトル中のエネミー最大数
Phoenix::s32 EnemyManager::GetBattleEnemyCount()
{
	return battleEnemyCount;
}