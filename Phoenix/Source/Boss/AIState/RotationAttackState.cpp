#include "RotationAttackState.h"
#include "../Boss.h"
#include "../../Player/Player.h"


void RotationAttackState::Init()
{
	stateType = AIStateType::RotationAttack;
	nextStateType = AIStateType::None;
	isChangeState = false;

	animationCnt = 0.0f;
}

void RotationAttackState::Update(Boss* boss, Player* player)
{
	if (149.0f / 60.0f <= animationCnt)
	{
		Phoenix::Math::Vector3 dir = player->GetPosition() - boss->GetPosition();
		Phoenix::f32 len = Phoenix::Math::Vector3Length(Phoenix::Math::Vector3(dir.x, 0.0f, dir.z));

		isChangeState = true;

		nextStateType = AIStateType::Wait;
	}
	if (animationCnt <= 0.53f)
	{
		Phoenix::Math::Vector3 dir = player->GetPosition() - boss->GetPosition();
		dir = Phoenix::Math::Vector3Normalize(dir);

		Phoenix::Math::Quaternion rotate = boss->GetRotate();
		Phoenix::Math::Matrix matrix = Phoenix::Math::MatrixRotationQuaternion(&rotate);
		Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(matrix._31, matrix._32, matrix._33);
		Phoenix::Math::Vector3 up = Phoenix::Math::Vector3(matrix._21, matrix._22, matrix._23);
		Phoenix::Math::Vector3 right = Phoenix::Math::Vector3(matrix._11, matrix._12, matrix._13);

		//Phoenix::Math::Vector3 axis = Phoenix::Math::Vector3Cross(forward, dir);
		Phoenix::f32 angle = acosf(Phoenix::Math::Vector3Dot(dir, forward));

		if (1e-8f < fabs(angle))
		{
			Phoenix::f32 angleR;
			angleR = acosf(Phoenix::Math::Vector3Dot(dir, right));
			angleR -= (90.0f * 0.01745f);

			if (0.0f < angleR) angle *= -1;

			Phoenix::Math::Quaternion q;
			q = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angle);
			//rotate *= q;

			Phoenix::Math::Quaternion rotateT = rotate;
			rotateT *= q;
			rotate = Phoenix::Math::QuaternionSlerp(rotate, rotateT, 0.17f);
		}
		boss->SetRotate(rotate);
	}

	animationCnt += 1 / 60.0f;
}