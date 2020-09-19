#pragma once

#include "Scene.h"
#include "Phoenix\FrameWork\Script\Lua.h"


class SceneLabo : public Scene
{
private:
	bool finished = false;
	
	// Lua
	std::unique_ptr<Phoenix::FrameWork::LuaSystem> luaSystem;
	Phoenix::s8 filePass[1024] = "";
	bool judgeLoad = false;
	Phoenix::s32 aliveCnt = 0;

public:
	SceneLabo() {}
	~SceneLabo() {}

public:
	static std::unique_ptr<SceneLabo> Create();
	void Construct(SceneSystem* sceneSystem) override;
	void Initialize() override;
	void Update(Phoenix::f32 elapsedTime) override;
	void Draw(Phoenix::f32 elapsedTime) override;
	void GUI() override;
	bool Finish() { return finished; }
};