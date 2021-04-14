#pragma once

#include <memory>
#include "Player.h"
#include "PlayerInput.h"
#include "Phoenix/FrameWork/StateMachine/StateMachine.h"


class PlayerController
{
private:
	// �X�e�[�g�Ǘ�
	std::shared_ptr<FSM<Player::AnimationState, std::shared_ptr<Player>, std::shared_ptr<PlayerInput>>> status;

	// ���͊Ǘ�
	std::shared_ptr<PlayerInput> input;

	// �v���C���[
	std::weak_ptr<Player> owner;

public:
	PlayerController() {}
	~PlayerController() {}

public:
	// ����
	static std::shared_ptr<PlayerController> Create();

	// �R���X�g���N�^
	void Construct(std::weak_ptr<Player> owner);

	// ������
	void Initialize();

	// �I����
	void Finalize();

	// �X�V
	void Update(Phoenix::f32 elapsedTime);

	// ���W�X�V
	void UpdatePosition(std::shared_ptr<Player> player, Phoenix::f32 elapsedTime);

	// �u�����h���[�g�X�V
	void UpdateBlendRate(std::shared_ptr<Player> player);
};