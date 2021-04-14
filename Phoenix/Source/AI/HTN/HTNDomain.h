#pragma once

#include <map>
#include <memory>
#include <vector>
#include "Phoenix/Math/PhoenixMath.h"


template<class Name, class State, class... Args>
class HTNDomain
{
public:
	struct Task
	{
		// 初期化関数
		std::function<void(Args...)> enter;

		// 実行関数
		std::function<bool(Args...)> update;

		// 終了化関数
		std::function<void(Args...)> exit;
	};

private:
	// キー名はプリミティブタスク名、値はプリミティブタスク
	std::unordered_map<Name, std::function<bool(State*)>> primitive;

	// キー名は複合タスク名、値は複合タスク
	std::unordered_multimap<Name, std::function<bool(State*, std::vector<Name>*)>> compound;

	// タスクの関数管理
	std::map<Name, Task> tasks;

public:
	HTNDomain() {}
	~HTNDomain() {}

public:
	// 生成
	static std::shared_ptr<HTNDomain> Create()
	{
		return std::make_shared<HTNDomain>();
	}

	// プリミティブタスクを追加
	void AddPrimitiveTask(Name name, std::function<bool(State*)> func)
	{
		primitive.insert(std::make_pair(name, func));
	}

	// 複合タスクを追加
	void AddCompoundTask(Name name, std::function<bool(State*, std::vector<Name>*)> func)
	{
		compound.insert(std::make_pair(name, func));
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
		tasks.insert(std::make_pair(name, Task()));
		tasks.at(name).enter = enter;
		tasks.at(name).update = update;
		tasks.at(name).exit = exit;
	}

	// プリミティブタスクの関数を実行
	bool RunPrimitiveFunction(Name taskName, State* state)
	{
		return primitive.at(taskName)(state);
	}

	// 複合タスクを取得
	std::unordered_multimap<Name, std::function<bool(State*, std::vector<Name>*)>> GetCompound()
	{
		return compound;
	}

	// タスクの関数管理を取得
	std::map<Name, Task> GetTasks()
	{
		return tasks;
	}
	
	// プリミティブタスクの数を取得
	Phoenix::sizeT GetPrimitiveSize(Name taskName)
	{
		return primitive.count(taskName);
	}

	// 複合タスクの数を取得
	Phoenix::sizeT GetCompoundSize(Name taskName)
	{
		return compound.count(taskName);
	}
};