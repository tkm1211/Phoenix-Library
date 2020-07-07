#include "Boss.h"


std::unique_ptr<Boss> Boss::Create()
{
	return std::make_unique<Boss>();
}

void Boss::Init(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	// モデル読み込み
	{
		model = std::make_unique<Phoenix::FrameWork::ModelObject>();
		model->Initialize(graphicsDevice);
		model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Boss\\Mutant\\Idle\\Breathing_Idle.fbx");
		//model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Player\\Paladin_WProp_J_Nordstrom\\Idle\\Breathing_Idle.fbx");
		//model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Mixamo\\Sword_And_Shield_Attack\\Sword_And_Shield_Attack.fbx");
	}

	// アニメーション読み込み
	{
		//model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardWalk\\Walking_With_Shopping_Bag.fbx", -1);
		//model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\Running.fbx", -1);
		//model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Roll\\Sprinting_Forward_Roll.fbx", -1);
		//model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack01\\Elbow_Uppercut_Combo.fbx", -1);
		//model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack02\\Uppercut_Jab.fbx", -1);
		//model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack03\\Strike_Foward_Jog.fbx", -1);
	}

	// アニメーションパラメーターの設定
	{
		animationState = AnimationState::Idle;
		//attackState = AttackAnimationState::End;
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
		pos = { 0,0,-1000.0f };
		rotate = { 0,0,0 };
		scale = { 1.5f,1.5f,1.5f };
		radius = 50.0f;
	}
}

void Boss::Update(Phoenix::Graphics::Camera& camera)
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

void Boss::Control(Phoenix::Graphics::Camera& camera)
{

}

void Boss::ChangeAnimation()
{

}

void Boss::ChangeAttackAnimation()
{

}

void Boss::GUI()
{

}