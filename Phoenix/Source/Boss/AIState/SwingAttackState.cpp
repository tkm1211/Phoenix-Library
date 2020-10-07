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
		Phoenix::Math::Vector3 dir = player->GetPosition() - boss->GetPosition();
		Phoenix::f32 len = Phoenix::Math::Vector3Length(Phoenix::Math::Vector3(dir.x, 0.0f, dir.z));

		isChangeState = true;

		if (player->IsAttack() && boss->IsAccumulationDamege())
		{
			nextStateType = AIStateType::Avoid;
		}
		/*else if ((boss->GetRadius() + player->GetRadius() + 100.0f) <= len
			&& player->GetAnimationState() == Player::AnimationState::Idle
			|| player->GetAnimationState() == Player::AnimationState::Roll
			|| player->GetAnimationState() == Player::AnimationState::Walk
			|| player->GetAnimationState() == Player::AnimationState::Run)
		{
			isChangeState = true;
			nextStateType = AIStateType::Move;
		}*/
		else
		{
			if (player->GetRadius() + boss->GetRadius() + 5.0f < len)
			{
				nextStateType = AIStateType::JumpAttack;
			}
			else
			{
#if 1
				Phoenix::s32 par = rand() % 100;

				if (swingDerivedNum < 2)
				{
					if (par <= 75)
					{
						nextStateType = AIStateType::SwingAttack02;
						swingDerivedNum++;
					}
					else nextStateType = AIStateType::RotationAttack;
				}
				else
				{
					if (par <= 75) nextStateType = AIStateType::RotationAttack;
					else nextStateType = AIStateType::SwingAttack02;

					swingDerivedNum = 0;
				}
#else
				nextStateType = AIStateType::RotationAttack;
#endif
			}
		}

		return;
	}
	if (animationCnt <= 0.84f)
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

void SwingAttackState02::Init()
{
	stateType = AIStateType::SwingAttack02;
	nextStateType = AIStateType::None;
	isChangeState = false;

	animationCnt = 0.0f;
}

void SwingAttackState02::Update(Boss* boss, Player* player)
{
	//if (2.0f <= animationCnt)
	if (!boss->GetModel()->IsPlaying())
	{
		isChangeState = true;

		if (player->IsAttack() && boss->IsAccumulationDamege())
		{
			nextStateType = AIStateType::Avoid;
		}
		else
		{
			nextStateType = AIStateType::Wait;
		}

		return;
	}
	if (animationCnt <= 0.46f)
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