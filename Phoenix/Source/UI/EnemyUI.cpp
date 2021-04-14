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

	damageSize = Phoenix::Math::Vector2(SizeWidth, SizeHeigth);
	damageTexPos = Phoenix::Math::Vector2(TexWidth, TexHeigth);

	hpPercent = 100.0f;
	damagePercent = 100.0f;
	startTime = 0.0f;

	downDamageGage = false;
}

void EnemyUI::Update(Phoenix::f32 hpPercent, Phoenix::f32 elapsedTime)
{
	if (this->hpPercent != hpPercent)
	{
		damagePercent = this->hpPercent;
		startTime = startTimeMax;
		downDamageGage = false;
	}

	if (0.0f < startTime)
	{
		startTime -= 1.0f * elapsedTime;
	}
	else if (damagePercent != hpPercent)
	{
		downDamageGage = true;
		startTime = 0.0f;
	}

	if (downDamageGage)
	{
		damagePercent = Phoenix::Math::f32Lerp(damagePercent, hpPercent, 0.05f * elapsedTime);

		if ((damagePercent - hpPercent) <= 0.01f)
		{
			downDamageGage = false;
			damagePercent = hpPercent;
		}
	}

	size.x = SizeWidth * (hpPercent / 100.0f);
	hpTexPos.x = TexWidth * (hpPercent / 100.0f);

	damageSize.x = SizeWidth * (damagePercent / 100.0f);
	damageTexPos.x = TexWidth * (damagePercent / 100.0f);

	this->hpPercent = hpPercent;
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

// ダメージ用サイズ取得
Phoenix::Math::Vector2 EnemyUI::GetDamageSize()
{
	return damageSize;
}

// ダメージ用切り取り位置を取得
Phoenix::Math::Vector2 EnemyUI::GetDamageTexPos()
{
	return damageTexPos;
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