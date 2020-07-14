#pragma once

#include "AIState.h"
#include "Phoenix/Math/PhoenixMath.h"


class MoveState : public AIState
{
private:
	/*static constexpr*/ float MoveSpeed = 5.0f;
	/*static constexpr*/ int MoveMaxCnt = 5 * 60;

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