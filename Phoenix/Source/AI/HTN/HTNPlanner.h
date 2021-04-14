#pragma once

#include <map>
#include <memory>
#include "HTNDomain.h"
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/FND/STD.h"


template<class Name, class State, class... Args>
class HTNPlanner
{
private:
	// ドメイン
	std::shared_ptr<HTNDomain<Name, State, Args...>> domain;

	// プランニング結果格納
	std::vector<Name> plan;

public:
	HTNPlanner() {}
	~HTNPlanner() {}

public:
	// 生成
	static std::shared_ptr<HTNPlanner> Create()
	{
		return std::make_shared<HTNPlanner>();
	}

	// コンストラクタ
	void Construct(std::shared_ptr<HTNDomain<Name, State, Args...>> domain)
	{
		this->domain = domain;
	}

	// 初期化
	void Initialize()
	{
		ClearPlan();
	}

	/// <summary>
	/// プランニング
	/// </summary>
	/// <param name="worldState"> ゲーム状態 </param>
	/// <param name="taskName"> 実行したいタスク名 </param>
	/// <returns> プランニング成功したか？ </returns>
	bool Planning(State worldState, Name taskName)
	{
		std::vector<Name> taskNames;
		taskNames.emplace_back(taskName);

		return Internal(worldState, taskNames, true);
	}

	// プランを取得
	std::vector<Name> GetPlan()
	{
		return plan;
	}

	// プラン初期化
	void ClearPlan()
	{
		plan.clear();
	}

private:
	// プランニング処理
	bool Internal(State worldState, std::vector<Name> taskNames, bool compound)
	{
		// taskNamesが空であれば終了
		if (taskNames.begin() == taskNames.end())
		{
			return true;
		}

		// taskNamesの最初のタスクをコピーし、taskNamesの最初のデータを削除
		Name taskName = taskNames.at(0);
		taskNames.erase(taskNames.begin());

		// taskNameと同一キーがあれば実行
		if (domain->GetPrimitiveSize(taskName) == 1 && !compound)
		{
			// 新しいステートに現在のステートをコピーする（再帰処理なのでステートを保持する必要がある）
			State newState;
			Phoenix::FND::MemCpy(&newState, &worldState, sizeof(State));

			// taskNameと同じ名前のプリミティブ関数を実行
			bool result = domain->RunPrimitiveFunction(taskName, &newState);
			if (result)
			{
				// プランニングに追加
				plan.emplace_back(taskName);

				// 次のプランを検索
				return Internal(newState, taskNames, false);
			}
			else
			{
				return false; //プリミティブが実行出来なかった。
			}
		}
		// taskNameと同じ名前のプリミティブタスクが無ければ複合タスクと同じ名前のキーが1つ以上あるか検索
		else if (domain->GetCompoundSize(taskName) > 0 && compound)
		{
			// taskNameと同じキーを持つデータと終了を持つイテレータを取得
			for (auto it : domain->GetCompound())
			{
				// タスクネームと同じタスクのときだけ処理を実行する
				if (it.first == taskName)
				{
					// タスク格納用変数宣言
					std::vector<Name> newTask;

					// 関数実行して新しく必要になるタスクを格納する
					bool result = it.second(&worldState, &newTask); //ステートを見てnewTaskに下位プランを格納

					//it.secondは関数ポインタで&state（ステート）, &newTask（タスクの名前文字列）を引数に持つ
					if (result)
					{
						// newTaskにtasksの残りを末尾に追加する内部関数
						ArrayMerge(&newTask, taskNames);

						// newTaskの内容からさらに下位階層のプランニングを行う
						result = Internal(worldState, newTask, false);
						if (result != false)
						{
							return result;
						}
					}
				}
			}
		}

		return false;
	}

	// 配列をマージさせる
	void ArrayMerge(std::vector<Name>* tasks, std::vector<Name> task)
	{
		for (auto it : task)
		{
			tasks->emplace_back(it);
		}
	}
};