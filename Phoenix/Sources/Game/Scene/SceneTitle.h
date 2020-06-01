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
	void Init(SceneManager* sceneManager) override;
	void UnInit() override;
	void Update(SceneManager* sceneManager) override;
	void Render() override;
	void ImGui() override;
};