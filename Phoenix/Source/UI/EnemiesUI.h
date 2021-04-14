#pragma once

#include <memory>
#include "UI.h"
#include "Phoenix/Graphics/Texture.h"
#include "Phoenix/Math/Vector.h"


class EnemyUI;
class EnemiesUI : public UI
{
private:
	std::vector<std::shared_ptr<EnemyUI>> uiList;

	std::shared_ptr<Phoenix::Graphics::ITexture> hp;
	std::shared_ptr<Phoenix::Graphics::ITexture> damage;
	std::shared_ptr<Phoenix::Graphics::ITexture> hpBack;

	Phoenix::Math::Vector2 pos = Phoenix::Math::Vector2(0.0f, 0.0f);

	Phoenix::s32 currentIndex = 0;

public:
	EnemiesUI() {}
	~EnemiesUI() {}

public:
	static std::shared_ptr<EnemiesUI> Create();

	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice) override;

	void Initialize() override {}
	void Initialize(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::s32 enemyRange);

	void Draw(Phoenix::Graphics::IGraphicsDevice* graphicsDevice, Phoenix::FrameWork::Quad* quad) override;

public:
	/// <summary>
	/// HP更新
	/// </summary>
	/// <param name="index"> エネミー要素数 </param>
	/// <param name="hpPercent"> 0 ~ 100 % </param>
	/// <param name="elapsedTime">経過時間</param>
	void Update(Phoenix::s32 index, Phoenix::f32 hpPercent, Phoenix::f32 elapsedTime);
	
	/// <summary>
	/// UIを追加
	/// </summary>
	/// <param name="index"> エネミー要素数 </param>
	/// <param name="ui"> エネミーUI </param>
	void AddUI(Phoenix::s32 index, std::shared_ptr<EnemyUI> ui);
};