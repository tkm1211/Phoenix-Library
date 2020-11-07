#pragma once

#include "Phoenix/Math/PhoenixMath.h"
#include "../../Enemy/EnemyState.h"


class DynamicDifficultyAdjuster
{
private:
	BattleEnenyState ideaState = BattleEnenyState::NoneState;

public:
	DynamicDifficultyAdjuster() {}
	~DynamicDifficultyAdjuster() {}

public:
	// ����
	static std::unique_ptr<DynamicDifficultyAdjuster> Create();

	// �R���X�g���N�^
	void Construct();

	// ������
	void Initialize();

	// �I����
	void Finalize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="skillLevel"> : �v���C���[�s���̍��v�X�R�A���番�͂����v���C���[���x�� </param>
	void Update(Phoenix::s32 skillLevel);

	// �G�l�~�[�X�e�[�g�̎擾
	BattleEnenyState GetBattleEnenyState();
};