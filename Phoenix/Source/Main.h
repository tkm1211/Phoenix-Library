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
#include "Phoenix/FrameWork/Component/Renderer.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "../Source/Loader/Loader.h"
#include "imgui.h"

//****************************************************************************
// メイン
//****************************************************************************
class Main : public Phoenix::FrameWork::Main
{
private:
	using Super = Phoenix::FrameWork::Main;
	Phoenix::Graphics::ModelData data;
	std::unique_ptr<Phoenix::Graphics::IShader> shader;
	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;
	std::vector<std::unique_ptr<Phoenix::FrameWork::Renderer>> renderer;
	Phoenix::Graphics::Camera camera;

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
	void Begin();
	void Render() override;
	void End();

protected:
	//// コンテナセットアップ
	//void OnSetupContainer(Phoenix::FrameWork::Container* container) override;

	//// ゲームセットアップ
	//void OnSetupGame(Phoenix::FrameWork::IGame* game) override;
};