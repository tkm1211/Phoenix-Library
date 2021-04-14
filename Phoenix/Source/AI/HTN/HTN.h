#pragma once

#include <map>
#include <memory>
#include "Phoenix/Math/PhoenixMath.h"
#include "HTNPlanner.h"
#include "HTNDomain.h"


template<class Name, class State, class... Args>
class HTN
{
private:
	// プランナー
	std::shared_ptr<HTNPlanner<Name, State, Args...>> planner;

	// ドメイン
	std::shared_ptr<HTNDomain<Name, State, Args...>> domain;

	// プラン実行中
	bool inProgress = false;

	// プラン番号
	Phoenix::s32 index = 0;

public:
	HTN() {}
	~HTN() {}

public:
	// 生成
	static std::shared_ptr<HTN> Create()
	{
		return std::make_shared<HTN>();
	}

	// コンストラクタ
	void Construct()
	{
		domain = HTNDomain<Name, State, Args...>::Create();

		planner = HTNPlanner<Name, State, Args...>::Create();
		planner->Construct(domain);
	}

	// 初期化
	void Initialize()
	{
		planner->Initialize();

		inProgress = false;
		index = 0;
	}

	// 更新
	void Update(Args... args)
	{
		std::vector<Name> plan = planner->GetPlan();
		auto tasks = domain->GetTasks();

		// プランのデータがあり、プラン実行中のとき
		if (!plan.empty() && inProgress)
		{
			// actがプラン最後の場合
			if (index == plan.size())
			{
				planner->ClearPlan(); // プランが最後まで実行されたらプラン内容を初期化
			}
			else
			{
				// タスクの実行関数が存在を確認
				auto it = tasks.find(plan.at(index));
				if (it != tasks.end())
				{
					// セットアップを実行
					it->second.enter(args...);

					// タスクを実行
					if (it->second.update(args...))
					{
						// タスクが終了すると次のタスクへ
						it->second.exit(args...);
						++index;
					}
				}
			}
		}
	}

	/// <summary>
	/// プランニング
	/// </summary>
	/// <param name="worldState"> ゲーム状態 </param>
	/// <param name="taskName"> 実行したいタスク名 </param>
	/// <returns> プランニング成功したか？ </returns>
	bool Planning(State worldState, Name taskName)
	{
		return planner->Planning(worldState, taskName);
	}

	// プリミティブタスクを追加
	void AddPrimitiveTask(Name name, std::function<bool(State*)> func)
	{
		domain->AddPrimitiveTask(name, func);
	}

	// 複合タスクを追加
	void AddCompoundTask(Name name, std::function<bool(State*, std::vector<Name>*)> func)
	{
		domain->AddCompoundTask(name, func);
	}

	/// <summary>
	/// タスクの初期化関数と実行関数と終了化関数を追加
	/// </summary>
	/// <param name="name"> タスク名 </param>
	/// <param name="enter"> タスクの初期化関数(Arg... : 可変長引数) </param>
	/// <param name="update"> タスクの実行関数(bool : タスク終了通知、Arg... : 可変長引数) </param>
	/// <param name="exit"> タスクの終了化関数(Arg... : 可変長引数) </param>
	void AddTask(Name name, std::function<void(Args...)> enter, std::function<bool(Args...)> update, std::function<void(Args...)> exit)
	{
		domain->AddTask(name, enter, update, exit);
	}

	// プラン実行中か
	bool InProgress()
	{
		return inProgress;
	}

	// プラン取得
	std::vector<Name> GetPlan()
	{
		return planner->GetPlan();
	}

	// プラン初期化
	void ClearPlan()
	{
		planner->ClearPlan();
	}
};