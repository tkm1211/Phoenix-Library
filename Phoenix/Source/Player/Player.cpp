#include "Player.h"
#include <fstream>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/polymorphic.hpp>
#include "Phoenix/FrameWork/Renderer/ModelRenderer.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "../Enemy/EnemyManager.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"

CEREAL_CLASS_VERSION(Player::AttackDataList, 1)
CEREAL_CLASS_VERSION(Player::AttackDatas, 1)
CEREAL_CLASS_VERSION(Player::AttackData, 1)


std::unique_ptr<Player> Player::Create()
{
	return std::make_unique<Player>();
}

void Player::Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	// モデル読み込み
	{
		model = std::make_unique<Phoenix::FrameWork::ModelObject>();
		model->Initialize(graphicsDevice);
		model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Idle\\Breathing_Idle.fbx");
	}

	// アニメーション読み込み
	Phoenix::s32 beginIndex, endIndex;
	{
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardWalk\\Walking_With_Shopping_Bag.fbx", -1); // 1
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\Running.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\SlowRunning.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Roll\\Sprinting_Forward_Roll.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Idle\\Ready_Idle.fbx", -1); // 5
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Forward\\Walk_Forward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Back\\Walk_Backward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Right\\Walk_Right.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Left\\Walk_Left.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Forward\\Forward_Step.fbx", -1); // 10
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Back\\Back_Step.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Right\\Right_Step.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Left\\Left_Step.fbx", -1);

		beginIndex = model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\RightPunch\\Righ_Punch_Begin.fbx", -1); // 14
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\RightPunch\\Righ_Punch_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\LeftPunch\\Left_Punch_Begin.fbx", -1); // 16
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\LeftPunch\\Left_Punch_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\RightHook\\Hook_Punch_Right_Begin.fbx", -1); // 18
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\RightHook\\Hook_Punch_Right_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\LeftHook\\Hook_Punch_Left_Begin.fbx", -1); // 20
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\LeftHook\\Hook_Punch_Left_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\RightKick\\Roundhouse_Kick_Right_Begin.fbx", -1); // 22
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\RightKick\\Roundhouse_Kick_Right_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\LeftKick\\Roundhouse_Kick_Left_Begin.fbx", -1); // 24
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\LeftKick\\Roundhouse_Kick_Left_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Punch\\Cross_Punch_Right.fbx", -1); // 26
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Punch\\Cross_Punch_Left.fbx", -1); // 27

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Hook\\Hook_Punch_Right.fbx", -1); // 28
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Hook\\Hook_Punch_Left.fbx", -1); // 29

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_Begin.fbx", -1); // 30
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_01.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_End.fbx", -1);

		endIndex = model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Mma_Kick.fbx", -1); // 33

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Damage\\Head_Hit_Small.fbx", -1); // 34
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Damage\\Head_Hit.fbx", -1); // 35

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Guard\\Ready_Idle.fbx", -1); // 36

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Death\\Dying.fbx", -1); // 37
	}

	// アニメーターデータ構築
	{
		// レイヤー追加
		Phoenix::s32 layerNum = 0;
		{
			layerNum = model->AddAnimationLayer();
			layerIndexList.insert(std::make_pair(LayerType::Base, layerNum));

			layerNum = model->AddAnimationLayer(56, 65);
			layerIndexList.insert(std::make_pair(LayerType::LowerBody, layerNum));
		}

		// ステート追加
		Phoenix::s32 stateNum = 0;
		{
			// ステート追加関数
			auto AddState = [&](StateType type, Phoenix::u32 animationIndex, Phoenix::u32 layerIndex)
			{
				stateNum = model->AddAnimationStateToLayer(animationIndex, layerIndex);
				stateIndexList.insert(std::make_pair(type, stateNum));
			};

			// ベースレイヤーにステート追加
			layerNum = layerIndexList.at(LayerType::Base);
			{
				for (Phoenix::s32 i = beginIndex; i <= endIndex; ++i)
				{
					model->AddAnimationStateToLayer(i, layerNum);
				}

				AddState(StateType::Idle, 0, layerNum);
				AddState(StateType::BattleIdle, 5, layerNum);
				AddState(StateType::DamageSmall, endIndex + 1, layerNum);
				AddState(StateType::DamageBig, endIndex + 2, layerNum);
				AddState(StateType::ForwardDedge, 10, layerNum);
				AddState(StateType::BackDedge, 11, layerNum);
				AddState(StateType::RightDedge, 12, layerNum);
				AddState(StateType::LeftDedge, 13, layerNum);
				AddState(StateType::Death, endIndex + 4, layerNum);

				Phoenix::s32 blendTreeIndex = model->AddBlendTreeToLayer(layerNum);
				model->AddBlendAnimationStateToBlendTree(1, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(3, Phoenix::Math::Vector3(1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
			}

			// 下半身レイヤーにブレンドツリー追加
			layerNum = layerIndexList.at(LayerType::LowerBody);
			{
				Phoenix::s32 blendTreeIndex = model->AddBlendTreeToLayer(layerNum);
				model->AddBlendAnimationStateToBlendTree(5, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(6, Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(7, Phoenix::Math::Vector3(0.0f, -1.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(8, Phoenix::Math::Vector3(1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(9, Phoenix::Math::Vector3(-1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
			}
		}
	}

	// コリジョン初期化
	{
		collisionDatas.resize(5);

		collisionDatas.at(0).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(0).radius = 0.5f;
		collisionDatas.at(0).boneIndex = model->GetBoneIndex("Hips");

		collisionDatas.at(1).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(1).radius = WeakAttackCollisionRadius;
		collisionDatas.at(1).boneIndex = model->GetBoneIndex("RightHandIndex1");

		collisionDatas.at(2).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(2).radius = WeakAttackCollisionRadius;
		collisionDatas.at(2).boneIndex = model->GetBoneIndex("LeftHandIndex1");

		collisionDatas.at(3).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(3).radius = WeakAttackCollisionRadius;
		collisionDatas.at(3).boneIndex = model->GetBoneIndex("RightFoot");

		collisionDatas.at(4).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(4).radius = WeakAttackCollisionRadius;
		collisionDatas.at(4).boneIndex = model->GetBoneIndex("LeftFoot");
	}

	// UI生成
	{
		ui = PlayerUI::Create();
	}

	// 攻撃データ生成
	{
		const char* fullPass = Phoenix::OS::Path::GetFullPath("..\\Data\\Document\\Player\\AttackDatas.atk");

		Player::AttackDataList data;
		Player::AttackDataList::Deserialize(data, fullPass);

		SetAttackDatasList(data);
	}

	// エネミーヒット
	{
		isHit.resize(EnemyManager::EnemyRange);
		for (auto hit : isHit)
		{
			hit = false;
		}

		isAttackJudgment.resize(EnemyManager::EnemyRange);
		for (auto judge : isAttackJudgment)
		{
			judge = false;
		}
	}
}

void Player::Initialize()
{
	// アニメーションパラメーターの設定
	{
		animationState = AnimationState::Idle;
		attackState = -1;
		isChangeAnimation = false;
		isAttack = false;
		speed = 0.0f;
		animationSpeed = AnimationSpeed60;
		attackReceptionTimeCnt = 0.0f;
	}

	// 待機モーション開始
	{
		model->PlayAnimation(layerIndexList.at(LayerType::Base), stateIndexList.at(StateType::Idle), 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
	}

	// トランスフォームの初期化
	{
		worldMatrix = Phoenix::Math::MatrixIdentity();
		pos = { 0,0,12.0f }; // tutorial : 135.0f , main : 12.0f
		rotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), 180.0f * 0.01745f);
		scale = { 1,1,1 };
		radius = 0.35f;
	}

	// パラメーターの初期化
	{
		life = MaxLife;
		invincible = false;
		alive = true;
		death = false;
		attackCollisionIndex = -1;
		accumulationDamege = 0;
		accumulationTimeCnt = 0;
		blendRate = { 0.0f, 0.0f, 0.0f };
		newRotate = rotate;
		rotateY = 180.0f * 0.01745f;
		inTerritory = false;
		isBattleMode = false;
		receptionStack = false;
		stackKey = AttackKey::None;
		behaviorScore = 0;
		attackDamage = 0;

		for (auto judge : isAttackJudgment)
		{
			judge = false;
		}
	}

	// 攻撃データファイルのタイムスタンプを取得
	{
		HANDLE hFile;
		FILETIME ftFileTime, ftLocalFileTime;

		const wchar_t* fullPass = Phoenix::OS::Path::GetFullPathW(L"..\\Data\\Document\\Player\\AttackDatas.atk");
		hFile = CreateFile(fullPass, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			GetFileTime(hFile, NULL, NULL, &ftFileTime);
			FileTimeToLocalFileTime(&ftFileTime, &ftLocalFileTime);
			FileTimeToSystemTime(&ftLocalFileTime, &stFileTime);

			CloseHandle(hFile);
		}
	}
}

void Player::Finalize()
{
	attackDatasList.attackDatas.clear();
}

void Player::Update(Phoenix::Graphics::Camera& camera, bool onControl, Phoenix::f32 elapsedTime, bool attackLoad)
{
	if (attackLoad)
	{
		HANDLE hFile;

		const wchar_t* fullPass = Phoenix::OS::Path::GetFullPathW(L"..\\Data\\Document\\Player\\AttackDatas.atk");
		hFile = CreateFile(fullPass, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			FILETIME ftFileTime, ftLocalFileTime;
			SYSTEMTIME newSTFileTime;

			newSTFileTime.wYear = 0;
			newSTFileTime.wMonth = 0;
			newSTFileTime.wDay = 0;
			newSTFileTime.wHour = 0;
			newSTFileTime.wMinute = 0;
			newSTFileTime.wSecond = 0;

			bool check = GetFileTime(hFile, NULL, NULL, &ftFileTime);
			CloseHandle(hFile);

			if (check)
			{
				FileTimeToLocalFileTime(&ftFileTime, &ftLocalFileTime);
				FileTimeToSystemTime(&ftLocalFileTime, &newSTFileTime);

				if (stFileTime.wYear != newSTFileTime.wYear
					|| stFileTime.wMonth != newSTFileTime.wMonth
					|| stFileTime.wDay != newSTFileTime.wDay
					|| stFileTime.wHour != newSTFileTime.wHour
					|| stFileTime.wMinute != newSTFileTime.wMinute
					|| stFileTime.wSecond != newSTFileTime.wSecond)
				{
					const char* fullPass = Phoenix::OS::Path::GetFullPath("..\\Data\\Document\\Player\\AttackDatas.atk");

					Player::AttackDataList data;
					Player::AttackDataList::Deserialize(data, fullPass);

					SetAttackDatasList(data);

					stFileTime = newSTFileTime;
				}
			}
		}
	}

	// ライフが０ならマネージャーの生存エネミーカウントを下げる
	if (life <= 0 && alive)
	{
		alive = false;
		ChangeAnimationState(AnimationState::Death);
		ChangeAnimation();
	}

	// 死亡アニメーション終了時に存在を消す
	if (!alive)
	{
		if (!model->IsPlaying())
		{
			death = true;
		}
		else
		{
			model->UpdateTransform(elapsedTime / 60.0f);
		}

		return;
	}

	// スコア計算
	{
		Phoenix::f32 score = static_cast<Phoenix::f32>(behaviorScore);
		{
			score = Phoenix::Math::f32Lerp(score, 0.0f, 0.05f * elapsedTime);
		}
		behaviorScore = static_cast<Phoenix::s32>(score);
	}

	// 蓄積ダメージの確認
	bool isAccumulationDamege = false;
	{
		isAccumulationDamege = AccumulationDamege();
	}

	// コントローラー操作(位置更新)
	{
		if (onControl && !isAccumulationDamege) Control(camera, elapsedTime);
	}

	// アニメーション変更
	{
		ChangeAnimation();
	}

	// アニメーション更新
	{
		if (isBattleMode)
		{
			model->SetBlendRate(Phoenix::Math::Vector3(-blendRate.x, blendRate.y, 0.0f));
		}
		else
		{
			model->SetBlendRate(blendRate.z);
		}

		model->UpdateTransform(elapsedTime / 60.0f);
	}

	// ワールド行列を作成
	{
		UpdateTrasform();
	}

	// コリジョン更新
	{
		Phoenix::Math::Matrix systemUnitTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		systemUnitTransform._11 = systemUnitTransform._22 = systemUnitTransform._33 = 0.01f;

		auto nodes = model->GetNodes();
		for (auto& data : collisionDatas)
		{
			Phoenix::Math::Matrix bone = nodes->at(data.boneIndex).worldTransform;
			bone *= systemUnitTransform * worldMatrix;
			data.pos = Phoenix::Math::Vector3(bone._41, bone._42, bone._43);
		}
	}

	// アタック判定中
	{
		AttackJudgment();
	}
}

void Player::UpdateTrasform()
{
	Phoenix::Math::Vector3 scale = this->scale;
	Phoenix::Math::Quaternion rotate = this->rotate;
	Phoenix::Math::Vector3 translate = pos;

	Phoenix::Math::Matrix S, R, T;
	S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
	R = Phoenix::Math::MatrixRotationQuaternion(&rotate);
	T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

	worldMatrix = S * R * T;
}

void Player::UpdateUI()
{
	Phoenix::f32 hp = static_cast<Phoenix::f32>(life);
	hp = hp <= 0 ? 0 : hp;

	ui->Update((hp / MaxLife) * 100.0f);
}

void Player::Control(Phoenix::Graphics::Camera& camera, Phoenix::f32 elapsedTime) // TODO : re -> player control
{
	Phoenix::f32 sX = 0.0f;
	Phoenix::f32 sY = 0.0f;

	sX = xInput[0].sX / 1000.0f;
	sY = xInput[0].sY / 1000.0f;

	sY = GetKeyState('W') < 0 ? -1.0f : sY;
	sY = GetKeyState('S') < 0 ? 1.0f : sY;
	sX = GetKeyState('A') < 0 ? -1.0f : sX;
	sX = GetKeyState('D') < 0 ? 1.0f : sX;

	// プレイヤーの最終方向を決定する角度を計算
	auto UpdateRotateY = [&](Phoenix::f32 sX, Phoenix::f32 sY, Phoenix::f32 cameraRotateY)
	{
		float len = sqrtf(sX * sX + sY * sY);

		if (len <= 0)
		{
			sX = 0;
			sY = 0;
		}

		float mag = 1 / len;

		sX *= mag;
		sY *= mag;

		rotateY = cameraRotateY + atan2f(sX, sY);
	};

	// プレイヤー回転
	auto RotatePlayer = [&](Phoenix::f32 angle, bool isBattleMode)
	{
		if (isBattleMode)
		{
			Phoenix::Math::Vector3 dir = Phoenix::Math::Vector3Normalize(targetPos - GetPosition());
			float len = sqrtf(dir.x * dir.x + dir.z * dir.z);

			if (len <= 0)
			{
				dir.x = 0;
				dir.z = 0;
			}

			float mag = 1 / len;

			dir.x *= mag;
			dir.z *= mag;

			Phoenix::f32 angleY = atan2f(dir.x, dir.z);

			newRotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angleY);
		}
		else
		{
			newRotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angle);
		}
	};

	auto RotatePlayerToAttack = [&]()
	{
		Phoenix::Math::Vector3 dir = targetPos - pos;
		dir = Phoenix::Math::Vector3Normalize(dir);
		dir.y = 0.0f;

		Phoenix::Math::Matrix m = Phoenix::Math::MatrixRotationQuaternion(&rotate);
		Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(m._31, m._32, m._33);
		forward.y = 0.0f;

		Phoenix::f32 angle;
		angle = acosf(Phoenix::Math::Vector3Dot(dir, forward));

		if (1e-8f < fabs(angle))
		{
			angle /= 0.01745f;

			if (angle <= 45.0f)
			{
				float len = sqrtf(dir.x * dir.x + dir.z * dir.z);

				if (len <= 0)
				{
					dir.x = 0;
					dir.z = 0;
				}

				float mag = 1 / len;

				dir.x *= mag;
				dir.z *= mag;

				Phoenix::f32 angleY = atan2f(dir.x, dir.z);

				newRotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angleY);
			}
		}
	};

	auto ChangeAnimation = [&](Phoenix::u32 index, Phoenix::u32 nextIndex)
	{
		if (attackDatasList.attackDatas.at(index).datas.size() - 1 <= attackComboState)
		{
			attackComboState = -1;
		}

		if (attackComboState == -1)
		{
			attackComboState = 0;
			attackReceptionTimeCnt = attackDatasList.attackDatas.at(nextIndex).datas.at(attackComboState).playBeginTime != -1 ? attackDatasList.attackDatas.at(nextIndex).datas.at(attackComboState).playBeginTime : 0.0f;

			receptionStack = false;
			stackKey = AttackKey::None;

			ChangeAnimationState(AnimationState::Attack, 0.0f);
			ChangeAttackAnimationState(attackDatasList.attackDatas.at(nextIndex).datas.at(attackComboState).animState, attackDatasList.attackDatas.at(nextIndex).datas.at(attackComboState).animIndex, attackDatasList.attackDatas.at(nextIndex).datas.at(attackComboState).playSpeed);

			speed = Attack01MoveSpeed;

			if (sX != 0.0f || sY != 0.0f)
			{
				UpdateRotateY(sX, sY, camera.GetRotateY());
				RotatePlayer(rotateY, isBattleMode);
			}

			RotatePlayerToAttack();
		}
	};

	auto JudgeDedgeIndex = [&]()
	{
		if (sX != 0.0f || sY != 0.0f)
		{
			UpdateRotateY(sX, sY, camera.GetRotateY());
			RotatePlayer(rotateY, true);

			if (fabsf(sX) <= fabsf(sY))
			{
				if (sY < 0.0f)
				{
					dedgeLayerIndex = stateIndexList.at(StateType::ForwardDedge);
				}
				if (sY > 0.0f)
				{
					dedgeLayerIndex = stateIndexList.at(StateType::BackDedge);
				}
			}
			else if (fabsf(sY) <= fabsf(sX))
			{
				if (sX < 0.0f)
				{
					dedgeLayerIndex = stateIndexList.at(StateType::RightDedge);
				}
				if (sX > 0.0f)
				{
					dedgeLayerIndex = stateIndexList.at(StateType::LeftDedge);
				}
			}
		}
		else
		{
			Phoenix::Math::Vector3 dir = targetPos - GetPosition();
			dir = Phoenix::Math::Vector3Normalize(dir);
			dir.y = 0.0f;

			Phoenix::f32 fictitiousCameraRotateY = atan2f(-dir.x, -dir.z);

			UpdateRotateY(0.0f, 1.0f, fictitiousCameraRotateY);
			RotatePlayer(rotateY, true);
			dedgeLayerIndex = stateIndexList.at(StateType::BackDedge);
		}
	};

	auto JudgeInput01 = [&](Phoenix::s32 index, Phoenix::s32 nextIndex)
	{
		if (!attackDatasList.attackDatas.at(index).datas.at(attackComboState).receptionStack)
		{
			if (attackDatasList.attackDatas.at(nextIndex).receptionKey == stackKey)
			{
				ChangeAnimation(index, nextIndex);

				Phoenix::s32 animIndex = attackDatasList.attackDatas.at(0).datas.at(0).animIndex;
				if (0 <= animIndex && animIndex <= 7 || 12 <= animIndex && animIndex <= 15)
				{
					if (soundSystem) soundSystem->Play(SoundType::SE_Player_Attack_Punch_Swing);
				}
				else if (8 <= animIndex && animIndex <= 11 || 16 <= animIndex && animIndex <= 19)
				{
					if (soundSystem) soundSystem->Play(SoundType::SE_Player_Attack_Kick_Swing);
				}
			}
		}
	};

	auto JudgeInput02 = [&](Phoenix::s32 index, Phoenix::s32 nextIndex, AttackKey key)
	{
		if (attackDatasList.attackDatas.at(nextIndex).receptionKey == key)
		{
			if (attackDatasList.attackDatas.at(index).datas.at(attackComboState).receptionStack)
			{
				receptionStack = true;
				stackKey = key;
			}
			else
			{
				ChangeAnimation(index, nextIndex);

				Phoenix::s32 animIndex = attackDatasList.attackDatas.at(0).datas.at(0).animIndex;
				if (0 <= animIndex && animIndex <= 7 || 12 <= animIndex && animIndex <= 15)
				{
					if (soundSystem) soundSystem->Play(SoundType::SE_Player_Attack_Punch_Swing);
				}
				else if (8 <= animIndex && animIndex <= 11 || 16 <= animIndex && animIndex <= 19)
				{
					if (soundSystem) soundSystem->Play(SoundType::SE_Player_Attack_Kick_Swing);
				}
			}
		}
	};

	// ダメージ中に回避に遷移
	if (animationState == AnimationState::Damage && model->IsPlaying())
	{
		// 回避ステートへ
		if ((xInput[0].bAt || GetAsyncKeyState(VK_SPACE) & 1) && model->GetLastTime() <= (15.0f / 60.0f))
		{
			ChangeAnimationState(AnimationState::Dedge, DedgeSpeed);
			JudgeDedgeIndex();
		}
		else
		{
			return;
		}
	}

	// ロックオン
	if (xInput[0].bRBs || GetKeyState('L') < 0)
	{
		InEnemyTerritory(true);
	}
	else
	{
		InEnemyTerritory(false);
	}

	// ブレンドレート計算
	{
		if (isBattleMode)
		{
			blendRate.x = Phoenix::Math::f32Lerp(blendRate.x, sX, 0.15f * elapsedTime);
			blendRate.y = Phoenix::Math::f32Lerp(blendRate.y, sY, 0.15f * elapsedTime);
		}

		blendRate.z = Phoenix::Math::Vector2Length(Phoenix::Math::Vector2(sX, sY));
		blendRate.z = 1.0f <= blendRate.z ? 1.0f : blendRate.z;
	}

	// 攻撃キーの入力確認
	AttackKey key = AttackKey::None;
	{
		if (xInput[0].bXt || GetAsyncKeyState('J') & 1)
		{
			key = AttackKey::WeakAttack;
		}
		if (xInput[0].bYt || GetAsyncKeyState('K') & 1)
		{
			key = AttackKey::StrongAttack;
		}
	}

	// 攻撃ステートへ
	if ((key != AttackKey::None) && 0 < attackDatasList.attackDatas.size() && ((animationState == AnimationState::Attack) || (animationState == AnimationState::Idle) || (animationState == AnimationState::Walk) || (animationState == AnimationState::Run)))
	{
		if (attackState == -1)
		{
			// 弱攻撃からスタートするため
			if (attackDatasList.attackDatas.at(0).receptionKey == key && 0 < attackDatasList.attackDatas.at(0).datas.size())
			{
				attackComboState = 0;
				attackReceptionTimeCnt = 0.0f < attackDatasList.attackDatas.at(0).datas.at(0).playBeginTime ? attackDatasList.attackDatas.at(0).datas.at(0).playBeginTime : 0.0f;

				ChangeAnimationState(AnimationState::Attack, 0.0f);
				ChangeAttackAnimationState(attackDatasList.attackDatas.at(0).datas.at(0).animState, attackDatasList.attackDatas.at(0).datas.at(0).animIndex, attackDatasList.attackDatas.at(0).datas.at(0).playSpeed);

				speed = Attack01MoveSpeed;

				if (sX != 0.0f || sY != 0.0f)
				{
					UpdateRotateY(sX, sY, camera.GetRotateY());
					RotatePlayer(rotateY, isBattleMode);
				}

				RotatePlayerToAttack();

				Phoenix::s32 animIndex = attackDatasList.attackDatas.at(0).datas.at(0).animIndex;
				if (0 <= animIndex && animIndex <= 7 || 12 <= animIndex && animIndex <= 15)
				{
					if (soundSystem) soundSystem->Play(SoundType::SE_Player_Attack_Punch_Swing);
				}
				else if (8 <= animIndex && animIndex <= 11 || 16 <= animIndex && animIndex <= 19)
				{
					if (soundSystem) soundSystem->Play(SoundType::SE_Player_Attack_Kick_Swing);
				}
			}
		}
		else if (0 < attackDatasList.attackDatas.at(attackState).datas.size())
		{
			Phoenix::s32 index = attackState;
			Phoenix::s32 wearNextIndex = attackDatasList.attackDatas.at(index).datas.at(attackComboState).weakDerivedAttackState;
			Phoenix::s32 strongNextIndex = attackDatasList.attackDatas.at(index).datas.at(attackComboState).strongDerivedAttackState;
			Phoenix::s32 endIndex = static_cast<Phoenix::s32>(attackDatasList.attackDatas.size());

			// 次の攻撃が発動するボタンの受付
			if (receptionStack)
			{
				if (wearNextIndex < endIndex && 0 <= wearNextIndex)
				{
					JudgeInput01(index, wearNextIndex);
				}
				if (strongNextIndex < endIndex && 0 <= strongNextIndex)
				{
					JudgeInput01(index, strongNextIndex);
				}
			}
			else if (attackDatasList.attackDatas.at(index).datas.at(attackComboState).receptionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt < attackDatasList.attackDatas.at(index).datas.at(attackComboState).receptionEndTime)
			{
				if (wearNextIndex < endIndex && 0 <= wearNextIndex)
				{
					JudgeInput02(index, wearNextIndex, key);
				}
				if (strongNextIndex < endIndex && 0 <= strongNextIndex)
				{
					JudgeInput02(index, strongNextIndex, key);
				}
			}
		}
	}
	else if ((animationState == AnimationState::Attack) && !model->IsPlaying())
	{
		Phoenix::s32 index = attackState;

		if (0 <= attackComboState && attackComboState < attackDatasList.attackDatas.at(index).datas.size() - 1)
		{
			++attackComboState;

			ChangeAnimationState(AnimationState::Attack, 0.0f);
			ChangeAttackAnimationState(attackDatasList.attackDatas.at(index).datas.at(attackComboState).animState, attackDatasList.attackDatas.at(index).datas.at(attackComboState).animIndex, attackDatasList.attackDatas.at(index).datas.at(attackComboState).playSpeed);
		}
		else
		{
			ChangeAnimationState(AnimationState::Idle, 0.0f);
			ChangeAttackAnimationState(-1, -1, 0.0f);

			isAttack = false;
			attackReceptionTimeCnt = 0.0f;
			attackComboState = -1;

			receptionStack = false;
			stackKey = AttackKey::None;
		}
	}

	if (isChangeAnimation && isAttack)
	{
		if (key == AttackKey::WeakAttack)
		{
			behaviorScore += WeakAttackScore;
		}
		else if (key == AttackKey::StrongAttack)
		{
			behaviorScore += StrongAttackScore;
		}
	}

	// 攻撃ステート以外
	if (!isAttack)
	{
		// 回避ステート
		{
			// 回避ステートへ
			if ((xInput[0].bAt || GetAsyncKeyState(VK_SPACE) & 1) && animationState != AnimationState::Dedge)
			{
				ChangeAnimationState(AnimationState::Dedge, DedgeSpeed);
				JudgeDedgeIndex();
			}
			// 回避ステート中
			else if (animationState == AnimationState::Dedge)
			{
				speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.025f * elapsedTime);
				// 待機ステートへ
				if (!model->IsPlaying())
				{
					ChangeAnimationState(AnimationState::Idle, 0.0f);
				}
			}
		}

		// 回避ステート以外
		if (animationState != AnimationState::Roll && animationState != AnimationState::Dedge)
		{
			// 移動ステート
			if (sX != 0.0f || sY != 0.0f)
			{
				UpdateRotateY(sX, sY, camera.GetRotateY());
				RotatePlayer(rotateY, isBattleMode);

				if (isBattleMode)
				{
					if (animationState != AnimationState::Walk)
					{
						ChangeAnimationState(AnimationState::Walk, BattleWalkSpeed);
					}
				}
				else
				{
					if ((!xInput[0].bRBs && !(GetKeyState(VK_SHIFT) < 0)) && animationState != AnimationState::Walk)
					{
						ChangeAnimationState(AnimationState::Walk, WalkSpeed);
					}
				}
			}
			// 待機ステートへ
			else if (animationState != AnimationState::Idle)
			{
				ChangeAnimationState(AnimationState::Idle, 0.0f);
			}
		}
	}
	// 攻撃中
	else
	{
		// 回避ステートへ
		if ((xInput[0].bAt || GetAsyncKeyState(VK_SPACE) & 1) && animationState != AnimationState::Dedge)
		{
			Phoenix::s32 index = attackState;

			// 次の攻撃が発動するボタンの受付
			if (attackDatasList.attackDatas.at(index).datas.at(attackComboState).dedgeReceptionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt < attackDatasList.attackDatas.at(index).datas.at(attackComboState).dedgeReceptionEndTime)
			{
				ChangeAnimationState(AnimationState::Dedge, DedgeSpeed);
				ChangeAttackAnimationState(-1, -1, 0.0f);

				isAttack = false;
				attackReceptionTimeCnt = 0.0f;

				JudgeDedgeIndex();
			}
		}

		// アタックアニメーションスピード計測
		attackReceptionTimeCnt += animationSpeed * elapsedTime / 60.0f; // animationSpeed / 60.0f
	}

	// 座標更新
	if (!isChangeAnimation)
	{
		rotate = Phoenix::Math::QuaternionSlerp(rotate, newRotate, 0.17f * elapsedTime);
		{
			if (animationState == AnimationState::Walk/* && !isBattleMode*/)
			{
				if (isBattleMode)
				{
					pos.x += sinf(rotateY) * ((speed + (BattleSlowRunSpeed * blendRate.z) * elapsedTime));
					pos.z += cosf(rotateY) * ((speed + (BattleSlowRunSpeed * blendRate.z) * elapsedTime));
				}
				else
				{
					pos.x += sinf(rotateY) * ((speed + (SlowRunSpeed * blendRate.z)) * elapsedTime);
					pos.z += cosf(rotateY) * ((speed + (SlowRunSpeed * blendRate.z)) * elapsedTime);
				}
			}
			else if (animationState != AnimationState::Attack)
			{
				pos.x += sinf(rotateY) * (speed * elapsedTime);
				pos.z += cosf(rotateY) * (speed * elapsedTime);
			}
		}

		if (animationState == AnimationState::Attack)
		{
			if (isAttack && 0.0f < speed)
			{
				Phoenix::Math::Matrix matrix = Phoenix::Math::MatrixRotationQuaternion(&rotate);
				Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(matrix._31, matrix._32, matrix._33);

				pos.x += forward.x * (speed * elapsedTime);
				pos.z += forward.z * (speed * elapsedTime);

				speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * elapsedTime);
			}
		}
	}
}

void Player::ChangeAnimation()
{
	if (!isChangeAnimation) return;

	Phoenix::u32 animationNum = static_cast<Phoenix::u32>(animationState);
	Phoenix::s32 baseLayerIndex = layerIndexList.at(LayerType::Base);
	Phoenix::s32 lowerBodyLayerIndex = layerIndexList.at(LayerType::LowerBody);

	// SE停止
	{
		if (soundSystem) soundSystem->Stop(SoundType::SE_Player_Walk);
		/*soundSystem->Stop(SoundType::SE_Player_Attack_Punch_Swing);
		soundSystem->Stop(SoundType::SE_Player_Attack_Punch_Hit_Right);
		soundSystem->Stop(SoundType::SE_Player_Attack_Punch_Hit_Heavy);
		soundSystem->Stop(SoundType::SE_Player_Attack_Kick_Swing);
		soundSystem->Stop(SoundType::SE_Player_Attack_Kick_Hit_Right);
		soundSystem->Stop(SoundType::SE_Player_Attack_Kick_Hit_Heavy);*/
	}

	switch (animationState)
	{
	case AnimationState::Idle:
		if (isBattleMode)
		{
			model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::BattleIdle), 1, 0.2f);
			model->SetLoopAnimation(true);
		}
		else
		{
			model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::Idle), 1, 0.4f);
			model->SetLoopAnimation(true);
		}
		break;

	case AnimationState::Walk:
		if (isBattleMode)
		{
			model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::BattleIdle), 1, 0.2f);
			model->SimultaneousPlayBlendTreeAniamation(lowerBodyLayerIndex, 0, 1, 0.2f);
			model->SetLoopAnimation(true);
			model->SetBlendLoopAnimation(true);
			if (soundSystem) soundSystem->Play(SoundType::SE_Player_Walk);
		}
		else
		{
			model->PlayBlendTreeAnimation(baseLayerIndex, 0, 1, 0.2f);
			model->SetLoopAnimation(true);
			if (soundSystem) soundSystem->Play(SoundType::SE_Player_Walk);
		}
		break;

	case AnimationState::Attack:
		ChangeAttackAnimation(baseLayerIndex);
		break;

	case AnimationState::Damage:
		if (damagePower == 0)
		{
			model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::DamageSmall), 1, 0.2f);
			model->SetEndTime(43.0f / 60.0f);
		}
		else if (damagePower == 1)
		{
			model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::DamageBig), 1, 0.2f);
		}
		model->SetLoopAnimation(false);
		break;

	case AnimationState::Dedge:
		model->PlayAnimation(baseLayerIndex, dedgeLayerIndex, 0, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(2.25f);
		if (soundSystem) soundSystem->Play(SoundType::SE_Player_Dedge);
		break;

	case AnimationState::Death:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::Death), 1, 0.2f);
		model->SetLoopAnimation(false);
		break;
	
	default: break;
	}

	isChangeAnimation = false;
}

