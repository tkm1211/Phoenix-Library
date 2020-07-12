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
		nextStateType = AIStateType::Wait;
		return;
	}

	Phoenix::Math::Vector3 dir = player->GetPosition() - boss->GetPosition();
	dir = Phoenix::Math::Vector3Normalize(dir);
	dir.y = 0.0f;

	Phoenix::Math::Quaternion rotate = boss->GetRotate();
	Phoenix::Math::Matrix m = Phoenix::Math::MatrixRotationQuaternion(&rotate);
	Phoenix::Math::Vector3 foward = Phoenix::Math::Vector3(m._31, m._32, m._33);
	Phoenix::f32 angle;

	if (!beginTrun)
	{
		Phoenix::Math::Vector3 axis;
		axis = Phoenix::Math::Vector3Cross(foward, dir);
		angle = acosf(Phoenix::Math::Vector3Dot(dir, foward));

		if (1e-8f < fabs(angle))
		{
			Phoenix::Math::Quaternion q;
			q = Phoenix::Math::QuaternionRotationAxis(axis, angle);
			rotate *= q;
		}
		boss->SetRotate(rotate);

		beginTrun = true;
	}
	if (animationCnt <= 1.68f)
	{
		Phoenix::Math::Vector3 bossPos = boss->GetPosition();
		Phoenix::Math::Matrix boneM = boss->GetModel()->GetBoneTransforms(0, boss->GetBoneIndex());

		angle = atan2f(foward.x, foward.z);

		bossPos.x += sinf(angle) * MoveSpeed;
		bossPos.y += boneM._42 - oldPosY;
		bossPos.z += cosf(angle) * MoveSpeed;
		boss->SetPosition(bossPos);

		oldPosY = boneM._42;
	}
	animationCnt += 1 / 60.0f;
}