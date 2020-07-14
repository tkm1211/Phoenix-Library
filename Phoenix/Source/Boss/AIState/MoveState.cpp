#include "MoveState.h"
#include "../Boss.h"
#include "../../Player/Player.h"


void MoveState::Init()
{
	stateType = AIStateType::Move;
	nextStateType = AIStateType::None;
	isChangeState = false;

	oldPos = { 0,0,0 };
	moveCnt = 0;
}

void MoveState::Update(Boss* boss, Player* player)
{
	Phoenix::Math::Vector3 dir = player->GetPosition() - boss->GetPosition();
	Phoenix::f32 len = Phoenix::Math::Vector3Length(Phoenix::Math::Vector3(dir.x, 0.0f, dir.z));
	dir = Phoenix::Math::Vector3Normalize(dir);

	if (SphereVsSphere(boss->GetPosition(), player->GetPosition(), boss->GetRadius() * 1.5f, player->GetRadius()))
	{
		isChangeState = true;
		nextStateType = AIStateType::SwingAttack01;
		return;
	}
	/*else if (((boss->GetRadius() + player->GetRadius() + 100.0f) <= len && len <= (boss->GetRadius() + player->GetRadius() + 600.0f))
		&& (player->GetAnimationState() == Player::AnimationState::Idle
		|| player->GetAnimationState() == Player::AnimationState::Roll
		|| player->GetAnimationState() == Player::AnimationState::Walk
		|| player->GetAnimationState() == Player::AnimationState::Run))
	{
		isChangeState = true;
		nextStateType = AIStateType::JumpAttack;
	}*/
	else if (MoveMaxCnt <= moveCnt++)
	{
		isChangeState = true;
		nextStateType = AIStateType::JumpAttack;
		return;
	}

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
	Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(matrix._31, matrix._32, matrix._33);
	Phoenix::Math::Vector3 up = Phoenix::Math::Vector3(matrix._21, matrix._22, matrix._23);
	Phoenix::Math::Vector3 right = Phoenix::Math::Vector3(matrix._11, matrix._12, matrix._13);

	//Phoenix::Math::Vector3 axis = Phoenix::Math::Vector3Cross(foward, dir);
	Phoenix::f32 angle = acosf(Phoenix::Math::Vector3Dot(dir, forward));

	if (1e-8f < fabs(angle))
	{
		Phoenix::f32 angleR;
		angleR = acosf(Phoenix::Math::Vector3Dot(dir, right));
		angleR -= (90.0f * 0.01745f);

		if (0.0f < angleR) angle *= -1;

		Phoenix::Math::Quaternion q;
		q = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angle);

		Phoenix::Math::Quaternion rotateT = rotate;
		rotateT *= q;
		rotate = Phoenix::Math::QuaternionSlerp(rotate, rotateT, 0.03f);
	}
	boss->SetRotate(rotate);

	matrix = Phoenix::Math::MatrixRotationQuaternion(&rotate);
	forward = Phoenix::Math::Vector3(matrix._31, matrix._32, matrix._33);
	
	angle = atan2f(forward.x, forward.z);
	
	Phoenix::Math::Vector3 pos = boss->GetPosition();
	pos.x += sinf(angle) * MoveSpeed;
	pos.z += cosf(angle) * MoveSpeed;
	boss->SetPosition(pos);
#endif
}