#pragma once

#include <memory>
#include "../EditorSystem/EditorSystem.h"
#include "Player/Player.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FrameWork/Shader/PBRShader.h"


class PlayerEditor : public IEditor
{
private:
	std::shared_ptr<Phoenix::Graphics::IGraphicsDevice> graphicsDevice;
	std::unique_ptr<Player> player;
	std::unique_ptr<Phoenix::Graphics::Camera> camera;
	std::shared_ptr<Phoenix::FrameWork::IShader> basicSkinShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> pbrShader;
	std::vector<bool> selected;
	Phoenix::s32 currentAttackNum = -1;

public:
	PlayerEditor() {}
	~PlayerEditor() {}

public:
	// 生成
	static std::shared_ptr<PlayerEditor> Create();

	// 初期化
	bool Initialize(std::shared_ptr<Phoenix::Graphics::IGraphicsDevice> graphicsDevice) override;

	// 終了化
	void Finalize() override;

	// 更新
	void Update(Phoenix::f32 elapsedTime) override;

	// 描画
	void Draw(Phoenix::f32 elapsedTime) override;

	// GUI
	void GUI() override;

public:
	/// <summary>
	/// 選択リセット
	/// </summary>
	void ResetSelected();

	/// <summary>
	/// 新規作成
	/// </summary>
	void NewFile();

	/// <summary>
	/// ファイルを開く
	/// </summary>
	void OpenFile();

	/// <summary>
	/// ファイルを保存
	/// </summary>
	void SaveFile();
};