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
	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
	virtual void SetUp() = 0;

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	virtual void CleanUp() = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <returns> ���̈ڍs����X�e�[�gID </returns>
	virtual T Update() = 0;

public:
	// �X�e�[�gID�擾
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
	// �X�e�[�g�}�V���̃Z�b�g�A�b�v
	virtual void SetUp() = 0;

	// �X�e�[�g�}�V���̃N���[���A�b�v
	virtual void CleanUp()
	{
		currentState.reset();
		stateList.clear();
	}

	// �X�V
	virtual void Update() = 0;

public:
	/// <summary>
	/// �X�e�[�g�ǉ�
	/// </summary>
	/// <param name="state"> �ǉ�����X�e�[�g��std::make_shared </param>
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
	/// ���݂̃X�e�[�gID�擾
	/// </summary>
	/// <returns> ���݂̃X�e�[�gID </returns>
	std::optional<const T&> GetCurrentStateName() const
	{
		if (currentState == nullptr)
		{
			return -1;
		}

		return currentState->GetID();
	}

	/// <summary>
	/// �w��̃X�e�[�g�ֈڍs
	/// </summary>
	/// <param name="nextStateID"> �ڍs��̃X�e�[�gID </param>
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
	// �Z�b�g�A�b�v
	virtual void SetUp() = 0;

	// �N���[���A�b�v
	virtual void CleanUp() = 0;

	// �X�V
	virtual void Update() = 0;
};