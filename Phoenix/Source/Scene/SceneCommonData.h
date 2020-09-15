#pragma once

#include <memory>
#include "../Player/Player.h"
#include "../Boss/Boss.h"
#include "../UI/UISystem.h"
#include "Phoenix/Types.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Shader/Shader.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FrameWork/Shader/StandardShader.h"
#include "Phoenix/FrameWork/Shader/PBRShader.h"


class SceneCommonData
{
public:
	std::shared_ptr<Player> player;
	std::shared_ptr<Boss> boss;
	std::shared_ptr<UISystem> uiSystem;
	std::shared_ptr<Phoenix::FrameWork::ModelObject> stageModel;
	std::shared_ptr<Phoenix::FrameWork::ModelObject> bossStageModel;
	std::shared_ptr<Phoenix::FrameWork::IShader> basicShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> basicSkinShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> standardShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> pbrShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> pbrSkinShader;
	std::shared_ptr<Phoenix::Graphics::Camera> camera;

	// エフェクト
	std::shared_ptr<Phoenix::Graphics::ITexture> targetMark;

public:
	SceneCommonData() {}
	~SceneCommonData() {}

public:
	// 生成
	static std::shared_ptr<SceneCommonData> Create();

	// 初期化
	void Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
};