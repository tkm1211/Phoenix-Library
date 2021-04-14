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

	static constexpr Phoenix::f32 startTimeMax = 100.0f;

private:
	Phoenix::Math::Vector2 size = Phoenix::Math::Vector2(0.0f, 0.0f);
	Phoenix::Math::Vector2 hpTexPos = Phoenix::Math::Vector2(0.0f, 0.0f);

	Phoenix::Math::Vector2 pos = Phoenix::Math::Vector2(0.0f, 0.0f);

	bool exit = false;

	Phoenix::Math::Vector2 damageSize = Phoenix::Math::Vector2(0.0f, 0.0f);
	Phoenix::Math::Vector2 damageTexPos = Phoenix::Math::Vector2(0.0f, 0.0f);

	Phoenix::f32 hpPercent = 0.0f;
	Phoenix::f32 damagePercent = 0.0f;
	Phoenix::f32 startTime = 0.0f;

	bool downDamageGage = false;

public:
	EnemyUI() {}
	~EnemyUI() {}

public:
	static std::shared_ptr<EnemyUI> Create();

	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice) override {}

	void Initialize() override;

	void Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad) override;

public:
	/// <summary>
	/// HP更新
	/// </summary>
	/// <param name="hpPercent">0 ~ 100 %</param>
	void Update(Phoenix::f32 hpPercent, Phoenix::f32 elapsedTime);

	// サイズ取得
	Phoenix::Math::Vector2 GetSize();

	// 切り取り位置を取得
	Phoenix::Math::Vector2 GetHPTexPos();

	// ダメージ用サイズ取得
	Phoenix::Math::Vector2 GetDamageSize();

	// ダメージ用切り取り位置を取得
	Phoenix::Math::Vector2 GetDamageTexPos();

	// キャラ頭上に表示
	bool GetExit();

	// キャラ頭上の座標
	Phoenix::Math::Vector2 GetPos();

	// キャラ頭上に表示
	void SetExit(bool exit);

	// キャラ頭上の座標
	void SetPos(Phoenix::Math::Vector2 pos);
};