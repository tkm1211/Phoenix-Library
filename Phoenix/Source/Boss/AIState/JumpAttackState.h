#pragma once

#include "AIState.h"
#include "Phoenix/Math/PhoenixMath.h"


class JumpAttackState : public AIState
{
private:
	/*static constexpr*/ Phoenix::f32 MoveSpeed = 0.05f;
	/*static constexpr*/ Phoenix::f32 JumpSpeed = 0.15f;
	/*static constexpr*/ Phoenix::f32 ForthSpeed = 0.004f;

	///*static constexpr*/ Phoenix::f32 MoveSpeed = 5.2f;
	///*static constexpr*/ Phoenix::f32 JumpSpeed = 0.05f;
	///*static constexpr*/ Phoenix::f32 ForthSpeed = 0.005f;
	///*static constexpr*/ Phoenix::Math::Vector3 Gravity = { 0.0f, -9.8f * 2.0f, 0.0f };
	//Phoenix::f32 t = 1.0f / ((1.68f * 60.0f) - (0.4f * 60.0f));

private:
	bool beginTrun = false;
	Phoenix::f32 oldPosY = 0.0f;
	Phoenix::f32 oldAngleY = 0.0f;
	Phoenix::f32 animationCnt = 0.0f;
	Phoenix::f32 speedY = 0.0f;
	Phoenix::f32 time = 0.0f;

	Phoenix::Math::Vector3 velocity = { 0.0f, 0.0f, 0.0f };
	Phoenix::Math::Vector3 acceleration = { 0.0f, 0.0f, 0.0f };

public:
	JumpAttackState() {}
	~JumpAttackState() {}

public:
	void Init() override;
	void Update(Boss* boss, Player* player) override;
	Phoenix::f32 GetAnimationCnt() { return animationCnt; }
	Phoenix::Math::Vector3 QuadraticEquation(const Phoenix::Math::Vector3& p, const Phoenix::Math::Vector3& t, Phoenix::f32 s, Phoenix::f32 g = -9.8f);
};
