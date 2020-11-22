#include "EditorSystem.h"
#include "../PlayerEditor/PlayerEditor.h"

#define CURRENT_EDITOR_NUM 0


// ����
std::unique_ptr<EditorSystem> EditorSystem::Create()
{
	return std::make_unique<EditorSystem>();
}

// ������
bool EditorSystem::Initialize(std::shared_ptr<Phoenix::Graphics::IGraphicsDevice> graphicsDevice)
{
	// �G�f�B�^�[�ǉ�
	{
		editors.emplace_back(PlayerEditor::Create());
	}

	// �G�f�B�^�[�I��
	{
		currentEditor = editors.at(CURRENT_EDITOR_NUM);
		currentEditor->Initialize(graphicsDevice);
	}

	return true;
}

// �I����
void EditorSystem::Finalize()
{
	currentEditor.reset();

	for (const auto& editor : editors)
	{
		editor->Finalize();
	}
}

// �X�V
void EditorSystem::Update(Phoenix::f32 elapsedTime)
{
	currentEditor->Update(elapsedTime);
}

// �`��
void EditorSystem::Draw(Phoenix::f32 elapsedTime)
{
	currentEditor->Draw(elapsedTime);
}

// GUI
void EditorSystem::GUI()
{
	currentEditor->GUI();
}