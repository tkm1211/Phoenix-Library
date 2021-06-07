#include "BattleEnemyController.h"
#include "../../Enemy/EnemyManager.h"
#include "../../Enemy/Enemy.h"
#include "../../Player/Player.h"


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
	notAttack = false;
	notAttackTime = 0.0f;
	notAttackTimeMax = 0.0f;
}

// �I����
void BattleEnemyController::Finalize()
{

}

// �X�V
void BattleEnemyController::Update(BattleEnemyState battleEnemyState, Meta::MetaData metaData, Phoenix::f32 elapsedTime)
{
	if (enemyManager->GetAliveEnemyCount() <= 0) return;
	if (enemyManager->GetBattleEnemyCount() <= 0) return;

	//bool inBattle = false;
	bool attackRight = true;
	bool dedgeRight = true;

	Phoenix::s32 index = -1;
	Phoenix::s32 nearEnemyIndex = -1;
	std::vector<Phoenix::s32> indices;

	Phoenix::f32 len = 0.0f;

	const auto& enemies = enemyManager->GetEnemies();

	switch (battleEnemyState)
	{
	case BattleEnemyState::Idle:
		for (const auto& enemy : enemies)
		{
			if (!enemy) continue;
			if (!enemy->GetAlive()) continue;
			if (!enemy->GetInBattle()) continue;

			if (enemy->GetBattleState() == BattleEnemyState::Idle)
			{
				if (enemy->InBattleTerritory() && (enemy->GetTypeTag() == Enemy::TypeTag::Small))
				{
					enemy->SetState(BattleEnemyState::Walk);
				}
				else
				{
					enemy->SetState(BattleEnemyState::Run);
				}
			}
		}
		break;

	case BattleEnemyState::Run:
		for (const auto& enemy : enemies)
		{
			if (!enemy) continue;
			if (!enemy->GetAlive()) continue;
			if (!enemy->GetInBattle()) continue;

			if (enemy->GetBattleState() == BattleEnemyState::Idle)
			{
				if (enemy->InBattleTerritory() && (enemy->GetTypeTag() == Enemy::TypeTag::Small))
				{
					enemy->SetState(BattleEnemyState::Walk);
				}
				else
				{
					enemy->SetState(BattleEnemyState::Run);
				}
			}
		}
		break;

	case BattleEnemyState::Attack:
		for (const auto& enemy : enemies)
		{
			if (!enemy) continue;

			++index;

			if (!enemy->GetAlive()) continue;
			if (!enemy->GetInBattle()) continue;
			if (enemy->GetBattleState() == BattleEnemyState::DamageSmall) continue;
			if (enemy->GetBattleState() == BattleEnemyState::DamageBig) continue;

			if (enemy->GetBattleState() == BattleEnemyState::Attack)
			{
				attackRight = false;
			}

			if (enemy->GetBattleState() == BattleEnemyState::Idle || enemy->GetBattleState() == BattleEnemyState::Walk || enemy->GetBattleState() == BattleEnemyState::Run)
			{
				indices.emplace_back(index);
			}
		}

		// �S�ẴG�l�~�[���U���ȊO�̏ꍇ
		if (attackRight)
		{
			if (!notAttack && 0 < indices.size())
			{
				Phoenix::s32 r = rand() % static_cast<Phoenix::s32>(indices.size());
				{
					notAttack = enemyManager->SetAttackRight(indices.at(r), (enemyManager->GetAliveEnemyCount() == 1));
					notAttackTimeMax = static_cast<Phoenix::f32>(10 + rand() % 90);
					notAttackTimeMax += static_cast<Phoenix::f32>(10 + rand() % 10);
				}

				// �U���������s����Ȃ������G�l�~�[�͑���Ɉڍs
				for (int i = 0; i < indices.size(); ++i)
				{
					if (i == r && notAttack) continue;

					const auto& enemy = enemies.at(indices.at(i));

					if (!enemy) continue;
					if (!enemy->GetAlive()) continue;
					if (!enemy->GetInBattle()) continue;

					if (enemy->GetBattleState() == BattleEnemyState::Idle)
					{
						if (enemy->InBattleTerritory() && (enemy->GetTypeTag() == Enemy::TypeTag::Small))
						{
							enemy->SetState(BattleEnemyState::Walk);
						}
						else
						{
							enemy->SetState(BattleEnemyState::Run);
						}
					}
				}
			}
			else if (notAttack)
			{
				if (notAttackTimeMax <= notAttackTime)
				{
					notAttack = false;
					notAttackTime = 0.0f;
				}
				else
				{
					notAttackTime += 1.0f * elapsedTime;
				}
			}
		}

		break;

	case BattleEnemyState::Dedge:
		for (const auto& enemy : enemies)
		{
			if (!enemy) continue;

			++index;

			if (!enemy->GetAlive()) continue;
			if (!enemy->GetInBattle()) continue;
			if (!enemy->InBattleTerritory()) continue;
			if (!enemy->JudgePlayerAttackRange()) continue;

			if (enemy->GetBattleState() == BattleEnemyState::Dedge)
			{
				dedgeRight = false;
			}

			if (enemy->GetBattleState() != BattleEnemyState::Dedge
			 && enemy->GetBattleState() != BattleEnemyState::DamageSmall
			 && enemy->GetBattleState() != BattleEnemyState::DamageBig
			 && enemy->GetBattleState() != BattleEnemyState::Attack)
			{
				Phoenix::f32 dis = enemy->GetDistanceToPlayer();

				if (dis < len || len == 0.0f)
				{
					nearEnemyIndex = index;
					len = dis;
				}
			}
		}

		if (dedgeRight && 0 <= nearEnemyIndex && nearEnemyIndex < enemies.size())
		{
			enemies.at(nearEnemyIndex)->SetState(BattleEnemyState::Dedge);
		}
		break;

	default: break;
	}

	// �����Ă���G�l�~�[�𕪂��ăv���C���[���͂�
	for (const auto& enemy : enemies)
	{
		if (!enemy) continue;

		if (enemy->GetBattleState() == BattleEnemyState::Run)
		{
			// �G�l�~�[�ƃv���C���[�̒����������v��

			// �ŒZ�̃G�l�~�[�͂��̂܂ܒǔ��A���̑��̓v���C���[�̐i�s�����ƕ��s����ɃS�[����ύX

			// 
		}
	}

	// HTN�p���[���h�X�e�[�g�X�V
	for (const auto& enemy : enemyManager->GetEnemies())
	{
		if (!enemy) continue;

		std::shared_ptr<EnemyWorldState> worldState = enemy->GetWorldState();
		{
			worldState->enemyState = enemy->GetBattleState();
			worldState->playing = enemy->GetModel()->IsPlaying();
			worldState->inTerritory = enemy->InBattleTerritory();
			worldState->inDistanceHit = enemy->InDistanceHitByAttack();
			worldState->hasAttackRight = enemy->GetAttackRight();
			worldState->playerAttacking = (metaData.playerState == static_cast<Phoenix::s32>(Player::AnimationState::Attack));
		}
	}
}

// �G�l�~�[�}�l�[�W���[�ݒ�
void BattleEnemyController::SetEnemyManager(std::shared_ptr<EnemyManager> enemyManager)
{
	this->enemyManager = enemyManager;
}