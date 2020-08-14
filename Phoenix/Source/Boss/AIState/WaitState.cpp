#include "WaitState.h"
#include "../Boss.h"
#include "../../Player/Player.h"


void WaitState::Init()
{
	stateType = AIStateType::Wait;
	nextStateType = AIStateType::None;
	isChangeState = false;
	waitTimer = 0;
}

void WaitState::Update(Boss* boss, Player* player)
{
	//if (WaitMaxTime <= waitTimer)
	if (!boss->GetModel()->IsPlaying())
	{
		waitTimer = 0;
		isChangeState = true;
		nextStateType = AIStateType::Move;
	}
	else
	{
		waitTimer++;
	}
}