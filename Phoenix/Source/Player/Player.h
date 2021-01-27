#pragma once

#include <memory>
#include <functional>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"
#include "../UI/PlayerUI.h"
#include "../Sound/SoundSystem.h"
#include "../Collision/Collision.h"


class Player
{
public:
	// アニメーションステート
	enum class AnimationState
	{
		Idle,
		Walk,
		Run,
		SlowRun,
		Roll,
		Attack,
		Damage,
		Dedge,
		Guard,
		Death,
	};

	// アニメーションレイヤー
	enum class LayerType
	{
		Base,
		LowerBody,
	};

	// ステート種類
	enum class StateType
	{
		Idle,
		BattleIdle,
		DamageSmall,
		DamageBig,
		ForwardDedge,
		BackDedge,
		RightDedge,
		LeftDedge,
		Death,
	};

	// 攻撃入力キー
	enum class AttackKey
	{
		WeakAttack,
		StrongAttack,
		None
	};

public:
	// 攻撃データ
	struct AttackData
	{
		Phoenix::s32 animState = 0; // アニメーションステート
		Phoenix::s32 animIndex = 0; // アニメーションレイヤーの番号

		Phoenix::f32 playSpeed = 1.0f; // 再生スピード
		Phoenix::f32 playBeginTime = 0.0f; // 再生開始時間（再生フレーム）
		Phoenix::f32 playEndTime = 0.0f; // 再生終了時間（再生フレーム）

		Phoenix::s32 collisionNum = 0; // 当たり判定の番号
		Phoenix::f32 collisionBeginTime = 0.0f; // 当たり判定開始時間（再生フレーム）
		Phoenix::f32 collisionEndTime = 0.0f; // 当たり判定終了時間（再生フレーム）

		bool receptionStack = false; // 次のアニメーションへ遷移するための入力受付をスタックするかどうか？
		Phoenix::f32 receptionBeginTime = 0.0f; // 次のアニメーションへ遷移するための入力受付開始時間
		Phoenix::f32 receptionEndTime = 0.0f; // 次のアニメーションへ遷移するための入力受付終了時間

		Phoenix::f32 dedgeReceptionBeginTime = 0.0f; // 回避へ遷移するための入力受付開始時間
		Phoenix::f32 dedgeReceptionEndTime = 0.0f; // 回避へ遷移するための入力受付終了時間

		Phoenix::s32 weakDerivedAttackState = -1; // 次の派生弱攻撃のステート
		Phoenix::s32 strongDerivedAttackState = -1; // 次の派生強攻撃のステート

		template<class Archive>
		void serialize(Archive& archive, Phoenix::u32 version);
	};

	// 攻撃データ集
	struct AttackDatas
	{
		AttackKey receptionKey = AttackKey::WeakAttack; // 入力キー
		std::vector<AttackData> datas;

		void SetKey(AttackKey key)
		{
			receptionKey = key;
		}
		void AddData(AttackData data)
		{
			datas.emplace_back(data);
		}

		template<class Archive>
		void serialize(Archive& archive, Phoenix::u32 version);
	};

	// 攻撃データ集リスト
	struct AttackDataList
	{
		std::vector<AttackDatas> attackDatas;

		AttackDataList() {}

		template<class Archive>
		void serialize(Archive& archive, Phoenix::u32 version);

		// シリアライズ
		static void Serialize(const AttackDataList& data, const char* filename);

		// デシリアライズ
		static bool Deserialize(AttackDataList& data, const char* filename);
	};

private:
	static constexpr Phoenix::f32 WalkSpeed = 0.021f;
	static constexpr Phoenix::f32 BattleWalkSpeed = 0.0105f;
	static constexpr Phoenix::f32 RunSpeed = 0.18f;
	static constexpr Phoenix::f32 SlowRunSpeed = 0.09f;
	static constexpr Phoenix::f32 BattleSlowRunSpeed = 0.045f;
	static constexpr Phoenix::f32 RollSpeed = 0.15f;
	static constexpr Phoenix::f32 DedgeSpeed = 0.15f;
	static constexpr Phoenix::f32 KnockBackSpeed = -0.03f;
	static constexpr Phoenix::f32 KnockBackDownSpeed = 0.0003f;
	static constexpr Phoenix::f32 Attack03Speed = 0.1f;
	static constexpr Phoenix::f32 AnimationSpeed30 = 30.0f; // 0.03333333f
	static constexpr Phoenix::f32 AnimationSpeed45 = 45.0f; // 0.02222222f
	static constexpr Phoenix::f32 AnimationSpeed60 = 60.0f; // 0.01666667f
	static constexpr Phoenix::f32 Attack01AnimationSpeed = 1.75f;
	static constexpr Phoenix::f32 Attack02AnimationSpeed = 2.0f;
	static constexpr Phoenix::f32 Attack03AnimationSpeed = 1.75f;
	static constexpr Phoenix::f32 Attack04AnimationSpeed = 1.15f;
	static constexpr Phoenix::f32 Attack05AnimationSpeed = 1.0f;
	static constexpr Phoenix::f32 Attack06AnimationSpeed = 1.0f;
	static constexpr Phoenix::f32 Attack01ReceptionStartTime = 1.3333332f; // Goalは、Animationの時間の長さから取得 // 20 * 0.0166666667f;
	static constexpr Phoenix::f32 Attack02ReceptionStartTime = 2.2f; // Goalは、Animationの時間の長さから取得 // 20 * 0.0166666667f;
	static constexpr Phoenix::f32 Attack01MoveSpeed = 0.1f;
	static constexpr Phoenix::f32 WeakAttackCollisionRadius = 0.25f;
	static constexpr Phoenix::f32 StrongAttackCollisionRadius = 0.75f;
	static constexpr Phoenix::s32 MaxLife = 100; // TODO : 調整必須
	static constexpr Phoenix::s32 AccumulationMaxDamege = 10; // TODO : 調整必須
	static constexpr Phoenix::s32 AccumulationTime = 60;

private:
	// モデル
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;

