#include "PlayerInput.h"


// 生成
std::shared_ptr<PlayerInput> PlayerInput::Create()
{
	return std::make_shared<PlayerInput>();
}

// 初期化
void PlayerInput::Initialize()
{
	type = InputType::Idle;

	sX = 0.0f;
	sY = 0.0f;

	blendRate = { 0.0f, 0.0f, 0.0f };

	lockOn = false;
}

// 終了化
void PlayerInput::Finalize()
{

}

// 更新
void PlayerInput::Update(bool onControl, Phoenix::f32 elapsedTime)
{
	// 入力初期化
	{
		type = InputType::Idle;

		sX = 0.0f;
		sY = 0.0f;

		lockOn = false;
	}

	if (!onControl)
	{
		return;
	}

	// ロックオン
	if ((xInput[0].bRBs || GetKeyState('L') < 0))
	{
		lockOn = true;
	}

	// 移動入力
	{
		sX = xInput[0].sX / 1000.0f;
		sY = xInput[0].sY / 1000.0f;

		sY = GetKeyState('W') < 0 ? -1.0f : sY;
		sY = GetKeyState('S') < 0 ? 1.0f : sY;
		sX = GetKeyState('A') < 0 ? -1.0f : sX;
		sX = GetKeyState('D') < 0 ? 1.0f : sX;

		if (sX != 0.0f || sY != 0.0f)
		{
			type = InputType::Walk;
		}
	}

	// ブレンドレート計算
	{
		if (lockOn)
		{
			blendRate.x = Phoenix::Math::f32Lerp(blendRate.x, sX, 0.15f * elapsedTime);
			blendRate.y = Phoenix::Math::f32Lerp(blendRate.y, sY, 0.15f * elapsedTime);
		}

		blendRate.z = Phoenix::Math::Vector2Length(Phoenix::Math::Vector2(sX, sY));
		blendRate.z = 1.0f <= blendRate.z ? 1.0f : blendRate.z;
	}

	// ボタン入力
	{
		if (xInput[0].bXt || GetAsyncKeyState('J') & 1)
		{
			type = InputType::WeakAttack;
		}
		if (xInput[0].bYt || GetAsyncKeyState('K') & 1)
		{
			type = InputType::StrongAttack;
		}
		if (xInput[0].bAt || GetAsyncKeyState(VK_SPACE) & 1)
		{
			type = InputType::Dedge;
		}
	}
}

// 入力種類の取得
PlayerInput::InputType PlayerInput::GetInputType()
{
	return type;
}

// 移動入力値の取得
Phoenix::f32 PlayerInput::GetSX()
{
	return sX;
}
Phoenix::f32 PlayerInput::GetSY()
{
	return sY;
}

// ブレンドレートの取得
Phoenix::Math::Vector3 PlayerInput::GetBlendRate()
{
	return blendRate;
}

// ロックオンの取得
bool PlayerInput::GetLockOn()
{
	return lockOn;
}