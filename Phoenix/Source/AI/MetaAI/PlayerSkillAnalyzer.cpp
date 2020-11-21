#include "PlayerSkillAnalyzer.h"


// ����
std::unique_ptr<PlayerSkillAnalyzer> PlayerSkillAnalyzer::Create()
{
	return std::make_unique<PlayerSkillAnalyzer>();
}

// �R���X�g���N�^
void PlayerSkillAnalyzer::Construct()
{

}

// ������
void PlayerSkillAnalyzer::Initialize()
{
	playerSkillLevel = 1;
	downScore = false;
}

// �I����
void PlayerSkillAnalyzer::Finalize()
{
	playerSkillLevel = 1;
}

// �X�V
void PlayerSkillAnalyzer::Update(Phoenix::s32 score)
{
	downScore = (score - oldPlayerScore) < 0 ? true : false;
	playerSkillLevel = score / levelUpExperiencePoint;

	if (playerSkillLevelRange <= playerSkillLevel)
	{
		playerSkillLevel = playerSkillLevelRange;
	}

	oldPlayerSkillLevel = playerSkillLevel;
	oldPlayerScore = score;
}

// ���͂����v���C���[���x���̎擾
Phoenix::s32 PlayerSkillAnalyzer::GetSkillLevel()
{
	return playerSkillLevel;
}

// ���͂����v���C���[���x�����������Ă��邩�擾
bool PlayerSkillAnalyzer::GetDownScore()
{
	return downScore;
}