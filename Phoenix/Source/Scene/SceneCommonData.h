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
		// エフェクトを解放します。再生中の場合は、再生が終了した後、自動的に解放されます。
		//ES_SAFE_RELEASE(effect);

		// エフェクト管理用インスタンスを破棄
		manager->Destroy();

		// サウンド用インスタンスを破棄
		//sound->Destroy();

		// 描画用インスタンスを破棄
		renderer->Destroy();
	}

public:
	// 生成
	static std::shared_ptr<SceneCommonData> Create();

	// 初期化
	void Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
};