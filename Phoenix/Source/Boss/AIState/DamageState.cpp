#include "DamageState.h"
#include "../Boss.h"
#include "../../Player/Player.h"


void DamageState::Init()
{
	stateType = AIStateType::Damage;
	nextStateType = AIStateType::None;
	isChangeState = false;
}

void DamageState::Update(Boss* boss, Player* player)
{
	if (!boss->GetModel()->IsPlaying())
	{
		isChangeState = true;
		nextStateType = AIStateType::Avoid;
	}
}