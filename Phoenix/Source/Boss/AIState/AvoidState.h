#pragma once

#include "AIState.h"
#include "Phoenix/Math/PhoenixMath.h"


class AvoidState : public AIState
{
private:
	/*static constexpr*/ float RollSpeed = 15.0f;

private:
	Phoenix::f32 animationCnt = 0.0f;

public:
	AvoidState() {}
	~AvoidState() {}

public:
	void Init() override;
	void Update(Boss* boss, Player* player) override;
};