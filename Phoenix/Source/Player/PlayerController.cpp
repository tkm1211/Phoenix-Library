#include "PlayerController.h"
#include "PlayerState.h"


// 生成
std::shared_ptr<PlayerController> PlayerController::Create()
{
	return std::make_shared<PlayerController>();
}

// コンストラクタ
void PlayerController::Construct(std::weak_ptr<Player> owner)
{
	// プレイヤー設定
	std::shared_ptr<Player> player;
	{
		this->owner = owner;

		player = this->owner.lock();
		if (!player) return;
	}

	// 入力初期化
	{
		input = PlayerInput::Create();
		input->Initialize();
	}

	// ステート初期化
	{
		// 生成
		{
			status = FSM<Player::AnimationState, std::shared_ptr<Player>, std::shared_ptr<PlayerInput>>::Create();
		}

		// 条件追加
		{
			status->AddTransition(Player::AnimationState::Idle, Player::AnimationState::Walk, PlayerState::Walk::Transition);
			status->AddTransition(Player::AnimationState::Idle, Player::AnimationState::Attack, PlayerState::Attack::Transition);
			status->AddTransition(Player::AnimationState::Idle, Player::AnimationState::Dedge, PlayerState::Dedge::Transition);

			status->AddTransition(Player::AnimationState::Walk, Player::AnimationState::Idle, PlayerState::Idle::TransitionFromWalk);
			status->AddTransition(Player::AnimationState::Walk, Player::AnimationState::Attack, PlayerState::Attack::Transition);
			status->AddTransition(Player::AnimationState::Walk, Player::AnimationState::Dedge, PlayerState::Dedge::Transition);

			status->AddTransition(Player::AnimationState::Attack, Player::AnimationState::Idle, PlayerState::Idle::TransitionFromAttack);
			status->AddTransition(Player::AnimationState::Attack, Player::AnimationState::Dedge, PlayerState::Dedge::TransitionFromAttack);

			status->AddTransition(Player::AnimationState::Damage, Player::AnimationState::Idle, PlayerState::Idle::Transition);
			status->AddTransition(Player::AnimationState::Damage, Player::AnimationState::Dedge, PlayerState::Dedge::TransitionFromDamage);
			
			status->AddTransition(Player::AnimationState::Dedge, Player::AnimationState::Idle, PlayerState::Idle::Transition);
			status->AddTransition(Player::AnimationState::Dedge, Player::AnimationState::Attack, PlayerState::Attack::TransitionFromDedge);

			status->AddTransitionOfAnyState(Player::AnimationState::Damage, PlayerState::Damage::Transition);
			status->AddTransitionOfAnyState(Player::AnimationState::Death, PlayerState::Death::Transition);
		}

		// 初期化追加
		{
			status->AddEnter(Player::AnimationState::Idle, PlayerState::Idle::Enter);
			status->AddEnter(Player::AnimationState::Walk, PlayerState::Walk::Enter);
			status->AddEnter(Player::AnimationState::Attack, PlayerState::Attack::Enter);
			status->AddEnter(Player::AnimationState::Damage, PlayerState::Damage::Enter);
			status->AddEnter(Player::AnimationState::Dedge, PlayerState::Dedge::Enter);
			status->AddEnter(Player::AnimationState::Death, PlayerState::Death::Enter);
		}

		// 更新追加
		{
			status->AddUpdate(Player::AnimationState::Idle, PlayerState::Idle::Update);
			status->AddUpdate(Player::AnimationState::Walk, PlayerState::Walk::Update);
			status->AddUpdate(Player::AnimationState::Attack, PlayerState::Attack::Update);
			status->AddUpdate(Player::AnimationState::Damage, PlayerState::Damage::Update);
			status->AddUpdate(Player::AnimationState::Dedge, PlayerState::Dedge::Update);
			status->AddUpdate(Player::AnimationState::Death, PlayerState::Death::Update);
		}

		// 終了化追加
		{
			status->AddExit(Player::AnimationState::Idle, PlayerState::Idle::Exit);
			status->AddExit(Player::AnimationState::Walk, PlayerState::Walk::Exit);
			status->AddExit(Player::AnimationState::Attack, PlayerState::Attack::Exit);
			status->AddExit(Player::AnimationState::Damage, PlayerState::Damage::Exit);
			status->AddExit(Player::AnimationState::Dedge, PlayerState::Dedge::Exit);
			status->AddExit(Player::AnimationState::Death, PlayerState::Death::Exit);
		}
	}
}

