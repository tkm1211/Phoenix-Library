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

// �T�C�Y�擾
Phoenix::Math::Vector2 EnemyUI::GetSize()
{
	return size;
}

// �؂���ʒu���擾
Phoenix::Math::Vector2 EnemyUI::GetHPTexPos()
{
	return hpTexPos;
}

// �L��������ɕ\��
bool EnemyUI::GetExit()
{
	return exit;
}

// �L��������̍��W
Phoenix::Math::Vector2 EnemyUI::GetPos()
{
	return pos;
}

// �L��������ɕ\��
void EnemyUI::SetExit(bool exit)
{
	this->exit = exit;
}

// �L��������̍��W
void EnemyUI::SetPos(Phoenix::Math::Vector2 pos)
{
	this->pos = pos;
}

void EnemyUI::Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad)
{
}