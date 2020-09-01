#pragma once

#include "AIState.h"
#include "Phoenix/Math/PhoenixMath.h"


class SwingAttackState01 : public AIState
{
private:
	/*static constexpr*/ float MoveSpeed = 0.05f;

private:
	Phoenix::f32 animationCnt = 0.0f;

public:
	SwingAttackState01() {}
	~SwingAttackState01() {}

public:
	void Init() override;
	void Update(Boss* boss, Player* player) override;
	Phoenix::f32 GetAnimationCnt() { return animationCnt; }
};

class SwingAttackState02 : public AIState
{
private:
	/*static constexpr*/ float MoveSpeed = 0.05f;

private:
	Phoenix::f32 animationCnt = 0.0f;

public:
	SwingAttackState02() {}
	~SwingAttackState02() {}

public:
	void Init() override;
	void Update(Boss* boss, Player* player) override;
	Phoenix::f32 GetAnimationCnt() { return animationCnt; }
};