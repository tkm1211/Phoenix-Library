#include "BattleEnemySystem.h"


namespace Meta
{
	// ����
	std::shared_ptr<BattleEnemySystem> BattleEnemySystem::Create()
	{
		return std::make_shared<BattleEnemySystem>();
	}

	/*
	// �R���X�g���N�^
	void BattleEnemySystem::Construct()
	{
		if (constructed) return;

		playerSkillAnalyzer = PlayerSkillAnalyzer::Create();
		dynamicDifficultyAdjuster = DynamicDifficultyAdjuster::Create();
		battleEnemyController = BattleEnemyController::Create();

		playerSkillAnalyzer->Construct();
		dynamicDifficultyAdjuster->Construct();
		battleEnemyController->Construct();

		constructed = true;
	}

	// ������
	void BattleEnemySystem::Initialize()
	{
		playerSkillAnalyzer->Initialize();
		dynamicDifficultyAdjuster->Initialize();
		battleEnemyController->Initialize();
	}

	// �I����
	void BattleEnemySystem::Finalize()
	{
		battleEnemyController->Initialize();
		dynamicDifficultyAdjuster->Initialize();
		playerSkillAnalyzer->Initialize();
	}

	// �X�V
	void BattleEnemySystem::Update(Phoenix::s32 score, Phoenix::f32 elapsedTime)
	{
		playerSkillAnalyzer->Update(score, elapsedTime);
		dynamicDifficultyAdjuster->Update(playerSkillAnalyzer->GetSkillLevel(), playerSkillAnalyzer->GetDownScore(), elapsedTime);
		battleEnemyController->Update(dynamicDifficultyAdjuster->GetBattleEnemyState(), elapsedTime);
	}

	// �Ǘ��N���X�̐ݒ�
	void BattleEnemySystem::SetManager(std::shared_ptr<EnemyManager> manager)
	{
		battleEnemyController->SetEnemyManager(manager);
	};
	*/

	// �R���X�g���N�^
	void BattleEnemySystem::Construct()
	{

	}

	// ������
	void BattleEnemySystem::Initialize()
	{
		prevPlayerAttackState = -1;
	}

	// �I����
	void BattleEnemySystem::Finalize()
	{
		
	}

