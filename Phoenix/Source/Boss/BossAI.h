#pragma once

#include <vector>
#include <memory>
#include "Phoenix/Types.h"
#include "AI.h"
#include "AIState/AIState.h"


class Boss;
class Player;
class BossAI : public AI
{
private:
	std::vector<std::shared_ptr<AIState>> states;
	AIState* currentState = nullptr;
	AIState* nextState = nullptr;
	Boss* boss = nullptr;
	Player* player = nullptr;
	bool isAI = false;

public:
	BossAI() {}
	~BossAI() {}

public:
	static std::unique_ptr<AI> Create();
	void Construct() override;
	void Initialize() override;
	void Update() override;
	void GUI() override;

public:
	void SharedBossPtr(Boss* boss) { this->boss = boss; }
	void SharedPlayerPtr(Player* player) { this->player = player; }

	AIStateType GetCurrentStateType()
	{
		if (currentState)
		{
			return currentState->GetStateType();
		}
		return AIStateType::None;
	}

	AIStateType GetNextStateType()
	{
		if (nextState)
		{
			return nextState->GetStateType();
		}
		return AIStateType::None;
	}

	AIState* GetCurrentState() { return currentState; }

private:
	template<class T>
	T* AddState()
	{
		std::shared_ptr<T> state = std::make_shared<T>();
		states.emplace_back(state);

		return state.get();
	}

	AIState* GetState(AIStateType stateType)
	{
		if (stateType == AIStateType::None) return nullptr;

		Phoenix::u32 index = static_cast<Phoenix::u32>(stateType);

		return states[index].get();
	}
};