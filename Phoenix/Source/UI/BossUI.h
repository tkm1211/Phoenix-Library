#pragma once

#include <memory>
#include "UI.h"
#include "Phoenix/Graphics/Texture.h"
#include "Phoenix/Math//Vector.h"


class BossUI : public UI
{
private:
	/*static constexpr*/ Phoenix::f32 TexWidth = 640.0f;
	/*static constexpr*/ Phoenix::f32 TexHeigth = 64.0f;

private:
	std::shared_ptr<Phoenix::Graphics::ITexture> hp;
	std::shared_ptr<Phoenix::Graphics::ITexture> hpBack;

	Phoenix::Math::Vector2 pos = Phoenix::Math::Vector2(0.0f, 0.0f);
	Phoenix::Math::Vector2 size = Phoenix::Math::Vector2(0.0f, 0.0f);
	Phoenix::Math::Vector2 hpTexPos = Phoenix::Math::Vector2(0.0f, 0.0f);

public:
	BossUI() {}
	~BossUI() {}

public:
	static std::shared_ptr<BossUI> Create();

	void Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice) override;

	void Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad) override;

public:
	/// <summary>
	/// HP�X�V
	/// </summary>
	/// <param name="hpPercent">0 ~ 100 %</param>
	void Update(Phoenix::f32 hpPercent);
};