#include "PlayerEditor.h"
#include "Phoenix/FND/Util.h"


// 生成
std::shared_ptr<PlayerEditor> PlayerEditor::Create()
{
	return std::make_shared<PlayerEditor>();
}

// 初期化
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

	Phoenix::Graphics::DeviceDX11* device = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());
	primitive = std::make_shared<GeometricPrimitive>(device->GetD3DDevice(), 1);

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

// 終了化
void PlayerEditor::Finalize()
{
	player->Finalize();
}

// 更新
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

		camera->ControllerCamera02(true, player->GetPosition(), Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), cameraLen, elapsedTime, lerp);
	}

	Phoenix::FrameWork::LightState* light = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader.get())->GetLight();
	light->direction = Phoenix::Math::Vector4(-camera->GetFront(), 1.0f);

	player->Update(*camera.get(), true, elapsedTime, false);
}

// 描画
void PlayerEditor::Draw(Phoenix::f32 elapsedTime)
{
	Phoenix::Graphics::DeviceDX11* device = static_cast<Phoenix::Graphics::DeviceDX11*>(graphicsDevice->GetDevice());

	pbrShader->Begin(graphicsDevice.get(), *camera);
	pbrShader->Draw(graphicsDevice.get(), player->GetWorldMatrix(), player->GetModel());
	pbrShader->End(graphicsDevice.get());

	if (currentAttackNum != -1)
	{
		auto& list = player->GetAttackDatasList();
		auto& datas = player->GetCollisionDatas();
		for (Phoenix::s32 i = 0; i < datas.size(); ++i)
		{
			for (Phoenix::s32 j = 0; j < list.attackDatas.at(currentAttackNum).datas.size(); ++j)
			{
				if (list.attackDatas.at(currentAttackNum).datas.at(j).collisionNum <= 0) continue;
				if (i == list.attackDatas.at(currentAttackNum).datas.at(j).collisionNum)
				{
					PrimitiveRender(device, datas.at(i).pos, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), Phoenix::Math::Vector3(0.25f, 0.25f, 0.25f));
					break;
				}
			}
		}
	}
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
		// メインメニューを表示している時の処理をここに書きます。
		if (ImGui::BeginMenu(u8"ファイル"))
		{
#pragma region New File
			if (ImGui::MenuItem(u8"新規作成"/*, u8"Ctrl+Shift+N"*/) && !saveCheck)
			{
				NewFile();
			}
#pragma endregion

#pragma region Open File
			if (ImGui::MenuItem(u8"ファイルを開く"/*, u8"Ctrl+O"*/) && !saveCheck)
			{
				OpenFile();
			}
#pragma endregion

#pragma region Save File
			if (ImGui::MenuItem(u8"ファイルを保存", u8"Ctrl+S") && !saveCheck)
			{
				SaveCheck();
			}
#pragma endregion

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"編集"))
		{
			ImGui::MenuItem("(dummy menu)", NULL, false, false); // enabledをfalseにすることでグレー表示できます。

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"ヘルプ"))
		{
			if (ImGui::MenuItem(u8"アニメーション番号 リスト") && !saveCheck)
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

	if (ImGui::Begin(u8"攻撃コンボ エディター"))
	{
		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem(u8"攻撃 リスト"))
			{
				Phoenix::s32 count = 0;
				auto& list = player->GetAttackDatasList();
				for (auto& attack : list.attackDatas)
				{
					if (count == 0)
					{
						if (ImGui::Selectable("Root", selected.at(count)) && !saveCheck)
						{
							ResetSelected();
							selected.at(count) = true;
							currentAttackNum = count;
						}
					}
					else
					{
						if (ImGui::Selectable(std::string("Attack" + std::to_string(count)).c_str(), selected.at(count)) && !saveCheck)
						{
							ResetSelected();
							selected.at(count) = true;
							currentAttackNum = count;
						}
					}

					++count;
				}

				if (ImGui::Button(u8"攻撃追加") && !saveCheck)
				{
					AddAttackData(static_cast<Phoenix::s32>(list.attackDatas.size()));
				}
				if (ImGui::Button(u8"選択中の攻撃削除") && currentAttackNum <= 0)
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

			if (currentAttackNum == 0) ImGui::Begin("Root");
			else ImGui::Begin(std::string("Attack" + std::to_string(currentAttackNum)).c_str());
			{
				if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
				{
					if (ImGui::BeginTabItem(u8"データ リスト"))
					{
						if (currentAttackNum == 0)
						{
							for (auto& data : attack.datas)
							{
								if (ImGui::TreeNode(u8"派生データ"))
								{
									if (!saveCheck)
									{
										ImGui::InputInt(u8"派生 弱攻撃 番号", &data.weakDerivedAttackState);
										ImGui::InputInt(u8"派生 強攻撃 番号", &data.strongDerivedAttackState);
										ImGui::Separator();

										if (attackCount < data.weakDerivedAttackState) data.weakDerivedAttackState = attackCount;
										if (attackCount < data.strongDerivedAttackState) data.strongDerivedAttackState = attackCount;

										if (data.weakDerivedAttackState <= 0) data.weakDerivedAttackState = 0;
										if (data.strongDerivedAttackState <= 0)  data.strongDerivedAttackState = 0;
									}
									ImGui::TreePop();
								}
							}
						}
						else
						{
							if (ImGui::TreeNode(u8"入力キー"))
							{
								ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(350, 60), ImGuiWindowFlags_NoTitleBar);
								{
									bool weakSelect = false;
									bool strongSelect = false;
									if (attack.receptionKey == Player::AttackKey::WeakAttack) weakSelect = true;
									if (attack.receptionKey == Player::AttackKey::StrongAttack) strongSelect = true;
									if (ImGui::Selectable(u8"弱攻撃キー (Xbox : X ボタン, Keyboard : J キー)", weakSelect) && !saveCheck)
									{
										attack.receptionKey = Player::AttackKey::WeakAttack;
									}
									if (ImGui::Selectable(u8"強攻撃キー (Xbox : Y ボタン, Keyboard : K キー)", strongSelect) && !saveCheck)
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
										/*ImGui::InputInt(u8"animState", &data.animState);
										ImGui::Separator();*/

										Phoenix::s32 index = data.animIndex + 1;
										{
											ImGui::InputInt(u8"アニメーション番号", &index);
											ImGui::Separator();
										}
										data.animIndex = index - 1;

										Phoenix::f32 beginTime = data.playBeginTime * 60.0f;
										Phoenix::f32 endTime = data.playEndTime * 60.0f;
										{
											ImGui::InputFloat(u8"再生速度", &data.playSpeed);
											ImGui::InputFloat(u8"再生開始フレーム", &beginTime);
											ImGui::InputFloat(u8"再生終了フレーム", &endTime);
											ImGui::Separator();
										}
										data.playBeginTime = beginTime / 60.0f;
										data.playEndTime = endTime / 60.0f;

										beginTime = data.collisionBeginTime * 60.0f;
										endTime = data.collisionEndTime * 60.0f;
										{
											ImGui::InputInt(u8"当たり判定番号", &data.collisionNum);
											ImGui::InputFloat(u8"当たり判定 開始フレーム", &beginTime);
											ImGui::InputFloat(u8"当たり判定 終了フレーム", &endTime);
											ImGui::Separator();
										}
										data.collisionBeginTime = beginTime / 60.0f;
										data.collisionEndTime = endTime / 60.0f;

										beginTime = data.receptionBeginTime * 60.0f;
										endTime = data.receptionEndTime * 60.0f;
										{
											ImGui::Checkbox(u8"入力スタック", &data.receptionStack);
											ImGui::InputFloat(u8"入力受付 開始フレーム", &beginTime);
											ImGui::InputFloat(u8"入力受付 終了フレーム", &endTime);
											ImGui::Separator();
										}
										data.receptionBeginTime = beginTime / 60.0f;
										data.receptionEndTime = endTime / 60.0f;

										beginTime = data.dedgeReceptionBeginTime * 60.0f;
										endTime = data.dedgeReceptionEndTime * 60.0f;
										{
											ImGui::InputFloat(u8"回避 入力受付 開始フレーム", &beginTime);
											ImGui::InputFloat(u8"回避 入力受付 終了フレーム", &endTime);
											ImGui::Separator();
										}
										data.dedgeReceptionBeginTime = beginTime / 60.0f;
										data.dedgeReceptionEndTime = endTime / 60.0f;


										ImGui::InputInt(u8"派生 弱攻撃 番号", &data.weakDerivedAttackState);
										ImGui::InputInt(u8"派生 強攻撃 番号", &data.strongDerivedAttackState);
										ImGui::Separator();

										if (attackCount < data.weakDerivedAttackState) data.weakDerivedAttackState = attackCount;
										if (attackCount < data.strongDerivedAttackState) data.strongDerivedAttackState = attackCount;

										if (data.weakDerivedAttackState <= 0) data.weakDerivedAttackState = 0;
										if (data.strongDerivedAttackState <= 0)  data.strongDerivedAttackState = 0;

										if (ImGui::Button(u8"削除"))
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

							if (ImGui::Button(u8"データ追加") && !saveCheck)
							{
								AddData(currentAttackNum);
							}
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

		ImGui::Begin(u8"アニメーション番号 リスト", &open);
		{
			ImGui::Text(u8"左ストレート 始め : 1");
			ImGui::Text(u8"左ストレート 終わり : 2");
			ImGui::Separator();

			ImGui::Text(u8"右ストレート 始め : 3");
			ImGui::Text(u8"右ストレート 終わり : 4");
			ImGui::Separator();

			ImGui::Text(u8"左フック 始め : 5");
			ImGui::Text(u8"左フック 終わり : 6");
			ImGui::Separator();

			ImGui::Text(u8"右フック 始め : 7");
			ImGui::Text(u8"右フック 終わり : 8");
			ImGui::Separator();

			ImGui::Text(u8"左キック 始め : 9");
			ImGui::Text(u8"左キック 終わり : 10");
			ImGui::Separator();

			ImGui::Text(u8"右キック 始め : 11");
			ImGui::Text(u8"右キック 終わり : 12");
			ImGui::Separator();

			ImGui::Text(u8"強 左ストレート : 13");
			ImGui::Separator();

			ImGui::Text(u8"強 右ストレート : 14");
			ImGui::Separator();

			ImGui::Text(u8"強 左フック : 15");
			ImGui::Separator();

			ImGui::Text(u8"強 右フック : 16");
			ImGui::Separator();

			ImGui::Text(u8"強 左ターンキック 始め : 17");
			ImGui::Text(u8"強 左ターンキック 中 : 18");
			ImGui::Text(u8"強 左ターンキック 終わり : 19");
			ImGui::Separator();

			ImGui::Text(u8"強 右ターンキック : 20");
			ImGui::Separator();

			ImGui::Text(u8"強 逆 左ターンキック : 21");
			ImGui::Separator();

			ImGui::Text(u8"強 逆 右ターンキック : 22");
			ImGui::Separator();
		}
		ImGui::End();

		helpOpen.at(0) = open;
	}

	if (saveCheck && !saveExit)
	{
		ImGui::SetNextWindowPos(ImVec2(width * 0.5f - 80.0f, height * 0.5f - 70), ImGuiCond_Always);
		ImGui::Begin(u8"確認");
		{
			ImGui::Text(u8"上書き保存しますか？");

			ImGui::Dummy(ImVec2(10.0f, 0.0f));

			ImGui::SameLine(20.0f, 0.0f);
			if (ImGui::Button(u8"はい"))
			{
				SaveFile();
			}

			ImGui::SameLine(100.0f, 0.0f);
			if (ImGui::Button(u8"いいえ"))
			{
				saveCheck = false;
			}
		}
		ImGui::End();
	}

	//player->GUI();

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
			ImGui::Begin(u8"保存");
			{
				std::string pass = Phoenix::OS::Path::GetFullPath(u8"..\\Data\\Document\\Player\\AttackDatas.atk");
				ImGui::Text(u8"保存しました。");
			}
			ImGui::End();
		}
	}
}

// 選択リセット
void PlayerEditor::ResetSelected()
{
	for (Phoenix::s32 i = 0; i < selected.size(); ++i)
	{
		selected.at(i) = false;
	}
}

// 新規作成
void PlayerEditor::NewFile()
{
	Player::AttackDataList dataList;
	player->SetAttackDatasList(dataList);

	AddAttackData(0);
}

// ファイルを開く
void PlayerEditor::OpenFile()
{
	const char* fullPass = Phoenix::OS::Path::GetFullPath("..\\Data\\Document\\Player\\AttackDatas.atk");

	Player::AttackDataList data;
	Player::AttackDataList::Deserialize(data, fullPass);

	player->SetAttackDatasList(data);
}

// ファイルを保存確認
void PlayerEditor::SaveCheck()
{
	saveCheck = true;
}

// ファイルを保存
void PlayerEditor::SaveFile()
{
	const char* fullPass = Phoenix::OS::Path::GetFullPath("..\\Data\\Document\\Player\\AttackDatas.atk");
	Player::AttackDataList::Serialize(player->GetAttackDatasList(), fullPass);

	saveExit = true;
	saveCount = 0;
}

void PlayerEditor::AddAttackData(Phoenix::s32 attackNum)
{
	Player::AttackDatas datas;
	player->GetAttackDatasList().attackDatas.emplace_back(datas);
	
	AddData(attackNum);
	
	selected.emplace_back(false);
}

void PlayerEditor::AddData(Phoenix::s32 attackNum)
{
	auto& attack = player->GetAttackDatasList().attackDatas.at(attackNum);

	Phoenix::s32 state = -1;
	for (auto& data : attack.datas)
	{
		state = data.animState;
		break;
	}
	if (state == -1)
	{
		auto& attackDatas = player->GetAttackDatasList().attackDatas;
		for (Phoenix::s32 i = 0; i < attackDatas.size(); ++i)
		{
			if (i == 0)
			{
				state = 0;
			}
			else
			{
				if (attackDatas.at(i).datas.size() <= 0) continue;
				if (state <= attackDatas.at(i).datas.at(0).animState)
				{
					state = attackDatas.at(i).datas.at(0).animState;
				}
			}
		}

		state += 1;
	}

	if (attackNum == 0) attack.receptionKey = Player::AttackKey::None;

	Player::AttackData data;
	data.animState = state;

	attack.datas.emplace_back(data);
}

void PlayerEditor::PrimitiveRender(Phoenix::Graphics::DeviceDX11* device, Phoenix::Math::Vector3 translate, Phoenix::Math::Vector3 rotate, Phoenix::Math::Vector3 scale)
{
	// ワールド行列を作成
	Phoenix::Math::Matrix W;
	{
		Phoenix::Math::Matrix S, R, T;
		S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
		R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
		T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

		W = S * R * T;
	}

	primitive->Render
	(
		device->GetD3DContext(),
		Phoenix::Math::ConvertToFloat4x4FromVector4x4(W * camera->GetView() * camera->GetProjection()),
		Phoenix::Math::ConvertToFloat4x4FromVector4x4(W),
		DirectX::XMFLOAT4(1, 1, 1, 1),
		DirectX::XMFLOAT4(0.0f, 0.6f, 0.0f, 0.5f),
		false
	);
}