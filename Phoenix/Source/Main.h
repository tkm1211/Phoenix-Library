#pragma once
#include <stdio.h>
#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FrameWork/Main.h"
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Loader/Loader.h"
#include "Phoenix/OS/Path.h"
#include "Phoenix/OS/Resource.h"
#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/FrameWork/Renderer/Renderer.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Shader/Shader.h"
#include "../Source/Loader/Loader.h"
#include "imgui.h"
#include "Player/Player.h"
#include "Boss/Boss.h"


//****************************************************************************
// メイン
//****************************************************************************
class Main : public Phoenix::FrameWork::Main
{
private:
	using Super = Phoenix::FrameWork::Main;
	std::unique_ptr<Player> player;
	std::unique_ptr<Boss> boss;
	std::unique_ptr<Phoenix::FrameWork::ModelObject> stageModel;
	std::vector<std::unique_ptr<Phoenix::FrameWork::Renderer>> renderer;
	std::unique_ptr<Phoenix::FrameWork::IShader> basicShader;
	std::unique_ptr<Phoenix::FrameWork::IShader> basicSkinShader;
	std::unique_ptr<Phoenix::FrameWork::IShader> standardShader;
	Phoenix::Graphics::Camera camera;
	bool cameraFlg = false;

public:
	Main() {}
	~Main() {}

public:
	// 初期化
	bool Initialize(Phoenix::uintPtr instance) override;

	// 終了化
	void Finalize() override;

	// 更新
	void Update() override;

	// 描画
	void Render() override;

	// GUI
	void GUI();

protected:
	//// コンテナセットアップ
	//void OnSetupContainer(Phoenix::FrameWork::Container* container) override;

	//// ゲームセットアップ
	//void OnSetupGame(Phoenix::FrameWork::IGame* game) override;
};