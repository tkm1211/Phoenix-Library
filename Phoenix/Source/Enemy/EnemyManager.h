#pragma once

#include <vector>
#include <memory>
#include "Enemy.h"
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/FrameWork/Component/Transform.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "../UI/EnemiesUI.h"
#include "../Boss/Boss.h"


//class Enemy;
class Player;
class EnemyManager : public std::enable_shared_from_this<EnemyManager>
{
public:
	static constexpr Phoenix::s32 EnemyRange = 5;

private:
	std::vector<std::shared_ptr<Enemy>> enemies;
	std::vector<std::shared_ptr<Enemy>> originEnemies;
	std::shared_ptr<Boss> boss;

	Phoenix::s32 aliveEnemyCount = 0;
	Phoenix::s32 battleEnemyCount = 0;

	std::shared_ptr<Player> player;
	std::shared_ptr<EnemiesUI> enemiesUI;

	Phoenix::s32 useOriginCnt = 0;

public:
	EnemyManager() {}
	~EnemyManager()
	{
		Finalize(); 
	}

public:
	// 生成
	static std::shared_ptr<EnemyManager> Create();

	// コンストラクタ
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	// 更新
	void Update(bool onControl, Phoenix::f32 elapsedTime);

	// UI更新
	void UpdateUI(Phoenix::s32 index);

	// 描画
	void Draw();

	// GUI
	void GUI();

public:
	// エネミー追加
	void AddEnemy(Enemy::TypeTag tag, Phoenix::FrameWork::Transform transform);

	// エネミー達解除
	void ResetEnemies();

	// エネミー生存最大数からカウントダウン
	void SubAliveEnemyCount(Phoenix::s32 sub);

	// 指定のエネミーに攻撃権を発行
	bool SetAttackRight(Phoenix::s32 enemyIndex, bool stackAttackRight);

	// 指定のエネミーをバトルモードに変更
	void SetBattleEnemy(Phoenix::s32 enemyIndex);

	// プレイヤーを設定
	void SetPlayer(std::shared_ptr<Player> player);

public:
	// エネミー達を取得
	std::vector<std::shared_ptr<Enemy>>& GetEnemies();

	// エネミー限界最大数
	Phoenix::s32 GetEnemyRange();

	// エネミー生存最大数
	Phoenix::s32 GetAliveEnemyCount();

	// バトル中のエネミー最大数
	Phoenix::s32 GetBattleEnemyCount();

	// エネミーUIを取得
	std::shared_ptr<EnemiesUI> GetEnemiesUI();
};