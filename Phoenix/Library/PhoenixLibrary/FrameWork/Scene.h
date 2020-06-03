#pragma once

#include <GraphicsDevice.h>


class SceneManager;
class Scene
{
protected:
	SceneManager* sceneManager;
	GraphicsDevice* graphicsDevice;

public:
	Scene() {}
	virtual ~Scene() {}

public:
	virtual void Init() = 0;
	virtual void UnInit() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	// Update��Render�̊ԂŃR�[������܂�
	virtual void ImGui() = 0;

	virtual void SetSceneManger(SceneManager* pSceneManager) = 0;
	virtual void SetGraphicsDevice(GraphicsDevice* pGraphicsDevice) = 0;
};

class SceneManager
{
private:
	Scene* mainScene;
	Scene* nextScene;
	Scene* stackScene;

	GraphicsDevice* graphicsDevice;

public:
	SceneManager() : mainScene(nullptr), nextScene(nullptr), stackScene(nullptr), graphicsDevice(nullptr) {}
	~SceneManager() {}

public:
	void Init(Scene* firstScene, GraphicsDevice* pGraphicsDevice);
	void UnInit();
	void Update();
	void Render();
	void ImGui();

	void ImGuiNewFrame();
	void ImGuiRender();

public:
	// Scene* scene : ���̃V�[��, bool nowSceneStack : ���݂̃V�[�����c���������ǂ����H
	void SetScene(Scene* scene, bool nowSceneStack = false);

	// �X�^�b�N�����V�[���ɖ߂��֐�
	void ReSetStackScene();
};