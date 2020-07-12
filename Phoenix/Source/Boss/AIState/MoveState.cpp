#include "MoveState.h"
#include "../Boss.h"
#include "../../Player/Player.h"


void MoveState::Init()
{
	stateType = AIStateType::Move;
	nextStateType = AIStateType::None;
	isChangeState = false;

	oldPos = { 0,0,0 };
}

void MoveState::Update(Boss* boss, Player* player)
{
	if (SphereVsSphere(boss->GetPosition(), player->GetPosition(), boss->GetRadius() * 10.0f, player->GetRadius()))
	{
		isChangeState = true;
		nextStateType = AIStateType::SwingAttack01;
	}
	/*else if (SphereVsSphere(boss->GetPosition(), player->GetPosition(), boss->GetRadius() * 12.0f, player->GetRadius()))
	{
		isChangeState = true;
		nextStateType = AIStateType::JumpAttack;
	}*/

	Phoenix::Math::Vector3 dir = player->GetPosition() - boss->GetPosition();
	dir = Phoenix::Math::Vector3Normalize(dir);

#if 0
	Phoenix::f32 angle = atan2f(dir.x, dir.z);
	boss->SetRotateY(angle);

	Phoenix::Math::Vector3 pos = boss->GetPosition();
	pos.x += sinf(angle) * MoveSpeed;
	pos.z += cosf(angle) * MoveSpeed;
	boss->SetPosition(pos);
#else
	Phoenix::Math::Quaternion rotate = boss->GetRotate();
	Phoenix::Math::Matrix matrix = Phoenix::Math::MatrixRotationQuaternion(&rotate);
	Phoenix::Math::Vector3 foward = { matrix._31, matrix._32, matrix._33 };

	Phoenix::Math::Vector3 axis = Phoenix::Math::Vector3Cross(foward, dir);
	Phoenix::f32 angle = acosf(Phoenix::Math::Vector3Dot(dir, foward));

	if (1e-8f < fabs(angle))
	{
		Phoenix::Math::Quaternion q;
		q = Phoenix::Math::QuaternionRotationAxis(axis, angle);

		Phoenix::Math::Quaternion rotateT = rotate;
		rotateT *= q;
		rotate = Phoenix::Math::QuaternionSlerp(rotate, rotateT, 0.03f);
	}
	boss->SetRotate(rotate);

	matrix = Phoenix::Math::MatrixRotationQuaternion(&rotate);
	foward = { matrix._31, matrix._32, matrix._33 };
	
	angle = atan2f(foward.x, foward.z);
	
	Phoenix::Math::Vector3 pos = boss->GetPosition();
	pos.x += sinf(angle) * MoveSpeed;
	pos.z += cosf(angle) * MoveSpeed;
	boss->SetPosition(pos);
#endif
}