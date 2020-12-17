#pragma once

#include <memory>
#include "../Player/Player.h"
#include "../Boss/Boss.h"
#include "../Enemy/EnemyManager.h"
#include "../Mannequin/Mannequin.h"
#include "../AI/MetaAI/MetaAI.h"
#include "../UI/UISystem.h"
#include "../UI/TargetMarkUI.h"
#include "../Primitive/GeometricPrimitive.h"
#include "../Sound/SoundSystem.h"
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
#include "Phoenix/FrameWork/FrameBuffer/FrameBuffer.h"
#include "Phoenix/FrameWork/Quad/Quad.h"
#include "Phoenix/FrameWork/IBL/IBL.h"
#include "Phoenix/FrameWork/ComputeShader/TestComputeShader.h"
#include "Phoenix/FrameWork/ComputeShader/BitonicSort.h"
#include "Phoenix/FrameWork/ComputeShader/GPUParticle.h"
#include "Phoenix/FrameWork/Script/Lua.h"


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
	virtual void Construct(SceneSystem* sceneSystem) = 0;
	virtual void Initialize() = 0;
	virtual void Update(Phoenix::f32 elapsedTime) = 0;
	virtual void Draw(Phoenix::f32 elapsedTime) = 0;
	virtual void GUI() = 0;
};

class SceneTitle : public Scene
{
private:
	const Phoenix::f32 dissolveSpeed = 0.5f;

private:
	Phoenix::FrameWork::ModelObject* stageModel = nullptr;
	Phoenix::FrameWork::IShader* pbrShader = nullptr;
	Phoenix::FrameWork::IShader* pbrSkinShader = nullptr;
	Phoenix::Graphics::Camera* camera = nullptr;
	SoundSystem<SoundType>* soundSystem = nullptr;

	// フレームバッファ
	std::unique_ptr<Phoenix::FrameWork::FrameBuffer> frameBuffer[3];

	// ポストプロセス
	std::unique_ptr<Phoenix::FrameWork::PostProcessingEffects> postProcessingEffects;

	// ブルーム
	std::unique_ptr<Phoenix::FrameWork::Quad> quad;
	std::unique_ptr<Phoenix::FrameWork::MSAAResolve> msaaResolve;
	std::unique_ptr<Phoenix::FrameWork::Bloom> bloom;

	// スカイマップ
	std::unique_ptr<Phoenix::FrameWork::SkyMap> skyMap;

	// タイトル
	std::unique_ptr<Phoenix::Graphics::ITexture> logo;
	std::unique_ptr<Phoenix::Graphics::ITexture> icon;
	std::unique_ptr<Phoenix::Graphics::ITexture> button;
	Phoenix::f32 dissolveThreshold = 0.0f;        //透過閾値
	Phoenix::f32 dissolveEmissiveWidth = 0.0f;    //発光閾値(ディゾルブ・エミッシブ)

	// フラグ
	bool isChangeScene = false;

private: // Debug
	std::shared_ptr<Phoenix::FrameWork::ModelObject> model;

public:
	SceneTitle() {}
	~SceneTitle() {}

public:
	void Construct(SceneSystem* sceneSystem) override;
	void Initialize() override;
	void Update(Phoenix::f32 elapsedTime) override;
	void Draw(Phoenix::f32 elapsedTime) override;
	void GUI() override;
};

class SceneTutorial : public Scene
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
	EnemyManager* enemyManager = nullptr;
	Mannequin* mannequin = nullptr;
	MetaAI* metaAI = nullptr;
	UISystem* uiSystem = nullptr;
	TargetMarkUI* targetMarkUI = nullptr;
	Phoenix::FrameWork::ModelObject* stageModel = nullptr;
	Phoenix::FrameWork::ModelObject* bossStageModel = nullptr;
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

	// トーンマップ
	std::unique_ptr<Phoenix::FrameWork::ToneMap> toneMap;

	// IBL
	std::unique_ptr<Phoenix::FrameWork::FrameBuffer> skyFrameBuffer[6];
	std::unique_ptr<Phoenix::FrameWork::IBL> ibl;

	// ヒットストップ
	bool isHitStop = false;
	Phoenix::s32 hitStopCnt = 0;
	Phoenix::s32 hitStopMaxCnt = 2;

	// カメラシェイク
	bool isCameraShake = false;
	Phoenix::Math::Vector3 shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::f32 shakeWidth = 0.0f;
	Phoenix::f32 shakeHeight = 0.0f;
	Phoenix::s32 cameraShakeCnt = 0;
	Phoenix::s32 cameraShakeMaxCnt = 0;

	// コントローラー振動
	Phoenix::f32 vibrationRight = 0.0f;
	Phoenix::f32 vibrationLeft = 0.0f;
	Phoenix::s32 vibrationMaxCnt = 0;

