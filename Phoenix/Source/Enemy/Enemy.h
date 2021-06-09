#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Component/Transform.h"
#include "Phoenix/FrameWork/SharedFromThis/SharedFromThis.h"
#include "EnemyState.h"
#include "EnemyTask.h"
#include "EnemyAIController.h"
#include "../AI/StateMachine/BattleEnemyAI.h"
#include "../AI/StateMachine/BattleEnemyState.h"
#include "../UI/EnemyUI.h"


class EnemyManager;
class Player;
class Enemy : public inheritable_enable_shared_from_this<Enemy>
{
public:
	static constexpr Phoenix::s32 LifeRange = 150;
	static constexpr Phoenix::s32 AccumulationMaxDamage = 80;

	static constexpr Phoenix::f32 IdleMaxCount = 100.0f;
	static constexpr Phoenix::f32 WalkSpeed = 0.045f;
	static constexpr Phoenix::f32 RunSpeed = 0.1f;
	static constexpr Phoenix::f32 DedgeSpeed = 0.35f;
	static constexpr Phoenix::f32 DamageSpeed = 0.1f;
	static constexpr Phoenix::f32 KnockBackSpeed = 0.5f;
	static constexpr Phoenix::f32 AttackSpeed = 0.1f;

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
		KnockBack,
		GettingUp,
		Dedge,
		Death,
	};

	template <class T>
	struct AttackData
	{
		T animState; // アニメーションステート
		Phoenix::s32 animIndex; // アニメーションレイヤーの番号

		Phoenix::f32 playSpeed; // 再生スピード
		Phoenix::f32 playBeginTime; // 再生開始時間（再生フレーム）
		Phoenix::f32 playEndTime; // 再生終了時間（再生フレーム）

		Phoenix::s32 collisionNum; // 当たり判定の番号
		Phoenix::f32 collisionBeginTime; // 当たり判定開始時間（再生フレーム）
		Phoenix::f32 collisionEndTime; // 当たり判定終了時間（再生フレーム）
	};

	template <class T>
	struct AttackDatas
	{
		std::vector<AttackData<T>> datas;

		void AddData(AttackData<T> data)
		{
			datas.emplace_back(data);
		}
	};

	struct HTNDatas
	{
		// 待機時間
		Phoenix::f32 idleCount = 0.0f;

		// 左右に歩く方向
		Phoenix::f32 moveX = 0.0f;

		// 攻撃ステート番号
		Phoenix::s32 attackIndex = 0;

		// 経過時間
		Phoenix::f32 elapsedTime = 0.0f;

		void Initialize()
		{
			idleCount = 0.0f;
			elapsedTime = 0.0f;
		}

		void SetElapsedTime(Phoenix::f32 elapsedTime)
		{
			this->elapsedTime = elapsedTime;
		}
	};

protected:
	// モデル
	std::shared_ptr<Phoenix::FrameWork::ModelObject> model;

	// トランスフォーム
	std::unique_ptr<Phoenix::FrameWork::Transform> transform;

	// 当たり判定データ
	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;

	// HPゲージUI
	std::shared_ptr<EnemyUI> ui;

	// AI
	EnemyMode currentMode = EnemyMode::Ordinary;
	//std::shared_ptr<OrdinaryAIEnemyAI> ordinaryAI;
	//std::shared_ptr<BattleEnemyAI> battleAI;
	std::shared_ptr<EnemyAIController> controller;

	// マネージャー
	std::weak_ptr<EnemyManager> owner;

	// アニメーション
	bool changeAnimation = false;
	bool changeAttackAnimation = false;
	BattleEnemyState changeState = BattleEnemyState::NoneState;
	Phoenix::f32 playBeginTime = 0.0f;
	Phoenix::f32 playEndTime = 0.0f;

	// 種類タグ
	TypeTag typeTag = TypeTag::Small;

	// パラメーター
	Phoenix::s32 life = 0;
	Phoenix::s32 lifeMax = 0;
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

	// 蓄積ダメージ
	Phoenix::s32 accumulationDamage = 0;

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

	// 走り方向転換
	bool changeOfCourse = false;

	// レイヤー番号
	std::map<LayerType, Phoenix::s32> layerIndexList;

	// レイヤー内のステート番号
	std::map<StateType, Phoenix::s32> stateIndexList;

	// HTN用変数
	std::shared_ptr<HTNDatas> htnDatas;

	// HTN用ワールドステート
	std::shared_ptr<EnemyWorldState> worldState;

	// ステージ端にいるか
	bool hitWall = false;

	// プレイヤーパラメーター
	Phoenix::Math::Vector3 playerPos;
	Phoenix::Math::Quaternion playerRotate;

	// 遠回り
	bool detour = false;
	Phoenix::Math::Vector3 targetPos;

private:
	// 攻撃ステート
	//std::shared_ptr<BattleEnemy::Attack<EnemyAttackState, Enemy>> attackState;
	EnemyAttackState changeAttackState = EnemyAttackState::NoneState;
	EnemyAttackState currentAttackState = EnemyAttackState::NoneState;

	// 攻撃データリスト
	std::vector<AttackDatas<EnemyAttackState>> attackDatasList;