	// UI
	std::shared_ptr<PlayerUI> ui;

	// ワールド行列
	Phoenix::Math::Matrix worldMatrix;

	// 座標
	Phoenix::Math::Vector3 pos;

	// 回転
	Phoenix::Math::Quaternion rotate;

	// スケール
	Phoenix::Math::Vector3 scale;

	// 半径
	Phoenix::f32 radius = 0.0f;

	// 移動速度
	Phoenix::f32 speed = 0.0f;

	// 新たな回転
	Phoenix::Math::Quaternion newRotate;

	// Y軸回転
	Phoenix::f32 rotateY = 0.0f;

	// アニメーションステート
	AnimationState animationState;

	// 攻撃ステート
	Phoenix::s32 attackState = 0;

	// 攻撃コンボステート
	Phoenix::s32 attackComboState = 0;

	// 現在の攻撃アニメーション番号
	Phoenix::s32 currentAttackAnimIndex = -1;

	// 攻撃データリスト
	AttackDataList attackDatasList;

	// アニメーションの切り替え
	bool isChangeAnimation = false;

	// 攻撃中
	bool isAttack = false;

	// 存在中
	bool alive = false;

	// 死亡中
	bool death = false;

	// 攻撃入力判定用カウント
	Phoenix::f32 attackReceptionTimeCnt;

	// ジャスト回避時間カウント
	Phoenix::f32 justDedgeTimeCnt;

	// アニメーション速度
	Phoenix::f32 animationSpeed;

	// 当たり判定データ
	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;

	// HP
	Phoenix::s32 life = 0;

	// アタックが当たったか？
	std::vector<bool> isHit;

	// アタックの判定中か？
	std::vector<bool> isAttackJudgment;

	// アタックダメージ数
	Phoenix::s32 attackDamage = 0;

	// 回避の無敵中か？
	bool invincible = false;

	// コリジョンデータの要素数
	Phoenix::s32 attackCollisionIndex = 0;

	// 攻撃の威力
	Phoenix::u32 attackPower = 0;

	// 蓄積ダメージ
	Phoenix::s32 accumulationDamege = 0;
	Phoenix::s32 accumulationTimeCnt = 0;

	// ダメージの威力
	Phoenix::u32 damagePower = 0;

	// ブレンド
	Phoenix::Math::Vector3 blendRate = { 0.0f, 0.0f, 0.0f };

	// エネミーテリトリーフラグ
	bool inTerritory = false;

	// バトルモード
	bool isBattleMode = false;
	Phoenix::u32 dedgeLayerIndex = 0;
	Phoenix::Math::Vector3 targetPos = { 0.0f, 0.0f, 0.0f };

	// 入力スタック
	bool receptionStack = false;
	AttackKey stackKey = AttackKey::None;

	// 行動スコア
	Phoenix::s32 behaviorScore = 0;
	const Phoenix::s32 WeakAttackScore = 100;
	const Phoenix::s32 StrongAttackScore = 300;

	// ファイルタイムスタンプ
	SYSTEMTIME stFileTime{};

	// サウンド
	std::shared_ptr<SoundSystem<SoundType>> soundSystem;

	// レイヤー番号
	std::map<LayerType, Phoenix::s32> layerIndexList;

	// レイヤー内のステート番号
	std::map<StateType, Phoenix::s32> stateIndexList;

	// ジャスト回避
	bool isJustDedge = false;

	// 無敵
	bool isInvincible = false;
	Phoenix::f32 invincibleTimeCnt = 0.0f;

public:
	Player() {}
	~Player() {}

public:
	// 生成
	static std::unique_ptr<Player> Create();

