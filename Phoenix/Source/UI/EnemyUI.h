#pragma once

#include <memory>
#include "UI.h"
#include "Phoenix/Graphics/Texture.h"
#include "Phoenix/Math//Vector.h"


class EnemyUI : public UI
{
public:
#if (defined(DEBUG) | defined(_DEBUG))
	static constexpr Phoenix::f32 SizeWidth = 640.0f;
	static constexpr Phoenix::f32 SizeHeigth = 64.0f;
#else
	static constexpr Phoenix::f32 SizeWidth = 640.0f * 1.5f;
	static constexpr Phoenix::f32 SizeHeigth = 64.0f * 1.5f;
#endif

	static constexpr Phoenix::f32 TexWidth = 640.0f;
	static constexpr Phoenix::f32 TexHeigth = 64.0f;

private:
	Phoenix::Math::Vector2 size = Phoenix::Math::Vector2(0.0f, 0.0f);
	Phoenix::Math::Vector2 hpTexPos = Phoenix::Math::Vector2(0.0f, 0.0f);

public:
	EnemyUI() {}
	~EnemyUI() {}

public:
	static std::shared_ptr<EnemyUI> Create();

	void Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice) override;

	void Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad) override;

public:
	/// <summary>
	/// HP更新
	/// </summary>
	/// <param name="hpPercent">0 ~ 100 %</param>
	void Update(Phoenix::f32 hpPercent);

	// サイズ取得
	Phoenix::Math::Vector2 GetSize();

	// 切り取り位置を取得
	Phoenix::Math::Vector2 GetHPTexPos();
};