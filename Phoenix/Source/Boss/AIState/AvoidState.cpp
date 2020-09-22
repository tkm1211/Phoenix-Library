#include "AvoidState.h"
#include "../../Player/Player.h"
#include "../Boss.h"


void AvoidState::Init()
{
	stateType = AIStateType::Avoid;
	nextStateType = AIStateType::None;
	isChangeState = false;

	animationCnt = 0.0f;
}

void AvoidState::Update(Boss* boss, Player* player)
{
	if (!boss->GetModel()->IsPlaying())
	{
		isChangeState = true;

		Phoenix::f32 dis = Phoenix::Math::Vector3Length(player->GetPosition() - boss->GetPosition());
		nextStateType = dis <= judgeDistance ? AIStateType::SwingAttack01 : AIStateType::JumpAttack;

		return;
	}
	if (animationCnt <= 0.0f)
	{
		Phoenix::Math::Quaternion rotate = boss->GetRotate();
		Phoenix::Math::Matrix matrix = Phoenix::Math::MatrixRotationQuaternion(&rotate);
		Phoenix::Math::Vector3 forward = { matrix._31, matrix._32, matrix._33 };

		Phoenix::Math::Vector3 right = { matrix._11, matrix._12, matrix._13 };
		if (rand() % 2)
		{
			right *= -1;
		}

		//Phoenix::Math::Vector3 axis = Phoenix::Math::Vector3Cross(forward, right);
		Phoenix::f32 angle = acosf(Phoenix::Math::Vector3Dot(right, forward));

		if (1e-8f < fabs(angle))
		{
			Phoenix::Math::Quaternion q;
			q = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angle);
			rotate *= q;
		}
		boss->SetRotate(rotate);
	}

	Phoenix::Math::Quaternion rotate = boss->GetRotate();
	Phoenix::Math::Matrix matrix = Phoenix::Math::MatrixRotationQuaternion(&rotate);
	Phoenix::Math::Vector3 foward = { matrix._31, matrix._32, matrix._33 };

	Phoenix::f32 angle = atan2f(foward.x, foward.z);

	Phoenix::Math::Vector3 pos = boss->GetPosition();
	pos.x += sinf(angle) * RollSpeed;
	pos.z += cosf(angle) * RollSpeed;

	boss->SetPosition(pos);
	animationCnt += 1.0f / 60.0f;
}