#include "PlayerEditor.h"
#include "Phoenix/FND/Util.h"


// ����
std::shared_ptr<PlayerEditor> PlayerEditor::Create()
{
	return std::make_shared<PlayerEditor>();
}

// ������
bool PlayerEditor::Initialize(std::shared_ptr<Phoenix::Graphics::IGraphicsDevice> graphicsDevice)
{
	this->graphicsDevice = graphicsDevice;

	player = Player::Create();
	player->Construct(graphicsDevice.get());
	player->Initialize();

	camera = std::make_unique<Phoenix::Graphics::Camera>();
	camera->SetEye(Phoenix::Math::Vector3(0.0f, 0.0f, 10.0f));
	camera->SetRotateX(0.5f);
	camera->SetRotateY(-2.7f);

	basicSkinShader = Phoenix::FrameWork::BasicSkinShader::Create();
	basicSkinShader->Initialize(graphicsDevice.get());

	pbrShader = Phoenix::FrameWork::PBRShader::Create();
	pbrShader->Initialize(graphicsDevice.get());

	selected.resize(player->GetAttackDatasList().attackDatas.size());
	ResetSelected();

	currentAttackNum = -1;

	return true;
}

// �I����
void PlayerEditor::Finalize()
{
	player->Finalize();
}

// �X�V
void PlayerEditor::Update(Phoenix::f32 elapsedTime)
{
	static Phoenix::f32 lerp = 1.0f;
	static Phoenix::f32 cameraLen = 6.0f;
	{
		lerp = Phoenix::Math::f32Lerp(lerp, 1.0f, 0.01f);
		cameraLen = Phoenix::Math::f32Lerp(cameraLen, 6.0f, 0.05f);

		camera->ControllerCamera02(player->GetPosition(), Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), cameraLen, lerp);
	}

	Phoenix::FrameWork::LightState* light = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader.get())->GetLight();
	light->direction = Phoenix::Math::Vector4(-camera->GetFront(), 1.0f);

	player->Update(*camera.get(), true, false);
}

// �`��
void PlayerEditor::Draw(Phoenix::f32 elapsedTime)
{
	pbrShader->Begin(graphicsDevice.get(), *camera);
	pbrShader->Draw(graphicsDevice.get(), player->GetWorldMatrix(), player->GetModel());
	pbrShader->End(graphicsDevice.get());
}