	// �X�V
	void BattleEnemySystem::Update(Phoenix::f32 elapsedTime)
	{
		if (metaData.playerState == -1) return; // �O�̂��߂�

		Phoenix::sizeT waitSize = attackWait.size();
		Phoenix::sizeT enemiesSize = metaData.enemiesState.size();
		if (waitSize < enemiesSize)
		{
			canDedge.resize(enemiesSize);
			attackWait.resize(enemiesSize);
			attackWaitCnt.resize(enemiesSize);
			attackWaitMaxCnt.resize(enemiesSize);

			for (Phoenix::sizeT i = waitSize; i < enemiesSize; ++i)
			{
				canDedge.at(i) = true;
				attackWait.at(i) = false;
				attackWaitCnt.at(i) = 0.0f;
				attackWaitMaxCnt.at(i) = 0.0f;
			}
		}

		if (prevPlayerAttackState != metaData.playerAttackState)
		{
			for (Phoenix::sizeT i = 0; i < canDedge.size(); ++i)
			{
				canDedge.at(i) = true;
			}
			prevPlayerAttackState = metaData.playerAttackState;
		}

		Player::AnimationState currentPlayerState = static_cast<Player::AnimationState>(metaData.playerState);
		BattleEnemyState currentEnemyState = BattleEnemyState::NoneState;
		BattleEnemyState recommendEnemyState = BattleEnemyState::NoneState;

		Phoenix::s32 enemyType = -1;

		Phoenix::f32 len = 0.0f;
		Phoenix::s32 size = static_cast<Phoenix::s32>(metaData.enemiesState.size());

		bool inBattleTerritory = false;
		bool inDistanceHitByAttack = false;
		
		bool isAttack = false;
		
		Phoenix::s32 attackEnemyIndex = -1;
		std::vector<Phoenix::s32> indices;

		for (Phoenix::s32 i = 0; i < size; ++i)
		{
			if (metaData.enemiesState.at(i) == -1) continue;

			currentEnemyState = static_cast<BattleEnemyState>(metaData.enemiesState.at(i));
			enemyType = metaData.enemiesType.at(i);

			bool dedge = canDedge.at(i);

			len = DistancePlayerAndEnemy(metaData.playerPos, metaData.enemiesPos.at(i));
			inBattleTerritory = InBattleTerritory(len);
			inDistanceHitByAttack = InDistanceHitByAttack(len, enemyType);
			recommendEnemyState = RecommendEnemyStateByPlayer(currentPlayerState, currentEnemyState, enemyType, dedge, inBattleTerritory, inDistanceHitByAttack);

			canDedge.at(i) = dedge;

			if (currentEnemyState == BattleEnemyState::Attack)
			{
				isAttack = true;
			}

			if (recommendEnemyState == BattleEnemyState::Attack)
			{
				indices.emplace_back(i);
			}
			else
			{
				metaData.enemiesState.at(i) = static_cast<Phoenix::s32>(recommendEnemyState);
			}
		}

		if (0 < indices.size())
		{
			Phoenix::s32 r = rand() % static_cast<Phoenix::s32>(indices.size());
			metaData.enemiesState.at(indices.at(r)) = static_cast<Phoenix::s32>(BattleEnemyState::Attack);
		}

		Phoenix::s32 index = -1;
		for (const auto& enemy : enemyManager->GetEnemies())
		{
			++index;

			if (!enemy) continue;
			if (!enemy->GetAlive()) continue;
			if (metaData.enemiesState.at(index) == 0) continue; // NonoState
			if (isAttack && (metaData.enemiesState.at(index) == static_cast<Phoenix::s32>(BattleEnemyState::Attack))) continue;
			if (attackWait.at(index) && (metaData.enemiesState.at(index) == static_cast<Phoenix::s32>(BattleEnemyState::Attack))) continue;
			if (metaData.playerIsInvincible && (metaData.enemiesState.at(index) == static_cast<Phoenix::s32>(BattleEnemyState::Dedge))) continue;

			BattleEnemyState state = static_cast<BattleEnemyState>(metaData.enemiesState.at(index));

			if (state != enemy->GetBattleState())
			{
				if (state == BattleEnemyState::Attack)
				{
					enemyManager->SetAttackRight(index, (enemyManager->GetAliveEnemyCount() == 1));

					attackWait.at(index) = true;
					attackWaitCnt.at(index) = 0.0f;
					attackWaitMaxCnt.at(index) = 100.0f;
				}
				else
				{
					enemy->SetState(state);
				}
			}
		}

		const auto& enemy = enemyManager->GetEnemies();
		for (Phoenix::sizeT i = 0; i < attackWait.size(); ++i)
		{
			if (attackWait.at(i) && (enemy.at(i)->GetBattleState() != BattleEnemyState::Attack))
			{
				if (attackWaitMaxCnt.at(i) <= attackWaitCnt.at(i))
				{
					attackWait.at(i) = false;
					attackWaitCnt.at(i) = 0.0f;
					attackWaitMaxCnt.at(i) = 0.0f;
				}
				else
				{
					attackWaitCnt.at(i) += 1.0f * elapsedTime;
				}
			}
		}
	}

	// �Ǘ��N���X�̐ݒ�
	void BattleEnemySystem::SetManager(std::shared_ptr<EnemyManager> manager)
	{
		this->enemyManager = manager;
	};

	// �v���C���[�ƃG�l�~�[�̋���
	Phoenix::f32 BattleEnemySystem::DistancePlayerAndEnemy(Phoenix::Math::Vector3 playerPos, Phoenix::Math::Vector3 enemyPos)
	{
		return Phoenix::Math::Vector3Length(playerPos - enemyPos);
	}

