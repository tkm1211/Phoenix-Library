#pragma once

#include "AIState.h"
#include "Phoenix/Math/PhoenixMath.h"


class RotationAttackState : public AIState
{
private:
	/*static constexpr*/ float MoveSpeed = 0.05f;

private:
	Phoenix::f32 animationCnt = 0.0f;

public:
	RotationAttackState() {}
	~RotationAttackState() {}

public:
	void Init() override;
	void Update(Boss* boss, Player* player) override;
	Phoenix::f32 GetAnimationCnt() { return animationCnt; }
};