#include "EnemyManager.h"
#include "Enemy.h"
#include "../Player/Player.h"
//#include "../UI/EnemiesUI.h"


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

	enemiesUI = EnemiesUI::Create();
	enemiesUI->Initialize(graphicsDevice, EnemyRange);

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
void EnemyManager::Update(bool onControl)
{
	for (auto enemy : enemies)
	{
		enemy->Update(onControl);
	}
}

// UI更新
void EnemyManager::UpdateUI(Phoenix::s32 index)
{
	if (index <= -1 || enemies.size() <= index)
	{
		enemiesUI->Update(index, 0.0f);
		return;
	}

	Phoenix::s32 life = enemies.at(index)->GetLife();

	Phoenix::f32 hp = static_cast<Phoenix::f32>(life);
	hp = hp <= 0 ? 0 : hp;

	enemiesUI->Update(index, (hp / enemies.at(index)->LifeRange) * 100.0f);
}

// 描画
void EnemyManager::Draw()
{

}

// エネミー追加
void EnemyManager::AddEnemy(Phoenix::FrameWork::Transform transform)
{
	Phoenix::s32 index = 0;
	for (auto enemy : enemies)
	{
		if (!enemy->GetEnable())
		{
			enemy->Initialize();
			enemy->SetEnable(true);
			enemy->SetAlive(true);
			enemy->SetDeath(false);
			enemy->SetTransform(transform);
			enemy->SetOwner(shared_from_this());
			enemy->SetPlayer(player);

			enemiesUI->AddUI(index, enemy->GetUI());

			++aliveEnemyCount;
			++battleEnemyCount; // TODO : delete.

			break;
		}

		++index;
	}
}

// エネミー達解除
void EnemyManager::ResetEnemies()
{
	for (auto enemy : enemies)
	{
		enemy->SetEnable(false);
		enemy->SetAlive(false);
		enemy->SetDeath(false);
		enemy->SetState(BattleEnemyState::Idle);
	}
}

// エネミー生存最大数からカウントダウン
void EnemyManager::SubAliveEnemyCount(Phoenix::s32 sub)
{
	aliveEnemyCount -= sub;
}

// 指定のエネミーに攻撃権を発行
bool EnemyManager::SetAttackRight(Phoenix::s32 enemyIndex, bool stackAttackRight)
{
	return enemies.at(enemyIndex)->SetAttackRight(stackAttackRight);
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

// エネミーUIを取得
std::shared_ptr<EnemiesUI> EnemyManager::GetEnemiesUI()
{
	return enemiesUI;
}