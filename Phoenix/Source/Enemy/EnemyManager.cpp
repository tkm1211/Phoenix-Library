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
	originEnemies.resize(EnemyRange);
	enemiesPos.resize(EnemyRange);
	enemiesState.resize(EnemyRange);
	enemiesType.resize(EnemyRange);

	for (Phoenix::s32 i = 0; i < EnemyRange; ++i)
	{
		enemiesPos.at(i) = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		enemiesState.at(i) = -1;
		enemiesType.at(i) = -1;
	}

	for (auto& enemy : originEnemies)
	{
		enemy = std::make_shared<Enemy>();
		enemy->Construct(graphicsDevice);
	}

	boss = Boss::Create();
	boss->Construct(graphicsDevice);

	enemiesUI = EnemiesUI::Create();
	enemiesUI->Initialize(graphicsDevice, EnemyRange);

	aliveEnemyCount = 0;
}

// 初期化
void EnemyManager::Initialize()
{
	//enemies.clear();

	for (Phoenix::s32 i = 0; i < EnemyRange; ++i)
	{
		enemiesPos.at(i) = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		enemiesState.at(i) = -1;
	}
}

// 終了化
void EnemyManager::Finalize()
{
	for (auto& enemy : enemies)
	{
		if (!enemy) continue;

		enemy->Finalize();
		enemy.reset();
	}
	enemies.clear();
}

// 更新
void EnemyManager::Update(bool onControl, Phoenix::f32 elapsedTime)
{
	Phoenix::s32 cnt = 0;
	for (auto enemy : enemies)
	{
		if (!enemy) continue;
		enemy->Update(onControl, elapsedTime);

		enemiesPos.at(cnt) = enemy->GetPosition();
		enemiesState.at(cnt) = static_cast<Phoenix::s32>(enemy->GetBattleState());
		enemiesType.at(cnt) = static_cast<Phoenix::s32>(enemy->GetTypeTag());

		++cnt;
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

	enemiesUI->Update(index, (hp / enemies.at(index)->GetLifeMax()) * 100.0f);
}

// 描画
void EnemyManager::Draw()
{

}

// エネミー追加
void EnemyManager::AddEnemy(Enemy::TypeTag tag, Phoenix::FrameWork::Transform transform)
{
	for (Phoenix::s32 i = 0; i < enemies.size(); ++i)
	{
		if (enemies.at(i))
		{
			if (enemies.at(i)->GetEnable()) continue;
		}

		if (tag == Enemy::TypeTag::Small)
		{
			enemies.at(i) = originEnemies.at(useOriginCnt);
			transform.SetScale({ 1.0f,1.0f,1.0f });
			++useOriginCnt;
		}
		else if (tag == Enemy::TypeTag::Large)
		{
			enemies.at(i) = boss;
			transform.SetScale({ 1.4f,1.4f,1.4f });
		}

		enemies.at(i)->Initialize();
		enemies.at(i)->SetEnable(true);
		enemies.at(i)->SetAlive(true);
		enemies.at(i)->SetDeath(false);
		enemies.at(i)->SetTransform(transform);
		enemies.at(i)->SetOwner(shared_from_this());
		enemies.at(i)->SetPlayer(player);

		enemiesUI->AddUI(i, enemies.at(i)->GetUI());

		++aliveEnemyCount;
		++battleEnemyCount; // TODO : delete.

		break;
	}
}

// エネミー達解除
void EnemyManager::ResetEnemies()
{
	for (auto enemy : originEnemies)
	{
		enemy->SetEnable(false);
		enemy->SetAlive(false);
		enemy->SetDeath(false);
		enemy->SetState(BattleEnemyState::Idle);
	}
	{
		boss->SetEnable(false);
		boss->SetAlive(false);
		boss->SetDeath(false);
		boss->SetState(BattleEnemyState::Idle);
	}

	for (Phoenix::s32 i = 0; i < enemies.size(); ++i)
	{
		enemies.at(i) = nullptr;
	}

	aliveEnemyCount = 0;
	battleEnemyCount = 0;
	useOriginCnt = 0;
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

// エネミー達の座標を取得
std::vector<Phoenix::Math::Vector3>& EnemyManager::GetEnemiesPos()
{
	return enemiesPos;
}

// エネミー達の状態を取得
std::vector<Phoenix::s32>& EnemyManager::GetEnemiesState()
{
	return enemiesState;
}

// エネミー達の種類を取得
std::vector<Phoenix::s32>& EnemyManager::GetEnemiesType()
{
	return enemiesType;
}