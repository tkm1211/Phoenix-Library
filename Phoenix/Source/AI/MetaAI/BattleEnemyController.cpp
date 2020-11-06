#include "BattleEnemyController.h"
#include "../../Enemy/EnemyManager.h"


// ����
std::unique_ptr<BattleEnemyController> BattleEnemyController::Create()
{
	return std::make_unique<BattleEnemyController>();
}

// �R���X�g���N�^
void BattleEnemyController::Construct()
{

}

// ������
void BattleEnemyController::Initialize()
{

}

// �I����
void BattleEnemyController::Finalize()
{

}

// �X�V
void BattleEnemyController::Update(EnenyBattleState enenyBattleState)
{
	switch (enenyBattleState)
	{
	case EnenyBattleState::Idle:
		if (0 < enemyManager->GetAliveEnemyCount())
		{
			if (0 < enemyManager->GetBattleEnemyCount())
			{

			}
		}

		break;

	case EnenyBattleState::Attack:

		break;

	default: break;
	}
}

// �G�l�~�[�}�l�[�W���[�ݒ�
void BattleEnemyController::SetEnemyManager(std::shared_ptr<EnemyManager> enemyManager)
{
	this->enemyManager = enemyManager;
}