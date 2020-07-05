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

	model = std::make_unique<Phoenix::FrameWork::ModelObject>();
	model->Initialize(graphicsDevice);
	model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Vampire_A_Lusth\\Wait\\Zombie_Idle.fbx");
	//model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Mixamo\\Sword_And_Shield_Attack\\Sword_And_Shield_Attack.fbx");

	model->LoadAnimation("..\\Data\\Assets\\Model\\Vampire_A_Lusth\\FowardWalk\\Walking_With_Shopping_Bag.fbx", -1);
	model->LoadAnimation("..\\Data\\Assets\\Model\\Vampire_A_Lusth\\FowardRun\\Running.fbx", -1);
	model->LoadAnimation("..\\Data\\Assets\\Model\\Vampire_A_Lusth\\Roll\\Sprinting_Forward_Roll.fbx", -1);

	animationState = AnimationState::Wait;
	isChangeAnimation = false;

	model->PlayAnimation(0, 1);
	model->UpdateTransform(1 / 60.0f);
	model->SetLoopAnimation(true);
	//model->PauseAnimation(true);

	worldMatrix = Phoenix::Math::MatrixIdentity();
	pos = { 0,0,0 };
	rotate = { 0,0,0 };
	scale = { 1,1,1 };

	speed = WalkSpeed;
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
		Phoenix::Math::Vector3 scale = this->scale;
		Phoenix::Math::Vector3 rotate = this->rotate;
		Phoenix::Math::Vector3 translate = pos;

		Phoenix::Math::Matrix S, R, T;
		S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
		R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
		T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);

		worldMatrix = S * R * T;
	}
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
		animationState = AnimationState::Wait;
	}

	if (animationState != AnimationState::Roll)
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
			if (xInput[0].bRBs && animationState != AnimationState::Run)
			{
				isChangeAnimation = true;
				speed = RunSpeed;
				animationState = AnimationState::Run;
			}
		}
		else
		{
			if (animationState != AnimationState::Wait)
			{
				isChangeAnimation = true;
				speed = 0.0f;
				animationState = AnimationState::Wait;
			}
		}
	}
}

void Player::ChangeAnimation()
{
	if (!isChangeAnimation) return;

	switch (animationState)
	{
	case Player::AnimationState::Wait:
		model->PlayAnimation(0, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		break;

	case Player::AnimationState::Walk:
		model->PlayAnimation(1, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		break;

	case Player::AnimationState::Run:
		model->PlayAnimation(2, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		break;

	case Player::AnimationState::Roll:
		model->PlayAnimation(3, 1, 0.2f);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		break;

	default: break;
	}

	isChangeAnimation = false;
}

void Player::GUI()
{
	static Phoenix::s32 animClip = 0;

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
}