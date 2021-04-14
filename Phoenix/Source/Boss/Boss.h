#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "../Enemy/Enemy.h"
#include "../UI/BossUI.h"


class Boss : public Enemy
{
public:
	static constexpr Phoenix::s32 LifeRange = 250;
	static constexpr Phoenix::s32 AccumulationMaxDamage = 80 * 2;

private:
	// 攻撃ステート
	//std::shared_ptr<BattleEnemy::Attack<BossAttackState, Boss>> attackState;
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

	// 攻撃ステートを移行
	void ChangeAttackAnimation() override;

	// 攻撃判定
	void AttackJudgment() override;

	// プレイヤーに攻撃が当たる距離に入っているか？
	bool InDistanceHitByAttack() override;

	// ダメージ
	bool Damage(Phoenix::s32 damage) override;

	// 蓄積ダメージ
	bool AccumulationDamage(Phoenix::s32 damage) override;

public:
	// 攻撃ステートを変更
	void SetAttackState(BossAttackState state);

	// 攻撃データリストを取得
	std::vector<AttackDatas<BossAttackState>> GetAtackDatasList() { return attackDatasList; }
};