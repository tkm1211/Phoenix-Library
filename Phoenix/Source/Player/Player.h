#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"


class Player
{
private:
	enum class AnimationState
	{
		Wait,
		Walk,
		Run,
		Roll,
		//BackWalk,
		//BackRun,
	};

private:
	/*static constexpr*/ float WalkSpeed = 2.1f;
	/*static constexpr*/ float RunSpeed = 18.0f;
	/*static constexpr*/ float RollSpeed = 10.0f;

private:
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;

	Phoenix::Math::Matrix worldMatrix;
	Phoenix::Math::Vector3 pos;
	Phoenix::Math::Vector3 rotate;
	Phoenix::Math::Vector3 scale;
	Phoenix::f32 speed;

	AnimationState animationState;
	bool isChangeAnimation;

public:
	Player() : worldMatrix(Phoenix::Math::MatrixIdentity()), speed(0.0f), animationState(AnimationState::Wait), isChangeAnimation(false) {}
	~Player() {}

public:
	static std::unique_ptr<Player> Create();
	void Init(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
	void Update(Phoenix::Graphics::Camera& camera);
	void Control(Phoenix::Graphics::Camera& camera);
	void ChangeAnimation();
	void GUI();

	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }
	Phoenix::Math::Matrix GetWorldMatrix() { return worldMatrix; }
	Phoenix::Math::Vector3 GetPosition() { return pos; }
};