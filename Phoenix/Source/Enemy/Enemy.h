#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Component/Transform.h"
#include "EnemyBattleState.h"


class EnemyManager;
class Enemy
{
private:
	static constexpr Phoenix::s32 LifeRange = 50;

public:
	enum class TypeTag
	{
		Small,  // 雑魚
		Medium, // 中ボス
		Large   // ボス
	};

private:
	bool enable = false;
	bool alive = false;
	bool inBattle = false;

	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;
	std::unique_ptr<Phoenix::FrameWork::Transform> transform;
	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;

	std::weak_ptr<EnemyManager> owner;

	Phoenix::s32 life = 0;
	Phoenix::f32 radius = 0.0f;

public:
	Enemy() {}
	~Enemy() { Finalize(); }

public:
	// 生成
	static std::shared_ptr<Enemy> Create();

	// コンストラクタ
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// GUI
	void GUI(Phoenix::s32 index);

public:
	// 有効フラグ設定
	void SetEnable(bool enable);

	// 存在フラグ設定
	void SetAlive(bool alive);

	// 戦闘中フラグ設定
	void SetInBattle(bool inBattle);

	// トランスフォームの設定
	void SetTransform(Phoenix::FrameWork::Transform transform);

	// エネミーマネージャーの設定
	void SetOwner(std::shared_ptr<EnemyManager> owner);

public:
	// 有効フラグ取得
	bool GetEnable();

	// 存在フラグ取得
	bool GetAlive();

	// 戦闘中フラグ取得
	bool GetInBattle();

	// トランスフォームの取得
	Phoenix::FrameWork::Transform GetTransform();


	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }

	const std::vector<Phoenix::FrameWork::CollisionData>* GetCollisionDatas() { return &collisionDatas; }

	Phoenix::Math::Matrix GetWorldMatrix() { return transform->GetWorldTransform(); }

	Phoenix::Math::Vector3 GetPosition() { return transform->GetTranslate(); }

	Phoenix::f32 GetRadius() { return radius; }
};