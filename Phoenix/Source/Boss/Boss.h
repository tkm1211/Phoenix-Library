#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"


class Boss
{
private:
	enum class AnimationState
	{
		Idle,
	};

private:
	/*static constexpr*/ float WalkSpeed = 2.1f;
	/*static constexpr*/ float RunSpeed = 18.0f;
	/*static constexpr*/ float RollSpeed = 15.0f;
	/*static constexpr*/ float Attack03Speed = 10.0f;
	/*static constexpr*/ float AnimationSpeed30 = 0.03333333f;
	/*static constexpr*/ float AnimationSpeed45 = 0.02222222f;
	/*static constexpr*/ float AnimationSpeed60 = 0.01666667f;
	/*static constexpr*/ float Attack01ReceptionStartTime = 1.3333332f; // GoalÇÕÅAAnimationÇÃéûä‘ÇÃí∑Ç≥Ç©ÇÁéÊìæ // 20 * 0.0166666667f;
	/*static constexpr*/ float Attack02ReceptionStartTime = 2.2f; // GoalÇÕÅAAnimationÇÃéûä‘ÇÃí∑Ç≥Ç©ÇÁéÊìæ // 20 * 0.0166666667f;

private:
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;

	Phoenix::Math::Matrix worldMatrix;
	Phoenix::Math::Vector3 pos;
	Phoenix::Math::Vector3 rotate;
	Phoenix::Math::Vector3 scale;
	Phoenix::f32 speed;

	AnimationState animationState;
	//AttackAnimationState attackState;
	bool isChangeAnimation;
	bool isAttack;
	float attackReceptionTimeCnt;
	float animationSpeed;

	Phoenix::f32 radius;

public:
	Boss() {}
	~Boss() {}

public:
	static std::unique_ptr<Boss> Create();
	void Init(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
	void Update(Phoenix::Graphics::Camera& camera);
	void Control(Phoenix::Graphics::Camera& camera);
	void ChangeAnimation();
	void ChangeAttackAnimation();
	void GUI();

	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }
	Phoenix::Math::Matrix GetWorldMatrix() { return worldMatrix; }
	Phoenix::Math::Vector3 GetPosition() { return pos; }
	Phoenix::f32 GetRadius() { return radius; }
};