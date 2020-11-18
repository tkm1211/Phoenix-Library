#include "EnemyUI.h"
#include "Phoenix/OS/Path.h"
#include "Phoenix/FND/Util.h"


std::shared_ptr<EnemyUI> EnemyUI::Create()
{
	return std::make_shared<EnemyUI>();
}

void EnemyUI::Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
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

void EnemyUI::Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad)
{
}