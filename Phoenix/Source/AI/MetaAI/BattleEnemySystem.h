#pragma once

#include "MetaAISystem.h"
#include "PlayerSkillAnalyzer.h"
#include "DynamicDifficultyAdjuster.h"
#include "BattleEnemyController.h"
#include "../../Enemy/EnemyManager.h"
#include "../../Player/Player.h"
#include "../../Enemy/EnemyState.h"


namespace Meta
{
	/*
	class BattleEnemySystem : public IMetaAISystem
	{
	private:
		std::unique_ptr<PlayerSkillAnalyzer> playerSkillAnalyzer;
		std::unique_ptr<DynamicDifficultyAdjuster> dynamicDifficultyAdjuster;
		std::unique_ptr<BattleEnemyController> battleEnemyController;

		bool constructed = false;

	public:
		BattleEnemySystem() {}
		~BattleEnemySystem() override {}

	public:
		// ����
		static std::shared_ptr<BattleEnemySystem> Create();

		// �R���X�g���N�^
		void Construct() override;

		// ������
		void Initialize() override;

		// �I����
		void Finalize() override;

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="score"> : �v���C���[�s���̍��v�X�R�A </param>
		/// <param name="elapsedTime"> : �o�ߎ��� </param>
		void Update(Phoenix::s32 score, Phoenix::f32 elapsedTime) override;

		/// <summary>
		/// �Ǘ��N���X�̐ݒ�
		/// </summary>
		/// <param name="manager"> : �Ǘ��N���X�̎��� </param>
		void SetManager(std::shared_ptr<EnemyManager> manager);
	};
	*/

	// �Q�[���V�[������K�v�Œ���̃f�[�^
	struct MetaData
	{
		Phoenix::Math::Vector3 playerPos;
		Phoenix::s32 playerState = -1;
		Phoenix::s32 playerAttackState = -1;
		bool playerIsInvincible = false;

		std::vector<Phoenix::Math::Vector3> enemiesPos;
		std::vector<Phoenix::s32> enemiesState;
		std::vector<Phoenix::s32> enemiesType;
	};

	class BattleEnemySystem : public IMetaAISystem
	{
	private:
		// �Q�[���V�[������̃f�[�^
		MetaData metaData;

		// �G�l�~�[�Ǘ�
		std::shared_ptr<EnemyManager> enemyManager;

		Phoenix::s32 prevPlayerAttackState = -1; // �O��̃v���C���[�U�����

		std::vector<bool> canDedge; // �e�G�l�~�[����𔻒�
		std::vector<bool> attackWait; // �U���ҋ@��
		std::vector<Phoenix::f32> attackWaitCnt; // ���̍U���������s�����܂ł̑ҋ@�J�E���g
		std::vector<Phoenix::f32> attackWaitMaxCnt; // ���̍U���������s�����܂ł̑ҋ@�ő�J�E���g

	public:
		BattleEnemySystem() {}
		~BattleEnemySystem() {}

	public:
		// ����
		static std::shared_ptr<BattleEnemySystem> Create();

		// �R���X�g���N�^
		void Construct() override;

		// ������
		void Initialize() override;

		// �I����
		void Finalize() override;

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="elapsedTime"> : �o�ߎ��� </param>
		void Update(Phoenix::f32 elapsedTime) override;

		/// <summary>
		/// �Q�[�����烁�^AI���N������֐�
		/// </summary>
		/// <param name="metaData"> : �Q�[�����烁�^AI�ŕK�v�ȃf�[�^ </param>
		void Sensor(MetaData metaData);

		/// <summary>
		/// �Ǘ��N���X�̐ݒ�
		/// </summary>
		/// <param name="manager"> : �Ǘ��N���X�̎��� </param>
		void SetManager(std::shared_ptr<EnemyManager> manager);

		// �v���C���[�ƃG�l�~�[�̋���
		Phoenix::f32 DistancePlayerAndEnemy(Phoenix::Math::Vector3 playerPos, Phoenix::Math::Vector3 enemyPos);

		// �v���C���[�̐퓬�G���A�ɐN��������
		bool InBattleTerritory(Phoenix::f32 distance);

		// �v���C���[�ɍU���������鋗���ɓ����Ă��邩�H
		bool InDistanceHitByAttack(Phoenix::f32 distance, Phoenix::s32 enemyType);

		// �G�l�~�[�̏�Ԃ��炨�����߂̃G�l�~�[�̃X�e�[�g��Ԃ�
		BattleEnemyState RecommendEnemyStateByEnemy(BattleEnemyState currentEnemyState, Phoenix::s32 enemyType, bool inBattleTerritory, bool inDistanceHitByAttack);

		// �v���C���[�̏�Ԃ��炨�����߂̃G�l�~�[�̃X�e�[�g��Ԃ�
		BattleEnemyState RecommendEnemyStateByPlayer(Player::AnimationState currentPlayerState, BattleEnemyState currentEnemyState, Phoenix::s32 enemyType, bool& canDedge, bool inBattleTerritory, bool inDistanceHitByAttack);

		// �V���ȃG�l�~�[�B�̏�Ԃ�����
		std::vector<Phoenix::s32>& GetNewEnemiesState();
	};
}