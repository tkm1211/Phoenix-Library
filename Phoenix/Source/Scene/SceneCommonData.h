#pragma once

#include <memory>
#include "../Player/Player.h"
#include "../Boss/Boss.h"
#include "../Enemy/EnemyManager.h"
#include "../Mannequin/Mannequin.h"
#include "../AI/MetaAI/MetaAI.h"
#include "../UI/UISystem.h"
#include "../UI/TargetMarkUI.h"
#include "../Sound/SoundSystem.h"
#include "Phoenix/Types.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Audio/Audio.h"
#include "Phoenix/FrameWork/Shader/Shader.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FrameWork/Shader/StandardShader.h"
#include "Phoenix/FrameWork/Shader/PBRShader.h"


class SceneCommonData
{
public:
	std::shared_ptr<Player> player;
	std::shared_ptr<Boss> boss;
	std::shared_ptr<EnemyManager> enemyManager;
	std::shared_ptr<Mannequin> mannequin;
	std::shared_ptr<MetaAI> metaAI;
	std::shared_ptr<TargetMarkUI> targetMarkUI;
	std::shared_ptr<UISystem> uiSystem;
	std::shared_ptr<Phoenix::FrameWork::ModelObject> stageModel;
	std::shared_ptr<Phoenix::FrameWork::ModelObject> bossStageModel;
	std::shared_ptr<Phoenix::FrameWork::IShader> basicShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> basicSkinShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> standardShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> pbrShader;
	std::shared_ptr<Phoenix::FrameWork::IShader> pbrSkinShader;
	std::shared_ptr<Phoenix::Graphics::Camera> camera;

	//// フレームバッファ
	//std::unique_ptr<Phoenix::FrameWork::FrameBuffer> frameBuffer[3];

	//// ポストプロセス
	//std::unique_ptr<Phoenix::FrameWork::PostProcessingEffects> postProcessingEffects;

	//// シャドウマップ
	//std::unique_ptr<Phoenix::FrameWork::FrameBuffer> shadowMap;
	//std::unique_ptr<Phoenix::Graphics::IShader> voidPS;
	//std::unique_ptr<Phoenix::Graphics::Camera> lightSpaceCamera;
	//std::unique_ptr<Phoenix::Graphics::IBuffer> shaderConstantsBuffer;
	//std::unique_ptr<Phoenix::Graphics::ISampler> comparisonSamplerState;
	////ShaderConstants shaderContexts;

	//// ブルーム
	//std::unique_ptr<Phoenix::FrameWork::Quad> quad;
	//std::unique_ptr<Phoenix::FrameWork::MSAAResolve> msaaResolve;
	//std::unique_ptr<Phoenix::FrameWork::Bloom> bloom;

	//// スカイマップ
	//std::unique_ptr<Phoenix::FrameWork::SkyMap> skyMap;

	//// トーンマップ
	//std::unique_ptr<Phoenix::FrameWork::ToneMap> toneMap;

	// エフェクト
	std::shared_ptr<Phoenix::Graphics::ITexture> targetMark;

	// BGM + SE
	std::shared_ptr<Phoenix::FrameWork::CXAudio2> bgm;
	std::shared_ptr<Phoenix::FrameWork::CXAudio2> se;

	// Sound
	std::shared_ptr<SoundSystem<SoundType>> soundSystem;

	// Tutorial
	std::shared_ptr<Phoenix::Graphics::ITexture> operatorUI;
	/*std::shared_ptr<Phoenix::Graphics::ITexture> avoidUI;
	std::shared_ptr<Phoenix::Graphics::ITexture> attackUI;
	std::shared_ptr<Phoenix::Graphics::ITexture> runUI;
	std::shared_ptr<Phoenix::Graphics::ITexture> targetUI;*/

public:
	SceneCommonData() {}
	~SceneCommonData() {}

public:
	// 生成
	static std::shared_ptr<SceneCommonData> Create();

	// 初期化
	void Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);
};