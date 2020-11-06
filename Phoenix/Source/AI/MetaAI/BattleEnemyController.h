#pragma once

#include <memory>
#include "DynamicDifficultyAdjuster.h"


class EnemyManager;
class BattleEnemyController
{
private:
	std::shared_ptr<EnemyManager> enemyManager;

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
	/// <param name="enenyBattleState"> : ���͂����v���C���[���x������œK�ȃG�l�~�[�s�� </param>
	void Update(EnenyBattleState enenyBattleState);

	// �G�l�~�[�}�l�[�W���[�ݒ�
	void SetEnemyManager(std::shared_ptr<EnemyManager> enemyManager);
};