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
	// ����
	static std::shared_ptr<PlayerEditor> Create();

	// ������
	bool Initialize(std::shared_ptr<Phoenix::Graphics::IGraphicsDevice> graphicsDevice) override;

	// �I����
	void Finalize() override;

	// �X�V
	void Update(Phoenix::f32 elapsedTime) override;

	// �`��
	void Draw(Phoenix::f32 elapsedTime) override;

	// GUI
	void GUI() override;

public:
	/// <summary>
	/// �I�����Z�b�g
	/// </summary>
	void ResetSelected();

	/// <summary>
	/// �V�K�쐬
	/// </summary>
	void NewFile();

	/// <summary>
	/// �t�@�C�����J��
	/// </summary>
	void OpenFile();

	/// <summary>
	/// �t�@�C����ۑ�
	/// </summary>
	void SaveFile();
};