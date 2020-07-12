#pragma once

#include "AIState.h"
#include "Phoenix/Math/PhoenixMath.h"


class JumpAttackState : public AIState
{
private:
	/*static constexpr*/ float MoveSpeed = 5.0f;

private:
	bool beginTrun = false;
	Phoenix::f32 oldPosY = 0.0f;
	Phoenix::f32 oldAngleY = 0.0f;
	Phoenix::f32 animationCnt = 0.0f;

public:
	JumpAttackState() {}
	~JumpAttackState() {}

public:
	void Init() override;
	void Update(Boss* boss, Player* player) override;
};