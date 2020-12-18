#pragma once

#include <memory>
#include "DynamicDifficultyAdjuster.h"


class EnemyManager;
class BattleEnemyController
{
private:
	std::shared_ptr<EnemyManager> enemyManager;

	bool notAttack = false;
	Phoenix::f32 notAttackTime = 0.0f;

public:
	BattleEnemyController() {}
	~BattleEnemyController() {}

public:
	// ����
	static std::unique_ptr<BattleEnemyController> Create();

	// �R���X�g���N�^
	void Construct();

	// ������
	void Initialize();

	// �I����
	void Finalize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="battleEnemyState"> : ���͂����v���C���[���x������œK�ȃG�l�~�[�s�� </param>
	/// <param name="elapsedTime"> : �o�ߎ��� </param>
	void Update(BattleEnemyState battleEnemyState, Phoenix::f32 elapsedTime);

	// �G�l�~�[�}�l�[�W���[�ݒ�
	void SetEnemyManager(std::shared_ptr<EnemyManager> enemyManager);
};