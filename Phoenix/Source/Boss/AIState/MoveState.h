#pragma once

#include "AIState.h"
#include "Phoenix/Math/PhoenixMath.h"


class MoveState : public AIState
{
private:
	/*static constexpr*/ Phoenix::f32 MoveSpeed = 0.05f;
	/*static constexpr*/ Phoenix::s32 MoveMaxCnt = 5 * 60;
	/*static constexpr*/ Phoenix::f32 judgeDistance = 5.0f;

private:
	Phoenix::Math::Vector3 oldPos;
	int moveCnt = 0;

public:
	MoveState() {}
	~MoveState() {}

public:
	void Init() override;
	void Update(Boss* boss, Player* player) override;
};