#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"


class Player
{
private:
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;

	Phoenix::Math::Matrix worldMatrix;
	Phoenix::Math::Vector3 pos;
	Phoenix::Math::Vector3 rotate;
	Phoenix::Math::Vector3 scale;
	Phoenix::f32 speed;

public:
	Player() : worldMatrix(Phoenix::Math::MatrixIdentity()) {}
	~Player() {}

public:
	static std::unique_ptr<Player> Create();
	void Init(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
	void Update(Phoenix::Graphics::Camera& camera);
	void GUI();

	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }
	Phoenix::Math::Matrix GetWorldMatrix() { return worldMatrix; }
	Phoenix::Math::Vector3 GetPosition() { return pos; }
};