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
	// �U���X�e�[�g
	//std::shared_ptr<BattleEnemy::Attack<BossAttackState, Boss>> attackState;
	BossAttackState changeAttackState = BossAttackState::NoneState;
	BossAttackState currentAttackState = BossAttackState::NoneState;

	// �U���f�[�^���X�g
	std::vector<AttackDatas<BossAttackState>> attackDatasList;

public:
	Boss() {}
	~Boss() {}

public:
	// ����
	static std::shared_ptr<Boss> Create();

	// �R���X�g���N�^
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice) override;

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V
	void Update(bool onControl, Phoenix::f32 elapsedTime) override;

	// �`��
	void Draw() override;

	// GUI
	void GUI(Phoenix::s32 index) override;

	// �U���X�e�[�g���ڍs
	void ChangeAttackAnimation() override;

	// �U������
	void AttackJudgment() override;

	// �v���C���[�ɍU���������鋗���ɓ����Ă��邩�H
	bool InDistanceHitByAttack() override;

	// �_���[�W
	bool Damage(Phoenix::s32 damage) override;

	// �~�σ_���[�W
	bool AccumulationDamage(Phoenix::s32 damage) override;

public:
	// �U���X�e�[�g��ύX
	void SetAttackState(BossAttackState state);

	// �U���f�[�^���X�g���擾
	std::vector<AttackDatas<BossAttackState>> GetAtackDatasList() { return attackDatasList; }
};