// GUI
void PlayerEditor::GUI()
{
	Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

	Phoenix::Graphics::Viewport* v = new Phoenix::Graphics::Viewport();
	context->GetViewports(1, &v);
	Phoenix::f32 width = v->width;
	Phoenix::f32 height = v->height;
	Phoenix::FND::SafeDelete(v);

	if (ImGui::BeginMainMenuBar())
	{
		// ���C�����j���[��\�����Ă��鎞�̏����������ɏ����܂��B
		if (ImGui::BeginMenu(u8"�t�@�C��"))
		{
#pragma region New File
			if (ImGui::MenuItem(u8"�V�K�쐬"/*, u8"Ctrl+Shift+N"*/))
			{
				NewFile();
			}
#pragma endregion

#pragma region Open File
			if (ImGui::MenuItem(u8"�t�@�C�����J��"/*, u8"Ctrl+O"*/))
			{
				OpenFile();
			}
#pragma endregion

#pragma region Save File
			if (ImGui::MenuItem(u8"�t�@�C����ۑ�"/*, u8"Ctrl+Shift+S"*/))
			{
				SaveFile();
			}
#pragma endregion

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"�ҏW"))
		{
			ImGui::MenuItem("(dummy menu)", NULL, false, false); // enabled��false�ɂ��邱�ƂŃO���[�\���ł��܂��B

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::SetNextWindowPos(ImVec2(width - 320, 26), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(320, height - 26), ImGuiCond_Once);
	ImGui::Begin("PlayerEditor");
	{
		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem(u8"AttackList"))
			{
				Phoenix::s32 count = 0;
				auto& list = player->GetAttackDatasList();
				for (auto& attack : list.attackDatas)
				{
					if (ImGui::Selectable(std::string("Attack" + std::to_string(count+1)).c_str(), selected.at(count)))
					{
						ResetSelected();
						selected.at(count) = true;
						currentAttackNum = count;
					}

					++count;
				}

				if (ImGui::Button("AddAttack"))
				{
					Player::AttackDatas datas;
					list.attackDatas.emplace_back(datas);
					selected.emplace_back(false);
				}

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();

	if (currentAttackNum != -1)
	{
		auto& list = player->GetAttackDatasList();
		auto& attack = list.attackDatas.at(currentAttackNum);

		ImGui::SetNextWindowPos(ImVec2(0, 26), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(400, height - 26), ImGuiCond_Once);
		ImGui::Begin(std::string("Attack" + std::to_string(currentAttackNum+1)).c_str());
		{
			if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem(u8"Data"))
				{
					if (ImGui::TreeNode("Input Key"))
					{
						ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(350, 75), ImGuiWindowFlags_NoTitleBar);
						{
							bool weakSelect = false;
							bool strongSelect = false;
							if (attack.receptionKey == Player::AttackKey::WeakAttack) weakSelect = true;
							if (attack.receptionKey == Player::AttackKey::StrongAttack) strongSelect = true;
							if (ImGui::Selectable("Weak Key (Xbox : X Button, Keyboard : J key)", weakSelect))
							{
								attack.receptionKey = Player::AttackKey::WeakAttack;
							}
							if (ImGui::Selectable("Strong Key (Xbox : Y Button, Keyboard : K key)", strongSelect))
							{
								attack.receptionKey = Player::AttackKey::StrongAttack;
							}
						}
						ImGui::EndChild();
						ImGui::TreePop();
					}

					Phoenix::s32 count = 0;
					for (auto& data : attack.datas)
					{
						if (ImGui::TreeNode(std::to_string(count+1).c_str()))
						{
							ImGui::InputInt("Animation State", &data.animState);
							ImGui::InputInt("Animation Index", &data.animIndex);
							ImGui::Separator();

							ImGui::InputFloat("Play Speed", &data.playSpeed);
							ImGui::InputFloat("Play Begin Time", &data.playBeginTime);
							ImGui::InputFloat("Play End Time", &data.playEndTime);
							ImGui::Separator();

							ImGui::InputInt("Collision Num", &data.collisionNum);
							ImGui::InputFloat("Collision Begin Time", &data.collisionBeginTime);
							ImGui::InputFloat("Collision End Time", &data.collisionEndTime);
							ImGui::Separator();

							ImGui::Checkbox("Reception Stack", &data.receptionStack);
							ImGui::InputFloat("Reception Begin Time", &data.receptionBeginTime);
							ImGui::InputFloat("Reception End Time", &data.receptionEndTime);
							ImGui::Separator();

							ImGui::InputFloat("Dedge Reception Begin Time", &data.dedgeReceptionBeginTime);
							ImGui::InputFloat("Dedge Reception End Time", &data.dedgeReceptionEndTime);
							ImGui::Separator();

							ImGui::InputInt("Weak Derived Attack State", &data.weakDerivedAttackState);
							ImGui::InputInt("Strong Derived Attack State", &data.strongDerivedAttackState);
							ImGui::Separator();

							ImGui::TreePop();
						}

						++count;
					}

					if (ImGui::Button("AddAttackData"))
					{
						Phoenix::s32 state = -1;
						for (auto& attack : list.attackDatas)
						{
							for (auto& data : attack.datas)
							{
								if (state <= data.animState)
								{
									state = data.animState;
								}
							}
						}
						
						Player::AttackData data;
						data.animState = state + 1;

						attack.datas.emplace_back(data);
					}

					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();
	}
}

// �I�����Z�b�g
void PlayerEditor::ResetSelected()
{
	for (Phoenix::s32 i = 0; i < selected.size(); ++i)
	{
		selected.at(i) = false;
	}
}

// �V�K�쐬
void PlayerEditor::NewFile()
{
	Player::AttackDataList data;
	player->SetAttackDatasList(data);
}

// �t�@�C�����J��
void PlayerEditor::OpenFile()
{
	const char* fullPass = Phoenix::OS::Path::GetFullPath("..\\Data\\Document\\Player\\AttackDatas.atk");

	Player::AttackDataList data;
	Player::AttackDataList::Deserialize(data, fullPass);

	player->SetAttackDatasList(data);
}

// �t�@�C����ۑ�
void PlayerEditor::SaveFile()
{
	const char* fullPass = Phoenix::OS::Path::GetFullPath("..\\Data\\Document\\Player\\AttackDatas.atk");
	Player::AttackDataList::Serialize(player->GetAttackDatasList(), fullPass);
}