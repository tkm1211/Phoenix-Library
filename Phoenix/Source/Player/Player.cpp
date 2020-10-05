#include "Player.h"
#include "Phoenix/FrameWork/Renderer/ModelRenderer.h"
#include "Phoenix/FrameWork/Shader/BasicShader.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"
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
#if 0
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardWalk\\Walking_With_Shopping_Bag.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\Running.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\SlowRunning.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Roll\\Sprinting_Forward_Roll.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack01\\Elbow_Uppercut_Combo.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack02\\Uppercut_Jab.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack03\\Strike_Foward_Jog.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Damage\\Head_Hit.fbx", -1); // Head_Hit Head_Hit02 Damage
#else
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardWalk\\Walking_With_Shopping_Bag.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\Running.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\SlowRunning.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Roll\\Sprinting_Forward_Roll.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack01\\LeftJab.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack02\\RightHook.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack03\\LeftHook.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack04\\RightHook2.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack05\\Uppercut.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack06\\RoundhouseKick.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Damage\\Head_Hit.fbx", -1); // Head_Hit Head_Hit02 Damage
#endif
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
		AttackData data;

		auto SetAttackData = [&]
		(
			AnimationState animationState,
			AttackAnimationState attackState,
			AnimationState nextAnimationState,
			AttackAnimationState nextAttackState,
			Phoenix::f32 playBeginTime,
			Phoenix::f32 playEndTime,
			Phoenix::f32 collisionBeginTime,
			Phoenix::f32 collisionEndTime,
			Phoenix::u32 collisionNum,
			Phoenix::f32 receptionBeginTime,
			Phoenix::f32 receptionEndTime,
			Phoenix::f32 nextMoveSpeed,
			Phoenix::f32 animationSpeed
		)
		{
			data.animationState = animationState;
			data.attackState = attackState;

			data.nextAnimationState = nextAnimationState;
			data.nextAttackState = nextAttackState;

			data.playBeginTime = playBeginTime == -1.0f ? -1.0f : playBeginTime / 60.0f;
			data.playEndTime = playEndTime == -1.0f ? -1.0f : playEndTime / 60.0f;

			data.collisionBeginTime = collisionBeginTime == -1.0f ? -1.0f : collisionBeginTime / 60.0f;
			data.collisionEndTime = collisionEndTime == -1.0f ? -1.0f : collisionEndTime / 60.0f;

			data.collisionNum = collisionNum;

			data.receptionBeginTime = receptionBeginTime == -1.0f ? -1.0f : receptionBeginTime / 60.0f;
			data.receptionEndTime = receptionEndTime == -1.0f ? -1.0f : receptionEndTime / 60.0f;

			data.nextMoveSpeed = nextMoveSpeed;
			data.animationSpeed = animationSpeed;
		};

		Phoenix::u32 endNum = static_cast<Phoenix::u32>(AttackAnimationState::End);
		for (Phoenix::u32 i = 0; i < endNum + 1; ++i)
		{
			switch (static_cast<AttackAnimationState>(i))
			{
			case AttackAnimationState::Attack01:
				SetAttackData(AnimationState::Attack01, AttackAnimationState::Attack01, AnimationState::Attack02, AttackAnimationState::Attack02, -1.0f, -1.0f, 18.0f, 30.0f, 2, 23.0f, 80.0f, 0.0f, Attack01AnimationSpeed);
				break;

			case AttackAnimationState::Attack02:
				SetAttackData(AnimationState::Attack02, AttackAnimationState::Attack02, AnimationState::Attack03, AttackAnimationState::Attack03, -1.0f, -1.0f, 60.0f, 85.0f, 1, 70.0f, 130.0f, 0.0f, Attack02AnimationSpeed);
				break;

			case AttackAnimationState::Attack03:
				SetAttackData(AnimationState::Attack03, AttackAnimationState::Attack03, AnimationState::Attack04, AttackAnimationState::Attack04, -1.0f, -1.0f, 30.0f, 40.0f, 2, 40.0f, 75.0f, 0.0f, Attack03AnimationSpeed);
				break;

			case AttackAnimationState::Attack04:
				SetAttackData(AnimationState::Attack04, AttackAnimationState::Attack04, AnimationState::Attack05, AttackAnimationState::Attack05, -1.0f, -1.0f, 17.0f, 27.0f, 1, 27.0f, 66.0f, 0.0f, Attack04AnimationSpeed);
				break;

			case AttackAnimationState::Attack05:
				SetAttackData(AnimationState::Attack05, AttackAnimationState::Attack05, AnimationState::Attack06, AttackAnimationState::Attack06, -1.0f, -1.0f, 24.0f, 35.0f, 2, 35.0f, 88.0f, 0.0f, Attack05AnimationSpeed);
				break;

			case AttackAnimationState::Attack06:
				SetAttackData(AnimationState::Attack06, AttackAnimationState::Attack06, AnimationState::Idle, AttackAnimationState::End, -1.0f, 111.0f, 51.0f, 70.0f, 3, 0.0f, 0.0f, 0.0f, Attack06AnimationSpeed);
				break;

			case AttackAnimationState::End:
				SetAttackData(AnimationState::Idle, AttackAnimationState::End, AnimationState::Attack01, AttackAnimationState::Attack01, -1.0f, -1.0f, -1.0f, -1.0f, 0, 0.0f, 0.0f, 0.0f, 1.0f);
				break;
			default: break;
			}

			attackDatas.emplace_back(data);
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
		blendRate = 0.0f;
		newRotate = rotate;
		rotateY = 180.0f * 0.01745f;
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
		model->SetBlendRate(blendRate);
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
		blendRate = Phoenix::Math::Vector2Length(Phoenix::Math::Vector2(sX, sY));
		blendRate = 1.0f <= blendRate ? 1.0f : blendRate;
	}

