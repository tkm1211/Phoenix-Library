#include "Main.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FND/STD.h"
#include "Phoenix/OS/Path.h"
#include "Phoenix/OS/ResourceManager.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/FrameWork/Renderer/ModelRenderer.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FrameWork/Shader/StandardShader.h"


namespace Phoenix
{
	namespace OS
	{
		std::unique_ptr<IMain> IMain::Create()
		{
			return std::make_unique<Main>();
		}
	}
}

bool Main::Initialize(Phoenix::uintPtr instance)
{
	Super::Initialize(instance);

	editorSystem = EditorSystem::Create();
	editorSystem->Initialize(graphicsDevice);

	return true;
}

void Main::Finalize()
{
	editorSystem->Finalize();
	Super::Finalize();
}

void Main::Update(Phoenix::f32 elapsedTime)
{
	// エディター更新
	{
		editorSystem->Update(elapsedTime);
	}

	// パラメーター調整用GUI
	{
		GUI();
	}
}

// GUI
void Main::GUI()
{
	editorSystem->GUI();
}

void Main::Render(Phoenix::f32 elapsedTime)
{
	editorSystem->Draw(elapsedTime);
}