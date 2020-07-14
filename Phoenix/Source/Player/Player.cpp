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

void Player::Init(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	//const char* filename = "..\\Data\\Assets\\Model\\Player\\MDL_Player_Attack.fbx";
	//const char* filename = "..\\Data\\Assets\\Model\\danbo_fbx\\danbo_atk.fbx";
	//const char* filename = "..\\Data\\Assets\\Model\\Hip_Hop_Dancing\\Hip_Hop_Dancing.fbx";
	//const char* filename = "..\\Data\\Assets\\Model\\Hip_Hop_Dancing_02\\Hip_Hop_Dancing.fbx";
	//const char* filename = "..\\Data\\Assets\\Model\\Catwalk_Walk_Turn_180_Tight_60\\Catwalk_Walk_Turn_180_Tight.fbx";
	//const char* filename = "..\\Data\\Assets\\Model\\Standing_Melee_Attack_360_High\\Standing_Melee_Attack_360_High.fbx";
	//const char* filename = "..\\Data\\Assets\\Model\\sandance_fbx\\sandance.fbx";
	//const char* filename = "..\\Data\\Assets\\Model\\Vampire_A_Lusth\\Wait\\Zombie_Idle.fbx";

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
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Roll\\Sprinting_Forward_Roll.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack01\\Elbow_Uppercut_Combo.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack02\\Uppercut_Jab.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack03\\Strike_Foward_Jog.fbx", -1);
	}

	// アニメーションパラメーターの設定
	{
		animationState = AnimationState::Idle;
		attackState = AttackAnimationState::End;
		isChangeAnimation = false;
		isAttack = false;
		speed = WalkSpeed;
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
		pos = { 0,0,0 };
		rotate = { 0,0,0 };
		//rotate = { 0,0,0,1 };
		scale = { 1,1,1 };
		radius = 50.0f;
	}
}

void Player::Update(Phoenix::Graphics::Camera& camera)
{
	// コントローラー操作(位置更新)
	{
		Control(camera);
	}

	// アニメーション変更
	{
		ChangeAnimation();
	}

	// アニメーション更新
	{
		model->UpdateTransform(1 / 60.0f);
	}

	// ワールド行列を作成
	{
		UpdateTrasform();
	}
}

void Player::UpdateTrasform()
{
	Phoenix::Math::Vector3 scale = this->scale;
	Phoenix::Math::Vector3 rotate = this->rotate;
	//Phoenix::Math::Quaternion rotate = this->rotate;
	Phoenix::Math::Vector3 translate = pos;

	Phoenix::Math::Matrix S, R, T;
	S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
	R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
	//R = Phoenix::Math::MatrixRotationQuaternion(&rotate);
	T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

	worldMatrix = S * R * T;
}

