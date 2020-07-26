#pragma once

#include <memory>
#include "../Player/Player.h"
#include "../Boss/Boss.h"
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
	Player* player = nullptr;
	Boss* boss = nullptr;
	Phoenix::FrameWork::ModelObject* stageModel = nullptr;
	Phoenix::FrameWork::IShader* basicShader = nullptr;
	Phoenix::FrameWork::IShader* basicSkinShader = nullptr;
	Phoenix::FrameWork::IShader* standardShader = nullptr;
	Phoenix::FrameWork::IShader* pbrShader = nullptr;
	Phoenix::Graphics::Camera* camera = nullptr;
	bool cameraFlg = false;

	std::shared_ptr<GeometricPrimitive> primitive;

	bool isHitCollision = false;

	//::Effekseer::Effect* hitEffect = nullptr;
	//::Effekseer::Handle hitEffectHandle = 0;

	bool isUpdate = false;

	std::unique_ptr<Phoenix::Graphics::ITexture> skyMap;
	std::shared_ptr<GeometricPrimitive> skyBox;
	std::shared_ptr<Phoenix::FrameWork::IShader> skyMapShader;

	bool enableMSAA = true;

	std::unique_ptr<Phoenix::FrameWork::FrameBuffer> frameBuffer[3];

	std::unique_ptr<Phoenix::FrameWork::Quad> quad;
	std::unique_ptr<Phoenix::FrameWork::MSAAResolve> msaaResolve;
	std::unique_ptr<Phoenix::FrameWork::Bloom> bloom;

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
};