private: // Debug
	std::shared_ptr<GeometricPrimitive> primitive;
	std::shared_ptr<GeometricPrimitive> boxPrimitive;
	Phoenix::Math::Vector2 texSize;

	Phoenix::Math::Vector3 tempCameraFouce = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::f32 sphereLinearSpeed = 0.0f;
	Phoenix::f32 distanceToFouceFromCamera = 0.0f;
	bool hit = false;

	Phoenix::Math::Vector3 particlePos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::Math::Vector3 bossHitParticlePos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::Math::Vector3 jumpAttackParticlePos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::Math::Vector4 particleNormal = Phoenix::Math::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	Phoenix::Math::Color particleMainColor = Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f);
	Phoenix::f32 particleLife = 1.0f;
	Phoenix::f32 particleSize = 1.0f;
	Phoenix::f32 particleScale = 1.0f;

	std::unique_ptr<Phoenix::FrameWork::IComputeShader> testComputeShader;
	std::unique_ptr<Phoenix::FrameWork::BitonicSort> bitonicSort;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> gpuParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> playerHitParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> bossHitParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> petalParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> soilParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> dusterParticle;

	Phoenix::f32 dis = 10.0f;
	Phoenix::f32 width = 30.0f;
	Phoenix::f32 height = 30.0f;
	Phoenix::f32 nearZ = 1.0f;
	Phoenix::f32 farZ = 30.0f;

	Phoenix::f32 playerAttackEndCount;
	Phoenix::f32 playerAttackEndMaxCount = 30;
	Phoenix::f32 grayScale;

	Phoenix::f32 pointLightDistance = 0.95f;

	Phoenix::Math::Vector3 stageScale = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);

	Phoenix::f32 adjustY = 1.25f;

	// 行動スコア
	Phoenix::s32 playerBehaviorScore = 0;
	Phoenix::s32 oldPlayerBehaviorScore = 0;
	Phoenix::s32 WeakAttackScore = 10;
	Phoenix::s32 StrongAttackScore = 30;

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

	bool onPointLight = false;

	bool isDrawUI = false;

	bool inTerritory = false;

private:
	struct DissolveCB
	{
		Phoenix::f32 dissolveThreshold;        //透過閾値
		Phoenix::f32 dissolveEmissiveWidth;    //発光閾値(ディゾルブ・エミッシブ)
		Phoenix::f32 dummy[2];
	};
	std::unique_ptr<Phoenix::Graphics::IShader> embeddedDissolvePixelShader;
	std::unique_ptr<Phoenix::Graphics::IBuffer> dissolveCB;
	std::unique_ptr<Phoenix::Graphics::ITexture> dissolveTexture;
	Phoenix::f32 dissolveThreshold;        //透過閾値
	Phoenix::f32 dissolveEmissiveWidth;    //発光閾値(ディゾルブ・エミッシブ)

public:
	SceneTutorial() {}
	~SceneTutorial() {}

public:
	void Construct(SceneSystem* sceneSystem) override;
	void Initialize() override;
	void Update(Phoenix::f32 elapsedTime) override;
	void Draw(Phoenix::f32 elapsedTime) override;
	void GUI() override;

