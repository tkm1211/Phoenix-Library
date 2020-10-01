#include "TargetMarkUI.h"
#include "Phoenix/OS/Path.h"


std::shared_ptr<TargetMarkUI> TargetMarkUI::Create()
{
	return std::make_unique<TargetMarkUI>();
}

void TargetMarkUI::Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	mark = Phoenix::Graphics::ITexture::Create();
	mark->Initialize(graphicsDevice->GetDevice(), Phoenix::OS::Path::Combine(Phoenix::OS::Path::GetCurrentDirectory(), "..\\Data\\Assets\\Texture\\UI\\TargetMark\\TargetMark02.png"), Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color(1.0f, 1.0f, 1.0f, 1.0f));

	auto SetMark = [&](TargetDataNames name, Phoenix::Math::Vector2 startPos, Phoenix::Math::Vector2 goalPos, Phoenix::Math::Vector2 texPos)
	{
		TargetData data;

		data.pos = startPos;
		data.goalPos = goalPos;
		data.texPos = texPos;

		marks.insert(std::make_pair(name, data));
	};
	SetMark(leftUp, Phoenix::Math::Vector2(-StartPosX, -StartPosY), Phoenix::Math::Vector2(-GoalPosX, -GoalPosY), Phoenix::Math::Vector2(TexWidth * 0.0f, 0.0f));
	SetMark(leftDown, Phoenix::Math::Vector2(-StartPosX, StartPosY), Phoenix::Math::Vector2(-GoalPosX, GoalPosY), Phoenix::Math::Vector2(TexWidth * 2.0f, 0.0f));
	SetMark(rightUp, Phoenix::Math::Vector2(StartPosX, -StartPosY), Phoenix::Math::Vector2(GoalPosX, -GoalPosY), Phoenix::Math::Vector2(TexWidth * 1.0f, 0.0f));
	SetMark(rightDown, Phoenix::Math::Vector2(StartPosX, StartPosY), Phoenix::Math::Vector2(GoalPosX, GoalPosY), Phoenix::Math::Vector2(TexWidth * 3.0f, 0.0f));

	SetMark(center, Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(0.0f, 0.0f), Phoenix::Math::Vector2(TexWidth * 6.0f, 0.0f));

	SetMark(centerUp, Phoenix::Math::Vector2(0.0f, -StartPosY - (SizeY * 0.5f)), Phoenix::Math::Vector2(0.0f, -GoalPosY - (SizeY * 0.5f)), Phoenix::Math::Vector2(TexWidth * 4.0f, 0.0f));
	SetMark(centerDown, Phoenix::Math::Vector2(0.0f, StartPosY + (SizeY * 0.5f)), Phoenix::Math::Vector2(0.0f, GoalPosY + (SizeY * 0.5f)), Phoenix::Math::Vector2(TexWidth * 4.0f, 0.0f));
	SetMark(centerLeft, Phoenix::Math::Vector2(-StartPosX - (SizeX * 0.5f), 0.0f), Phoenix::Math::Vector2(-GoalPosX - (SizeX * 0.5f), 0.0f), Phoenix::Math::Vector2(TexWidth * 5.0f, 0.0f));
	SetMark(centerRight, Phoenix::Math::Vector2(StartPosX + (SizeX * 0.5f), 0.0f), Phoenix::Math::Vector2(GoalPosX + (SizeX * 0.5f), 0.0f), Phoenix::Math::Vector2(TexWidth * 5.0f, 0.0f));
}

void TargetMarkUI::Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad)
{
	if (!isUpdate) return;

	for (const auto& it : marks)
	{
		quad->Draw(graphicsDevice, mark.get(), originPos + it.second.pos - (Phoenix::Math::Vector2(TexWidth, TexHeigth) * 0.5f), Phoenix::Math::Vector2(SizeX, SizeY) * 0.5f, it.second.texPos, Phoenix::Math::Vector2(TexWidth, TexHeigth));
	}
}

void TargetMarkUI::MarkInitialize()
{
	marks.at(leftUp).pos = Phoenix::Math::Vector2(-StartPosX, -StartPosY);
	marks.at(leftDown).pos = Phoenix::Math::Vector2(-StartPosX, StartPosY);
	marks.at(rightUp).pos = Phoenix::Math::Vector2(StartPosX, -StartPosY);
	marks.at(rightDown).pos = Phoenix::Math::Vector2(StartPosX, StartPosY);

	marks.at(centerUp).pos = Phoenix::Math::Vector2(0.0f, -StartPosY - (SizeY * 0.5f));
	marks.at(centerDown).pos = Phoenix::Math::Vector2(0.0f, StartPosY + (SizeY * 0.5f));
	marks.at(centerLeft).pos = Phoenix::Math::Vector2(-StartPosX - (SizeX * 0.5f), 0.0f);
	marks.at(centerRight).pos = Phoenix::Math::Vector2(StartPosX + (SizeX * 0.5f), 0.0f);
}

void TargetMarkUI::Update(Phoenix::Math::Vector2 targetPos)
{
	if (!isUpdate) return;

	originPos = targetPos;

	originPos.x = static_cast<Phoenix::f32>(static_cast<Phoenix::s32>(originPos.x));
	originPos.y = static_cast<Phoenix::f32>(static_cast<Phoenix::s32>(originPos.y));

	for (auto& it : marks)
	{
		it.second.pos = Phoenix::Math::Vector2Lerp(it.second.pos, it.second.goalPos, 0.1f);

		it.second.pos.x = static_cast<Phoenix::f32>(static_cast<Phoenix::s32>(it.second.pos.x));
		it.second.pos.y = static_cast<Phoenix::f32>(static_cast<Phoenix::s32>(it.second.pos.y));
	}

	Phoenix::f32 len = Phoenix::Math::Vector2Length(marks.at(leftUp).pos - marks.at(leftUp).goalPos);
	if (len <= 1e-8f) isUpdate = false;
}

void TargetMarkUI::LockOnTarget()
{
	isUpdate = true;
	MarkInitialize();
}