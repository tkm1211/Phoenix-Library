#pragma once

#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"


class Mannequin
{
private:
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;
	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;

	Phoenix::Math::Matrix worldMatrix;
	Phoenix::Math::Vector3 pos;
	Phoenix::Math::Quaternion rotate;
	Phoenix::Math::Vector3 scale;
	Phoenix::f32 radius;

public:
	Mannequin() {}
	~Mannequin() {}

public:
	static std::unique_ptr<Mannequin> Create();
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
	void Initialize();
	void Update(Phoenix::Graphics::Camera& camera, bool onControl);
	void UpdateTrasform();
	void GUI();

	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }
	const std::vector<Phoenix::FrameWork::CollisionData> GetCollisionDatas() { return collisionDatas; }

	Phoenix::Math::Matrix GetWorldMatrix() { return worldMatrix; }
	Phoenix::Math::Vector3 GetPosition() { return pos; }
	Phoenix::f32 GetRadius() { return radius; }

};