	// コンストラクタ
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	// 更新
	void Update(Phoenix::Graphics::Camera& camera, bool onControl, Phoenix::f32 elapsedTime, bool attackLoad = true);

	// トランスフォーム更新
	void UpdateTrasform();

	// UI更新
	void UpdateUI();

	// 操作更新
	void Control(Phoenix::Graphics::Camera& camera, Phoenix::f32 elapsedTime, bool control);

	// アニメーション移行
	void ChangeAnimation();

	// 攻撃アニメーション移行
	void ChangeAttackAnimation(Phoenix::s32 layerIndex);

	// 攻撃判定
	void AttackJudgment();

	// GUI
	void GUI();

	// ジャスト回避判定
	bool OnJustDedge();

	// ダメージ更新
	bool Damage(int damage, Phoenix::u32 damagePower);

	// 蓄積ダメージ
	bool AccumulationDamege();

	// ターゲット方向に回転
	void Rotation(Phoenix::Math::Vector3 targetPos);

	// エネミーの索敵範囲内での行動
	void InEnemyTerritory(bool inTerritory);

	// アニメーション変更
	void ChangeAnimationState(AnimationState state, Phoenix::f32 moveSpeed = 0.0f);

	// 攻撃アニメーション変更
	void ChangeAttackAnimationState(Phoenix::s32 state, Phoenix::s32 attackAnimIndex, Phoenix::f32 speed);

	// ジャスト回避変更
	void ChangeJustDedge();

	// 入力判定
	bool CheckHitKey(AttackKey key);

public:
	// 座標を設定
	void SetPosition(Phoenix::Math::Vector3 pos) { this->pos = pos; }

	// 攻撃がヒット
	void SetIsHit(Phoenix::s32 index) { isHit.at(index) = true; }

	// ジャスト回避を設定
	void SetIsJustDedge(bool isJustDedge) { this->isJustDedge = isJustDedge; }

	// ロックオンを設定
	void SetBattleMode(bool isBattleMode) { this->isBattleMode = isBattleMode; }

	// ロックオンするターゲット座標を設定
	void SetTargetPos(Phoenix::Math::Vector3 targetPos) { this->targetPos = targetPos; }

	// 攻撃データリストを設定
	void SetAttackDatasList(AttackDataList data) { attackDatasList = data; }

	// サウンドシステムを設定
	void SetSoundSystem(std::shared_ptr<SoundSystem<SoundType>> soundSystem) { this->soundSystem = soundSystem; }

public:
	// モデル取得
	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }

	// ワールド行列取得
	Phoenix::Math::Matrix GetWorldMatrix() { return worldMatrix; }

	// 座標取得
	Phoenix::Math::Vector3 GetPosition() { return pos; }

	// 回転取得
	Phoenix::Math::Quaternion GetRotate() { return rotate; }

	// 半径取得
	Phoenix::f32 GetRadius() { return radius; }

	// HP取得
	Phoenix::s32 GetHP() { return life; }

	// 攻撃種類取得
	Phoenix::u32 GetAttackPower() { return attackPower; }

	// 行動スコア取得
	Phoenix::s32 GetScore() { return behaviorScore; }

	// 攻撃力取得
	Phoenix::s32 GetAttackDamage() { return attackDamage; }

	// 攻撃ステート取得
	Phoenix::s32 GetAttackState() { return attackState; }

	// 攻撃当たり判定の番号取得
	Phoenix::s32 GetAttackCollisionIndex() { return attackCollisionIndex; }

	// アニメーションステート取得
	AnimationState GetAnimationState() { return animationState; }

	// 攻撃データリスト取得
	AttackDataList& GetAttackDatasList() { return attackDatasList; }

	// UI取得
	std::shared_ptr<PlayerUI> GetUI() { return ui; }

	// 当たり判定データ取得
	const std::vector<Phoenix::FrameWork::CollisionData> GetCollisionDatas() { return collisionDatas; }

	// 攻撃当たり判定をヒット済みか取得
	const std::vector<bool> IsAttackJudgment() { return isAttackJudgment; }
	
	// 無敵中
	bool Invincible() { return invincible; }

	// 攻撃中
	bool IsAttack() { return isAttack; }

	// ダメージ中
	bool IsDamage() { return animationState == AnimationState::Damage; }

	// ジャスト回避中
	bool IsJustDedge() { return isJustDedge; }

	// 無敵中
	bool IsInvincible() { return isInvincible; }
	
	// エネミーの索敵範囲内にいるか判定
	bool OnEnemyTerritory() { return inTerritory; }

	// 存在中
	bool GetAlive() { return alive; }

	// 死亡中
	bool GetDeath() { return death; }

	// 回避中
	bool GetDodging() { return (animationState == AnimationState::Dedge); }

	// 構え中
	bool GetLockOn() { return isBattleMode; }
};