#if 0
	if (animationState == AnimationState::Damage)
	{
		if (isChangeAnimation)
		{
			sX = 0.0f;
			sY = -1.0f;

			if (sX != 0.0f || sY != 0.0f)
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

				Phoenix::Math::Vector3 oldAngle = rotate;
				oldAngle.y = camera.GetRotateY() + atan2f(sX, sY);
				rotate = oldAngle;
			}
		}

		/*pos.x += sinf(rotate.y) * speed;
		pos.z += cosf(rotate.y) * speed;

		speed += KnockBackDownSpeed;*/
	}
	else if (xInput[0].bXt && animationState != AnimationState::Roll)
	{
		if (!isAttack && ((animationState != AnimationState::Attack01) && (animationState != AnimationState::Attack02) && (animationState != AnimationState::Attack03)) && attackState == AttackAnimationState::End)
		{
			attackReceptionTimeCnt = 0;
			animationSpeed = AnimationSpeed30;
			isChangeAnimation = true;
			isAttack = true;
			isHit = false;
			animationState = AnimationState::Attack01;
			attackState = AttackAnimationState::Attack01;

			if (sX != 0.0f || sY != 0.0f)
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

				Phoenix::Math::Vector3 oldAngle = rotate;
				oldAngle.y = camera.GetRotateY() + atan2f(sX, sY);
				rotate = oldAngle;
			}
		}
		else if (isAttack && ((animationState != AnimationState::Attack02) && (animationState != AnimationState::Attack03)) && attackState == AttackAnimationState::Attack01)
		{
			//float len = model->GetLength(); // test
			if (Attack01ReceptionStartTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= model->GetLength())
			{
				attackReceptionTimeCnt = 0;
				animationSpeed = AnimationSpeed30;
				isChangeAnimation = true;
				isAttack = true;
				isHit = false;
				animationState = AnimationState::Attack02;
				attackState = AttackAnimationState::Attack02;

				if (sX != 0.0f || sY != 0.0f)
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

					Phoenix::Math::Vector3 oldAngle = rotate;
					oldAngle.y = camera.GetRotateY() + atan2f(sX, sY);
					rotate = oldAngle;
				}
			}
		}
		else if (isAttack && ((animationState != AnimationState::Attack01) && (animationState != AnimationState::Attack03)) && attackState == AttackAnimationState::Attack02)
		{
			//float len = model->GetLength(); // test
			if (Attack02ReceptionStartTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= model->GetLength())
			{
				attackReceptionTimeCnt = 0;
				animationSpeed = AnimationSpeed45;
				speed = Attack03Speed;
				isChangeAnimation = true;
				isAttack = true;
				isHit = false;
				animationState = AnimationState::Attack03;
				attackState = AttackAnimationState::Attack03;

				if (sX != 0.0f || sY != 0.0f)
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

					Phoenix::Math::Vector3 oldAngle = rotate;
					oldAngle.y = camera.GetRotateY() + atan2f(sX, sY);
					rotate = oldAngle;
				}
			}
		}
	}
	else if (isAttack)
	{
		if (((animationState == AnimationState::Attack01) || (animationState == AnimationState::Attack02) || (animationState == AnimationState::Attack03)) && !model->IsPlaying())
		{
			attackReceptionTimeCnt = 0;
			isChangeAnimation = true;
			isAttack = false;
			animationState = AnimationState::Idle;
			attackState = AttackAnimationState::End;
		}
		if (xInput[0].bAt
			&& ((1.5f <= attackReceptionTimeCnt && attackState == AttackAnimationState::Attack01)
			|| (3.0f <= attackReceptionTimeCnt && attackState == AttackAnimationState::Attack02)))
		{
			speed = RollSpeed;
			attackReceptionTimeCnt = 0;
			animationSpeed = 0;
			isChangeAnimation = true;
			isAttack = false;
			animationState = AnimationState::Roll;
			attackState = AttackAnimationState::End;

			if (sX != 0.0f || sY != 0.0f)
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

				Phoenix::Math::Vector3 oldAngle = rotate;
				oldAngle.y = camera.GetRotateY() + atan2f(sX, sY);
				rotate = oldAngle;
			}
		}
		else if (animationState == AnimationState::Attack03 && attackState == AttackAnimationState::Attack03 && model->IsPlaying())
		{
			pos.x += sinf(rotate.y) * speed;
			pos.z += cosf(rotate.y) * speed;
		}

		attackReceptionTimeCnt += animationSpeed;
	}
	else if (((animationState != AnimationState::Attack01) && (animationState != AnimationState::Attack02) && (animationState != AnimationState::Attack03)))
	{
		if (xInput[0].bAt && animationState != AnimationState::Roll)
		{
			attackReceptionTimeCnt = 0;
			isChangeAnimation = true;
			invincible = false;
			speed = RollSpeed;
			animationState = AnimationState::Roll;
		}
		else if (animationState == AnimationState::Roll && model->IsPlaying())
		{
			if (0.34f <= attackReceptionTimeCnt && attackReceptionTimeCnt <= 0.92f)
			{
				invincible = true;
			}
			else
			{
				invincible = false;
			}
			
			pos.x += sinf(rotate.y) * speed;
			pos.z += cosf(rotate.y) * speed;

			attackReceptionTimeCnt += animationSpeed;
		}
		else if (animationState == AnimationState::Roll && !model->IsPlaying())
		{
			attackReceptionTimeCnt = 0;
			isChangeAnimation = true;
			invincible = false;
			speed = 0.0f;
			animationState = AnimationState::Idle;
		}
	}

	if (animationState != AnimationState::Roll && ((animationState != AnimationState::Attack01) && (animationState != AnimationState::Attack02) && (animationState != AnimationState::Attack03)) && animationState != AnimationState::Damage)
	{
		if (sX != 0.0f || sY != 0.0f)
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

			len = sqrtf(sX * sX + sY * sY);

			Phoenix::Math::Vector3 oldAngle = rotate;
			oldAngle.y = camera.GetRotateY() + atan2f(sX, sY);
			rotate = oldAngle;

			if (animationState == AnimationState::Walk)
			{
				pos.x += sinf(oldAngle.y) * (speed + (SlowRunSpeed * blendRate));
				pos.z += cosf(oldAngle.y) * (speed + (SlowRunSpeed * blendRate));
			}
			else
			{
				pos.x += sinf(oldAngle.y) * speed;
				pos.z += cosf(oldAngle.y) * speed;
			}

			if (!xInput[0].bRBs && animationState != AnimationState::Walk)
			{
				isChangeAnimation = true;
				speed = WalkSpeed;
				animationState = AnimationState::Walk;
			}
			if ((xInput[0].bRBs || GetKeyState(VK_SHIFT) < 0) && animationState != AnimationState::Run)
			{
				isChangeAnimation = true;
				speed = RunSpeed;
				animationState = AnimationState::Run;
			}
		}
		else
		{
			if (animationState != AnimationState::Idle)
			{
				isChangeAnimation = true;
				speed = 0.0f;
				animationState = AnimationState::Idle;
			}
		}
	}