private:
	void JudgeHit();
	void JudgeExtrude(Phoenix::Math::Vector3 playerPos, Phoenix::Math::Vector3 enemyPos, Phoenix::f32 playerRadius, Phoenix::f32 enemyRadius);
	void UpdateCamera();
	void UpdateGameCamera(Phoenix::Math::Vector3 playerPos, Phoenix::Math::Vector3 enemyPos);

	void PrimitiveRender(Phoenix::Graphics::DeviceDX11* device, Phoenix::Math::Vector3 translate, Phoenix::Math::Vector3 rotate, Phoenix::Math::Vector3 scale, bool isBox = false);
	Phoenix::Math::Vector3 WorldToScreen(const Phoenix::Math::Vector3& worldPosition);
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
	//Boss* boss = nullptr;
	EnemyManager* enemyManager = nullptr;
	MetaAI* metaAI = nullptr;
	UISystem* uiSystem = nullptr;
	TargetMarkUI* targetMarkUI = nullptr;
	SoundSystem<SoundType>* soundSystem = nullptr;
	Phoenix::FrameWork::ModelObject* stageModel = nullptr;
	Phoenix::FrameWork::ModelObject* bossStageModel = nullptr;
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

	// トーンマップ
	std::unique_ptr<Phoenix::FrameWork::ToneMap> toneMap;

	// IBL
	std::unique_ptr<Phoenix::FrameWork::FrameBuffer> skyFrameBuffer[6];
	std::unique_ptr<Phoenix::FrameWork::IBL> ibl;

	// ヒットストップ
	bool isHitStop = false;
	Phoenix::s32 hitStopCnt = 0;
	Phoenix::s32 hitStopMaxCnt = 1;

	// カメラシェイク
	bool isCameraShake = false;
	Phoenix::Math::Vector3 shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::f32 shakeWidth = 0.0f;
	Phoenix::f32 shakeHeight = 0.0f;
	Phoenix::s32 cameraShakeCnt = 0;
	Phoenix::s32 cameraShakeMaxCnt = 0;

	// コントローラー振動
	Phoenix::f32 vibrationRight = 0.0f;
	Phoenix::f32 vibrationLeft = 0.0f;
	Phoenix::s32 vibrationMaxCnt = 0;

	// 行動スコア
	Phoenix::s32 playerBehaviorScore = 0;
	Phoenix::s32 oldPlayerBehaviorScore = 0;
	Phoenix::s32 WeakAttackScore = 10;
	Phoenix::s32 StrongAttackScore = 30;

	bool inTerritory = false;

private: // Debug
	std::shared_ptr<GeometricPrimitive> primitive;
	std::shared_ptr<GeometricPrimitive> cylinderPrimitive;
	Phoenix::Math::Vector2 texSize;

	Phoenix::Math::Vector3 tempCameraFouce = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::f32 sphereLinearSpeed = 0.0f;
	Phoenix::f32 distanceToFouceFromCamera = 0.0f;
	bool hit = false;

	Phoenix::Math::Vector3 particlePos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::Math::Vector3 bossHitParticlePos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::Math::Vector3 jumpAttackParticlePos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::Math::Vector4 particleNormal = Phoenix::Math::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	Phoenix::Math::Color particleMainColor = Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f);
	Phoenix::f32 particleLife = 1.0f;
	Phoenix::f32 particleSize = 1.0f;
	Phoenix::f32 particleScale = 1.0f;

	std::unique_ptr<Phoenix::FrameWork::IComputeShader> testComputeShader;
	std::unique_ptr<Phoenix::FrameWork::BitonicSort> bitonicSort;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> gpuParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> playerHitParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> bossHitParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> petalParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> soilParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> dusterParticle[3];

	Phoenix::f32 dis = 10.0f;
	Phoenix::f32 width = 30.0f;
	Phoenix::f32 height = 30.0f;
	Phoenix::f32 nearZ = 1.0f;
	Phoenix::f32 farZ = 30.0f;

	Phoenix::f32 playerAttackEndCount;
	Phoenix::f32 playerAttackEndMaxCount = 30;
	Phoenix::f32 grayScale;

	Phoenix::f32 pointLightDistance = 0.95f;

	Phoenix::f32 stageRadius = 14.0f;

	Phoenix::f32 adjustY = 1.25f;

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
	bool onPointLight = false;
	bool isDrawUI = false;
	bool onPlayerEditor = false;

private:
	struct DissolveCB
	{
		Phoenix::f32 dissolveThreshold;        //透過閾値
		Phoenix::f32 dissolveEmissiveWidth;    //発光閾値(ディゾルブ・エミッシブ)
		Phoenix::f32 dummy[2];
	};
	std::unique_ptr<Phoenix::Graphics::IShader> embeddedDissolvePixelShader;
	std::unique_ptr<Phoenix::Graphics::IShader> pbrDissolvePixelShader;
	std::unique_ptr<Phoenix::Graphics::IBuffer> dissolveCB;
	std::unique_ptr<Phoenix::Graphics::ITexture> bossRedTexture;
	std::unique_ptr<Phoenix::Graphics::ITexture> dissolveTexture;
	std::unique_ptr<Phoenix::Graphics::ITexture> emissiveTexture;
	Phoenix::f32 dissolveThreshold;        //透過閾値
	Phoenix::f32 dissolveEmissiveWidth;    //発光閾値(ディゾルブ・エミッシブ)
	bool isTurn = false;

