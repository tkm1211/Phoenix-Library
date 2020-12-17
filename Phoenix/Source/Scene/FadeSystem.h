#pragma once

#include "SceneSystem.h"


class FadeSystem
{
private:
	const Phoenix::f32 dissolveSpeed = 1.0f;

private:
	bool working = false;
	bool isTrun = false;
	SceneType type = SceneType::Title;
	Phoenix::f32 dissolveThreshold = 0.0f;

	std::unique_ptr<Phoenix::FrameWork::Quad> quad;
	std::unique_ptr<Phoenix::Graphics::ITexture> fade;

public:
	FadeSystem() {}
	~FadeSystem() {}

public:
	static std::shared_ptr<FadeSystem> Create();

	bool Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);

	void Finalize();

	void Update(SceneSystem* sceneSystem, Phoenix::f32 elapsedTime);

	void Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);

	void OnFade(SceneType sceneType);

	bool GetWorking() { return working; }
};