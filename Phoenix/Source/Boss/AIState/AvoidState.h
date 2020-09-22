#pragma once

#include "AIState.h"
#include "Phoenix/Math/PhoenixMath.h"


class AvoidState : public AIState
{
private:
	/*static constexpr*/ Phoenix::f32 RollSpeed = 0.15f;
	/*static constexpr*/ Phoenix::f32 judgeDistance = 5.0f;

private:
	Phoenix::f32 animationCnt = 0.0f;

public:
	AvoidState() {}
	~AvoidState() {}

public:
	void Init() override;
	void Update(Boss* boss, Player* player) override;
};