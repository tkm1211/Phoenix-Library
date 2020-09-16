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
#include "Scene/SceneSystem.h"


//****************************************************************************
// ���C��
//****************************************************************************
class Main : public Phoenix::FrameWork::Main
{
private:
	using Super = Phoenix::FrameWork::Main;
	std::unique_ptr<SceneSystem> sceneSystem;

public:
	Main() {}
	~Main() {}

public:
	// ������
	bool Initialize(Phoenix::uintPtr instance) override;

	// �I����
	void Finalize() override;

	// �X�V
	void Update(Phoenix::f32 elapsedTime) override;

	// �`��
	void Render(Phoenix::f32 elapsedTime) override;

	// GUI
	void GUI();

protected:
	//// �R���e�i�Z�b�g�A�b�v
	//void OnSetupContainer(Phoenix::FrameWork::Container* container) override;

	//// �Q�[���Z�b�g�A�b�v
	//void OnSetupGame(Phoenix::FrameWork::IGame* game) override;
};