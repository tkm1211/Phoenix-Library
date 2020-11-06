#pragma once

#include <map>
#include <memory>
#include "Phoenix/Math/PhoenixMath.h"
#include "MetaAISystem.h"


enum class MetaType;
class MetaAI
{
private:
	std::map<Phoenix::s32, std::shared_ptr<IMetaAISystem>> systems;

	bool canUpdate = false;
	Phoenix::s32 type = 0;
	Phoenix::s32 score = 0;
	Phoenix::s32 previousScore = 0;

public:
	MetaAI() {}
	~MetaAI() {}

public:
	// 生成
	static std::unique_ptr<MetaAI> Create();

	// コンストラクタ
	void Construct();

	// 初期化
	void Initialize();

	// 終了化
	void Finalize();

	// 更新
	void Update();

	/// <summary>
	/// ゲームからメタAIを起動する関数
	/// </summary>
	/// <param name="type"> : 動かしたいメタAIのタイプ </param>
	/// <param name="score"> : プレイヤー行動の合計スコア </param>
	void Sensor(Phoenix::s32 type, Phoenix::s32 score);

public:
	/// <summary>
	/// メタAI用のシステム追加
	/// </summary>
	/// <typeparam name="T"> : システムの型 </typeparam>
	/// <param name="type"> : システムの種類 </param>
	template <class T>
	void AddSystem(Phoenix::s32 type)
	{
		systems.insert(std::make_pair(type, std::make_shared<T>()));
	}

	/// <summary>
	/// メタAI用のシステム追加
	/// </summary>
	/// <typeparam name="T">  : システムの型 </typeparam>
	/// <param name="type"> : システムの種類 </param>
	/// <param name="system"> : システムの実体 </param>
	template <class T>
	void AddSystem(Phoenix::s32 type, std::shared_ptr<T> system)
	{
		systems.insert(std::make_pair(type, system));
	}
};