public:
	Enemy() {}
	virtual ~Enemy() {}

public:
	// 生成
	static std::shared_ptr<Enemy> Create();

	// コンストラクタ
	virtual void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);

	// 初期化
	virtual void Initialize();

	// 終了化
	virtual void Finalize();

	// 更新
	virtual void Update(bool onControl, Phoenix::f32 elapsedTime);

	// トランスフォーム更新
	virtual void UpdateTransform();

	// 座標更新
	virtual void UpdateTranslate(Phoenix::f32 elapsedTime);

	// 回転更新
	virtual void UpdateRotate(Phoenix::f32 elapsedTime);

	// アニメーション更新
	virtual void UpdateAnimation(Phoenix::f32 elapsedTime);

	// AI更新
	virtual void UpdateAI(Phoenix::f32 elapsedTime);

	// 当たり判定更新
	virtual void UpdateCollision();

	// UI更新
	virtual void UpdateUI(Phoenix::Math::Vector2 pos, Phoenix::f32 elapsedTime);

	// 描画
	virtual void Draw();

	// GUI
	virtual void GUI(Phoenix::s32 index);

	// アニメーションを移行
	virtual void ChangeAnimation();

	// 攻撃ステートを移行
	virtual void ChangeAttackAnimation();

	// 攻撃判定
	virtual void AttackJudgment();

	// プレイヤーとの距離計測
	virtual void DistanceMeasurement();

	// プレイヤーの攻撃視野にいるか判定
	virtual bool JudgePlayerAttackRange();

	// 新たな回転値の更新
	virtual void UpdateNewRotate();

	// プレイヤーの戦闘エリアに侵入したか
	virtual bool InBattleTerritory();

	// プレイヤーに攻撃が当たる距離に入っているか？
	virtual bool InDistanceHitByAttack();

	// ダメージ
	virtual bool Damage(Phoenix::s32 damage);

	// 蓄積ダメージ
	virtual bool AccumulationDamage(Phoenix::s32 damage);

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

	// アニメーションを変更
	void SetAnimation(BattleEnemyState state);

	// 攻撃権を発行
	bool SetAttackRight(bool stackAttackRight);

	// 攻撃ステートを変更
	void SetAttackState(EnemyAttackState state);

	// プレイヤーパラメーターを設定
	void SetPlayerParameter(Phoenix::Math::Vector3 pos, Phoenix::Math::Quaternion rotate);

	// 移動スピード設定
	void SetMoveSpeed(Phoenix::f32 speed);

	// 移動方向の指定
	void SetMoveInput(Phoenix::f32 moveX, Phoenix::f32 moveY);

	// ステージ端にいるか設定
	void SetHitWall(bool hitWall);

	// 方向転換の設定
	void SetChangeOfCourse(bool changeOfCourse);

	// 遠回りの設定
	void SetDetour(bool detour);

	// 目的地の設定
	void SetTargetPos(Phoenix::Math::Vector3 targetPos);

public:
	// 有効フラグ取得
	bool GetEnable();

	// 存在フラグ取得
	bool GetAlive();

	// 死亡フラグ取得
	bool GetDeath();

	// 戦闘中フラグ取得
	bool GetInBattle();

	// ステージ端にいるか取得
	bool GetHitWall();

	// 方向転換の取得
	bool GetChangeOfCourse();

	// 遠回りの取得
	bool GetDetour();

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

	// モデルを取得
	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }

	// 当たり判定データを取得
	const std::vector<Phoenix::FrameWork::CollisionData>* GetCollisionDatas() { return &collisionDatas; }

	// ワールド行列を取得
	Phoenix::Math::Matrix GetWorldMatrix() { return transform->GetWorldTransform(); }

	// 座標を取得
	Phoenix::Math::Vector3 GetPosition() { return transform->GetTranslate(); }

	// 半径を取得
	Phoenix::f32 GetRadius() { return radius; }

	// 攻撃判定中
	bool IsAttackJudgment() { return isAttackJudgment; }

	// 攻撃の当たり判定番号を取得
	Phoenix::u32 GetAttackCollisionIndex() { return attackCollisionIndex; }

	// HPゲージUIを取得
	std::shared_ptr<EnemyUI> GetUI() { return ui; }

	// HPを取得
	Phoenix::s32 GetLife() { return life; }

	// 最大HPを取得
	Phoenix::s32 GetLifeMax() { return lifeMax; }

	// 種類タグ
	TypeTag GetTypeTag() { return typeTag; }

	// 攻撃中の判定
	bool UnderAttack() { return /*battleAI->GetCurrentStateName() == BattleEnemyState::Attack*/false; }

	// 攻撃権を取得
	bool GetAttackRight() { return stackAttackRight; }

	// アニメーション変更中
	bool GetChangeAnimation() { return changeAnimation; }

	// 攻撃データリストを取得
	std::vector<AttackDatas<EnemyAttackState>> GetAtackDatasList() { return attackDatasList; }

	// HTN用データを取得
	std::shared_ptr<HTNDatas> GetHTNDatas() { return htnDatas; }

	// HTN用ワールドステートを取得
	std::shared_ptr<EnemyWorldState> GetWorldState() { return worldState; }
};