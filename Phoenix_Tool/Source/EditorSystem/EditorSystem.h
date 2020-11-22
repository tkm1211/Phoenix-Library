#pragma once

#include <memory>
#include <vector>
#include "Phoenix/Types.h"
#include "Phoenix/Graphics/GraphicsDevice.h"


class IEditor
{
public:
	// 初期化
	virtual bool Initialize(std::shared_ptr<Phoenix::Graphics::IGraphicsDevice> graphicsDevice) = 0;

	// 終了化
	virtual void Finalize() = 0;

	// 更新
	virtual void Update(Phoenix::f32 elapsedTime) = 0;

	// 描画
	virtual void Draw(Phoenix::f32 elapsedTime) = 0;

	// GUI
	virtual void GUI() = 0;
};

class EditorSystem
{
private:
	std::vector<std::shared_ptr<IEditor>> editors;
	std::shared_ptr<IEditor> currentEditor;

public:
	EditorSystem() {}
	~EditorSystem() {}

public:
	// 生成
	static std::unique_ptr<EditorSystem> Create();

	// 初期化
	bool Initialize(std::shared_ptr<Phoenix::Graphics::IGraphicsDevice> graphicsDevice);

	// 終了化
	void Finalize();

	// 更新
	void Update(Phoenix::f32 elapsedTime);

	// 描画
	void Draw(Phoenix::f32 elapsedTime);

	// GUI
	void GUI();
};