void Player::ChangeAttackAnimation(Phoenix::s32 layerIndex)
{
	Phoenix::s32 index = attackState;
	Phoenix::u32 animIndex = static_cast<Phoenix::u32>(attackDatasList.attackDatas.at(index).datas.at(attackComboState).animIndex);
	Phoenix::f32 animationSpeed = attackDatasList.attackDatas.at(index).datas.at(attackComboState).playSpeed;
	Phoenix::f32 beginTime = attackDatasList.attackDatas.at(index).datas.at(attackComboState).playBeginTime;
	Phoenix::f32 endTime = attackDatasList.attackDatas.at(index).datas.at(attackComboState).playEndTime;

	if (0 <= animIndex && animIndex <= 11 || 16 <= animIndex && animIndex <= 18)
	{
		model->PlayAnimation(layerIndex, animIndex, 0, 0.2f);
	}
	else
	{
		model->PlayAnimation(layerIndex, animIndex, 1, 0.2f);
	}
	model->SetLoopAnimation(false);
	model->SetSpeed(animationSpeed);

	if (0.0f <= beginTime)
	{
		model->SetBeginTime(beginTime);
		model->SetCurrentTime(beginTime);
	}
	if (0.0f < endTime)
	{
		model->SetEndTime(endTime);
	}
}

