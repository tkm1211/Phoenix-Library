#pragma once

#include <memory>
#include "../Player/Player.h"
#include "../Boss/Boss.h"
#include "Phoenix/Types.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Shader/Shader.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"


class SceneSystem;
class Scene
{
protected:
	SceneSystem* sceneSystem = nullptr;
	Phoenix::Graphics::IGraphicsDevice* graphicsDevice = nullptr;

public:
	Scene() {}
	virtual ~Scene() {}

public:
	virtual void Init(SceneSystem* sceneSystem) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void GUI() = 0;
};

class SceneTitle : public Scene
{
private:


public:
	SceneTitle() {}
	~SceneTitle() {}

public:
	void Init(SceneSystem* sceneSystem) override;
	void Update() override;
	void Draw() override;
	void GUI() override;
};

class SceneGame : public Scene
{
private:
	std::unique_ptr<Player> player;
	std::unique_ptr<Boss> boss;
	std::unique_ptr<Phoenix::FrameWork::ModelObject> stageModel;
	std::unique_ptr<Phoenix::FrameWork::IShader> basicShader;
	std::unique_ptr<Phoenix::FrameWork::IShader> basicSkinShader;
	std::unique_ptr<Phoenix::FrameWork::IShader> standardShader;
	Phoenix::Graphics::Camera camera;
	bool cameraFlg = false;

public:
	SceneGame() {}
	~SceneGame() {}

public:
	void Init(SceneSystem* sceneSystem) override;
	void Update() override;
	void Draw() override;
	void GUI() override;
};