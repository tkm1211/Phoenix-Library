#pragma once

#include "Phoenix/Math/PhoenixMath.h"


class PlayerSkillAnalyzer
{
private:
	Phoenix::s32 playerSkillLevel = 1;
	Phoenix::s32 playerSkillLevelRange = 10;
	Phoenix::s32 levelUpExperiencePoint = 10;

public:
	PlayerSkillAnalyzer() {}
	~PlayerSkillAnalyzer() {}

public:
	// ����
	static std::unique_ptr<PlayerSkillAnalyzer> Create();

	// �R���X�g���N�^
	void Construct();

	// ������
	void Initialize();

	// �I����
	void Finalize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="score"> : �v���C���[�s���̍��v�X�R�A </param>
	void Update(Phoenix::s32 score);

	// ���͂����v���C���[���x���̎擾
	Phoenix::s32 GetSkillLevel();
};