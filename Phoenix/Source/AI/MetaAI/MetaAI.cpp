#include "MetaAI.h"


// ����
std::unique_ptr<MetaAI> MetaAI::Create()
{
	return std::make_unique<MetaAI>();
}

// �R���X�g���N�^
void MetaAI::Construct()
{
	for (const auto& [key, value] : systems)
	{
		value->Construct();
	}
}

// ������
void MetaAI::Initialize()
{
	for (const auto& [key, value] : systems)
	{
		value->Initialize();
	}
}

// �I����
void MetaAI::Finalize()
{
	for (const auto& [key, value] : systems)
	{
		value->Initialize();
	}
}

// �X�V
void MetaAI::Update()
{
	if (500 <= notUpdatedTime)
	{
		systems[0]->Update(score);
		notUpdatedTime = 0;
	}
	else if (100 <= notUpdatedTime++)
	{
		systems[0]->Update(-10);
	}
	if (!canUpdate || type == -1) return;

	systems[type]->Update(score);

	canUpdate = false;
	type = -1;
	previousScore = score;
	score = 0;
	notUpdatedTime = 0;
}

// �Q�[�����烁�^AI���N������֐�
void MetaAI::Sensor(Phoenix::s32 type, Phoenix::s32 score)
{
	canUpdate = true;
	this->type = type;
	this->score = score;
	notUpdatedTime = 0;
}