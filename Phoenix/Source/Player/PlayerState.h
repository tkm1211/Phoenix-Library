#pragma once

#include <memory>
#include "Player.h"
#include "PlayerInput.h"


namespace PlayerState
{
	namespace Idle
	{
		bool Transition(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		bool TransitionFromWalk(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		bool TransitionFromAttack(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Enter(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Update(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Exit(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
	}

	namespace Walk
	{
		bool Transition(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Enter(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Update(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Exit(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
	}

	namespace Attack
	{
		bool Transition(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		bool TransitionFromDedge(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Enter(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Update(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Exit(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
	}

	namespace Damage
	{
		bool Transition(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Enter(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Update(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Exit(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
	}

	namespace Dedge
	{
		bool Transition(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		bool TransitionFromAttack(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		bool TransitionFromDamage(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Enter(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Update(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Exit(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
	}

	namespace Death
	{
		bool Transition(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Enter(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Update(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
		void Exit(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input);
	}
}