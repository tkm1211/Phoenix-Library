#pragma once

#include "Phoenix/Math/PhoenixMath.h"


class IMetaAISystem
{
public:
	IMetaAISystem() {}
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
	/// <param name="score"> : プレイヤー行動の合計スコア </param>
	virtual void Update(Phoenix::s32 score) = 0;
};