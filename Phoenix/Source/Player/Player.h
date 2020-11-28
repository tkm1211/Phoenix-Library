#pragma once

#include <memory>
#include <functional>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"
#include "../UI/PlayerUI.h"


//******************************************************
//球体　vs　球体
//******************************************************
static bool SphereVsSphere(Phoenix::Math::Vector3 pos1, Phoenix::Math::Vector3 pos2, float r1, float r2)
{
	if ((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) + (pos2.z - pos1.z) * (pos2.z - pos1.z) <= (r1 + r2) * (r1 + r2))
	{
		return true;
	}
	else return false;
}

static bool CircleVsCircle(Phoenix::Math::Vector2 pos1, Phoenix::Math::Vector2 pos2, float r1, float r2)
{
	if ((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) <= (r1 + r2) * (r1 + r2))
	{
		return true;
	}
	else return false;
}

static float GetSqDistancePoint2Segment(Phoenix::Math::Vector3 point, Phoenix::Math::Vector3 seg_start, Phoenix::Math::Vector3 seg_end)
{
	const float eqsilon = 1.0e-5f;	// 誤差吸収用の微小な値

	// 線分の始点から終点へのベクトル
	Phoenix::Math::Vector3 segment_sub = Phoenix::Math::Vector3(seg_end.x - seg_start.x, seg_end.y - seg_start.y, seg_end.z - seg_start.z);

	// 線分の始点から点へのベクトル
	Phoenix::Math::Vector3 segment_point = Phoenix::Math::Vector3(point.x - seg_start.x, point.y - seg_start.y, point.z - seg_start.z);

	// 射影ベクトル
	Phoenix::Math::Vector3 cross_point;

	// 2ベクトルの内積
	float dot = (segment_sub.x * segment_point.x) + (segment_sub.y * segment_point.y) + (segment_sub.z * segment_point.z);
	if (dot < eqsilon)
	{// 内積が負なら、線分の始点が最近傍
		return ((segment_point.x * segment_point.x) + (segment_point.y * segment_point.y) + (segment_point.z * segment_point.z));
	}

	// 点から線分の終点へのベクトル
	segment_point = Phoenix::Math::Vector3(seg_end.x - point.x, seg_end.y - point.y, seg_end.z - point.z);

	// 2ベクトルの内積
	dot = (segment_sub.x * segment_point.x) + (segment_sub.y * segment_point.y) + (segment_sub.z * segment_point.z);
	if (dot < eqsilon)
	{// 内積が負なら、線分の終点が最近傍
		return ((segment_point.x * segment_point.x) + (segment_point.y * segment_point.y) + (segment_point.z * segment_point.z));
	}

	// 上記のどちらにも該当しない場合、線分上に落とした射影が最近傍
	// (本来ならサインで求めるが、外積の大きさ/線分のベクトルの大きさで求まる)
	cross_point.x = segment_sub.y * segment_point.z - segment_sub.z * segment_point.y;
	cross_point.y = segment_sub.z * segment_point.x - segment_sub.x * segment_point.z;
	cross_point.z = segment_sub.x * segment_point.y - segment_sub.y * segment_point.x;
	return (((cross_point.x * cross_point.x) + (cross_point.y * cross_point.y) + (cross_point.z * cross_point.z))
		/ ((segment_sub.x * segment_sub.x) + (segment_sub.y * segment_sub.y) + (segment_sub.z * segment_sub.z)));
}
static bool SphereVsCapsule(Phoenix::Math::Vector3 sphere, Phoenix::Math::Vector3 capsule1, Phoenix::Math::Vector3 capsule2, float sphereRadius, float capsuleRadius)
{
	// 球の中心とカプセルの線分の距離（の二乗）を計算
	float dis = GetSqDistancePoint2Segment(sphere, capsule1, capsule2);

	// 距離（の二乗）が半径の和（の二乗）より小さければ当たっている
	float radius = sphereRadius + capsuleRadius;

	if (radius * radius < dis) return false;

	return true;
}

