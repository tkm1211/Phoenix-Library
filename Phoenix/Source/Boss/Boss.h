#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "AI.h"
#include "BossAI.h"
#include "AIState/AIState.h"
#include "../Player/Player.h"
#include "../UI/BossUI.h"


class Boss
{
private:
	/*static constexpr*/ Phoenix::s32 MaxLife = 1000; // TODO : 調整必須
	/*static constexpr*/ Phoenix::s32 AccumulationMaxDamege = 90; // TODO : 調整必須
	/*static constexpr*/ Phoenix::s32 AccumulationTime = 5 * 60;

private:
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;
	std::unique_ptr<Phoenix::FrameWork::ModelObject> effectModel;
	std::unique_ptr<AI> ai;
	std::shared_ptr<BossUI> ui;

	BossAI* bossAI = nullptr;
	Player* player = nullptr;

	AIStateType currentType = AIStateType::None;
	AIStateType nextType = AIStateType::None;

	Phoenix::Math::Matrix worldMatrix;
	Phoenix::Math::Vector3 pos;
	//Phoenix::Math::Vector3 rotate;
	Phoenix::Math::Quaternion rotate;
	Phoenix::Math::Vector3 scale;
	Phoenix::f32 radius;
	Phoenix::f32 speed;

	Phoenix::u32 boneIndex = -1;

	Phoenix::Math::Vector3 oldPos;

	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;

	// HP
	Phoenix::s32 life = 0;

	// アタックが当たったか？
	bool isHit = false;

	// アタックの判定中か？
	bool isAttackJudgment = false;

	// コリジョンデータの要素数
	Phoenix::u32 attackCollisionIndex = 0;

	// 蓄積ダメージ
	Phoenix::s32 accumulationDamege = 0;
	Phoenix::s32 accumulationTimeCnt = 0;
	bool isChangeAccumulationDamege = false;

public:
	Boss() :
		worldMatrix(Phoenix::Math::MatrixIdentity()),
		speed(0.0f),
		radius(0.0f) 
	{}
	~Boss() {}

public:
	static std::unique_ptr<Boss> Create();
	void Init(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Player* player);
	void Update();
	void UpdateUI();
	void ChangeAnimation(AIStateType type);
	void AttackJudgment();
	void GUI();
	void Damage(int damage) { life -= damage; accumulationDamege += damage; }
	void AccumulationDamege();

	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }
	Phoenix::FrameWork::ModelObject* GetEffectModel() { return effectModel.get(); }
	Phoenix::Math::Matrix GetWorldMatrix() { return worldMatrix; }
	Phoenix::Math::Vector3 GetPosition() { return pos; }
	//Phoenix::Math::Vector3 GetRotate() { return rotate; }
	Phoenix::Math::Quaternion GetRotate() { return rotate; }
	Phoenix::f32 GetRadius() { return radius; }
	Phoenix::u32 GetBoneIndex() { return boneIndex; }
	std::vector<Phoenix::FrameWork::CollisionData>* GetCollisionDatas() { return &collisionDatas; }
	bool IsAttackJudgment() { return isAttackJudgment; }
	Phoenix::u32 GetAttackCollisionIndex() { return attackCollisionIndex; }
	BossUI* GetUI() { return ui.get(); }
	bool IsJumpAttack() { return (currentType == AIStateType::JumpAttack) ? true : false; }
	bool IsChangeAccumulationDamege()
	{
		if (!isChangeAccumulationDamege) return false;

		isChangeAccumulationDamege = false;
		return true;
	}

	void SetPosition(Phoenix::Math::Vector3 pos) { this->pos = pos; }
	//void SetRotate(Phoenix::Math::Vector3 rotate) { this->rotate = rotate; }
	void SetRotate(Phoenix::Math::Quaternion rotate) { this->rotate = rotate; }
	void SetIsHit(bool isHit) { this->isHit = isHit; }
};