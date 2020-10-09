#include "SceneLabo.h"
#include "SceneSystem.h"
#include "Phoenix/FND/Operation.h"


std::unique_ptr<SceneLabo> SceneLabo::Create()
{
	return std::make_unique<SceneLabo>();
}

void SceneLabo::Construct(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
	display = sceneSystem->GetDisplay();
	graphicsDevice = sceneSystem->GetGraphicsDevice();
	commonData = sceneSystem->GetSceneCommonData();

	luaSystem = Phoenix::FrameWork::LuaSystem::Create();
	luaSystem->Initialize(graphicsDevice);
}

void SceneLabo::Initialize()
{
	finished = false;

	judgeLoad = false;
	aliveCnt = 0;
}

void SceneLabo::Update(Phoenix::f32 elapsedTime)
{

}

void SceneLabo::Draw(Phoenix::f32 elapsedTime)
{

}

void SceneLabo::GUI()
{
	ImGui::Begin("Labo");
	{
		if (ImGui::Button("Finish Labo"))
		{
			finished = true;
		}

		if (ImGui::TreeNode("Lua"))
		{
			if (ImGui::TreeNode("Load Script"))
			{
				ImGui::InputText("File Name : (Script.lua)", filePass, 1024);
				if (ImGui::Button("Inport"))
				{
					const wchar_t* fullPass = Phoenix::OS::Path::GetFullPathW(L"..\\Data\\Script");

					TCHAR szFile[MAX_PATH];
					std::wstring fileNameW = std::wstring(fullPass);

					if (GetFileName(0, szFile, sizeof(szFile) / sizeof(TCHAR), fileNameW.c_str(), L".lua", true))
					{
						size_t len = 0;
						errno_t err = 0;

						//変換
						err = wcstombs_s(&len, filePass, sizeof(filePass), szFile, _TRUNCATE);
					}
				}
				if (ImGui::Button("Load"))
				{
					judgeLoad = luaSystem->LoadScript(filePass);
					aliveCnt = 1;
				}
				if (judgeLoad && 0 < aliveCnt)
				{
					aliveCnt++;

					if (5 * 60 <= aliveCnt)
					{
						judgeLoad = false;
						aliveCnt = 0;
					}

					judgeLoad ? ImGui::Text(u8"ロードに成功") : ImGui::Text(u8"ロードに失敗");
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Run Function"))
			{
				if (ImGui::Button("Run"))
				{
					luaSystem->BeginFunction("Run");
					{
						luaSystem->CallFunction(0);
					}
					luaSystem->EndFunction();
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Get Global Val"))
			{
				ImGui::Text("%d", luaSystem->GetGlobalValue<Phoenix::s32>("num"));

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}
	ImGui::End();
}