#pragma once

#include "Phoenix/Math/PhoenixMath.h"


class IMetaAISystem
{
public:
	virtual ~IMetaAISystem() {}

public:
	// コンストラクタ
	virtual void Construct() = 0;

	// 初期化
	virtual void Initialize() = 0;

	// 終了化
	virtual void Finalize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="elapsedTime"> : 経過時間 </param>
	virtual void Update(Phoenix::f32 elapsedTime) = 0;
};