void Player::Control(Phoenix::Graphics::Camera& camera)
{
	Phoenix::f32 sX = 0.0f;
	Phoenix::f32 sY = 0.0f;

	sX = xInput[0].sX / 1000.0f;
	sY = xInput[0].sY / 1000.0f;

	sY = GetKeyState('W') < 0 ? -1.0f : sY;
	sY = GetKeyState('S') < 0 ? 1.0f : sY;
	sX = GetKeyState('A') < 0 ? -1.0f : sX;
	sX = GetKeyState('D') < 0 ? 1.0f : sX;

	if (xInput[0].bXt && animationState != AnimationState::Roll)
	{
		if (!isAttack && animationState != AnimationState::Attack && attackState == AttackAnimationState::End)
		{
			attackReceptionTimeCnt = 0;
			animationSpeed = AnimationSpeed30;
			isChangeAnimation = true;
			isAttack = true;
			animationState = AnimationState::Attack;
			attackState = AttackAnimationState::Attack01;
		}
		else if (isAttack && animationState == AnimationState::Attack && attackState == AttackAnimationState::Attack01)
		{
			//float len = model->GetLength(); // test
			if (Attack01ReceptionStartTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= model->GetLength())
			{
				attackReceptionTimeCnt = 0;
				animationSpeed = AnimationSpeed30;
				isChangeAnimation = true;
				isAttack = true;
				animationState = AnimationState::Attack;
				attackState = AttackAnimationState::Attack02;
			}
		}
		else if (isAttack && animationState == AnimationState::Attack && attackState == AttackAnimationState::Attack02)
		{
			//float len = model->GetLength(); // test
			if (Attack02ReceptionStartTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= model->GetLength())
			{
				attackReceptionTimeCnt = 0;
				animationSpeed = AnimationSpeed45;
				speed = Attack03Speed;
				isChangeAnimation = true;
				isAttack = true;
				animationState = AnimationState::Attack;
				attackState = AttackAnimationState::Attack03;
			}
		}
	}
	else if (isAttack)
	{
		if (animationState == AnimationState::Attack && !model->IsPlaying())
		{
			attackReceptionTimeCnt = 0;
			isChangeAnimation = true;
			isAttack = false;
			animationState = AnimationState::Idle;
			attackState = AttackAnimationState::End;
		}
		else if (animationState == AnimationState::Attack && attackState == AttackAnimationState::Attack03 && model->IsPlaying())
		{
			pos.x += sinf(rotate.y) * speed;
			pos.z += cosf(rotate.y) * speed;
		}

		attackReceptionTimeCnt += animationSpeed;
	}
	else if (animationState != AnimationState::Attack)
	{
		if (xInput[0].bAt && animationState != AnimationState::Roll)
		{
			isChangeAnimation = true;
			speed = RollSpeed;
			animationState = AnimationState::Roll;
		}
		else if (animationState == AnimationState::Roll && model->IsPlaying())
		{
			pos.x += sinf(rotate.y) * speed;
			pos.z += cosf(rotate.y) * speed;
		}
		else if (animationState == AnimationState::Roll && !model->IsPlaying())
		{
			isChangeAnimation = true;
			speed = 0.0f;
			animationState = AnimationState::Idle;
		}
	}

	if (animationState != AnimationState::Roll && animationState != AnimationState::Attack)
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

			/*Phoenix::Math::Matrix matrix = Phoenix::Math::MatrixRotationQuaternion(&rotate);
			Phoenix::Math::Vector3 foward = { matrix._31, matrix._32, matrix._33 };
			Phoenix::Math::Vector3 dir = { sX, 0.0f, sY };

			Phoenix::Math::Vector3 axis = Phoenix::Math::Vector3Cross(foward, dir);
			Phoenix::f32 angle = acosf(Phoenix::Math::Vector3Dot(dir, foward));

			if (1e-8f < fabs(angle))
			{
				Phoenix::Math::Quaternion q;
				q = Phoenix::Math::QuaternionRotationAxis(axis, angle);

				Phoenix::Math::Quaternion rotateT = rotate;
				rotateT *= q;
				rotate = Phoenix::Math::QuaternionSlerp(rotate, rotateT, 0.03f);
			}*/

			Phoenix::Math::Vector3 oldAngle = rotate;
			oldAngle.y = camera.GetRotateY() + atan2f(sX, sY);
			rotate = oldAngle;

			pos.x += sinf(oldAngle.y) * speed;
			pos.z += cosf(oldAngle.y) * speed;

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
}

void Player::ChangeAnimation()
{
	if (!isChangeAnimation) return;

	switch (animationState)
	{
	case AnimationState::Idle:
		model->PlayAnimation(0, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		break;

	case AnimationState::Walk:
		model->PlayAnimation(1, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		break;

	case AnimationState::Run:
		model->PlayAnimation(2, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		break;

	case AnimationState::Roll:
		model->PlayAnimation(3, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		model->SetSpeed(1.5f);
		break;

	case AnimationState::Attack:
		ChangeAttackAnimation();
		break;
	
	default: break;
	}

	isChangeAnimation = false;
}

void Player::ChangeAttackAnimation()
{
	switch (attackState)
	{
	case AttackAnimationState::Attack01:
		model->PlayAnimation(4, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		model->SetSpeed(2.0f);
		break;

	case AttackAnimationState::Attack02:
		model->PlayAnimation(5, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		model->SetSpeed(2.0f);
		break;

	case AttackAnimationState::Attack03:
		model->PlayAnimation(6, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		model->SetSpeed(1.5f);
		break;

	case AttackAnimationState::End: break;
	default: break;
	}
}

void Player::GUI()
{
	static Phoenix::s32 animClip = 0;

	if (ImGui::TreeNode("Player"))
	{
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
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Animation"))
		{
			ImGui::InputInt("AnimClip", &animClip);
			if (ImGui::Button("Play"))
			{
				model->PlayAnimation(0, animClip);
			}
			if (ImGui::Button("LoopPlay"))
			{
				model->SetLoopAnimation(true);
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}