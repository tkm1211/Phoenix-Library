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
	const char* filename = "..\\Data\\Assets\\Model\\Hip_Hop_Dancing_02\\Hip_Hop_Dancing.fbx";
	//const char* filename = "..\\Data\\Assets\\Model\\Catwalk_Walk_Turn_180_Tight_60\\Catwalk_Walk_Turn_180_Tight.fbx";
	//const char* filename = "..\\Data\\Assets\\Model\\Standing_Melee_Attack_360_High\\Standing_Melee_Attack_360_High.fbx";
	//const char* filename = "..\\Data\\Assets\\Model\\Walking_With_Shopping_Bag\\Walking_With_Shopping_Bag.fbx";

	model = std::make_unique<Phoenix::FrameWork::ModelObject>();
	model->Initialize(graphicsDevice);
	model->Load(graphicsDevice, Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), filename));
	//model->PlayAnimation(0, 0);

	worldMatrix = Phoenix::Math::MatrixIdentity();
	pos = { 0,0,0 };
	rotate = { 0,0,0 };
	scale = { 1,1,1 };

	speed = 5.0f;
}

void Player::Update(Phoenix::Graphics::Camera& camera)
{
	Phoenix::f32 sX = 0.0f;
	Phoenix::f32 sY = 0.0f;

	sX = xInput[0].sX / 1000.0f;
	sY = xInput[0].sY / 1000.0f;

	sY = GetKeyState('W') < 0 ? -1.0f : sY;
	sY = GetKeyState('S') < 0 ? 1.0f : sY;
	sX = GetKeyState('A') < 0 ? -1.0f : sX;
	sX = GetKeyState('D') < 0 ? 1.0f : sX;

	if (sX != 0.0f || sY != 0.0f)
	{
		Phoenix::Math::Vector2 vec;

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
	}

	// ƒ[ƒ‹ƒhs—ñ‚ðì¬
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

	model->UpdateTransform(1 / 60.0f);
}

void Player::GUI()
{
	static Phoenix::s32 animClip = 0;

	if (ImGui::TreeNode("Transform"))
	{
		ImGui::DragFloat3("pos", &pos.x);
		ImGui::DragFloat3("rotate", &rotate.x);
		ImGui::DragFloat3("scale", &scale.x);
		ImGui::DragFloat("speed", &speed);
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