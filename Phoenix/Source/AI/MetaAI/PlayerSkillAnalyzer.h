#pragma once

#include "Phoenix/Math/PhoenixMath.h"


class PlayerSkillAnalyzer
{
private:
	Phoenix::s32 playerSkillLevel = 1;
	Phoenix::s32 playerSkillLevelRange = 100;
	Phoenix::s32 levelUpExperiencePoint = 10;

	Phoenix::s32 oldPlayerSkillLevel = 1;
	Phoenix::s32 oldPlayerScore = 0;

	bool downScore = false;

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
	/// <param name="elapsedTime"> : �o�ߎ��� </param>
	void Update(Phoenix::s32 score, Phoenix::f32 elapsedTime);

	// ���͂����v���C���[���x���̎擾
	Phoenix::s32 GetSkillLevel();

	// ���͂����v���C���[���x�����������Ă��邩�擾
	bool GetDownScore();
};