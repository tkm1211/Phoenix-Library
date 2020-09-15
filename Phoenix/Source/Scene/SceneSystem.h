#pragma once

#include <vector>
#include <memory>
#include "Scene.h"
#include "Phoenix/OS/Display.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "SceneCommonData.h"


enum class SceneType
{
	Title,
	Game
};

class FadeSystem;
class SceneSystem
{
private:
	std::vector<std::shared_ptr<Scene>> scenes;
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;
	Scene* stackScene = nullptr;

	Phoenix::OS::IDisplay* display = nullptr;
	Phoenix::Graphics::IGraphicsDevice* graphicsDevice = nullptr;
	std::shared_ptr<SceneCommonData> commonData;
	std::shared_ptr<FadeSystem> fadeSystem;

public:
	SceneSystem() {}
	~SceneSystem() {}

public:
	static std::unique_ptr<SceneSystem> Create();
	void Init(Phoenix::OS::IDisplay* display, Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
	void Update();
	void Draw();
	void GUI();
	void ChangeScene(SceneType sceneType, bool stack, bool fade);
	void SetScene(SceneType sceneType);
	void ReSetStackScene();

	Phoenix::OS::IDisplay* GetDisplay() { return display; }
	Phoenix::Graphics::IGraphicsDevice* GetGraphicsDevice() { return graphicsDevice; }
	SceneCommonData* GetSceneCommonData() { return commonData.get(); }

	bool GetOnFade();

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