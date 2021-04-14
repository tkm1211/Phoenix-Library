#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/FrameWork/Input/InputDevice.h"


class PlayerInput
{
public:
	enum class InputType
	{
		Idle,
		Walk,
		WeakAttack,
		StrongAttack,
		Dedge
	};

private:
	// 入力種類
	InputType type = InputType::Idle;

	// 移動入力値
	Phoenix::f32 sX = 0.0f;
	Phoenix::f32 sY = 0.0f;

	// ブレンドレート
	Phoenix::Math::Vector3 blendRate = { 0.0f, 0.0f, 0.0f };

	// ロックオン
	bool lockOn = false;

public:
	PlayerInput() {}
	~PlayerInput() {}

public:
	// 生成
	static std::shared_ptr<PlayerInput> Create();

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	// 更新
	void Update(bool onControl, Phoenix::f32 elapsedTime);

	// 入力種類の取得
	InputType GetInputType();

	// 移動入力値の取得
	Phoenix::f32 GetSX();
	Phoenix::f32 GetSY();

	// ブレンドレートの取得
	Phoenix::Math::Vector3 GetBlendRate();

	// ロックオンの取得
	bool GetLockOn();
};