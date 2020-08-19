#pragma once

#include "AIState.h"


class DamageState : public AIState
{
public:
	DamageState() {}
	~DamageState() {}

public:
	void Init() override;
	void Update(Boss* boss, Player* player) override;
};