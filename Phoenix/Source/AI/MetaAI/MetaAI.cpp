#include "MetaAI.h"


// 生成
std::unique_ptr<MetaAI> MetaAI::Create()
{
	return std::make_unique<MetaAI>();
}

// コンストラクタ
void MetaAI::Construct()
{
	for (const auto& [key, value] : systems)
	{
		value->Construct();
	}
}

// 初期化
void MetaAI::Initialize()
{
	for (const auto& [key, value] : systems)
	{
		value->Initialize();
	}

	notUpdatedTime = 0;
}

// 終了化
void MetaAI::Finalize()
{
	for (const auto& [key, value] : systems)
	{
		value->Initialize();
	}
}

// 更新
void MetaAI::Update()
{
	//if (!canUpdate || type == -1) return;
	if (type == -1) return;

	systems[type]->Update(score);

	canUpdate = false;
	type = -1;
	previousScore = score;
	score = 0;
	notUpdatedTime = 0;
}

// ゲームからメタAIを起動する関数
void MetaAI::Sensor(Phoenix::s32 type, Phoenix::s32 score)
{
	canUpdate = true;
	this->type = type;
	this->score = score;
	notUpdatedTime = 0;
}