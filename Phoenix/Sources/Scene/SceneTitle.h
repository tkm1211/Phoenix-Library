#pragma once

#include <DX11Math.h>

#include "SceneManager.h"


class SceneTitle : public Scene
{
public:
	Phoenix::Lib::Vector3 vector3;

public:
	SceneTitle() {}
	~SceneTitle() {}

public:
	void Init() override;
	void UnInit() override;
	void Update() override;
	void Render() override;
	void ImGui() override;

	void SetSceneManger(SceneManager* pSceneManager) override;
	void SetGraphicsDevice(GraphicsDevice* pGraphicsDevice) override;
};