void Player::AttackJudgment()
{
	if (isAttack)
	{
		auto Judgment = [&](Phoenix::s32 index)
		{
			for (Phoenix::s32 i = 0; i < isHit.size(); ++i)
			{
				if (isHit.at(i))
				{
					isAttackJudgment.at(i) = false;
					return;
				}

				isAttackJudgment.at(i) = true;
			}

			attackCollisionIndex = index;
		};
		auto NoJudgment = [&]()
		{
			for (Phoenix::s32 i = 0; i < isHit.size(); ++i)
			{
				isAttackJudgment.at(i) = false;
				isHit.at(i) = false;
			}
			attackCollisionIndex = -1;
		};

		Phoenix::s32 index = attackState;

		// 当たり判定
		if (attackDatasList.attackDatas.at(index).datas.at(attackComboState).collisionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt < attackDatasList.attackDatas.at(index).datas.at(attackComboState).collisionEndTime)
		{
			Judgment(attackDatasList.attackDatas.at(index).datas.at(attackComboState).collisionNum);
			if (attackDatasList.attackDatas.at(index).receptionKey == AttackKey::WeakAttack)
			{
				attackPower = 0;
				attackDamage = 10;
				collisionDatas.at(attackCollisionIndex).radius = WeakAttackCollisionRadius;
			}
			else if (attackDatasList.attackDatas.at(index).receptionKey == AttackKey::StrongAttack)
			{
				attackPower = 1;
				attackDamage = 20;
				collisionDatas.at(attackCollisionIndex).radius = StrongAttackCollisionRadius;
			}
			else
			{
				attackPower = 0;
				attackDamage = 0;
				collisionDatas.at(attackCollisionIndex).radius = WeakAttackCollisionRadius;
			}
		}
		else
		{
			NoJudgment();
		}
	}
	else
	{
		for (Phoenix::s32 i = 0; i < isHit.size(); ++i)
		{
			isAttackJudgment.at(i) = false;
		}
	}
}

