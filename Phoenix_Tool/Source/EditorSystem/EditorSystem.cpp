#include "EditorSystem.h"
#include "../PlayerEditor/PlayerEditor.h"

#define CURRENT_EDITOR_NUM 0


// 生成
std::unique_ptr<EditorSystem> EditorSystem::Create()
{
	return std::make_unique<EditorSystem>();
}

// 初期化
bool EditorSystem::Initialize(std::shared_ptr<Phoenix::Graphics::IGraphicsDevice> graphicsDevice)
{
	// エディター追加
	{
		editors.emplace_back(PlayerEditor::Create());
	}

	// エディター選択
	{
		currentEditor = editors.at(CURRENT_EDITOR_NUM);
		currentEditor->Initialize(graphicsDevice);
	}

	return true;
}

// 終了化
void EditorSystem::Finalize()
{
	currentEditor.reset();

	for (const auto& editor : editors)
	{
		editor->Finalize();
	}
}

// 更新
void EditorSystem::Update(Phoenix::f32 elapsedTime)
{
	currentEditor->Update(elapsedTime);
}

// 描画
void EditorSystem::Draw(Phoenix::f32 elapsedTime)
{
	currentEditor->Draw(elapsedTime);
}

// GUI
void EditorSystem::GUI()
{
	currentEditor->GUI();
}