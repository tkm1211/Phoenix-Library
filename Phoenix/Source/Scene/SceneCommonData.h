#pragma once

#include <memory>
#include "../Player/Player.h"
#include "../Boss/Boss.h"
#include "Phoenix/Types.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Shader/Shader.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FrameWork/Shader/StandardShader.h"
#include "Phoenix/FrameWork/Shader/PBRShader.h"
#include "../../Effekseer/include/Effekseer/Effekseer.h"
#include "../../Effekseer/include/EffekseerRendererDX11/EffekseerRendererDX11.h"
#include "../../Effekseer/include/EffekseerSoundXAudio2/EffekseerSoundXAudio2.h"


class SceneCommonData
{
public:
	std::shared_ptr<Player> player;
	std::shared_ptr<Boss> boss;
	std::shared_ptr<Phoenix::FrameWork::ModelObject> stageModel;
	std::shared_ptr<Phoenix::FrameWork::IShader> basicShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> basicSkinShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> standardShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> pbrShader;
	std::shared_ptr<Phoenix::Graphics::Camera> camera;

	::EffekseerRenderer::Renderer* renderer = nullptr;
	::Effekseer::Manager* manager = nullptr;

public:
	SceneCommonData() {}
	~SceneCommonData()
	{
		// �G�t�F�N�g��������܂��B�Đ����̏ꍇ�́A�Đ����I��������A�����I�ɉ������܂��B
		//ES_SAFE_RELEASE(effect);

		// �G�t�F�N�g�Ǘ��p�C���X�^���X��j��
		manager->Destroy();

		// �T�E���h�p�C���X�^���X��j��
		//sound->Destroy();

		// �`��p�C���X�^���X��j��
		renderer->Destroy();
	}

public:
	// ����
	static std::shared_ptr<SceneCommonData> Create();

	// ������
	void Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
};