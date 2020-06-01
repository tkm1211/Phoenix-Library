#pragma once


class SceneManager;
class Scene
{
public:
	virtual void Init(SceneManager* sceneManager) = 0;
	virtual void UnInit() = 0;
	virtual void Update(SceneManager* sceneManager) = 0;
	virtual void Render() = 0;
	virtual void ImGui() = 0;
};

class SceneManager
{
public:
	Scene* mainScene;
	Scene* nextScene;
	Scene* stackScene;

public:
	SceneManager() : mainScene(nullptr), nextScene(nullptr), stackScene(nullptr) {}
	~SceneManager() {}

public:
	void Init();
	void UnInit();
	void Update();
	void Render();
	void ImGui();

public:
	// Scene* scene : 次のシーン, bool nowSceneStack : 現在のシーンを残したいかどうか？
	void SetScene(Scene* scene, bool nowSceneStack = false);

	// スタックしたシーンに戻す関数
	void ReSetStackScene();
};