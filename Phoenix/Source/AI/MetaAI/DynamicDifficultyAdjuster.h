#pragma once

#include "Phoenix/Math/PhoenixMath.h"
#include "../../Enemy/EnemyState.h"


class DynamicDifficultyAdjuster
{
private:
	BattleEnemyState ideaState = BattleEnemyState::NoneState;
	Phoenix::f32 notUpdatedTime = 0.0f;

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
	/// <param name="downScore"> : �v���C���[�s���̍��v�X�R�A���������Ă��邩 </param>
	/// <param name="elapsedTime"> : �o�ߎ��� </param>
	void Update(Phoenix::s32 skillLevel, bool downScore, Phoenix::f32 elapsedTime);

	// �G�l�~�[�X�e�[�g�̎擾
	BattleEnemyState GetBattleEnemyState();
};