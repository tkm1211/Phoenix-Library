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
	// ���͎��
	InputType type = InputType::Idle;

	// �ړ����͒l
	Phoenix::f32 sX = 0.0f;
	Phoenix::f32 sY = 0.0f;

	// �u�����h���[�g
	Phoenix::Math::Vector3 blendRate = { 0.0f, 0.0f, 0.0f };

	// ���b�N�I��
	bool lockOn = false;

public:
	PlayerInput() {}
	~PlayerInput() {}

public:
	// ����
	static std::shared_ptr<PlayerInput> Create();

	// ������
	void Initialize();

	// �I����
	void Finalize();

	// �X�V
	void Update(bool onControl, Phoenix::f32 elapsedTime);

	// ���͎�ނ̎擾
	InputType GetInputType();

	// �ړ����͒l�̎擾
	Phoenix::f32 GetSX();
	Phoenix::f32 GetSY();

	// �u�����h���[�g�̎擾
	Phoenix::Math::Vector3 GetBlendRate();

	// ���b�N�I���̎擾
	bool GetLockOn();
};