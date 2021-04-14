#include "PlayerController.h"
#include "PlayerState.h"


// ����
std::shared_ptr<PlayerController> PlayerController::Create()
{
	return std::make_shared<PlayerController>();
}

// �R���X�g���N�^
void PlayerController::Construct(std::weak_ptr<Player> owner)
{
	// �v���C���[�ݒ�
	std::shared_ptr<Player> player;
	{
		this->owner = owner;

		player = this->owner.lock();
		if (!player) return;
	}

	// ���͏�����
	{
		input = PlayerInput::Create();
		input->Initialize();
	}

	// �X�e�[�g������
	{
		// ����
		{
			status = FSM<Player::AnimationState, std::shared_ptr<Player>, std::shared_ptr<PlayerInput>>::Create();
		}

		// �����ǉ�
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

		// �������ǉ�
		{
			status->AddEnter(Player::AnimationState::Idle, PlayerState::Idle::Enter);
			status->AddEnter(Player::AnimationState::Walk, PlayerState::Walk::Enter);
			status->AddEnter(Player::AnimationState::Attack, PlayerState::Attack::Enter);
			status->AddEnter(Player::AnimationState::Damage, PlayerState::Damage::Enter);
			status->AddEnter(Player::AnimationState::Dedge, PlayerState::Dedge::Enter);
			status->AddEnter(Player::AnimationState::Death, PlayerState::Death::Enter);
		}

		// �X�V�ǉ�
		{
			status->AddUpdate(Player::AnimationState::Idle, PlayerState::Idle::Update);
			status->AddUpdate(Player::AnimationState::Walk, PlayerState::Walk::Update);
			status->AddUpdate(Player::AnimationState::Attack, PlayerState::Attack::Update);
			status->AddUpdate(Player::AnimationState::Damage, PlayerState::Damage::Update);
			status->AddUpdate(Player::AnimationState::Dedge, PlayerState::Dedge::Update);
			status->AddUpdate(Player::AnimationState::Death, PlayerState::Death::Update);
		}

		// �I�����ǉ�
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

// ������
void PlayerController::Initialize()
{
	// �v���C���[�ݒ�
	std::shared_ptr<Player> player = owner.lock();

	// ������
	{
		input->Initialize();
		status->Start(Player::AnimationState::Idle, player, input);
	}
}

// �I����
void PlayerController::Finalize()
{

}

// �X�V
void PlayerController::Update(Phoenix::f32 elapsedTime)
{
	// �v���C���[�ݒ�
	std::shared_ptr<Player> player = owner.lock();

	// �C���v�b�g�X�V
	{
		input->Update(player->OnControl(), elapsedTime);
	}

	// �X�e�[�g�X�V
	{
		status->Update(player, input);
	}

	// ���W�X�V
	{
		UpdatePosition(player, elapsedTime);
	}

	// �u�����h���[�g�X�V
	{
		UpdateBlendRate(player);
	}
}

// ���W�X�V
void PlayerController::UpdatePosition(std::shared_ptr<Player> player, Phoenix::f32 elapsedTime)
{
	if (!player->IsChangeAnimation())
	{
		// �p�����[�^�[�̎擾
		Player::AnimationState state = status->GetCurrenState();
		Phoenix::Math::Quaternion rotate = player->GetRotate();
		Phoenix::Math::Quaternion newRotate = player->GetNewRotate();
		Phoenix::Math::Vector3 blendRate = input->GetBlendRate();
		Phoenix::Math::Vector3 pos = player->GetPosition();
		Phoenix::f32 rotateY = player->GetRotateY();
		Phoenix::f32 speed = player->GetSpeed();

		// ���炩�Ȃɉ�]
		rotate = Phoenix::Math::QuaternionSlerp(rotate, newRotate, 0.17f * elapsedTime);

		// �ړ��X�V
		{
			if (state == Player::AnimationState::Walk)
			{
				if (player->GetLockOn()) // �\���Ă�����
				{
					pos.x += sinf(rotateY) * ((speed + (Player::BattleSlowRunSpeed * blendRate.z) * elapsedTime));
					pos.z += cosf(rotateY) * ((speed + (Player::BattleSlowRunSpeed * blendRate.z) * elapsedTime));
				}
				else // ���ʂ̏��
				{
					pos.x += sinf(rotateY) * ((speed + (Player::SlowRunSpeed * blendRate.z)) * elapsedTime);
					pos.z += cosf(rotateY) * ((speed + (Player::SlowRunSpeed * blendRate.z)) * elapsedTime);
				}
			}
			else if (state != Player::AnimationState::Attack) // ����Ȃǂ̈ړ��̏ꍇ
			{
				pos.x += sinf(rotateY) * (speed * elapsedTime);
				pos.z += cosf(rotateY) * (speed * elapsedTime);
			}
		}

		// �U�����̑O�i�X�e�b�v��
		if (state == Player::AnimationState::Attack && 0.0f < speed)
		{
			Phoenix::Math::Matrix matrix = Phoenix::Math::MatrixRotationQuaternion(&rotate);
			Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(matrix._31, matrix._32, matrix._33);

			pos.x += forward.x * (speed * elapsedTime);
			pos.z += forward.z * (speed * elapsedTime);

			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * elapsedTime);
		}

		// �p�����[�^�[�̐ݒ�
		player->SetPosition(pos);
		player->SetRotate(rotate);
		player->SetSpeed(speed);
	}
}

// �u�����h���[�g�X�V
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