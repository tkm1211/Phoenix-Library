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

	helpOpen.resize(10);
	for (Phoenix::s32 i = 0; i < helpOpen.size(); ++i)
	{
		helpOpen.at(i) = false;
	}

	saveExit = false;
	saveCheck = false;
	saveCount = 0;

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
	if (GetAsyncKeyState(VK_CONTROL) < 0)
	{
		if (GetAsyncKeyState('S') < 0)
		{
			if (keyCount == 0)
			{
				SaveCheck();
			}
			++keyCount;
		}
		else
		{
			keyCount = 0;
		}
	}

	static Phoenix::f32 lerp = 1.0f;
	static Phoenix::f32 cameraLen = 6.0f;
	{
		lerp = Phoenix::Math::f32Lerp(lerp, 1.0f, 0.01f * elapsedTime);
		cameraLen = Phoenix::Math::f32Lerp(cameraLen, 6.0f, 0.05f * elapsedTime);

		camera->ControllerCamera02(true, player->GetPosition(), Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), cameraLen, lerp);
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
	static bool open = true;

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
			if (ImGui::MenuItem(u8"�V�K�쐬"/*, u8"Ctrl+Shift+N"*/) && !saveCheck)
			{
				NewFile();
			}
#pragma endregion

#pragma region Open File
			if (ImGui::MenuItem(u8"�t�@�C�����J��"/*, u8"Ctrl+O"*/) && !saveCheck)
			{
				OpenFile();
			}
#pragma endregion

#pragma region Save File
			if (ImGui::MenuItem(u8"�t�@�C����ۑ�", u8"Ctrl+S") && !saveCheck)
			{
				SaveCheck();
			}
