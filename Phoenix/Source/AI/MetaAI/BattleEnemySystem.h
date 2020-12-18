#pragma once

#include "MetaAISystem.h"
#include "PlayerSkillAnalyzer.h"
#include "DynamicDifficultyAdjuster.h"
#include "BattleEnemyController.h"
#include "../../Enemy/EnemyManager.h"


namespace Meta
{
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
}