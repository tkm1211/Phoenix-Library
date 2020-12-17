#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Component/Transform.h"
#include "EnemyState.h"
#include "../AI/StateMachine/BattleEnemyAI.h"
#include "../UI/EnemyUI.h"


class EnemyManager;
class Player;
class Enemy : public std::enable_shared_from_this<Enemy>
{
public:
	static constexpr Phoenix::s32 LifeRange = 150;

public:
	enum class TypeTag
	{
		Small,  // 雑魚
		Medium, // 中ボス
		Large   // ボス
	};

	enum class LayerType
	{
		Base,
		LowerBody,
	};

	enum class StateType
	{
		Idle,
		Run,
		DamageSmall,
		DamageBig,
		Dedge,
		Death,
	};

	struct AttackData
	{
		EnemyAttackState animState; // アニメーションステート
		Phoenix::s32 animIndex; // アニメーションレイヤーの番号

		Phoenix::f32 playSpeed; // 再生スピード
		Phoenix::f32 playBeginTime; // 再生開始時間（再生フレーム）
		Phoenix::f32 playEndTime; // 再生終了時間（再生フレーム）

		Phoenix::s32 collisionNum; // 当たり判定の番号
		Phoenix::f32 collisionBeginTime; // 当たり判定開始時間（再生フレーム）
		Phoenix::f32 collisionEndTime; // 当たり判定終了時間（再生フレーム）
	};

	struct AttackDatas
	{
		std::vector<AttackData> datas;

		void AddData(AttackData data)
		{
			datas.emplace_back(data);
		}
	};

private:
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;
	std::unique_ptr<Phoenix::FrameWork::Transform> transform;
	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;
	std::vector<AttackDatas> attackDatasList;
	std::shared_ptr<EnemyUI> ui;

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
	EnemyAttackState currentAttackState = EnemyAttackState::NoneState;

	// パラメーター
	Phoenix::s32 life = 0;
	Phoenix::f32 radius = 0.0f;

	// 新たな回転値
	Phoenix::Math::Quaternion newRotate;

	// コリジョンデータの要素数
	Phoenix::u32 attackCollisionIndex = 0;

	// 攻撃の威力
	Phoenix::u32 attackPower = 0;

	// プレイヤーとの距離
	Phoenix::f32 distanceToPlayer = 0.0f;

	// 移動スピード
	Phoenix::f32 moveSpeed = 0.0f;

	// 移動方向
	Phoenix::f32 moveX = 0.0f;
	Phoenix::f32 moveY = 0.0f;

	// フラグ
	bool enable = false;
	bool alive = false;
	bool death = false;
	bool inBattle = false;

	// アタックが当たったか？
	bool isHit = false;

	// アタックの判定中か？
	bool isAttackJudgment = false;

	// 攻撃アニメーション時間
	float attackReceptionTimeCnt = 0.0f;
	float attackAnimationSpeed = 0.0f;

	// 攻撃権
	bool stackAttackRight = false;

	// レイヤー番号
	std::map<LayerType, Phoenix::s32> layerIndexList;

	// レイヤー内のステート番号
	std::map<StateType, Phoenix::s32> stateIndexList;

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
	void Update(bool onControl, Phoenix::f32 elapsedTime);

	// トランスフォーム更新
	void UpdateTrasform();

	// UI更新
	void UpdateUI(Phoenix::Math::Vector2 pos);

	// 描画
	void Draw();

	// GUI
	void GUI(Phoenix::s32 index);

	// アニメーションを移行
	void ChangeAnimation();

	// 攻撃ステートを移行
	void ChangeAttackAnimation();

	// 攻撃判定
	void AttackJudgment();

	// プレイヤーとの距離計測
	void DistanceMeasurement();

	// プレイヤーの攻撃視野にいるか判定
	bool JudgePlayerAttackRange();

	// 新たな回転値の更新
	void UpdateNewRotate();

public:
	// 有効フラグ設定
	void SetEnable(bool enable);

	// 存在フラグ設定
	void SetAlive(bool alive);

	// 死亡フラグ設定
	void SetDeath(bool death);

	// 戦闘中フラグ設定
	void SetInBattle(bool inBattle);

	// 攻撃ヒット設定
	void SetIsHit(bool isHit) { this->isHit = isHit; }

	// トランスフォームの設定
	void SetTransform(Phoenix::FrameWork::Transform transform);

	// トランスレートの設定
	void SetTranslate(Phoenix::Math::Vector3 translate);

	// 新たな回転値を設定
	void SetNewRotate(Phoenix::Math::Quaternion newRotate);

	// エネミーマネージャーの設定
	void SetOwner(std::shared_ptr<EnemyManager> owner);

	// ステートを変更
	void SetState(BattleEnemyState state, bool forcedChange = false);

	// 攻撃権を発行
	bool SetAttackRight(bool stackAttackRight);

	// 攻撃ステートを変更
	void SetAttackState(EnemyAttackState state);

	// プレイヤーを設定
	void SetPlayer(std::shared_ptr<Player> player);

	// 移動スピード設定
	void SetMoveSpeed(Phoenix::f32 speed);

	// 移動方向の指定
	void SetMoveInput(Phoenix::f32 moveX, Phoenix::f32 moveY);

	// ダメージ
	void Damage(int damage);

public:
	// 有効フラグ取得
	bool GetEnable();

	// 存在フラグ取得
	bool GetAlive();

	// 死亡フラグ取得
	bool GetDeath();

	// 戦闘中フラグ取得
	bool GetInBattle();

	// トランスフォームの取得
	Phoenix::FrameWork::Transform GetTransform();

	// 攻撃の威力の取得
	Phoenix::u32 GetAttackPower() { return attackPower; }

	// 移動スピード取得
	Phoenix::f32 GetMoveSpeed() { return moveSpeed; }

	// プレイヤーとの距離を取得
	Phoenix::f32 GetDistanceToPlayer() { return distanceToPlayer; }

	// バトルモードのステート取得
	BattleEnemyState GetBattleState();

	// プレイヤーの戦闘エリアに侵入したか
	bool InBattleTerritory();

	// プレイヤーに攻撃が当たる距離に入っているか？
	bool InDistanceHitByAttack();

	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }

	const std::vector<Phoenix::FrameWork::CollisionData>* GetCollisionDatas() { return &collisionDatas; }

	Phoenix::Math::Matrix GetWorldMatrix() { return transform->GetWorldTransform(); }

	Phoenix::Math::Vector3 GetPosition() { return transform->GetTranslate(); }

	Phoenix::f32 GetRadius() { return radius; }

	bool IsAttackJudgment() { return isAttackJudgment; }

	Phoenix::u32 GetAttackCollisionIndex() { return attackCollisionIndex; }

	std::shared_ptr<EnemyUI> GetUI() { return ui; }

	Phoenix::s32 GetLife() { return life; }

	bool UnderAttack() { return battleAI->GetCurrentStateName() == BattleEnemyState::Attack; }
};