#pragma endregion

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"�ҏW"))
		{
			ImGui::MenuItem("(dummy menu)", NULL, false, false); // enabled��false�ɂ��邱�ƂŃO���[�\���ł��܂��B

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"�w���v"))
		{
			if (ImGui::MenuItem(u8"�A�j���[�V�����ԍ� ���X�g") && !saveCheck)
			{
				helpOpen.at(0) = true;
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	/*if (ImGui::CollapsingHeader("Test", &open))
	{
		ImGui::Text("Test");
	}*/

	ImGui::SetNextWindowPos(ImVec2(width - 320, 26), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(320, height - 26), ImGuiCond_Once);

	if (ImGui::Begin(u8"�U���R���{ �G�f�B�^�["))
	{
		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem(u8"�U�� ���X�g"))
			{
				Phoenix::s32 count = 0;
				auto& list = player->GetAttackDatasList();
				for (auto& attack : list.attackDatas)
				{
					if (ImGui::Selectable(std::string("Attack" + std::to_string(count + 1)).c_str(), selected.at(count)) && !saveCheck)
					{
						ResetSelected();
						selected.at(count) = true;
						currentAttackNum = count;
					}

					++count;
				}

				if (ImGui::Button(u8"�U���ǉ�") && !saveCheck)
				{
					Player::AttackDatas datas;
					list.attackDatas.emplace_back(datas);
					selected.emplace_back(false);
				}
				if (ImGui::Button(u8"�I�𒆂̍U���폜") && currentAttackNum != -1)
				{
					list.attackDatas.erase(list.attackDatas.begin() + currentAttackNum);

					for (auto& attack : list.attackDatas)
					{
						for (auto& data : attack.datas)
						{
							if (currentAttackNum <= data.animState) --data.animState;
							if (currentAttackNum <= data.weakDerivedAttackState) --data.weakDerivedAttackState;
							if (currentAttackNum <= data.strongDerivedAttackState) --data.strongDerivedAttackState;
						}
					}
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
		Phoenix::s32 attackCount = static_cast<Phoenix::s32>(list.attackDatas.size());
		if (0 < attackCount)
		{
			auto& attack = list.attackDatas.at(currentAttackNum);

			ImGui::SetNextWindowPos(ImVec2(0, 26), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(400, height - 26), ImGuiCond_Once);
			ImGui::Begin(std::string("Attack" + std::to_string(currentAttackNum + 1)).c_str());
			{
				if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
				{
					if (ImGui::BeginTabItem(u8"�f�[�^ ���X�g"))
					{
						if (ImGui::TreeNode(u8"���̓L�["))
						{
							ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(350, 60), ImGuiWindowFlags_NoTitleBar);
							{
								bool weakSelect = false;
								bool strongSelect = false;
								if (attack.receptionKey == Player::AttackKey::WeakAttack) weakSelect = true;
								if (attack.receptionKey == Player::AttackKey::StrongAttack) strongSelect = true;
								if (ImGui::Selectable(u8"��U���L�[ (Xbox : X �{�^��, Keyboard : J �L�[)", weakSelect) && !saveCheck)
								{
									attack.receptionKey = Player::AttackKey::WeakAttack;
								}
								if (ImGui::Selectable(u8"���U���L�[ (Xbox : Y �{�^��, Keyboard : K �L�[)", strongSelect) && !saveCheck)
								{
									attack.receptionKey = Player::AttackKey::StrongAttack;
								}
							}
							ImGui::EndChild();
							ImGui::TreePop();
						}

						Phoenix::s32 deleteIndex = -1;
						Phoenix::s32 count = 0;
						for (auto& data : attack.datas)
						{
							if (ImGui::TreeNode(std::to_string(count + 1).c_str()))
							{
								if (!saveCheck)
								{
									Phoenix::s32 index = data.animIndex + 1;
									{
										ImGui::InputInt(u8"�A�j���[�V�����ԍ�", &index);
										ImGui::Separator();
									}
									data.animIndex = index - 1;

									Phoenix::f32 beginTime = data.playBeginTime * 60.0f;
									Phoenix::f32 endTime = data.playEndTime * 60.0f;
									{
										ImGui::InputFloat(u8"�Đ����x", &data.playSpeed);
										ImGui::InputFloat(u8"�Đ��J�n�t���[��", &beginTime);
										ImGui::InputFloat(u8"�Đ��I���t���[��", &endTime);
										ImGui::Separator();
									}
									data.playBeginTime = beginTime / 60.0f;
									data.playEndTime = endTime / 60.0f;

									beginTime = data.collisionBeginTime * 60.0f;
									endTime = data.collisionEndTime * 60.0f;
									{
										ImGui::InputInt(u8"�����蔻��ԍ�", &data.collisionNum);
										ImGui::InputFloat(u8"�����蔻�� �J�n�t���[��", &beginTime);
										ImGui::InputFloat(u8"�����蔻�� �I���t���[��", &endTime);
										ImGui::Separator();
									}
									data.collisionBeginTime = beginTime / 60.0f;
									data.collisionEndTime = endTime / 60.0f;

									beginTime = data.receptionBeginTime * 60.0f;
									endTime = data.receptionEndTime * 60.0f;
									{
										ImGui::Checkbox(u8"���̓X�^�b�N", &data.receptionStack);
										ImGui::InputFloat(u8"���͎�t �J�n�t���[��", &beginTime);
										ImGui::InputFloat(u8"���͎�t �I���t���[��", &endTime);
										ImGui::Separator();
									}
									data.receptionBeginTime = beginTime / 60.0f;
									data.receptionEndTime = endTime / 60.0f;

									beginTime = data.dedgeReceptionBeginTime * 60.0f;
									endTime = data.dedgeReceptionEndTime * 60.0f;
									{
										ImGui::InputFloat(u8"��� ���͎�t �J�n�t���[��", &beginTime);
										ImGui::InputFloat(u8"��� ���͎�t �I���t���[��", &endTime);
										ImGui::Separator();
									}
									data.dedgeReceptionBeginTime = beginTime / 60.0f;
									data.dedgeReceptionEndTime = endTime / 60.0f;

									Phoenix::s32 weak = data.weakDerivedAttackState + 1;
									Phoenix::s32 strong = data.strongDerivedAttackState + 1;
									{
										ImGui::InputInt(u8"�h�� ��U�� �ԍ�", &weak);
										ImGui::InputInt(u8"�h�� ���U�� �ԍ�", &strong);
										ImGui::Separator();
									}
									if (attackCount < weak) weak = attackCount;
									if (attackCount < strong) strong = attackCount;

									if (weak <= 0) weak = 0;
									if (strong <= 0) strong = 0;

									data.weakDerivedAttackState = weak - 1;
									data.strongDerivedAttackState = strong - 1;

									if (ImGui::Button(u8"�폜"))
									{
										deleteIndex = count;
									}
									ImGui::Separator();
								}
								ImGui::TreePop();
							}

							++count;
						}

						if (deleteIndex != -1)
						{
							attack.datas.erase(attack.datas.begin() + deleteIndex);
						}

						if (ImGui::Button(u8"�f�[�^�ǉ�") && !saveCheck)
						{
							Phoenix::s32 state = -1;
							for (auto& data : attack.datas)
							{
								state = data.animState;
								break;
							}

							Player::AttackData data;
							data.animState = state == -1 ? 0 : state;

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

	if (helpOpen.at(0))
	{
		bool open = helpOpen.at(0);

		ImGui::Begin(u8"�A�j���[�V�����ԍ� ���X�g", &open);
		{
			ImGui::Text(u8"���X�g���[�g �n�� : 1");
			ImGui::Text(u8"���X�g���[�g �I��� : 2");
			ImGui::Separator();

			ImGui::Text(u8"�E�X�g���[�g �n�� : 3");
			ImGui::Text(u8"�E�X�g���[�g �I��� : 4");
			ImGui::Separator();

			ImGui::Text(u8"���t�b�N �n�� : 5");
			ImGui::Text(u8"���t�b�N �I��� : 6");
			ImGui::Separator();

			ImGui::Text(u8"�E�t�b�N �n�� : 7");
			ImGui::Text(u8"�E�t�b�N �I��� : 8");
			ImGui::Separator();

			ImGui::Text(u8"���L�b�N �n�� : 9");
			ImGui::Text(u8"���L�b�N �I��� : 10");
			ImGui::Separator();

			ImGui::Text(u8"�E�L�b�N �n�� : 11");
			ImGui::Text(u8"�E�L�b�N �I��� : 12");
			ImGui::Separator();

			ImGui::Text(u8"�� ���X�g���[�g : 13");
			ImGui::Separator();

			ImGui::Text(u8"�� �E�X�g���[�g : 14");
			ImGui::Separator();

			ImGui::Text(u8"�� ���t�b�N : 15");
			ImGui::Separator();

			ImGui::Text(u8"�� �E�t�b�N : 16");
			ImGui::Separator();

			ImGui::Text(u8"�� ���L�b�N �n�� : 17");
			ImGui::Text(u8"�� ���L�b�N �� : 18");
			ImGui::Text(u8"�� ���L�b�N �I��� : 19");
			ImGui::Separator();

			ImGui::Text(u8"�� �E�L�b�N : 20");
			ImGui::Separator();
		}
		ImGui::End();

		helpOpen.at(0) = open;
	}

	if (saveCheck && !saveExit)
	{
		ImGui::SetNextWindowPos(ImVec2(width * 0.5f - 80.0f, height * 0.5f - 70), ImGuiCond_Always);
		ImGui::Begin(u8"�m�F");
		{
			ImGui::Text(u8"�㏑���ۑ����܂����H");

			ImGui::Dummy(ImVec2(10.0f, 0.0f));

			ImGui::SameLine(20.0f, 0.0f);
			if (ImGui::Button(u8"�͂�"))
			{
				SaveFile();
			}

			ImGui::SameLine(100.0f, 0.0f);
			if (ImGui::Button(u8"������"))
			{
				saveCheck = false;
			}
		}
		ImGui::End();
	}

	if (saveExit)
	{
		if (5 * 60 <= saveCount++)
		{
			saveExit = false;
			saveCount = 0;
		}
		else
		{
			saveCheck = false;

			ImGui::SetNextWindowPos(ImVec2(width - 300.0f, height - 70), ImGuiCond_Always);
			ImGui::Begin(u8"�ۑ�");
			{
				std::string pass = Phoenix::OS::Path::GetFullPath(u8"..\\Data\\Document\\Player\\AttackDatas.atk");
				ImGui::Text(u8"�ۑ����܂����B");
			}
			ImGui::End();
		}
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

// �t�@�C����ۑ��m�F
void PlayerEditor::SaveCheck()
{
	saveCheck = true;
}

// �t�@�C����ۑ�
void PlayerEditor::SaveFile()
{
	const char* fullPass = Phoenix::OS::Path::GetFullPath("..\\Data\\Document\\Player\\AttackDatas.atk");
	Player::AttackDataList::Serialize(player->GetAttackDatasList(), fullPass);

	saveExit = true;
	saveCount = 0;
}