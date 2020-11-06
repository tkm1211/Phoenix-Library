#include "Player.h"
#include "Phoenix/FrameWork/Renderer/ModelRenderer.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"


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
		//model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Mixamo\\Sword_And_Shield_Attack\\Sword_And_Shield_Attack.fbx");
	}

	// アニメーション読み込み
	{
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardWalk\\Walking_With_Shopping_Bag.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\Running.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\SlowRunning.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Roll\\Sprinting_Forward_Roll.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\LeftPunch\\Left_Punch_Begin.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\LeftPunch\\Left_Punch_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\RightPunch\\Righ_Punch_Begin.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\RightPunch\\Righ_Punch_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\TurnKick\\Turn_Kick_Begin.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\TurnKick\\Turn_Kick_01.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\TurnKick\\Turn_Kick_End.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Damage\\Head_Hit.fbx", -1);
		
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Idle\\Ready_Idle.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Forward\\Walk_Forward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Back\\Walk_Backward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Right\\Walk_Right.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Left\\Walk_Left.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Forward\\Forward_Step.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Back\\Back_Step.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Right\\Right_Step.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Left\\Left_Step.fbx", -1);


		model->AddAnimationLayer(0);

		model->AddAnimationLayer(1);
		model->AddAnimationLayer(2);
		model->AddAnimationLayer(3);
		model->AddAnimationLayer(4);

		model->AddAnimationLayer(5);
		model->AddAnimationLayer(6);

		model->AddAnimationLayer(7);
		model->AddAnimationLayer(8);

		model->AddAnimationLayer(9);
		model->AddAnimationLayer(10);
		model->AddAnimationLayer(11);

		model->AddAnimationLayer(12);

		model->AddAnimationLayer(13);
		model->AddAnimationLayer(13, 56, 65); // 14

		model->AddAnimationLayer(18);
		model->AddAnimationLayer(19);
		model->AddAnimationLayer(20);
		model->AddAnimationLayer(21);


		model->AddBlendAnimationToLayer(14, 14, Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f));
		model->AddBlendAnimationToLayer(15, 14, Phoenix::Math::Vector3(0.0f, -1.0f, 0.0f));
		model->AddBlendAnimationToLayer(16, 14, Phoenix::Math::Vector3(1.0f, 0.0f, 0.0f));
		model->AddBlendAnimationToLayer(17, 14, Phoenix::Math::Vector3(-1.0f, 0.0f, 0.0f));

		model->AddBlendAnimationToLayer(static_cast<Phoenix::u32>(AnimationState::SlowRun), static_cast<Phoenix::u32>(AnimationState::Walk), Phoenix::Math::Vector3(1.0f, 0.0f, 0.0f));
	}

	// コリジョン初期化
	{
		collisionDatas.resize(4);

		collisionDatas.at(0).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(0).radius = 0.5f;
		collisionDatas.at(0).boneIndex = model->GetBoneIndex("Hips");

		collisionDatas.at(1).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(1).radius = 0.25f;
		collisionDatas.at(1).boneIndex = model->GetBoneIndex("RightHandIndex1");

		collisionDatas.at(2).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(2).radius = 0.25f;
		collisionDatas.at(2).boneIndex = model->GetBoneIndex("LeftHandIndex1");

		collisionDatas.at(3).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(3).radius = 0.25f;
		collisionDatas.at(3).boneIndex = model->GetBoneIndex("RightFoot");
	}

	// UI生成
	{
		ui = PlayerUI::Create();
	}

	// アタックデータ生成
	{
		auto SetAttackData = [&]
		(
			AttackAnimationState animState,
			Phoenix::s32 animIndex,

			Phoenix::f32 playSpeed,
			Phoenix::f32 playBeginTime,
			Phoenix::f32 playEndTime,

			Phoenix::s32 collisionNum,
			Phoenix::f32 collisionBeginTime,
			Phoenix::f32 collisionEndTime,

			bool receptionStack,
			Phoenix::f32 receptionBeginTime,
			Phoenix::f32 receptionEndTime
		)
		{
			AttackData data;

			data.animState = animState;
			data.animIndex = animIndex;

			data.playSpeed = playSpeed;
			data.playBeginTime = playBeginTime == -1.0f ? -1.0f : playBeginTime / 60.0f;
			data.playEndTime = playEndTime == -1.0f ? -1.0f : playEndTime / 60.0f;

			data.collisionNum = collisionNum;
			data.collisionBeginTime = collisionBeginTime == -1.0f ? -1.0f : collisionBeginTime / 60.0f;
			data.collisionEndTime = collisionEndTime == -1.0f ? -1.0f : collisionEndTime / 60.0f;

			data.receptionStack = receptionStack;
			data.receptionBeginTime = receptionBeginTime == -1.0f ? -1.0f : receptionBeginTime / 60.0f;
			data.receptionEndTime = receptionEndTime == -1.0f ? -1.0f : receptionEndTime / 60.0f;

			return data;
		};

		// 弱攻撃
		{
			// 右ストレート
			{
				AttackDatas datas;

				// 入力キー設定
				datas.SetKey(AttackKey::WeakAttack);

				datas.AddData(SetAttackData(AttackAnimationState::Attack01, 7, 2.0f, -1.0f, -1.0f, 1, 13.0f, 23.0f, true, 13.0f, 23.0f));
				datas.AddData(SetAttackData(AttackAnimationState::Attack01, 8, 1.0f, 24.0f, 47.0f, 0, -1.0f, -1.0f, false, 24.0f, 47.0f));

				attackDatasList.emplace_back(datas);
			}

			// 左ストレート
			{
				AttackDatas datas;

				// 入力キー設定
				datas.SetKey(AttackKey::WeakAttack);

				datas.AddData(SetAttackData(AttackAnimationState::Attack02, 5, 2.0f, -1.0f, -1.0f, 2, 10.0f, 20.0f, true, 15.0f, 20.0f));
				datas.AddData(SetAttackData(AttackAnimationState::Attack02, 6, 1.0f, 21.0f, 47.0f, 0, -1.0f, -1.0f, false, 21.0f, 47.0f));

				attackDatasList.emplace_back(datas);
			}
		}

		// 強攻撃
		{
			// 回転キック
			{
				AttackDatas datas;

				// 入力キー設定
				datas.SetKey(AttackKey::StrongAttack);

				datas.AddData(SetAttackData(AttackAnimationState::Attack03, 9, 1.25f, -1.0f, -1.0f, 0, -1.0f, -1.0f, false, -1.0f, -1.0f));
				datas.AddData(SetAttackData(AttackAnimationState::Attack03, 10, 2.0f, 25.0f, 46.0f, 3, 43.0f, 46.0f, false, -1.0f, -1.0f));
				datas.AddData(SetAttackData(AttackAnimationState::Attack03, 11, 1.0f, 47.0f, -1.0f, 0, -1.0f, -1.0f, false, -1.0f, -1.0f));

				attackDatasList.emplace_back(datas);
			}
		}
	}
}