class Player
{
public:
#if 0
	enum class AnimationState
	{
		Idle,
		Walk,
		Run,
		SlowRun,
		Roll,
		Attack01,
		Attack02,
		Attack03,
		Damage
	};
#else
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
#endif

#if 0
	enum class AttackAnimationState
	{
		Attack01,
		Attack02,
		Attack03,
		End
	};
#elif 0
	enum class AttackAnimationState
	{
		Attack01_1,
		Attack01_2,
		Attack02_1,
		Attack02_2,
		Attack02_3,
		Attack02_4,
		Attack02_5,
		Attack02_6,
		Attack03_1,
		End
	};
#else
	/*enum class AttackAnimationState
	{
		Attack01,
		Attack02,
		Attack03,
		Attack04,
		Attack05,
		Attack06,

		Attack07,
		Attack08,
		Attack09,
		Attack10,
		Attack11,
		Attack12,

		End
	};*/
#endif

	enum class AttackKey
	{
		WeakAttack,
		StrongAttack,
		None
	};

public:
	struct AttackData
	{
		Phoenix::s32 animState = -1; // アニメーションステート
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

	struct AttackDataList
	{
		std::vector<AttackDatas> attackDatas;

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
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;
	std::shared_ptr<PlayerUI> ui;

	Phoenix::Math::Matrix worldMatrix;
	Phoenix::Math::Vector3 pos;
	//Phoenix::Math::Vector3 rotate;
	Phoenix::Math::Quaternion rotate;
	Phoenix::Math::Vector3 scale;
	Phoenix::f32 radius = 0.0f;
	Phoenix::f32 speed = 0.0f;

	//Phoenix::Math::Vector3 newRotate;
	Phoenix::Math::Quaternion newRotate;
	Phoenix::f32 rotateY = 0.0f;

	AnimationState animationState;
	Phoenix::s32 attackState;
	Phoenix::s32 attackComboState = -1;
	Phoenix::s32 currentAttackAnimIndex = -1;
	//std::vector<AttackData> attackDatas;
	AttackDataList attackDatasList;

	bool isChangeAnimation;
	bool isAttack;
	bool alive = false;
	bool death = false;
	float attackReceptionTimeCnt;
	float animationSpeed;

	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;

	// HP
	Phoenix::s32 life = 0;

	// アタックが当たったか？
	//bool isHit = false;
	std::vector<bool> isHit;

	// アタックの判定中か？
	//bool isAttackJudgment = false;
	std::vector<bool> isAttackJudgment;

	// アタックダメージ数
	Phoenix::s32 attackDamage = 0;

	// 回避の無敵中か？
	bool invincible = false;

	// コリジョンデータの要素数
	Phoenix::u32 attackCollisionIndex = 0;

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

	SYSTEMTIME stFileTime;

public:
	Player() :
		worldMatrix(Phoenix::Math::MatrixIdentity()), 
		speed(0.0f), 
		animationState(AnimationState::Idle),
		attackState(-1), 
		isChangeAnimation(false), 
		isAttack(false),
		attackReceptionTimeCnt(0.0f), 
		animationSpeed(0.0f),
		radius(0.0f)
	{}
	~Player() { /*Finalize();*/ }

public:
	static std::unique_ptr<Player> Create();
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
	void Initialize();
	void Finalize();
	void Update(Phoenix::Graphics::Camera& camera, bool onControl, bool attackLoad = true);
	void UpdateTrasform();
	void UpdateUI();
	void Control(Phoenix::Graphics::Camera& camera);
	void ChangeAnimation();
	void ChangeAttackAnimation(Phoenix::u32 animationNum);
	void AttackJudgment();
	void GUI();
	void Damage(int damage, Phoenix::u32 damagePower);
	bool AccumulationDamege();
	void Rotation(Phoenix::Math::Vector3 targetPos)
	{
		Phoenix::Math::Vector3 dir = targetPos - pos;
		dir = Phoenix::Math::Vector3Normalize(dir);
		dir.y = 0.0f;

		//Phoenix::Math::Quaternion rotate = boss->GetRotate();
		Phoenix::Math::Matrix m = Phoenix::Math::MatrixRotationQuaternion(&rotate);
		Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(m._31, m._32, m._33);
		Phoenix::Math::Vector3 up = Phoenix::Math::Vector3(m._21, m._22, m._23);
		Phoenix::Math::Vector3 right = Phoenix::Math::Vector3(m._11, m._12, m._13);
		forward.y = 0.0f;

		Phoenix::f32 angle;
		angle = acosf(Phoenix::Math::Vector3Dot(dir, forward));

		// 回転
		if (1e-8f < fabs(angle))
		{
			Phoenix::f32 angleR;
			angleR = acosf(Phoenix::Math::Vector3Dot(dir, right));
			angleR -= (90.0f * 0.01745f);

			if (0.0f < angleR) angle *= -1;

			Phoenix::Math::Quaternion q;
			q = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angle);
			//rotate *= q;

			Phoenix::Math::Quaternion rotateT = rotate;
			rotateT *= q;
			//rotate = Phoenix::Math::QuaternionSlerp(rotate, rotateT, 0.17f);
			rotate = rotateT;
		}
	}
	void InEnemyTerritory(bool inTerritory)
	{
		if (this->inTerritory != inTerritory && animationState != AnimationState::Attack)
		{
			ChangeAnimationState(AnimationState::Idle, 0.0f);

			Phoenix::Math::Vector3 dir = Phoenix::Math::Vector3Normalize(targetPos - GetPosition());
			float len = sqrtf(dir.x * dir.x + dir.z * dir.z);

			if (len <= 0)
			{
				dir.x = 0;
				dir.z = 0;
			}

			float mag = 1 / len;

			dir.x *= mag;
			dir.z *= mag;

			Phoenix::f32 angleY = atan2f(dir.x, dir.z);

			newRotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angleY);
		}
		this->inTerritory = inTerritory;
		SetBattleMode(inTerritory);
	}

