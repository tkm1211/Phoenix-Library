#pragma once

#include "AIState.h"


class WaitState : public AIState
{
private:
	/*static constexpr*/ int WaitMaxTime = 5 * 60;

private:
	int waitTimer = 0;

public:
	WaitState() {}
	~WaitState() {}

public:
	void Init() override;
	void Update(Boss* boss, Player* player) override;
};