public:
	SceneGame() {}
	~SceneGame() {}

public:
	void Construct(SceneSystem* sceneSystem) override;
	void Initialize() override;
	void Update(Phoenix::f32 elapsedTime) override;
	void Draw(Phoenix::f32 elapsedTime) override;
	void GUI() override;

private:
	void PrimitiveRender(Phoenix::Graphics::DeviceDX11* device, Phoenix::Math::Vector3 translate, Phoenix::Math::Vector3 rotate, Phoenix::Math::Vector3 scale);
	void CylinderPrimitiveRender(Phoenix::Graphics::DeviceDX11* device, Phoenix::Math::Vector3 cp1Translate, Phoenix::Math::Vector3 cp2Translate, Phoenix::Math::Vector3 cyilinderTranslate, Phoenix::Math::Vector3 rotate, Phoenix::Math::Vector3 scale, Phoenix::Math::Vector3 cyilinderScale);
	Phoenix::Math::Vector3 WorldToScreen(const Phoenix::Math::Vector3& worldPosition);
};

class SceneGameClear : public Scene
{
private:
	const Phoenix::f32 addBright = 0.0025f;

private:
	std::unique_ptr<Phoenix::FrameWork::Quad> quad;
	std::unique_ptr<Phoenix::Graphics::ITexture> logo;
	std::unique_ptr<Phoenix::Graphics::ITexture> button;

	// フレームバッファ
	std::unique_ptr<Phoenix::FrameWork::FrameBuffer> frameBuffer;

	// トーンマップ
	std::unique_ptr<Phoenix::FrameWork::ToneMap> toneMap;

	bool isChangeScene = false;

	Phoenix::f32 bright = 0.1f; //明度
	Phoenix::f32 contrast = 2.0f; //濃淡
	Phoenix::f32 saturate = 0.5f; //彩度
	Phoenix::Math::Color screenColor = { 1.0f, 1.0f, 1.0f, 1.0f }; //色調

	bool isTurn = false;

public:
	SceneGameClear() {}
	~SceneGameClear() {}

public:
	void Construct(SceneSystem* sceneSystem) override;
	void Initialize() override;
	void Update(Phoenix::f32 elapsedTime) override;
	void Draw(Phoenix::f32 elapsedTime) override;
	void GUI() override;
};

class SceneGameOver : public Scene
{
private:
	const Phoenix::f32 addBright = 0.0025f;

private:
	std::unique_ptr<Phoenix::FrameWork::Quad> quad;
	std::unique_ptr<Phoenix::Graphics::ITexture> logo;
	std::unique_ptr<Phoenix::Graphics::ITexture> button[2];

	// フレームバッファ
	std::unique_ptr<Phoenix::FrameWork::FrameBuffer> frameBuffer;

	// トーンマップ
	std::unique_ptr<Phoenix::FrameWork::ToneMap> toneMap;

	bool isChangeScene = false;
	bool isRight = false;

	Phoenix::f32 bright = 0.1f; //明度
	Phoenix::f32 contrast = 2.0f; //濃淡
	Phoenix::f32 saturate = 0.5f; //彩度
	Phoenix::Math::Color screenColor = { 1.0f, 1.0f, 1.0f, 1.0f }; //色調

	bool isTurn = false;

public:
	SceneGameOver() {}
	~SceneGameOver() {}

public:
	void Construct(SceneSystem* sceneSystem) override;
	void Initialize() override;
	void Update(Phoenix::f32 elapsedTime) override;
	void Draw(Phoenix::f32 elapsedTime) override;
	void GUI() override;
};

