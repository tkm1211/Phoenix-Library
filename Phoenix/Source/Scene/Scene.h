#pragma once

#include <memory>
#include "../Player/Player.h"
#include "../Boss/Boss.h"
#include "../UI/UISystem.h"
#include "../Primitive/GeometricPrimitive.h"
#include "Phoenix/Types.h"
#include "Phoenix/OS/Display.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/Graphics/Texture.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Shader/Shader.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FrameWork/Shader/StandardShader.h"
#include "Phoenix/FrameWork/Shader/PBRShader.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
//#include "../../Effekseer/include/Effekseer/Effekseer.h"
//#include "../../Effekseer/include/EffekseerRendererDX11/EffekseerRendererDX11.h"
//#include "../../Effekseer/include/EffekseerSoundXAudio2/EffekseerSoundXAudio2.h"
#include "Phoenix/FrameWork/FrameBuffer/FrameBuffer.h"
#include "Phoenix/FrameWork/Quad/Quad.h"
#include "Phoenix/FrameWork/IBL/IBL.h"
#include "Phoenix/FrameWork/ComputeShader/TestComputeShader.h"
#include "Phoenix/FrameWork/ComputeShader/BitonicSort.h"
#include "Phoenix/FrameWork/ComputeShader/GPUParticle.h"


class SceneSystem;
class SceneCommonData;
class Scene
{
protected:
	SceneSystem* sceneSystem = nullptr;
	Phoenix::OS::IDisplay* display = nullptr;
	Phoenix::Graphics::IGraphicsDevice* graphicsDevice = nullptr;
	SceneCommonData* commonData = nullptr;

public:
	Scene() {}
	virtual ~Scene() {}

public:
	virtual void Init(SceneSystem* sceneSystem) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void GUI() = 0;
};

class SceneTitle : public Scene
{
private:
	//::Effekseer::Effect* effect = nullptr;
	//::Effekseer::Handle handle = 0;
	std::shared_ptr<Phoenix::FrameWork::ModelObject> model;

public:
	SceneTitle() {}
	~SceneTitle()
	{
		// エフェクトを解放します。再生中の場合は、再生が終了した後、自動的に解放されます。
		//ES_SAFE_RELEASE(effect);
	}

public:
	void Init(SceneSystem* sceneSystem) override;
	void Update() override;
	void Draw() override;
	void GUI() override;
};

class SceneGame : public Scene
{
private:
	struct ShaderConstants
	{
		Phoenix::Math::Matrix lightViewProjection = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
		Phoenix::Math::Vector3 color = Phoenix::Math::Vector3(0.65f, 0.65f, 0.65f);
		Phoenix::f32 bias = 0.0008f;
	};

private:
	// 共通データのアドレス
	Player* player = nullptr;
	Boss* boss = nullptr;
	UISystem* uiSystem = nullptr;
	Phoenix::FrameWork::ModelObject* stageModel = nullptr;
	Phoenix::FrameWork::IShader* basicShader = nullptr;
	Phoenix::FrameWork::IShader* basicSkinShader = nullptr;
	Phoenix::FrameWork::IShader* standardShader = nullptr;
	Phoenix::FrameWork::IShader* pbrShader = nullptr;
	Phoenix::FrameWork::IShader* pbrSkinShader = nullptr;
	Phoenix::FrameWork::IShader* currentShader = nullptr;
	Phoenix::Graphics::Camera* camera = nullptr;
	Phoenix::Graphics::ITexture* targetMark = nullptr;

	// フレームバッファ
	std::unique_ptr<Phoenix::FrameWork::FrameBuffer> frameBuffer[3];

	// ポストプロセス
	std::unique_ptr<Phoenix::FrameWork::PostProcessingEffects> postProcessingEffects;

	// シャドウマップ
	std::unique_ptr<Phoenix::FrameWork::FrameBuffer> shadowMap;
	std::unique_ptr<Phoenix::Graphics::IShader> voidPS;
	std::unique_ptr<Phoenix::Graphics::Camera> lightSpaceCamera;
	std::unique_ptr<Phoenix::Graphics::IBuffer> shaderConstantsBuffer;
	std::unique_ptr<Phoenix::Graphics::ISampler> comparisonSamplerState;
	ShaderConstants shaderContexts;

	// ブルーム
	std::unique_ptr<Phoenix::FrameWork::Quad> quad;
	std::unique_ptr<Phoenix::FrameWork::MSAAResolve> msaaResolve;
	std::unique_ptr<Phoenix::FrameWork::Bloom> bloom;

	// スカイマップ
	std::unique_ptr<Phoenix::FrameWork::SkyMap> skyMap;

	// IBL
	std::unique_ptr<Phoenix::FrameWork::FrameBuffer> skyFrameBuffer[6];
	std::unique_ptr<Phoenix::FrameWork::IBL> ibl;

private: // Debug
	std::shared_ptr<GeometricPrimitive> primitive;
	Phoenix::Math::Vector2 texSize;

	Phoenix::Math::Vector3 particlePos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::Math::Vector4 particleNormal = Phoenix::Math::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	Phoenix::Math::Color particleMainColor = Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f);
	Phoenix::f32 particleLife = 1.0f;
	Phoenix::f32 particleSize = 1.0f;
	Phoenix::f32 particleScale = 1.0f;

	std::unique_ptr<Phoenix::FrameWork::IComputeShader> testComputeShader;
	std::unique_ptr<Phoenix::FrameWork::BitonicSort> bitonicSort;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> gpuParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> playerHitParticle;

	Phoenix::f32 dis = 10.0f;
	Phoenix::f32 width = 30.0f;
	Phoenix::f32 height = 30.0f;
	Phoenix::f32 nearZ = 1.0f;
	Phoenix::f32 farZ = 30.0f;

	bool cameraFlg = false;
	bool lockOnCamera = false;
	bool isHitCollision = false;
	bool isUpdate = false;
	bool isPlayerUpdate = false;
	bool isBossUpdate = false;
	bool enableMSAA = false;
	bool shadowBlend = false;
	bool bloomBlend = false;
	bool isPBR = false;
	bool active[10] = { false };

public:
	SceneGame() {}
	~SceneGame()
	{
		// エフェクトを解放します。再生中の場合は、再生が終了した後、自動的に解放されます。
		//ES_SAFE_RELEASE(hitEffect);
	}

public:
	void Init(SceneSystem* sceneSystem) override;
	void Update() override;
	void Draw() override;
	void GUI() override;

private:
	void PrimitiveRender(Phoenix::Graphics::DeviceDX11* device, Phoenix::Math::Vector3 translate, Phoenix::Math::Vector3 rotate, Phoenix::Math::Vector3 scale);
	Phoenix::Math::Vector3 WorldToScreen(const Phoenix::Math::Vector3& worldPosition);
};