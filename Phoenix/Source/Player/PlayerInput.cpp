#include "PlayerInput.h"


// ����
std::shared_ptr<PlayerInput> PlayerInput::Create()
{
	return std::make_shared<PlayerInput>();
}

// ������
void PlayerInput::Initialize()
{
	type = InputType::Idle;

	sX = 0.0f;
	sY = 0.0f;

	blendRate = { 0.0f, 0.0f, 0.0f };

	lockOn = false;
}

// �I����
void PlayerInput::Finalize()
{

}

// �X�V
void PlayerInput::Update(bool onControl, Phoenix::f32 elapsedTime)
{
	// ���͏�����
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

	// ���b�N�I��
	if ((xInput[0].bRBs || GetKeyState('L') < 0))
	{
		lockOn = true;
	}

	// �ړ�����
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

	// �u�����h���[�g�v�Z
	{
		if (lockOn)
		{
			blendRate.x = Phoenix::Math::f32Lerp(blendRate.x, sX, 0.15f * elapsedTime);
			blendRate.y = Phoenix::Math::f32Lerp(blendRate.y, sY, 0.15f * elapsedTime);
		}

		blendRate.z = Phoenix::Math::Vector2Length(Phoenix::Math::Vector2(sX, sY));
		blendRate.z = 1.0f <= blendRate.z ? 1.0f : blendRate.z;
	}

	// �{�^������
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

// ���͎�ނ̎擾
PlayerInput::InputType PlayerInput::GetInputType()
{
	return type;
}

// �ړ����͒l�̎擾
Phoenix::f32 PlayerInput::GetSX()
{
	return sX;
}
Phoenix::f32 PlayerInput::GetSY()
{
	return sY;
}

// �u�����h���[�g�̎擾
Phoenix::Math::Vector3 PlayerInput::GetBlendRate()
{
	return blendRate;
}

// ���b�N�I���̎擾
bool PlayerInput::GetLockOn()
{
	return lockOn;
}