#pragma once

#include <vector>
#include <memory>
#include "Scene.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "SceneCommonData.h"


enum class SceneType
{
	Title,
	Game
};

class SceneSystem
{
private:
	std::vector<std::shared_ptr<Scene>> scenes;
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;
	Scene* stackScene = nullptr;

	Phoenix::Graphics::IGraphicsDevice* graphicsDevice = nullptr;
	std::shared_ptr<SceneCommonData> commonData;

public:
	SceneSystem() {}
	~SceneSystem() {}

public:
	static std::unique_ptr<SceneSystem> Create();
	void Init(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
	void Update();
	void Draw();
	void GUI();
	void ChangeScene(SceneType sceneType, bool stack);
	void ReSetStackScene();
	Phoenix::Graphics::IGraphicsDevice* GetGraphicsDevice() { return graphicsDevice; }
	SceneCommonData* GetSceneCommonData() { return commonData.get(); }

public:
	template<class T>
	T* AddScene()
	{
		std::shared_ptr<T> scene = std::make_shared<T>();
		scenes.emplace_back(scene);
		return scene.get();
	}

	Scene* GetScene(SceneType sceneType)
	{
		Phoenix::u32 index = static_cast<Phoenix::u32>(sceneType);
		return scenes[index].get();
	}
};