#include "SwingAttackState.h"
#include "../Boss.h"
#include "../../Player/Player.h"


void SwingAttackState01::Init()
{
	stateType = AIStateType::SwingAttack01;
	nextStateType = AIStateType::None;
	isChangeState = false;

	animationCnt = 0.0f;
}

void SwingAttackState01::Update(Boss* boss, Player* player)
{
	if (1.75f <= animationCnt)
	{
		isChangeState = true;
		nextStateType = AIStateType::SwingAttack02;
		return;
	}
	if (animationCnt <= 0.0f)
	{
		Phoenix::Math::Vector3 dir = player->GetPosition() - boss->GetPosition();
		dir = Phoenix::Math::Vector3Normalize(dir);

		Phoenix::Math::Quaternion rotate = boss->GetRotate();
		Phoenix::Math::Matrix matrix = Phoenix::Math::MatrixRotationQuaternion(&rotate);
		Phoenix::Math::Vector3 foward = { matrix._31, matrix._32, matrix._33 };

		Phoenix::Math::Vector3 axis = Phoenix::Math::Vector3Cross(foward, dir);
		Phoenix::f32 angle = acosf(Phoenix::Math::Vector3Dot(dir, foward));

		if (1e-8f < fabs(angle))
		{
			Phoenix::Math::Quaternion q;
			q = Phoenix::Math::QuaternionRotationAxis(axis, angle);
			rotate *= q;
		}
		boss->SetRotate(rotate);
	}

	animationCnt += 1 / 60.0f;
}

void SwingAttackState02::Init()
{
	stateType = AIStateType::SwingAttack02;
	nextStateType = AIStateType::None;
	isChangeState = false;

	animationCnt = 0.0f;
}

void SwingAttackState02::Update(Boss* boss, Player* player)
{
	if (2.0f <= animationCnt)
	{
		isChangeState = true;
		nextStateType = AIStateType::Wait;
		return;
	}
	animationCnt += 1 / 60.0f;
}