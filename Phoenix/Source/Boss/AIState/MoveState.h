#pragma once

#include "AIState.h"
#include "Phoenix/Math/PhoenixMath.h"


class MoveState : public AIState
{
private:
	/*static constexpr*/ float MoveSpeed = 5.0f;

private:
	Phoenix::Math::Vector3 oldPos;

public:
	MoveState() {}
	~MoveState() {}

public:
	void Init() override;
	void Update(Boss* boss, Player* player) override;
};