void Player::Initialize()
{
	// アニメーションパラメーターの設定
	{
		animationState = AnimationState::Idle;
		attackState = AttackAnimationState::End;
		isChangeAnimation = false;
		isAttack = false;
		speed = 0.0f;
		animationSpeed = AnimationSpeed60;
		attackReceptionTimeCnt = 0.0f;
	}

	// 待機モーション開始
	{
		model->PlayAnimation(0, 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		//model->PauseAnimation(true);
	}

	// トランスフォームの初期化
	{
		worldMatrix = Phoenix::Math::MatrixIdentity();
		pos = { 0,0,12.0f }; // tutorial : 135.0f , main : 12.0f
		//rotate = { 0,180.0f * 0.01745f,0 };
		rotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), 180.0f * 0.01745f);
		scale = { 1,1,1 };
		radius = 0.5f;
	}

	// パラメーターの初期化
	{
		life = MaxLife;
		isHit = false;
		invincible = false;
		isAttackJudgment = false;
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
	}
}

void Player::Update(Phoenix::Graphics::Camera& camera, bool onControl)
{
	bool isAccumulationDamege = false;

	// 蓄積ダメージの確認
	{
		isAccumulationDamege = AccumulationDamege();
	}

	// コントローラー操作(位置更新)
	{
		if (onControl && !isAccumulationDamege) Control(camera);
	}

	// アニメーション変更
	{
		ChangeAnimation();
	}

	// アニメーション更新
	{
		model->UpdateTransform(1 / 60.0f);

		if (isBattleMode)
		{
			model->SetBlendRate(Phoenix::Math::Vector3(blendRate.x, -blendRate.y, 0.0f));
		}
		else
		{
			model->SetBlendRate(blendRate.z);
		}
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
	//Phoenix::Math::Vector3 rotate = this->rotate;
	Phoenix::Math::Quaternion rotate = this->rotate;
	Phoenix::Math::Vector3 translate = pos;

	Phoenix::Math::Matrix S, R, T;
	S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
	//R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
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

void Player::Control(Phoenix::Graphics::Camera& camera) // TODO : re -> player control
{
	if (animationState == AnimationState::Damage && model->IsPlaying()) return;

	Phoenix::f32 sX = 0.0f;
	Phoenix::f32 sY = 0.0f;

	sX = xInput[0].sX / 1000.0f;
	sY = xInput[0].sY / 1000.0f;

	sY = GetKeyState('W') < 0 ? -1.0f : sY;
	sY = GetKeyState('S') < 0 ? 1.0f : sY;
	sX = GetKeyState('A') < 0 ? -1.0f : sX;
	sX = GetKeyState('D') < 0 ? 1.0f : sX;

	// ブレンドレート計算
	{
		if (isBattleMode)
		{
			blendRate.x = sX;
			blendRate.y = sY;
		}

		blendRate.z = Phoenix::Math::Vector2Length(Phoenix::Math::Vector2(sX, sY));
		blendRate.z = 1.0f <= blendRate.z ? 1.0f : blendRate.z;
	}

	// プレイヤーの最終方向を決定する角度を計算
	auto UpdateRotateY = [&]()
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

		rotateY = camera.GetRotateY() + atan2f(sX, sY);
	};

	// プレイヤー回転
	auto RotatePlayer = [&](Phoenix::f32 angle)
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
			newRotate = rotate;
			newRotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angle);
		}
	};

	auto ChangeAnimation = [&](Phoenix::u32 index, Phoenix::u32 nextIndex)
	{
		if (attackDatasList.at(index).datas.size() - 1 <= attackComboState)
		{
			attackComboState = -1;
		}

		if (attackComboState == -1)
		{
			attackComboState = 0;
			attackReceptionTimeCnt = 0.0f;

			receptionStack = false;
			stackKey = AttackKey::None;

			ChangeAnimationState(AnimationState::Attack, 0.0f);
			ChangeAttackAnimationState(attackDatasList.at(nextIndex).datas.at(attackComboState).animState, attackDatasList.at(nextIndex).datas.at(attackComboState).animIndex, attackDatasList.at(nextIndex).datas.at(attackComboState).playSpeed);
		}
	};

	auto JudgeDedgeIndex = [&]()
	{
		if (sX != 0.0f || sY != 0.0f)
		{
			UpdateRotateY();
			RotatePlayer(rotateY);

			if (sY < 0.0f)
			{
				dedgeLayerIndex = 15;
			}
			if (sY > 0.0f)
			{
				dedgeLayerIndex = 16;
			}
			if (sX < 0.0f)
			{
				dedgeLayerIndex = 17;
			}
			if (sX > 0.0f)
			{
				dedgeLayerIndex = 18;
			}

			//Phoenix::Math::Vector3 dir = targetPos - GetPosition();
			//dir = Phoenix::Math::Vector3Normalize(dir);
			//dir.y = 0.0f;

			//Phoenix::f32 newRotateY = atan2f(-dir.x, -dir.z) + atan2f(sX, sY);

			//Phoenix::Math::Vector3 f = { sinf(newRotateY),  0.0f, cosf(newRotateY) };

			///*Phoenix::Math::Quaternion newQ = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), newRotateY);
			//Phoenix::Math::Matrix newM = Phoenix::Math::MatrixRotationQuaternion(&newQ);
			//Phoenix::Math::Vector3 newForward = Phoenix::Math::Vector3(newM._31, newM._32, newM._33);
			//newForward.y = 0.0f;*/

			//Phoenix::Math::Quaternion q = rotate/* * newQ*/;
			//Phoenix::Math::Matrix matrix = Phoenix::Math::MatrixRotationQuaternion(&q);
			//Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(matrix._31, matrix._32, matrix._33);

			//Phoenix::Math::Vector3 cameraForword = camera.GetFront();
			//cameraForword.y = 0.0f;

			//Phoenix::f32 angle = acosf(Phoenix::Math::Vector3Dot(dir, cameraForword)) / 0.01745f;
			////if (45.0f <= angle && angle <= 135.0f) f *= -1;

			//if (fabsf(f.x) < fabsf(f.z))
			//{
			//	if (f.z < 0.0f)
			//	{
			//		dedgeLayerIndex = 15;
			//		rotateY = atan2f(-dir.x, -dir.z) + atan2f(0.0f, -1.0f);
			//	}
			//	if (f.z > 0.0f)
			//	{
			//		dedgeLayerIndex = 16;
			//		rotateY = atan2f(-dir.x, -dir.z) + atan2f(0.0f, 1.0f);
			//	}
			//}
			//if (fabsf(f.z) < fabsf(f.x))
			//{
			//	if (f.x < 0.0f)
			//	{
			//		dedgeLayerIndex = 17;
			//		rotateY = atan2f(-dir.x, -dir.z) + atan2f(-1.0f, 0.0f);
			//	}
			//	if (f.x > 0.0f)
			//	{
			//		dedgeLayerIndex = 18;
			//		rotateY = atan2f(-dir.x, -dir.z) + atan2f(1.0f, 0.0f);
			//	}
			//}

			//rotateY = newRotateY;

			/*Phoenix::Math::Quaternion q = rotate * newRotateY;
			Phoenix::Math::Matrix matrix = Phoenix::Math::MatrixRotationQuaternion(&q);
			Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(matrix._31, matrix._32, matrix._33);
			Phoenix::Math::Vector3 up = Phoenix::Math::Vector3(matrix._21, matrix._22, matrix._23);
			Phoenix::Math::Vector3 right = Phoenix::Math::Vector3(matrix._11, matrix._12, matrix._13);

			forward.y = 0.0f;

			Phoenix::f32 angleSX, angleSY;
			angleSX = acosf(Phoenix::Math::Vector3Dot(dir, forward));
			angleSY = acosf(Phoenix::Math::Vector3Dot(dir, right));

			if (1e-8f < fabs(angleSX))
			{
				Phoenix::f32 angleR;
				{
					angleR = acosf(Phoenix::Math::Vector3Dot(dir, right));
					angleR -= (90.0f * 0.01745f);
					if (0.0f < angleR) angleSX *= -1;
				}
				{
					angleR = acosf(Phoenix::Math::Vector3Dot(dir, forward));
					angleR -= (90.0f * 0.01745f);
					if (0.0f < angleR) angleSY *= -1;
				}

				if (fabsf(forward.z) < fabsf(forward.x))
				{
					if (angleSX < 0.0f)
					{
						dedgeLayerIndex = 17;
						rotateY = atan2f(dir.x, dir.z);
						rotateY += 270.0f * 0.01745f;

					}
					if (angleSX > 0.0f)
					{
						dedgeLayerIndex = 18;
						rotateY = atan2f(dir.x, dir.z);
						rotateY += 90.0f * 0.01745f;
					}
				}
				else if (fabsf(forward.x) < fabsf(forward.z))
				{
					if (angleSY < 0.0f)
					{
						dedgeLayerIndex = 15;
						rotateY = atan2f(dir.x, dir.z);
						rotateY += 0.0f * 0.01745f;
					}
					if (angleSY > 0.0f)
					{
						dedgeLayerIndex = 16;
						rotateY = atan2f(dir.x, dir.z);
						rotateY += 180.0f * 0.01745f;
					}
				}
			}*/
		}
	};

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
	if ((key != AttackKey::None) && isBattleMode && ((animationState == AnimationState::Attack) || (animationState == AnimationState::Idle) || (animationState == AnimationState::Walk) || (animationState == AnimationState::Run)))
	{
		if (attackState == AttackAnimationState::End)
		{
			// 弱攻撃からスタートするため
			if (attackDatasList[0].receptionKey == key)
			{
				attackComboState = 0;
				attackReceptionTimeCnt = 0.0f;

				ChangeAnimationState(AnimationState::Attack, 0.0f);
				ChangeAttackAnimationState(attackDatasList.at(0).datas.at(0).animState, attackDatasList.at(0).datas.at(0).animIndex, attackDatasList.at(0).datas.at(0).playSpeed);
			}
		}
		else
		{
			Phoenix::u32 index = static_cast<Phoenix::u32>(attackState);
			Phoenix::u32 nextIndex = index + 1;

			// 次の攻撃が発動するボタンの受付
			if (receptionStack)
			{
				if (nextIndex < attackDatasList.size())
				{
					if (!attackDatasList.at(index).datas.at(attackComboState).receptionStack)
					{
						if (attackDatasList.at(nextIndex).receptionKey == stackKey)
						{
							ChangeAnimation(index, nextIndex);
						}
					}
				}
			}
			else if (attackDatasList.at(index).datas.at(attackComboState).receptionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= attackDatasList.at(index).datas.at(attackComboState).receptionEndTime)
			{
				if (nextIndex < attackDatasList.size())
				{
					if (attackDatasList.at(nextIndex).receptionKey == key)
					{
						if (attackDatasList.at(index).datas.at(attackComboState).receptionStack)
						{
							receptionStack = true;
							stackKey = key;
						}
						else
						{
							ChangeAnimation(index, nextIndex);
						}
					}
				}
			}
		}
	}
	else if ((animationState == AnimationState::Attack) && !model->IsPlaying())
	{
		Phoenix::u32 index = static_cast<Phoenix::u32>(attackState);

		if (0 <= attackComboState && attackComboState < attackDatasList.at(index).datas.size() - 1)
		{
			++attackComboState;

			ChangeAnimationState(AnimationState::Attack, 0.0f);
			ChangeAttackAnimationState(attackDatasList.at(index).datas.at(attackComboState).animState, attackDatasList.at(index).datas.at(attackComboState).animIndex, attackDatasList.at(index).datas.at(attackComboState).playSpeed);
		}
		else
		{
			ChangeAnimationState(AnimationState::Idle, 0.0f);
			ChangeAttackAnimationState(AttackAnimationState::End, -1, 0.0f);

			isAttack = false;
			attackReceptionTimeCnt = 0.0f;
			attackComboState = -1;

			receptionStack = false;
			stackKey = AttackKey::None;
		}
	}

	// 攻撃ステート以外
	if (!isAttack)
	{
		// 回避ステート
		if (isBattleMode)
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
				speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.025f);
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
				UpdateRotateY();
				RotatePlayer(rotateY);

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
					if ((xInput[0].bRBs || GetKeyState(VK_SHIFT) < 0) && animationState != AnimationState::Run)
					{
						ChangeAnimationState(AnimationState::Run, RunSpeed);
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
		if (isBattleMode)
		{
			// 回避ステートへ
			if ((xInput[0].bAt || GetAsyncKeyState(VK_SPACE) & 1) && animationState != AnimationState::Dedge)
			{
				Phoenix::u32 index = static_cast<Phoenix::u32>(attackState);

				// 次の攻撃が発動するボタンの受付
				if (attackDatasList.at(index).datas.at(attackComboState).receptionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= attackDatasList.at(index).datas.at(attackComboState).receptionEndTime)
				{
					ChangeAnimationState(AnimationState::Dedge, DedgeSpeed);
					ChangeAttackAnimationState(AttackAnimationState::End, -1, 0.0f);

					isAttack = false;
					attackReceptionTimeCnt = 0.0f;

					JudgeDedgeIndex();
				}
			}
		}

		// アタックアニメーションスピード計測
		attackReceptionTimeCnt += animationSpeed / 60.0f;
	}

	// 座標更新
	if (!isChangeAnimation)
	{
		rotate = Phoenix::Math::QuaternionSlerp(rotate, newRotate, 0.17f);
		{
			if (animationState == AnimationState::Walk/* && !isBattleMode*/)
			{
				if (isBattleMode)
				{
					pos.x += sinf(rotateY) * (speed + (BattleSlowRunSpeed * blendRate.z));
					pos.z += cosf(rotateY) * (speed + (BattleSlowRunSpeed * blendRate.z));
				}
				else
				{
					pos.x += sinf(rotateY) * (speed + (SlowRunSpeed * blendRate.z));
					pos.z += cosf(rotateY) * (speed + (SlowRunSpeed * blendRate.z));
				}
			}
			else
			{
				pos.x += sinf(rotateY) * speed;
				pos.z += cosf(rotateY) * speed;
			}
		}
	}
}

