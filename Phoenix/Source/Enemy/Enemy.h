#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Component/Transform.h"
#include "EnemyState.h"
#include "../AI/StateMachine/BattleEnemyAI.h"


class EnemyManager;
class Player;
class Enemy : public std::enable_shared_from_this<Enemy>
{
private:
	static constexpr Phoenix::s32 LifeRange = 50;

public:
	enum class TypeTag
	{
		Small,  // 雑魚
		Medium, // 中ボス
		Large   // ボス
	};

private:
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;
	std::unique_ptr<Phoenix::FrameWork::Transform> transform;
	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;

	// AI
	EnemyMode currentMode = EnemyMode::Ordinary;
	//std::shared_ptr<OrdinaryAIEnemyAI> ordinaryAI;
	std::shared_ptr<BattleEnemyAI> battleAI;

	// マネージャー
	std::weak_ptr<EnemyManager> owner;

	// プレイヤー
	std::shared_ptr<Player> player;

	// アニメーション
	bool changeAnimation = false;
	bool changeAttackAnimation = false;
	BattleEnemyState changeState = BattleEnemyState::NoneState;
	EnemyAttackState changeAttackState = EnemyAttackState::NoneState;

	// パラメーター
	Phoenix::s32 life = 0;
	Phoenix::f32 radius = 0.0f;

	// コリジョンデータの要素数
	Phoenix::u32 attackCollisionIndex = 0;

	// フラグ
	bool enable = false;
	bool alive = false;
	bool inBattle = false;

	// アタックが当たったか？
	bool isHit = false;

	// アタックの判定中か？
	bool isAttackJudgment = false;

public:
	Enemy() {}
	~Enemy()
	{
		//Finalize();
	}

public:
	// 生成
	static std::shared_ptr<Enemy> Create();

	// コンストラクタ
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	// 更新
	void Update();

	// UI更新
	void UpdateUI();

	// 描画
	void Draw();

	// GUI
	void GUI(Phoenix::s32 index);

public:
	// 有効フラグ設定
	void SetEnable(bool enable);

	// 存在フラグ設定
	void SetAlive(bool alive);

	// 戦闘中フラグ設定
	void SetInBattle(bool inBattle);

	// 攻撃ヒット設定
	void SetIsHit(bool isHit) { this->isHit = isHit; }

	// トランスフォームの設定
	void SetTransform(Phoenix::FrameWork::Transform transform);

	// トランスレートの設定
	void SetTranslate(Phoenix::Math::Vector3 translate);

	// エネミーマネージャーの設定
	void SetOwner(std::shared_ptr<EnemyManager> owner);

	// 攻撃権を発行
	void SetAttackRight();

	// 攻撃ステートを変更
	void SetAttackState(EnemyAttackState state);

	// プレイヤーを設定
	void SetPlayer(std::shared_ptr<Player> player);

	// アニメーションを移行
	void ChangeAnimation();

	// 攻撃ステートを移行
	void ChangeAttackAnimation();

	// ダメージ
	void Damage(int damage);

public:
	// 有効フラグ取得
	bool GetEnable();

	// 存在フラグ取得
	bool GetAlive();

	// 戦闘中フラグ取得
	bool GetInBattle();

	// トランスフォームの取得
	Phoenix::FrameWork::Transform GetTransform();

	// バトルモードのステート取得
	BattleEnemyState GetBattleState();


	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }

	const std::vector<Phoenix::FrameWork::CollisionData>* GetCollisionDatas() { return &collisionDatas; }

	Phoenix::Math::Matrix GetWorldMatrix() { return transform->GetWorldTransform(); }

	Phoenix::Math::Vector3 GetPosition() { return transform->GetTranslate(); }

	Phoenix::f32 GetRadius() { return radius; }

	bool IsAttackJudgment() { return isAttackJudgment; }

	Phoenix::u32 GetAttackCollisionIndex() { return attackCollisionIndex; }
};