	// �v���C���[�̐퓬�G���A�ɐN��������
	bool BattleEnemySystem::InBattleTerritory(Phoenix::f32 distance)
	{
		if (distance <= 5.0f)
		{
			return true;
		}

		return false;
	}

	// �v���C���[�ɍU���������鋗���ɓ����Ă��邩�H
	bool BattleEnemySystem::InDistanceHitByAttack(Phoenix::f32 distance, Phoenix::s32 enemyType)
	{
		switch (enemyType)
		{
		case 0: // �G���G
			if (distance <= 1.5f)
			{
				return true;
			}
			break;

		case 2: // �{�X
			if (distance <= 2.5f)
			{
				return true;
			}
			break;
		default: break;
		}

		return false;
	}

	// �������߂̃X�e�[�g��Ԃ�
	BattleEnemyState BattleEnemySystem::RecommendEnemyStateByEnemy(BattleEnemyState currentEnemyState, Phoenix::s32 enemyType, bool inBattleTerritory, bool inDistanceHitByAttack)
	{
		BattleEnemyState recommendEnemyState = BattleEnemyState::NoneState;

		/*switch (currentEnemyState)
		{
		case BattleEnemyState::Idle:
			break;
		case BattleEnemyState::Walk:
			break;
		case BattleEnemyState::Run:
			break;
		case BattleEnemyState::Attack:
			break;
		case BattleEnemyState::Dedge:
			break;
		case BattleEnemyState::DamageSmall: case BattleEnemyState::DamageBig:
			break;
		default: break;
		}*/

		if (inBattleTerritory)
		{
			if (inDistanceHitByAttack)
			{
				recommendEnemyState = BattleEnemyState::Attack;
			}
			else if (enemyType == 0)
			{
				recommendEnemyState = BattleEnemyState::Walk;
			}
		}
		else
		{
			if (currentEnemyState == BattleEnemyState::Idle)
			{
				recommendEnemyState = BattleEnemyState::Run;
			}
		}

		if (currentEnemyState == BattleEnemyState::Attack)
		{
			recommendEnemyState = BattleEnemyState::NoneState;
		}

		return recommendEnemyState;
	}

	// �v���C���[�̏�Ԃ��炨�����߂̃G�l�~�[�̃X�e�[�g��Ԃ�
	BattleEnemyState BattleEnemySystem::RecommendEnemyStateByPlayer(Player::AnimationState currentPlayerState, BattleEnemyState currentEnemyState, Phoenix::s32 enemyType, bool& canDedge, bool inBattleTerritory, bool inDistanceHitByAttack)
	{
		BattleEnemyState recommendEnemyState = BattleEnemyState::NoneState;

		switch (currentPlayerState)
		{
		case Player::AnimationState::Idle:
		case Player::AnimationState::Walk:
		case Player::AnimationState::Damage:
		case Player::AnimationState::Dedge:
			recommendEnemyState = RecommendEnemyStateByEnemy(currentEnemyState, enemyType, inBattleTerritory, inDistanceHitByAttack);
			break;

		case Player::AnimationState::Attack:
			if (inBattleTerritory)
			{
				if (canDedge && (currentEnemyState != BattleEnemyState::Attack))
				{
					Phoenix::s32 r = rand() % 100;
					if (r < 65)
					{
						recommendEnemyState = BattleEnemyState::NoneState;
					}
					else
					{
						recommendEnemyState = BattleEnemyState::Dedge;
					}

					canDedge = false;
				}
			}
			else
			{
				if (currentEnemyState == BattleEnemyState::Idle)
				{
					recommendEnemyState = BattleEnemyState::Run;
				}
			}
			break;

		default: break;
		}

		return recommendEnemyState;
	}

	// �Q�[�����烁�^AI���N������֐�
	void BattleEnemySystem::Sensor(MetaData metaData)
	{
		this->metaData = metaData;
	}

	// �V���ȃG�l�~�[�B�̏�Ԃ�����
	std::vector<Phoenix::s32>& BattleEnemySystem::GetNewEnemiesState()
	{
		return metaData.enemiesState;
	}
}