class SceneEvent : public Scene
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
	TargetMarkUI* targetMarkUI = nullptr;
	Phoenix::FrameWork::ModelObject* stageModel = nullptr;
	Phoenix::FrameWork::ModelObject* bossStageModel = nullptr;
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

	// トーンマップ
	std::unique_ptr<Phoenix::FrameWork::ToneMap> toneMap;

	// IBL
	std::unique_ptr<Phoenix::FrameWork::FrameBuffer> skyFrameBuffer[6];
	std::unique_ptr<Phoenix::FrameWork::IBL> ibl;

	// ヒットストップ
	bool isHitStop = false;
	Phoenix::s32 hitStopCnt = 0;
	Phoenix::s32 hitStopMaxCnt = 2;

	// カメラシェイク
	bool isCameraShake = false;
	Phoenix::Math::Vector3 shake = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::f32 shakeWidth = 0.0f;
	Phoenix::f32 shakeHeight = 0.0f;
	Phoenix::s32 cameraShakeCnt = 0;
	Phoenix::s32 cameraShakeMaxCnt = 0;

	// コントローラー振動
	Phoenix::f32 vibrationRight = 0.0f;
	Phoenix::f32 vibrationLeft = 0.0f;
	Phoenix::s32 vibrationMaxCnt = 0;

	// Lua
	bool runned = false;
	std::unique_ptr<Phoenix::FrameWork::LuaSystem> luaSystem;

private: // Debug
	std::shared_ptr<GeometricPrimitive> primitive;
	Phoenix::Math::Vector2 texSize;

	Phoenix::Math::Vector3 tempCameraFouce = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::f32 sphereLinearSpeed = 0.0f;
	Phoenix::f32 distanceToFouceFromCamera = 0.0f;
	bool hit = false;

	Phoenix::Math::Vector3 particlePos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::Math::Vector3 bossHitParticlePos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::Math::Vector3 jumpAttackParticlePos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
	Phoenix::Math::Vector4 particleNormal = Phoenix::Math::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	Phoenix::Math::Color particleMainColor = Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f);
	Phoenix::f32 particleLife = 1.0f;
	Phoenix::f32 particleSize = 1.0f;
	Phoenix::f32 particleScale = 1.0f;

	std::unique_ptr<Phoenix::FrameWork::IComputeShader> testComputeShader;
	std::unique_ptr<Phoenix::FrameWork::BitonicSort> bitonicSort;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> gpuParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> playerHitParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> bossHitParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> petalParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> soilParticle;
	std::unique_ptr<Phoenix::FrameWork::GPUParticle> dusterParticle[3];

	Phoenix::f32 dis = 10.0f;
	Phoenix::f32 width = 30.0f;
	Phoenix::f32 height = 30.0f;
	Phoenix::f32 nearZ = 1.0f;
	Phoenix::f32 farZ = 30.0f;

	Phoenix::f32 playerAttackEndCount;
	Phoenix::f32 playerAttackEndMaxCount = 30;

	Phoenix::f32 pointLightDistance = 0.95f;

	Phoenix::f32 stageRadius = 14.0f;

	Phoenix::f32 adjustY = 1.25f;

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

	bool onPointLight = false;

	bool isDrawUI = false;

private:
	struct DissolveCB
	{
		Phoenix::f32 dissolveThreshold;        //透過閾値
		Phoenix::f32 dissolveEmissiveWidth;    //発光閾値(ディゾルブ・エミッシブ)
		Phoenix::f32 dummy[2];
	};
	std::unique_ptr<Phoenix::Graphics::IShader> embeddedDissolvePixelShader;
	std::unique_ptr<Phoenix::Graphics::IShader> pbrDissolvePixelShader;
	std::unique_ptr<Phoenix::Graphics::IBuffer> dissolveCB;
	std::unique_ptr<Phoenix::Graphics::ITexture> bossRedTexture;
	std::unique_ptr<Phoenix::Graphics::ITexture> dissolveTexture;
	std::unique_ptr<Phoenix::Graphics::ITexture> emissiveTexture;
	Phoenix::f32 dissolveThreshold;        //透過閾値
	Phoenix::f32 dissolveEmissiveWidth;    //発光閾値(ディゾルブ・エミッシブ)
	bool isTurn = false;

public:
	SceneEvent() {}
	~SceneEvent() {}

public:
	void Construct(SceneSystem* sceneSystem) override;
	void Initialize() override;
	void Update(Phoenix::f32 elapsedTime) override;
	void Draw(Phoenix::f32 elapsedTime) override;
	void GUI() override;

private:
	void PrimitiveRender(Phoenix::Graphics::DeviceDX11* device, Phoenix::Math::Vector3 translate, Phoenix::Math::Vector3 rotate, Phoenix::Math::Vector3 scale);
	Phoenix::Math::Vector3 WorldToScreen(const Phoenix::Math::Vector3& worldPosition);
};