// 初期化
void PlayerController::Initialize()
{
	// プレイヤー設定
	std::shared_ptr<Player> player = owner.lock();

	// 初期化
	{
		input->Initialize();
		status->Start(Player::AnimationState::Idle, player, input);
	}
}

// 終了化
void PlayerController::Finalize()
{

}

// 更新
void PlayerController::Update(Phoenix::f32 elapsedTime)
{
	// プレイヤー設定
	std::shared_ptr<Player> player = owner.lock();

	// インプット更新
	{
		input->Update(player->OnControl(), elapsedTime);
	}

	// ステート更新
	{
		status->Update(player, input);
	}

	// 座標更新
	{
		UpdatePosition(player, elapsedTime);
	}

	// ブレンドレート更新
	{
		UpdateBlendRate(player);
	}
}

// 座標更新
void PlayerController::UpdatePosition(std::shared_ptr<Player> player, Phoenix::f32 elapsedTime)
{
	if (!player->IsChangeAnimation())
	{
		// パラメーターの取得
		Player::AnimationState state = status->GetCurrenState();
		Phoenix::Math::Quaternion rotate = player->GetRotate();
		Phoenix::Math::Quaternion newRotate = player->GetNewRotate();
		Phoenix::Math::Vector3 blendRate = input->GetBlendRate();
		Phoenix::Math::Vector3 pos = player->GetPosition();
		Phoenix::f32 rotateY = player->GetRotateY();
		Phoenix::f32 speed = player->GetSpeed();

		// 滑らかなに回転
		rotate = Phoenix::Math::QuaternionSlerp(rotate, newRotate, 0.17f * elapsedTime);

		// 移動更新
		{
			if (state == Player::AnimationState::Walk)
			{
				if (player->GetLockOn()) // 構えている状態
				{
					pos.x += sinf(rotateY) * ((speed + (Player::BattleSlowRunSpeed * blendRate.z) * elapsedTime));
					pos.z += cosf(rotateY) * ((speed + (Player::BattleSlowRunSpeed * blendRate.z) * elapsedTime));
				}
				else // 普通の状態
				{
					pos.x += sinf(rotateY) * ((speed + (Player::SlowRunSpeed * blendRate.z)) * elapsedTime);
					pos.z += cosf(rotateY) * ((speed + (Player::SlowRunSpeed * blendRate.z)) * elapsedTime);
				}
			}
			else if (state != Player::AnimationState::Attack) // 回避などの移動の場合
			{
				pos.x += sinf(rotateY) * (speed * elapsedTime);
				pos.z += cosf(rotateY) * (speed * elapsedTime);
			}
		}

		// 攻撃中の前進ステップ時
		if (state == Player::AnimationState::Attack && 0.0f < speed)
		{
			Phoenix::Math::Matrix matrix = Phoenix::Math::MatrixRotationQuaternion(&rotate);
			Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(matrix._31, matrix._32, matrix._33);

			pos.x += forward.x * (speed * elapsedTime);
			pos.z += forward.z * (speed * elapsedTime);

			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * elapsedTime);
		}

		// パラメーターの設定
		player->SetPosition(pos);
		player->SetRotate(rotate);
		player->SetSpeed(speed);
	}
}

// ブレンドレート更新
void PlayerController::UpdateBlendRate(std::shared_ptr<Player> player)
{
	Phoenix::Math::Vector3 blendRate = input->GetBlendRate();

	if (input->GetLockOn())
	{
		player->GetModel()->SetBlendRate(Phoenix::Math::Vector3(-blendRate.x, blendRate.y, 0.0f));
	}
	else
	{
		player->GetModel()->SetBlendRate(blendRate.z);
	}
}