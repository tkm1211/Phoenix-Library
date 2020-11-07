#pragma once

#include <map>
#include <memory>
#include <optional>
#include "Phoenix/Math/PhoenixMath.h"


template <class T>
class State
{
private:
	T id;

public:
	State() = delete;
	explicit State(T id) : id(id) {}
	virtual ~State() = default;

public:
	// 状態に入ったときに呼ばれる関数
	virtual void SetUp() = 0;

	// 次の状態に移る前に呼ばれる関数
	virtual void CleanUp() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <returns> 次の移行するステートID </returns>
	virtual T Update() = 0;

public:
	// ステートID取得
	const T& GetID() { return id; }
};

template <class T>
class StateMachine
{
protected:
	std::map<T, std::shared_ptr<State<T>>> stateList;
	std::shared_ptr<State<T>> currentState;

public:
	StateMachine() = default;
	virtual ~StateMachine() = default;

public:
	// ステートマシンのセットアップ
	virtual void SetUp() = 0;

	// ステートマシンのクリーンアップ
	virtual void CleanUp()
	{
		currentState.reset();
		stateList.clear();
	}

	// 更新
	virtual void Update() = 0;

public:
	/// <summary>
	/// ステート追加
	/// </summary>
	/// <param name="state"> 追加するステートのstd::make_shared </param>
	void AddState(const std::shared_ptr<State<T>>& state)
	{
		if (state == nullptr) return;

		auto it = stateList.find(state->GetID());
		if (it != stateList.end())
		{
			return;
		}

		stateList[state->GetID()] = state;
	}

	/// <summary>
	/// 現在のステートID取得
	/// </summary>
	/// <returns> 現在のステートID </returns>
	std::optional<const T&> GetCurrentStateName() const
	{
		if (currentState == nullptr)
		{
			return -1;
		}

		return currentState->GetID();
	}

	/// <summary>
	/// 指定のステートへ移行
	/// </summary>
	/// <param name="nextStateID"> 移行先のステートID </param>
	void GoToState(T nextStateID)
	{
		auto it = stateList.find(nextStateID);
		if (it == stateList.end())
		{
			return;
		}

		if (currentState != nullptr)
		{
			currentState->CleanUp();
		}

		currentState = stateList[nextStateID];
		currentState->SetUp();
	}
};

class AIBase
{
public:
	// セットアップ
	virtual void SetUp() = 0;

	// クリーンアップ
	virtual void CleanUp() = 0;

	// 更新
	virtual void Update() = 0;
};