void Player::Damage(int damage, Phoenix::u32 damagePower)
{
	life -= damage;
	accumulationDamege += damage;
	this->damagePower = damagePower;
}

bool Player::AccumulationDamege()
{
	if (animationState == AnimationState::Damage)
	{
		if (!model->IsPlaying())
		{
			if (animationState != AnimationState::Idle)
			{
				attackReceptionTimeCnt = 0;
				isAttack = false;
				isChangeAnimation = true;
				speed = 0.0f;
				animationState = AnimationState::Idle;
				attackState = -1;
			}
		}
	}

	if (accumulationDamege == 0) return false;

	if (AccumulationMaxDamege <= accumulationDamege)
	{
		isChangeAnimation = true;
		isAttack = false;
		speed = 0.0f;
		animationState = AnimationState::Damage;

		attackState = -1;
		attackReceptionTimeCnt = 0;

		accumulationDamege = 0;
		accumulationTimeCnt = 0;

		return true;
	}
	else if (AccumulationTime <= accumulationTimeCnt++)
	{
		accumulationDamege = 0;
		accumulationTimeCnt = 0;
	}

	return false;
}

void Player::GUI()
{
	static Phoenix::s32 animClip = 0;

	if (ImGui::TreeNode("Player"))
	{
		if (ImGui::TreeNode("Prameter"))
		{
			ImGui::Text("HP : %d", life);
			ImGui::Text("BehaviorScore : %d", behaviorScore);
			ImGui::Checkbox("BattleMode", &isBattleMode);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Transform"))
		{
			ImGui::DragFloat3("pos", &pos.x);
			ImGui::DragFloat3("rotate", &rotate.x);
			ImGui::DragFloat3("scale", &scale.x);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Speed"))
		{
			ImGui::Text("speed : %f", speed);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Bone"))
		{
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

template<class Archive>
void Player::AttackData::serialize(Archive& archive, Phoenix::u32 version)
{
	archive
	(
		CEREAL_NVP(animState),
		CEREAL_NVP(animIndex),

		CEREAL_NVP(playSpeed),
		CEREAL_NVP(playBeginTime),
		CEREAL_NVP(playEndTime),

		CEREAL_NVP(collisionNum),
		CEREAL_NVP(collisionBeginTime),
		CEREAL_NVP(collisionEndTime),

		CEREAL_NVP(receptionStack),
		CEREAL_NVP(receptionBeginTime),
		CEREAL_NVP(receptionEndTime),

		CEREAL_NVP(dedgeReceptionBeginTime),
		CEREAL_NVP(dedgeReceptionEndTime),

		CEREAL_NVP(weakDerivedAttackState),
		CEREAL_NVP(strongDerivedAttackState)
	);
}

template<class Archive>
void Player::AttackDatas::serialize(Archive& archive, Phoenix::u32 version)
{
	archive
	(
		CEREAL_NVP(receptionKey),
		CEREAL_NVP(datas)
	);
}

template<class Archive>
void Player::AttackDataList::serialize(Archive& archive, Phoenix::u32 version)
{
	archive
	(
		CEREAL_NVP(attackDatas)
	);
}

// シリアライズ
void Player::AttackDataList::Serialize(const Player::AttackDataList& data, const char* filename)
{
	// バイナリ
	{
		std::ofstream stream(filename, std::ios::binary);
		if (stream.is_open())
		{
			cereal::BinaryOutputArchive archive(stream);

			archive(CEREAL_NVP(data));
		}
	}
}

// デシリアライズ
bool Player::AttackDataList::Deserialize(Player::AttackDataList& data, const char* filename)
{
	std::ifstream stream(filename, std::ios::binary);
	if (stream.is_open())
	{
		cereal::BinaryInputArchive archive(stream);

		archive(CEREAL_NVP(data));
	}

	return true;
}