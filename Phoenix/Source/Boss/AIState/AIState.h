#pragma once


enum class AIStateType
{
	Wait,			// �ҋ@
	Move,			// �ړ�
	Avoid,			// ���
	SwingAttack01,	// �E����U���i�ߋ����U���j
	SwingAttack02,	// ������U���i�ߋ����U���j
	RotationAttack,	// ��]�U���i�ߋ����U���j
	JumpAttack,		// �W�����v�U���i�������U���j
	Damage,			// �Ђ��
	None			// ���ł��Ȃ�
};

class Boss;
class Player;
class AIState
{
protected:
	AIStateType stateType = AIStateType::None; // �e�p���N���X�œK�؂ȃ^�C�v��ݒ肵�Ă��������B
	AIStateType nextStateType = AIStateType::None;
	bool isChangeState = false;

public:
	AIState() {}
	virtual ~AIState() {}

public:
	virtual void Init() = 0;
	virtual void Update(Boss* boss, Player* player) = 0;

public:
	AIStateType GetStateType() { return stateType; }
	AIStateType GetNextStateType() { return nextStateType; }
	bool IsChangeState()
	{
		bool isChange = isChangeState;
		isChangeState = false;

		return isChange;
	}
};