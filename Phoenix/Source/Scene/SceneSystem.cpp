#include "SceneSystem.h"
#include "FadeSystem.h"
#include "../Player/Player.h"
#include "../Boss/Boss.h"
#include "Phoenix/Types.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Shader/Shader.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FrameWork/Shader/StandardShader.h"


std::unique_ptr<SceneSystem> SceneSystem::Create()
{
	return std::make_unique<SceneSystem>();
}

void SceneSystem::Initialize(Phoenix::OS::IDisplay* display, Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	this->display = display;
	this->graphicsDevice = graphicsDevice;
	commonData = SceneCommonData::Create();
	commonData->Initialize(graphicsDevice);

	fadeSystem = FadeSystem::Create();
	fadeSystem->Initialize(graphicsDevice);

	labo = SceneLabo::Create();
	labo->Construct(this);
	onLabo = false;

	AddScene<SceneTitle>();
	AddScene<SceneGame>();
	AddScene<SceneGameClear>();
	AddScene<SceneGameOver>();
	ChangeScene(SceneType::Title, false, false);

	for (Phoenix::u32 i = 0; i < scenes.size(); ++i)
	{
		scenes.at(i)->Construct(this);
	}
}

void SceneSystem::Update(Phoenix::f32 elapsedTime)
{
	// ƒ‰ƒ{
	{
		if (labo->Finish())
		{
			onLabo = false;
			labo->Initialize();
		}

		if (onLabo)
		{
			labo->Update(elapsedTime);
			return;
		}

		if (GetKeyState(VK_CONTROL) < 0)
		{
			if (GetKeyState('L') < 0)
			{
				if (keyCnt == 0)
				{
					onLabo = true;
					labo->Initialize();
				}

				keyCnt++;
			}
			else
			{
				keyCnt = 0;
			}
		}
	}

	// XVˆ—
	if (stackScene)
	{
		stackScene->Update(elapsedTime);
	}
	if (nextScene)
	{
		currentScene = nextScene;
		nextScene = nullptr;
		currentScene->Initialize();
	}
	fadeSystem->Update(this);
	currentScene->Update(elapsedTime);
}

void SceneSystem::GUI()
{
	if (onLabo)
	{
		labo->GUI();
		return;
	}

	currentScene->GUI();
}

void SceneSystem::ChangeScene(SceneType sceneType, bool stack, bool fade)
{
	if (stack)
	{
		stackScene = currentScene;
	}

	if (fade)
	{
		fadeSystem->OnFade(sceneType);
	}
	else
	{
		SetScene(sceneType);
	}
}

void SceneSystem::SetScene(SceneType sceneType)
{
	nextScene = GetScene(sceneType);
}

void SceneSystem::ReSetStackScene()
{
	currentScene = stackScene;
	stackScene = nullptr;
}

void SceneSystem::Draw(Phoenix::f32 elapsedTime)
{
	if (onLabo)
	{
		labo->Draw(elapsedTime);
		return;
	}

	if (stackScene)
	{
		stackScene->Draw(elapsedTime);
	}
	currentScene->Draw(elapsedTime);
	fadeSystem->Draw(graphicsDevice);
}

bool SceneSystem::GetOnFade()
{
	return fadeSystem->GetWorking();
}