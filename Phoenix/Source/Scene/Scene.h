#pragma once

#include <memory>
#include "../Player/Player.h"
#include "../Boss/Boss.h"
#include "../Primitive/GeometricPrimitive.h"
#include "Phoenix/Types.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Shader/Shader.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FrameWork/Shader/StandardShader.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../../Effekseer/include/Effekseer/Effekseer.h"
#include "../../Effekseer/include/EffekseerRendererDX11/EffekseerRendererDX11.h"
#include "../../Effekseer/include/EffekseerSoundXAudio2/EffekseerSoundXAudio2.h"


class SceneSystem;
class SceneCommonData;
class Scene
{
protected:
	SceneSystem* sceneSystem = nullptr;
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
	::Effekseer::Effect* effect = nullptr;
	::Effekseer::Handle handle = 0;

public:
	SceneTitle() {}
	~SceneTitle()
	{
		// エフェクトを解放します。再生中の場合は、再生が終了した後、自動的に解放されます。
		ES_SAFE_RELEASE(effect);
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
	Player* player = nullptr;
	Boss* boss = nullptr;
	Phoenix::FrameWork::ModelObject* stageModel = nullptr;
	Phoenix::FrameWork::IShader* basicShader = nullptr;
	Phoenix::FrameWork::IShader* basicSkinShader = nullptr;
	Phoenix::FrameWork::IShader* standardShader = nullptr;
	Phoenix::Graphics::Camera* camera = nullptr;
	bool cameraFlg = false;

	std::shared_ptr<GeometricPrimitive> primitive;

	bool isHitCollision = false;

	::Effekseer::Effect* hitEffect = nullptr;
	::Effekseer::Handle hitEffectHandle = 0;

public:
	SceneGame() {}
	~SceneGame()
	{
		// エフェクトを解放します。再生中の場合は、再生が終了した後、自動的に解放されます。
		ES_SAFE_RELEASE(hitEffect);
	}

public:
	void Init(SceneSystem* sceneSystem) override;
	void Update() override;
	void Draw() override;
	void GUI() override;

private:
	void PrimitiveRender(Phoenix::Graphics::DeviceDX11* device, Phoenix::Math::Vector3 translate, Phoenix::Math::Vector3 rotate, Phoenix::Math::Vector3 scale);
};