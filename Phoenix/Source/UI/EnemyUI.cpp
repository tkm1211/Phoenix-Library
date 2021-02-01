#include "EnemyUI.h"
#include "Phoenix/OS/Path.h"
#include "Phoenix/FND/Util.h"


std::shared_ptr<EnemyUI> EnemyUI::Create()
{
	return std::make_shared<EnemyUI>();
}

void EnemyUI::Initialize()
{
	size = Phoenix::Math::Vector2(SizeWidth, SizeHeigth);
	hpTexPos = Phoenix::Math::Vector2(TexWidth, TexHeigth);
}

void EnemyUI::Update(Phoenix::f32 hpPercent)
{
	size.x = SizeWidth * (hpPercent / 100.0f);
	hpTexPos.x = TexWidth * (hpPercent / 100.0f);
}

// サイズ取得
Phoenix::Math::Vector2 EnemyUI::GetSize()
{
	return size;
}

// 切り取り位置を取得
Phoenix::Math::Vector2 EnemyUI::GetHPTexPos()
{
	return hpTexPos;
}

// キャラ頭上に表示
bool EnemyUI::GetExit()
{
	return exit;
}

// キャラ頭上の座標
Phoenix::Math::Vector2 EnemyUI::GetPos()
{
	return pos;
}

// キャラ頭上に表示
void EnemyUI::SetExit(bool exit)
{
	this->exit = exit;
}

// キャラ頭上の座標
void EnemyUI::SetPos(Phoenix::Math::Vector2 pos)
{
	this->pos = pos;
}

void EnemyUI::Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad)
{
}