	// アニメーション変更
	void ChangeAnimationState(AnimationState state, Phoenix::f32 moveSpeed = 0.0f)
	{
		isChangeAnimation = true;
		animationState = state;

		speed = moveSpeed;
	};

	// 攻撃アニメーション変更
	void ChangeAttackAnimationState(Phoenix::s32 state, Phoenix::s32 attackAnimIndex, Phoenix::f32 speed)
	{
		isAttack = true;

		attackState = state;
		currentAttackAnimIndex = attackAnimIndex;
		animationSpeed = speed;
	};

	bool CheckHitKey(AttackKey key)
	{
		switch (key)
		{
		case AttackKey::WeakAttack:
			if ((xInput[0].bXt || GetAsyncKeyState('J') & 1))
			{
				return true;
			}
			break;

		case AttackKey::StrongAttack:
			if ((xInput[0].bYt || GetAsyncKeyState('K') & 1))
			{
				return true;
			}
			break;

		default: break;
		}

		return false;
	}

	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }
	Phoenix::Math::Matrix GetWorldMatrix() { return worldMatrix; }
	Phoenix::Math::Vector3 GetPosition() { return pos; }
	//Phoenix::Math::Vector3 GetRotate() { return rotate; }
	Phoenix::Math::Quaternion GetRotate() { return rotate; }
	Phoenix::f32 GetRadius() { return radius; }
	Phoenix::s32 GetHP() { return life; }
	Phoenix::u32 GetAttackPower() { return attackPower; }
	Phoenix::s32 GetScore() { return behaviorScore; }
	Phoenix::s32 GetAttackDamage() { return attackDamage; }
	AnimationState GetAnimationState() { return animationState; }
	AttackDataList& GetAttackDatasList() { return attackDatasList; }
	bool GetDodging() { return (animationState == AnimationState::Dedge); }
	const std::vector<Phoenix::FrameWork::CollisionData> GetCollisionDatas() { return collisionDatas; }
	//bool IsAttackJudgment() { return isAttackJudgment; }
	std::vector<bool> IsAttackJudgment() { return isAttackJudgment; }
	bool Invincible() { return invincible; }
	Phoenix::u32 GetAttackCollisionIndex() { return attackCollisionIndex; }
	std::shared_ptr<PlayerUI> GetUI() { return ui; }
	bool IsAttack() { return isAttack; }
	bool IsDamage() { return animationState == AnimationState::Damage; }

	bool GetAlive() { return alive; }
	bool GetDeath() { return death; }

	bool OnEnemyTerritory() { return inTerritory; }

	void SetPosition(Phoenix::Math::Vector3 pos) { this->pos = pos; }
	//void SetIsHit(bool isHit) { this->isHit = isHit; }
	void SetIsHit(Phoenix::s32 index) { isHit.at(index) = true; }

	void SetBattleMode(bool isBattleMode) { this->isBattleMode = isBattleMode; }
	void SetTargetPos(Phoenix::Math::Vector3 targetPos) { this->targetPos = targetPos; }
	void SetAttackDatasList(AttackDataList data) { attackDatasList = data; }
};