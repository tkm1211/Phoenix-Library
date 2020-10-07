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
	gravityCnt = 0;

	velocity = { 0.0f, 0.0f, 0.0f };
	acceleration = { 0.0f, 0.0f, 0.0f };
	moveSpeed = 0.0f;
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
		}
		else
		{
			beginTrun = true;
		}
		boss->SetRotate(rotate);

		//Phoenix::f32 dis = Phoenix::Math::Vector3Length(player->GetPosition() - boss->GetPosition());

		//velocity = QuadraticEquation(boss->GetPosition(), player->GetPosition(), 10.0f);
		//velocity.y *= 2.0f;
		//velocity.y = abs(velocity.y);

		//velocity.y /= MoveSpeed;
		//velocity.y *= dis / ((1.68f * 60.0f) - (0.4f * 60.0f));

		//float maxT = ((2 * velocity.y) / 9.8f);
		//velocity.y = boss->GetPosition().y + (velocity.y * maxT) - ((1.0f / 2.0f) * 9.8f * (maxT * maxT));

		Phoenix::Math::Vector3 playerPos = player->GetPosition();
		Phoenix::Math::Vector3 bossPos = boss->GetPosition();
		speedY = JumpSpeed;
		gravityCnt = 0;
		if (!OnChangeTarget(bossPos, playerPos, 300.0f))
		{
			isChangeState = true;
			nextStateType = AIStateType::Move;
			return;
		}
		else
		{
			shotY = shot[0].y < shot[1].y ? shot[1].y : shot[0].y;
			shotY *= 0.01f;
			nextY = shotY - g * 0.5f;

			Phoenix::f32 t = (1.68f - 0.4f) / (1.0f / 60.0f);
			moveSpeed = fabs(Phoenix::Math::Vector3Length(playerPos - bossPos)) / t;
			moveSpeed += 0.05f;
		}
		//speedY = Phoenix::Math::Vector2Length(Phoenix::Math::Vector2(velocity.x, velocity.z));
	}
	else if (animationCnt <= 1.68f)
	{
		Phoenix::Math::Vector3 bossPos = boss->GetPosition();
		Phoenix::Math::Matrix boneM = boss->GetModel()->GetBoneTransforms(0, boss->GetBoneIndex());

#if 1
		angle = atan2f(forward.x, forward.z);

		// Add Y
		{
			nextY -= g;
			bossPos.y += nextY / 5.0f;
			if (bossPos.y <= 0.0f)
			{
				bossPos.y = 0.0f;
			}
		}

		if (0.0f < bossPos.y)
		{
			bossPos.x += sinf(angle) * moveSpeed;
			bossPos.z += cosf(angle) * moveSpeed;
		}

		//Phoenix::f32 yMax = 50.0f;
		//Phoenix::f32 g = 9.8f * 0.01f;
		//Phoenix::f32 t = animationCnt - 0.4f;
		//t = t == 0.0f ? 0.0f : t / (1.68f - 0.4f);

		////bossPos.y += boneM._42 - oldPosY;
		////bossPos.y += speedY; // TODO : Add PosY
		////if (t <= 1.0f)
		//{
		//	Phoenix::f32 addY = sqrt(2.0f * g * yMax);
		//	addY -= 0.5f;
		//	addY = addY * g * t * t;

		//	bossPos.y = sqrt(2.0f * g * yMax) * t - 0.5f * g * t * t;

		//	gravityCnt++;
		//	g = gravityCnt == 1 ? 9.8f * 0.5f : 9.8f;
		//	bossPos.y -= g * gravityCnt * 0.01f;
		//	if (bossPos.y <= 0.0f)
		//	{
		//		bossPos.y = 0.0f;
		//	}
		//}
		boss->SetPosition(bossPos);

		oldPosY = boneM._42;
		speedY -= ForthSpeed; // TODO : Add PosY
#else
		/*speedY -= ForthSpeed;
		bossPos.x += velocity.x * (1 / 60.0f);
		bossPos.y += speedY;
		bossPos.z += velocity.z * (1 / 60.0f);*/

		bossPos += velocity * (1.0f / ((1.68f * 60.0f) - (0.4f * 60.0f)));
		acceleration = Gravity;
		velocity += acceleration * (1.0f / ((1.68f * 60.0f) - (0.4f * 60.0f)));

		if (bossPos.y <= 0.0f)
		{
			bossPos.y = 0.0f;
		}

		boss->SetPosition(bossPos);
#endif
	}
	animationCnt += 1 / 60.0f;
}

Phoenix::Math::Vector3 JumpAttackState::QuadraticEquation(const Phoenix::Math::Vector3& p, const Phoenix::Math::Vector3& t, Phoenix::f32 s, Phoenix::f32 g)
{
	Phoenix::Math::Vector3 tv;
	Phoenix::f32 v = s;

	Phoenix::Math::Vector2 xz = { t.x - p.x, t.z - p.z };

	Phoenix::f32 x = Phoenix::Math::Vector2Length(xz);
	Phoenix::f32 y = t.y - p.y;

	Phoenix::f32 x2 = x * x;
	Phoenix::f32 v2 = v * v;

	Phoenix::f32 A = ((g * x2) / (2.0f * v2));

	Phoenix::f32 a = A;
	Phoenix::f32 b = x;
	Phoenix::f32 c = A - y;

	Phoenix::f32 b2 = b * b;
	Phoenix::f32 ac = 4.0f * a * c;

	Phoenix::f32 kai = b2 - ac;

	Phoenix::Math::Vector3 X = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::s32 num = 0;
	if (0 < kai || 0 == kai)
	{
		Phoenix::f32 B = -b + sqrtf(abs(kai));
		Phoenix::f32 ta = 2.0f * a;

		Phoenix::f32 tanTheta = B / ta;
		Phoenix::f32 theta = atanf(tanTheta);

		Phoenix::Math::Vector3 v0;
		v0 = Phoenix::Math::Vector3Normalize(t - p);
		X.x = v0.x * cosf(theta) * s;
		X.y = sinf(theta) * s;
		X.z = v0.z * cosf(theta) * s;
	}

	return X;
}

bool JumpAttackState::OnChangeTarget(Phoenix::Math::Vector3 start, Phoenix::Math::Vector3 target, Phoenix::f32 speed)
{
	startPos = start;
	targetPos = target;

	Phoenix::Math::Vector3 disVec = targetPos - startPos;

	Phoenix::Math::Vector2 xz = { disVec.x, disVec.z };

	Phoenix::f32 disY = disVec.y;
	Phoenix::f32 disX = fabs(Phoenix::Math::Vector2Length(xz));
	//bool toBack = (disVec.x < 0) ? true : false;

	Phoenix::f32 g = -9.8f * 0.01f;
	Phoenix::f32 a = (g * disX * disX) / (2.0f * speed * speed);
	Phoenix::f32 b = disX / a;
	Phoenix::f32 c = (a - disY) / a;

	Phoenix::f32 root = pow(-c + (b * b) / 4.0f, 0.5f);

	Phoenix::f32 ts = (b * b / 4.0f) - c;
	if (ts < 0.0f)
	{
		return false;
	}
	else
	{
		Phoenix::f32 angle[2];
		angle[0] = atanf((-b / 2.0f) + root);
		angle[1] = atanf((-b / 2.0f) - root);

		for (Phoenix::s32 i = 0; i < 2; ++i)
		{
			shot[i].y = sinf(angle[i]) * speed;
		}
	}

	return true;
}