#else

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
#if 0
		Phoenix::Math::Vector3 oldAngle = rotate;
		oldAngle.y = camera.GetRotateY() + atan2f(sX, sY);
		rotate = oldAngle;
#elif 0
		newRotate = rotate;
		newRotate.y = camera.GetRotateY() + atan2f(sX, sY);
#else
		newRotate = rotate;
		newRotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angle);
#endif
	};

	// アニメーション変更
	auto ChangeAnimationState = [&](AnimationState state, Phoenix::f32 moveSpeed = 0.0f)
	{
		isChangeAnimation = true;
		animationState = state;
		
		speed = moveSpeed;
	};

	// 攻撃アニメーション変更
	auto ChangeAttackAnimationState = [&](AttackAnimationState state, Phoenix::f32 speed)
	{
		isAttack = true;

		attackState = state;
		animationSpeed = speed;
	};

	// 攻撃ステートへ
	if ((xInput[0].bXt || GetAsyncKeyState(VK_RETURN) & 1) && (isAttack || (animationState == AnimationState::Idle) || (animationState == AnimationState::Walk) || (animationState == AnimationState::Run)))
	{
		Phoenix::u32 index = static_cast<Phoenix::u32>(attackState);

		// 次の攻撃が発動するボタンの受付
		if (attackDatas[index].receptionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= attackDatas[index].receptionEndTime)
		{
			AnimationState oldState = animationState;
			Phoenix::u32 nextIndex = index + static_cast<Phoenix::u32>(1);
			Phoenix::f32 nextAnimaitonSpeed = attackDatas.size() != nextIndex ? attackDatas[nextIndex].animationSpeed : attackDatas[0].animationSpeed;

			ChangeAnimationState(attackDatas[index].nextAnimationState, attackDatas[index].nextMoveSpeed);
			ChangeAttackAnimationState(attackDatas[index].nextAttackState, nextAnimaitonSpeed);

			if (oldState != animationState)
			{
				attackReceptionTimeCnt = 0.0f;
			}

			if (sX != 0.0f || sY != 0.0f)
			{
				UpdateRotateY();
				RotatePlayer(rotateY);
			}

			if (attackState == AttackAnimationState::End)
			{
				isAttack = false;
			}
			else if (attackState == AttackAnimationState::Attack01)
			{
				pos.x += sinf(rotateY) * Attack01MoveSpeed;
				pos.z += cosf(rotateY) * Attack01MoveSpeed;
			}
		}
	}
	// 待機ステートへ
	else if (isAttack && !model->IsPlaying())
	{
		ChangeAnimationState(AnimationState::Idle, 0.0f);
		ChangeAttackAnimationState(AttackAnimationState::End, 0.0f);
		
		isAttack = false;
		attackReceptionTimeCnt = 0.0f;
	}

	// 攻撃ステート以外
	if (!isAttack)
	{
		// 回避ステートへ
		if ((xInput[0].bAt || GetAsyncKeyState(VK_SPACE) & 1) && animationState != AnimationState::Roll)
		{
			ChangeAnimationState(AnimationState::Roll, RollSpeed);

			if (sX != 0.0f || sY != 0.0f)
			{
				UpdateRotateY();
				RotatePlayer(rotateY);
			}
		}
		// 回避ステート中
		else if (animationState == AnimationState::Roll)
		{
			// 待機ステートへ
			if (!model->IsPlaying())
			{
				ChangeAnimationState(AnimationState::Idle, 0.0f);
			}
		}

		// 回避ステート以外
		if (animationState != AnimationState::Roll)
		{
			// 移動ステート
			if (sX != 0.0f || sY != 0.0f)
			{
				UpdateRotateY();
				RotatePlayer(rotateY);

				if ((!xInput[0].bRBs && !(GetKeyState(VK_SHIFT) < 0)) && animationState != AnimationState::Walk)
				{
					ChangeAnimationState(AnimationState::Walk, WalkSpeed);
				}
				if ((xInput[0].bRBs || GetKeyState(VK_SHIFT) < 0) && animationState != AnimationState::Run)
				{
					ChangeAnimationState(AnimationState::Run, RunSpeed);
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
		if ((xInput[0].bAt || GetAsyncKeyState(VK_SPACE) & 1) && animationState != AnimationState::Roll)
		{
			Phoenix::u32 index = static_cast<Phoenix::u32>(attackState);

			// 次の攻撃が発動するボタンの受付
			if (attackDatas[index].receptionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= attackDatas[index].receptionEndTime)
			{
				ChangeAnimationState(AnimationState::Roll, RollSpeed);
				ChangeAttackAnimationState(AttackAnimationState::End, 0.0f);

				isAttack = false;
				attackReceptionTimeCnt = 0.0f;

				if (sX != 0.0f || sY != 0.0f)
				{
					UpdateRotateY();
					RotatePlayer(rotateY);
				}
			}
		}

		// アタックアニメーションスピード計測
		attackReceptionTimeCnt += animationSpeed / 60.0f;
	}

	// 座標更新
	if (!isChangeAnimation)
	{
		//rotate = Phoenix::Math::Vector3Lerp(rotate, newRotate, 0.05f);
		rotate = Phoenix::Math::QuaternionSlerp(rotate, newRotate, 0.17f);

		/*Phoenix::Math::Matrix rotateMatirx = Phoenix::Math::MatrixRotationQuaternion(&rotate);
		Phoenix::f32 rotateY = asinf(rotateMatirx._31);*/

		if (animationState == AnimationState::Walk)
		{
			pos.x += sinf(rotateY) * (speed + (SlowRunSpeed * blendRate));
			pos.z += cosf(rotateY) * (speed + (SlowRunSpeed * blendRate));
		}
		else
		{
			pos.x += sinf(rotateY) * speed;
			pos.z += cosf(rotateY) * speed;
		}
	}
#endif
}

void Player::ChangeAnimation()
{
	if (!isChangeAnimation) return;

	Phoenix::u32 animationNum = static_cast<Phoenix::u32>(animationState);
	switch (animationState)
	{
	case AnimationState::Idle:
		model->PlayAnimation(animationNum, 1, 0.1f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		break;

	case AnimationState::Walk:
		model->PlayAnimation(animationNum, 1, 0.2f);
		model->PlayBlendAnimation(static_cast<Phoenix::u32>(AnimationState::SlowRun), 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		model->SetBlendLoopAnimation(true);
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

	case AnimationState::Attack01:
	case AnimationState::Attack02:
	case AnimationState::Attack03:
	case AnimationState::Attack04:
	case AnimationState::Attack05:
	case AnimationState::Attack06:
		ChangeAttackAnimation(animationNum);
		break;

	case AnimationState::Damage:
		model->PlayAnimation(animationNum, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		break;
	
	default: break;
	}

	isChangeAnimation = false;
}

void Player::ChangeAttackAnimation(Phoenix::u32 animationNum)
{
#if 0
	switch (attackState)
	{
	case AttackAnimationState::Attack01:
		model->PlayAnimation(animationNum, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		model->SetSpeed(2.0f);
		break;

	case AttackAnimationState::Attack02:
		model->PlayAnimation(animationNum, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		model->SetSpeed(2.0f);
		break;

	case AttackAnimationState::Attack03:
		model->PlayAnimation(animationNum, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		model->SetSpeed(1.5f);
		break;

	case AttackAnimationState::End: break;
	default: break;
	}
#else
	Phoenix::u32 index = static_cast<Phoenix::u32>(attackState);
	Phoenix::f32 beginTime = attackDatas[index].playBeginTime;
	Phoenix::f32 endTime = attackDatas[index].playEndTime;
	Phoenix::f32 animationSpeed = attackDatas[index].animationSpeed;

	model->PlayAnimation(animationNum, 1, 0.2f);
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
#endif
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

#if 0
		float time = attackReceptionTimeCnt * 60.0f;
		if (attackState == AttackAnimationState::Attack01)
		{
			if (36.0f <= time && time <= 46.0f)
			{
				Judgment(2);
			}
			else if (68.0f <= time && time <= 80.0f)
			{
				Judgment(1);
			}
			else
			{
				NoJudgment();
			}
		}
		else if (attackState == AttackAnimationState::Attack02)
		{
			if (40.0f <= time && time <= 55.0f)
			{
				Judgment(2);
			}
			else if (75.0f <= time && time <= 85.0f)
			{
				Judgment(1);
			}
			else if (96.0f <= time && time <= 107.0f)
			{
				Judgment(2);
			}
			else if (113.0f <= time && time <= 120.0f)
			{
				Judgment(1);
			}
			else if (129.0f <= time && time <= 139.0f)
			{
				Judgment(2);
			}
			else if (146.0f <= time && time <= 156.0f)
			{
				Judgment(1);
			}
			else
			{
				NoJudgment();
			}
		}
		else if (attackState == AttackAnimationState::Attack03)
		{
			if (22.0f <= time && time <= 42.0f)
			{
				Judgment(3);
			}
			else
			{
				NoJudgment();
			}
		}
#elif 0
		float time = attackReceptionTimeCnt * 60.0f;
		if (animationState == AnimationState::Attack01)
		{
			if (36.0f <= time && time <= 46.0f)
			{
				Judgment(2);
			}
			else if (68.0f <= time && time <= 80.0f)
			{
				Judgment(1);
			}
			else
			{
				NoJudgment();
			}
		}
		else if (animationState == AnimationState::Attack02)
		{
			if (40.0f <= time && time <= 55.0f)
			{
				Judgment(2);
			}
			else if (75.0f <= time && time <= 85.0f)
			{
				Judgment(1);
			}
			else if (96.0f <= time && time <= 107.0f)
			{
				Judgment(2);
			}
			else if (113.0f <= time && time <= 120.0f)
			{
				Judgment(1);
			}
			else if (129.0f <= time && time <= 139.0f)
			{
				Judgment(2);
			}
			else if (146.0f <= time && time <= 156.0f)
			{
				Judgment(1);
			}
			else
			{
				NoJudgment();
			}
		}
		else if (animationState == AnimationState::Attack03)
		{
			if (22.0f <= time && time <= 42.0f)
			{
				Judgment(3);
			}
			else
			{
				NoJudgment();
			}
		}
#else
		//if (animationState == AnimationState::)
		Phoenix::u32 index = static_cast<Phoenix::u32>(attackState);

		// 当たり判定
		if (attackDatas[index].collisionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= attackDatas[index].collisionEndTime)
		{
			Judgment(attackDatas[index].collisionNum);
		}
		else
		{
			NoJudgment();
		}
#endif
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
				ImGui::DragFloat("Rate", &blendRate, 0.01f, 0.0f, 1.0f);
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