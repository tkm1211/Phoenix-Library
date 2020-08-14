#include "JumpAttackState.h"
#include "../Boss.h"
#include "../../Player/Player.h"


void JumpAttackState::Init()
{
	stateType = AIStateType::JumpAttack;
	nextStateType = AIStateType::None;
	isChangeState = false;

	beginTrun = false;
	oldPosY = 0.0f;
	oldAngleY = 0.0f;
	animationCnt = 0.0f;
}

void JumpAttackState::Update(Boss* boss, Player* player)
{
	if (!boss->GetModel()->IsPlaying())
	{
		isChangeState = true;
		nextStateType = AIStateType::Move;
		return;
	}

	Phoenix::Math::Vector3 dir = player->GetPosition() - boss->GetPosition();
	dir = Phoenix::Math::Vector3Normalize(dir);
	dir.y = 0.0f;

	Phoenix::Math::Quaternion rotate = boss->GetRotate();
	Phoenix::Math::Matrix m = Phoenix::Math::MatrixRotationQuaternion(&rotate);
	Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(m._31, m._32, m._33);
	Phoenix::Math::Vector3 up = Phoenix::Math::Vector3(m._21, m._22, m._23);
	Phoenix::Math::Vector3 right = Phoenix::Math::Vector3(m._11, m._12, m._13);
	forward.y = 0.0f;

	Phoenix::f32 angle;
	angle = acosf(Phoenix::Math::Vector3Dot(dir, forward));

	if (animationCnt <= 0.4f)
	{
		/*Phoenix::Math::Vector3 axis;
		axis = Phoenix::Math::Vector3Cross(forward, dir);*/
		//angle = acosf(Phoenix::Math::Vector3Dot(dir, forward));

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

			speedY = JumpSpeed;
		}
		else
		{
			beginTrun = true;
		}
		boss->SetRotate(rotate);
	}
	else if (animationCnt <= 1.68f)
	{
		Phoenix::Math::Vector3 bossPos = boss->GetPosition();
		Phoenix::Math::Matrix boneM = boss->GetModel()->GetBoneTransforms(0, boss->GetBoneIndex());

		angle = atan2f(forward.x, forward.z);

		bossPos.x += sinf(angle) * MoveSpeed;
		//bossPos.y += boneM._42 - oldPosY;
		bossPos.y += speedY; // TODO : Add PosY
		bossPos.z += cosf(angle) * MoveSpeed;
		if (bossPos.y <= 0.0f)
		{
			bossPos.y = 0.0f;
		}
		boss->SetPosition(bossPos);

		oldPosY = boneM._42;
		speedY -= ForthSpeed; // TODO : Add PosY
	}
	animationCnt += 1 / 60.0f;
}