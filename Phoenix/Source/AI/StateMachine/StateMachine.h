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

protected:
	bool canChangeState = true; // ��{�́A���ł��X�e�[�g�ڍs�ł���悤�ɂ��Ă���

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

	// �X�e�[�g�̈ڍs���ł��邩�H
	bool CanChangeState() { return canChangeState; }
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
	virtual T Update() = 0;

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
	T GetCurrentStateName() const
	{
		if (currentState == nullptr)
		{
			return static_cast<T>(-1);
		}

		return currentState->GetID();
	}

	/// <summary>
	/// �w��̃X�e�[�g�ֈڍs
	/// </summary>
	/// <param name="nextStateID"> �ڍs��̃X�e�[�gID </param>
	/// <param name="forcedChange"> �����I�ɃX�e�[�g�ڍs���� </param>
	/// <returns> �X�e�[�g�ڍs���� : 0, �X�e�[�g�ڍs���s : -1 </returns>
	Phoenix::s32 GoToState(T nextStateID, bool forcedChange = false)
	{
		auto it = stateList.find(nextStateID);
		if (it == stateList.end())
		{
			return -1;
		}

		if (currentState != nullptr)
		{
			if (!currentState->CanChangeState() && !forcedChange)
			{
				return -1;
			}
			else
			{
				currentState->CleanUp();
			}
		}

		currentState = stateList[nextStateID];
		currentState->SetUp();

		return 0;
	}
};