void Player::ChangeAnimation()
{
	if (!isChangeAnimation) return;

	Phoenix::u32 animationNum = static_cast<Phoenix::u32>(animationState);
	switch (animationState)
	{
	case AnimationState::Idle:
		if (isBattleMode)
		{
			model->PlayAnimation(13, 1, 0.2f);
			model->UpdateTransform(1 / 60.0f);
			model->SetLoopAnimation(true);
		}
		else
		{
			model->PlayAnimation(animationNum, 1, 0.1f);
			model->UpdateTransform(1 / 60.0f);
			model->SetLoopAnimation(true);
		}
		break;

	case AnimationState::Walk:
		if (isBattleMode)
		{
			model->PlayAnimation(13, 1, 0.2f);
			model->PlayBlendAnimation(14, 1, 0.2f);
			model->UpdateTransform(1 / 60.0f);
			model->SetLoopAnimation(true);
			model->SetBlendLoopAnimation(true);
		}
		else
		{
			model->PlayAnimation(animationNum, 1, 0.2f);
			model->UpdateTransform(1 / 60.0f);
			model->SetLoopAnimation(true);
		}
		break;

	case AnimationState::Run:
		model->PlayAnimation(animationNum, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		break;

	case AnimationState::Roll:
		model->PlayAnimation(animationNum, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		model->SetSpeed(1.5f);
		break;

	case AnimationState::Attack:
		ChangeAttackAnimation(animationNum);
		break;

	case AnimationState::Damage:
		model->PlayAnimation(animationNum, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		break;

	case AnimationState::Dedge:
		model->PlayAnimation(dedgeLayerIndex, 0, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		model->SetSpeed(1.85f);

		/*if (dedgeLayerIndex == 15)
		{
			model->SetSpeed(1.85f);
		}
		else if(dedgeLayerIndex == 16)
		{
			model->SetSpeed(1.75f);
		}
		else
		{
			model->SetSpeed(1.5f);
		}*/
		break;
	
	default: break;
	}

	isChangeAnimation = false;
}

void Player::ChangeAttackAnimation(Phoenix::u32 animationNum)
{
	Phoenix::u32 index = static_cast<Phoenix::u32>(attackState);
	Phoenix::u32 animIndex = static_cast<Phoenix::u32>(attackDatasList.at(index).datas.at(attackComboState).animIndex);
	Phoenix::f32 animationSpeed = attackDatasList.at(index).datas.at(attackComboState).playSpeed;
	Phoenix::f32 beginTime = attackDatasList.at(index).datas.at(attackComboState).playBeginTime;
	Phoenix::f32 endTime = attackDatasList.at(index).datas.at(attackComboState).playEndTime;

	model->PlayAnimation(animIndex, 0, 0.2f);
	model->UpdateTransform(1 / 60.0f);
	model->SetLoopAnimation(false);
	model->SetSpeed(animationSpeed);

	if (beginTime != -1.0f)
	{
		model->SetBeginTime(beginTime);
		model->SetCurrentTime(beginTime);
	}
	if (endTime != -1.0f)
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
			if (isHit)
			{
				isAttackJudgment = false;
				return;
			}

			isAttackJudgment = true;
			attackCollisionIndex = index;
		};
		auto NoJudgment = [&]()
		{
			isAttackJudgment = false;
			isHit = false;
			attackCollisionIndex = -1;
		};

		Phoenix::u32 index = static_cast<Phoenix::u32>(attackState);

		// 当たり判定
		if (attackDatasList.at(index).datas.at(attackComboState).collisionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= attackDatasList.at(index).datas.at(attackComboState).collisionEndTime)
		{
			Judgment(attackDatasList.at(index).datas.at(attackComboState).collisionNum);
		}
		else
		{
			NoJudgment();
		}
	}
	else
	{
		isAttackJudgment = false;
	}
}

void Player::Damage(int damage)
{
	life -= damage;
	accumulationDamege += damage;
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
				attackState = AttackAnimationState::End;
			}
		}
		else
		{
			//speed += KnockBackDownSpeed;
		}
	}

	if (accumulationDamege == 0) return false;

	if (AccumulationMaxDamege <= accumulationDamege)
	{
		isChangeAnimation = true;
		isAttack = false;
		speed = 0.0f;
		animationState = AnimationState::Damage;

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
			ImGui::DragFloat("WalkSpeed", &WalkSpeed, 0.1f);
			ImGui::DragFloat("RunSpeed", &RunSpeed, 0.1f);
			ImGui::DragFloat("RollSpeed", &RollSpeed, 0.1f);
			ImGui::DragFloat("KnockBackSpeed", &KnockBackSpeed, 0.1f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Bone"))
		{
			//ImGui::ListBox("BoneListBox\n(single select)", &boneIndex, model->GetBoneNode()->at(0).name.data(), model->GetBoneNode()->at(0).name.size(), 4);
			//ImGui::ListBox("NodeListBox\n(single select)", &nodeIndex, model->GetNodes()->data(), model->GetNodes()->size(), 4);
			//ImGui::ListBox("ListBox\n(single select)", &boneIndex, model->GetBoneNames().data(), model->GetBoneNames().size(), 4);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Animation"))
		{
			ImGui::InputInt("AnimClip", &animClip);
			ImGui::Text("Len : %f", model->GetLength());
			ImGui::Text("RunLen : %f", attackReceptionTimeCnt);
			if (ImGui::Button("Play"))
			{
				model->PlayAnimation(0, animClip);
			}
			if (ImGui::Button("LoopPlay"))
			{
				model->SetLoopAnimation(true);
			}
			ImGui::TreePop();

			if (ImGui::TreeNode("Blend"))
			{
				ImGui::DragFloat("Rate", &blendRate.x, 0.01f, 0.0f, 1.0f);
				if (ImGui::Button("PlayBlendAnim"))
				{
					isChangeAnimation = true;
					//speed = WalkSpeed;
					animationState = AnimationState::Walk;
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}