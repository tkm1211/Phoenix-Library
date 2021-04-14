#pragma once

#include <memory>
#include "Player.h"
#include "PlayerInput.h"
#include "Phoenix/FrameWork/StateMachine/StateMachine.h"


class PlayerController
{
private:
	// ステート管理
	std::shared_ptr<FSM<Player::AnimationState, std::shared_ptr<Player>, std::shared_ptr<PlayerInput>>> status;

	// 入力管理
	std::shared_ptr<PlayerInput> input;

	// プレイヤー
	std::weak_ptr<Player> owner;

public:
	PlayerController() {}
	~PlayerController() {}

public:
	// 生成
	static std::shared_ptr<PlayerController> Create();

	// コンストラクタ
	void Construct(std::weak_ptr<Player> owner);

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	// 更新
	void Update(Phoenix::f32 elapsedTime);

	// 座標更新
	void UpdatePosition(std::shared_ptr<Player> player, Phoenix::f32 elapsedTime);

	// ブレンドレート更新
	void UpdateBlendRate(std::shared_ptr<Player> player);
};