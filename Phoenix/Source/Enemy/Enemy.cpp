#include "Enemy.h"
#include "EnemyManager.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"


// 生成
std::shared_ptr<Enemy> Enemy::Create()
{
	return std::make_shared<Enemy>();
}

// コンストラクタ
void Enemy::Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	// モデル読み込み
	{
		model = std::make_unique<Phoenix::FrameWork::ModelObject>();
		model->Initialize(graphicsDevice);
		model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Enemy\\Idle\\Idle.fbx"); // "..\\Data\\Assets\\Model\\Enemy\\Idle\\Idle.fbx"  // "..\\Data\\Assets\\Model\\Boss\\Mutant\\Idle\\Mutant_Roaring.fbx"
		model->AddAnimationLayer(0);
	}

	// アニメーション読み込み
	{
		//model->LoadAnimation("..\\Data\\Assets\\Model\\Boss\\Mutant\\Run\\Mutant_Run.fbx", -1);
		//model->AddAnimationLayer(1);
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

	// トランスフォームの初期化
	{
		transform = std::make_unique<Phoenix::FrameWork::Transform>();
	}

	// その他のパラメータ初期化
	{
		Initialize();
	}
}

// 初期化
void Enemy::Initialize()
{
	// 待機モーション開始
	{
		model->PlayAnimation(0, 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
	}

	// パラメーター
	{
		enable = false;
		alive = false;
		inBattle = false;

		life = LifeRange;
		radius = 0.5f;
	}
}

// 終了化
void Enemy::Finalize()
{
	collisionDatas.clear();
	transform.reset();
	model.reset();
}

// 更新
void Enemy::Update()
{
	// アニメーション更新
	{
		model->UpdateTransform(1 / 60.0f);
	}

	// トランスフォーム更新
	{
		transform->Update();
	}

	// コリジョン更新
	{
		Phoenix::Math::Matrix systemUnitTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		systemUnitTransform._11 = systemUnitTransform._22 = systemUnitTransform._33 = 0.01f;

		auto nodes = model->GetNodes();
		for (auto& data : collisionDatas)
		{
			Phoenix::Math::Matrix bone = nodes->at(data.boneIndex).worldTransform;
			bone *= systemUnitTransform * transform->GetWorldTransform();
			data.pos = Phoenix::Math::Vector3(bone._41, bone._42, bone._43);
		}
	}

	// ライフが０ならマネージャーの生存エネミーカウントを下げる
	if (life <= 0)
	{
		if (std::shared_ptr<EnemyManager> manager = owner.lock())
		{
			manager->SubAliveEnemyCount(1);
		}
	}
}

// 描画
void Enemy::Draw()
{
}

// GUI
void Enemy::GUI(Phoenix::s32 index)
{
	std::string name = std::string("Enemy") + std::to_string(index);
	if (ImGui::TreeNode(name.c_str()))
	{
		transform->GUI();
		ImGui::TreePop();
	}
}

// 有効化設定
void Enemy::SetEnable(bool enable)
{
	this->enable = enable;
}

// 存在フラグ設定
void Enemy::SetAlive(bool alive)
{
	this->alive = alive;
}

// 戦闘中フラグ設定
void Enemy::SetInBattle(bool inBattle)
{
	this->inBattle = inBattle;
}

// トランスフォームの設定
void Enemy::SetTransform(Phoenix::FrameWork::Transform transform)
{
	this->transform->SetTranslate(transform.GetTranslate());
	this->transform->SetRotate(transform.GetRotate());
	this->transform->SetScale(transform.GetScale());
}

// エネミーマネージャーの設定
void Enemy::SetOwner(std::shared_ptr<EnemyManager> owner)
{
	this->owner = owner;
}

// 有効フラグ取得
bool Enemy::GetEnable()
{
	return enable;
}

// 存在フラグ取得
bool Enemy::GetAlive()
{
	return alive;
}

// 戦闘中フラグ取得
bool Enemy::GetInBattle()
{
	return inBattle;
}

// トランスフォームの取得
Phoenix::FrameWork::Transform Enemy::GetTransform()
{
	return *transform.get();
}