#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "../Enemy/Enemy.h"
#include "../UI/BossUI.h"


//class Boss
//{
//private:
//	/*static constexpr*/ Phoenix::s32 MaxLife = 1000; // TODO : 調整必須
//	/*static constexpr*/ Phoenix::s32 AccumulationMaxDamege = 60; // TODO : 調整必須
//	/*static constexpr*/ Phoenix::s32 AccumulationTime = 5 * 60;
//
//private:
//	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;
//	std::unique_ptr<Phoenix::FrameWork::ModelObject> effectModel;
//	std::unique_ptr<AI> ai;
//	std::shared_ptr<BossUI> ui;
//
//	BossAI* bossAI = nullptr;
//	Player* player = nullptr;
//
//	AIStateType currentType = AIStateType::None;
//	AIStateType nextType = AIStateType::None;
//
//	Phoenix::Math::Matrix worldMatrix;
//	Phoenix::Math::Vector3 pos;
//	//Phoenix::Math::Vector3 rotate;
//	Phoenix::Math::Quaternion rotate;
//	Phoenix::Math::Vector3 scale;
//	Phoenix::f32 radius;
//	Phoenix::f32 speed;
//
//	Phoenix::u32 boneIndex = -1;
//
//	Phoenix::Math::Vector3 oldPos;
//
//	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;
//
//	// HP
//	Phoenix::s32 life = 0;
//
//	// アタックが当たったか？
//	bool isHit = false;
//
//	// アタックの判定中か？
//	bool isAttackJudgment = false;
//
//	// コリジョンデータの要素数
//	Phoenix::u32 attackCollisionIndex = 0;
//
//	// 蓄積ダメージ
//	Phoenix::s32 accumulationDamege = 0;
//	Phoenix::s32 accumulationTimeCnt = 0;
//	bool isChangeAccumulationDamege = false;
//
//	// ディゾルブ
//	bool isDissolve = false;
//	Phoenix::f32 animCnt = 0.0f;
//
//	// ジャンプ攻撃スタート時
//	bool isJumpAttackStart = false;
//
//	// ジャンプ攻撃にスタートを一度オンしたか確認
//	bool onJumpAttackStart = false;
//
//public:
//	Boss() :
//		worldMatrix(Phoenix::Math::MatrixIdentity()),
//		speed(0.0f),
//		radius(0.0f) 
//	{}
//	~Boss() {}
//
//public:
//	static std::unique_ptr<Boss> Create();
//	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Player* player);
//	void Initialize();
//	void Update(bool onControl);
//	void UpdateUI();
//	void ChangeAnimation(AIStateType type);
//	void AttackJudgment();
//	void GUI();
//	void Damage(int damage)
//	{
//		life -= damage;
//		if (!IsJumpAttack())
//		{
//			accumulationDamege += damage;
//		}
//	}
//	void AccumulationDamege();
//
//	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }
//	Phoenix::FrameWork::ModelObject* GetEffectModel() { return effectModel.get(); }
//	Phoenix::Math::Matrix GetWorldMatrix() { return worldMatrix; }
//	Phoenix::Math::Vector3 GetPosition() { return pos; }
//	//Phoenix::Math::Vector3 GetRotate() { return rotate; }
//	Phoenix::Math::Quaternion GetRotate() { return rotate; }
//	Phoenix::f32 GetRadius() { return radius; }
//	Phoenix::s32 GetHP() { return life; }
//	Phoenix::u32 GetBoneIndex() { return boneIndex; }
//	std::vector<Phoenix::FrameWork::CollisionData>* GetCollisionDatas() { return &collisionDatas; }
//	bool IsAttackJudgment() { return isAttackJudgment; }
//	Phoenix::u32 GetAttackCollisionIndex() { return attackCollisionIndex; }
//	BossUI* GetUI() { return ui.get(); }
//	bool IsAccumulationDamege() { return (0.0f < accumulationDamege); }
//	bool IsJumpAttack() { return (currentType == AIStateType::JumpAttack) ? true : false; }
//	bool IsChangeAccumulationDamege()
//	{
//		if (!isChangeAccumulationDamege) return false;
//
//		isChangeAccumulationDamege = false;
//		return true;
//	}
//	bool IsDissolve() { return isDissolve; }
//	bool IsJumpAttackStart() { return isJumpAttackStart; }
//
//	void SetPosition(Phoenix::Math::Vector3 pos) { this->pos = pos; }
//	//void SetRotate(Phoenix::Math::Vector3 rotate) { this->rotate = rotate; }
//	void SetRotate(Phoenix::Math::Quaternion rotate) { this->rotate = rotate; }
//	void SetIsHit(bool isHit) { this->isHit = isHit; }
//};

class Boss : public Enemy
{
public:
	static constexpr Phoenix::s32 LifeRange = 1000;

private:
	// 攻撃ステート
	std::shared_ptr<BattleEnemy::Attack<BossAttackState, Boss>> attackState;
	BossAttackState changeAttackState = BossAttackState::NoneState;
	BossAttackState currentAttackState = BossAttackState::NoneState;

	// 攻撃データリスト
	std::vector<AttackDatas<BossAttackState>> attackDatasList;

public:
	Boss() {}
	~Boss() {}

public:
	// 生成
	static std::shared_ptr<Boss> Create();

	// コンストラクタ
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice) override;

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新
	void Update(bool onControl, Phoenix::f32 elapsedTime) override;

	// 描画
	void Draw() override;

	// GUI
	void GUI(Phoenix::s32 index) override;

	// アニメーションを移行
	//void ChangeAnimation() override;

	// 攻撃ステートを移行
	void ChangeAttackAnimation() override;

	// 攻撃判定
	void AttackJudgment() override;

public:
	// 攻撃ステートを変更
	void SetAttackState(BossAttackState state);
};