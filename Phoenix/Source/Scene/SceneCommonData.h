#pragma once

#include <memory>
#include "../Player/Player.h"
#include "../Boss/Boss.h"
#include "Phoenix/Types.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Shader/Shader.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FrameWork/Shader/StandardShader.h"


class SceneCommonData
{
public:
	std::shared_ptr<Player> player;
	std::shared_ptr<Boss> boss;
	std::shared_ptr<Phoenix::FrameWork::ModelObject> stageModel;
	std::shared_ptr<Phoenix::FrameWork::IShader> basicShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> basicSkinShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> standardShader;
	std::shared_ptr<Phoenix::Graphics::Camera> camera;

public:
	SceneCommonData() {}
	~SceneCommonData() {}

public:
	// ê∂ê¨
	static std::shared_ptr<SceneCommonData> Create();

	// èâä˙âª
	void Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
};