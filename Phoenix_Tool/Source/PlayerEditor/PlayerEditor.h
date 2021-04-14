#pragma once

#include <memory>
#include "../EditorSystem/EditorSystem.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "Player/Player.h"
#include "Primitive/GeometricPrimitive.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FrameWork/Shader/PBRShader.h"


class PlayerEditor : public IEditor
{
private:
	std::shared_ptr<Phoenix::Graphics::IGraphicsDevice> graphicsDevice;
	std::shared_ptr<Player> player;
	std::unique_ptr<Phoenix::Graphics::Camera> camera;
	std::shared_ptr<Phoenix::FrameWork::IShader> basicSkinShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> pbrShader;
	std::shared_ptr<GeometricPrimitive> primitive;
	std::vector<bool> selected;
	Phoenix::s32 currentAttackNum = -1;
	std::vector<bool> helpOpen;
	Phoenix::s32 keyCount = 0;

	bool saveExit = false;
	bool saveCheck = false;
	Phoenix::s32 saveCount = 0;

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
	/// �t�@�C����ۑ��m�F
	/// </summary>
	void SaveCheck();

	/// <summary>
	/// �t�@�C����ۑ�
	/// </summary>
	void SaveFile();

	void AddAttackData(Phoenix::s32 attackNum);

	void AddData(Phoenix::s32 attackNum);

	void PrimitiveRender(Phoenix::Graphics::DeviceDX11* device, Phoenix::Math::Vector3 translate, Phoenix::Math::Vector3 rotate, Phoenix::Math::Vector3 scale);
};