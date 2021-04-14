#include "PlayerState.h"


namespace PlayerState
{
	namespace Idle
	{
		bool Transition(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			if (!owner->GetModel()->IsPlaying())
			{
				return true;
			}

			return false;
		}

		bool TransitionFromWalk(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			if (input->GetInputType() == PlayerInput::InputType::Idle)
			{
				return true;
			}

			return false;
		}

		bool TransitionFromAttack(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			if (!owner->IsAttack() && !owner->GetModel()->IsPlaying())
			{
				return true;
			}

			return false;
		}

		void Enter(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			owner->ChangeAnimationState(Player::AnimationState::Idle, 0.0f);
			owner->ChangeAttackAnimationState(0, 0, 0.0f, false);
		}

		void Update(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			// 構えている状態か判定
			if (input->GetLockOn())
			{
				owner->ChangeLockOn(true, Player::AnimationState::Idle);
			}
			else
			{
				owner->ChangeLockOn(false, Player::AnimationState::Idle);
			}
		}

		void Exit(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			
		}
	}

	namespace Walk
	{
		bool Transition(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			if (input->GetInputType() == PlayerInput::InputType::Walk)
			{
				return true;
			}

			return false;
		}

		void Enter(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			// 構えている状態か判定
			if (input->GetLockOn())
			{
				owner->ChangeAnimationState(Player::AnimationState::Walk, Player::BattleWalkSpeed);
			}
			else
			{
				owner->ChangeAnimationState(Player::AnimationState::Walk, Player::WalkSpeed);
			}
		}

		void Update(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			Phoenix::f32 sX = input->GetSX();
			Phoenix::f32 sY = input->GetSY();

			// 構えている状態か判定
			if (input->GetLockOn())
			{
				owner->ChangeLockOn(true, Player::AnimationState::Walk, Player::BattleWalkSpeed);
			}
			else
			{
				owner->ChangeLockOn(false, Player::AnimationState::Walk, Player::WalkSpeed);
			}

			// 回転更新
			{
				owner->UpdateRotate(sX, sY);
			}
		}

		void Exit(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{

		}
	}

	namespace Attack
	{
		bool Transition(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			PlayerInput::InputType key = input->GetInputType();
			if ((key == PlayerInput::InputType::WeakAttack) || (key == PlayerInput::InputType::StrongAttack))
			{
				return true;
			}

			return false;
		}

		bool TransitionFromDedge(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			PlayerInput::InputType key = input->GetInputType();
			if (((key == PlayerInput::InputType::WeakAttack) || (key == PlayerInput::InputType::StrongAttack)) && owner->IsInvincible())
			{
				return true;
			}

			return false;
		}

		void Enter(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			owner->ChangeAttackAnimationState(0, 0, 0.0f, false, true);
		}

		void Update(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			Phoenix::f32 sX = input->GetSX();
			Phoenix::f32 sY = input->GetSY();

			PlayerInput::InputType key = input->GetInputType();
			Player::AttackDataList attackDatasList = owner->GetAttackDatasList();

			Phoenix::s32 attackState = owner->GetAttackState();
			Phoenix::s32 attackComboState = owner->GetAttackComboState();

			// 攻撃ステートへ
			if (((key == PlayerInput::InputType::WeakAttack) || (key == PlayerInput::InputType::StrongAttack)) && 0 < attackDatasList.attackDatas.size())
			{
				if (0 < attackDatasList.attackDatas.at(attackState).datas.size())
				{
					Phoenix::s32 index = attackState;
					Phoenix::s32 wearNextIndex = attackDatasList.attackDatas.at(index).datas.at(attackComboState).weakDerivedAttackState;
					Phoenix::s32 strongNextIndex = attackDatasList.attackDatas.at(index).datas.at(attackComboState).strongDerivedAttackState;
					Phoenix::s32 endIndex = static_cast<Phoenix::s32>(attackDatasList.attackDatas.size());
					Phoenix::f32 attackReceptionTimeCnt = owner->GetAttackReceptionTimeCnt();
					Player::AttackKey attackKey = (key == PlayerInput::InputType::WeakAttack) ? Player::AttackKey::WeakAttack : Player::AttackKey::StrongAttack;

					// 次の攻撃が発動するボタンの受付
					if (owner->GetReceptionStack())
					{
						if (wearNextIndex < endIndex && 0 <= wearNextIndex)
						{
							owner->JudgeAttackStackInput(sX, sY, index, wearNextIndex);
						}
						if (strongNextIndex < endIndex && 0 <= strongNextIndex)
						{
							owner->JudgeAttackStackInput(sX, sY, index, strongNextIndex);
						}
					}
					else if ((index == 0) || attackDatasList.attackDatas.at(index).datas.at(attackComboState).receptionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt < attackDatasList.attackDatas.at(index).datas.at(attackComboState).receptionEndTime)
					{
						if (wearNextIndex < endIndex && 0 <= wearNextIndex)
						{
							owner->JudgeAttackInput(sX, sY, index, wearNextIndex, attackKey);
						}
						if (strongNextIndex < endIndex && 0 <= strongNextIndex)
						{
							owner->JudgeAttackInput(sX, sY, index, strongNextIndex, attackKey);
						}
					}
				}
			}
			else if (!owner->GetModel()->IsPlaying())
			{
				owner->JudgeAttackCombo();
			}
		}

		void Exit(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
		}
	}

	namespace Damage
	{
		bool Transition(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			if (Player::AccumulationMaxDamege <= owner->GetAccumulationDamege())
			{
				return true;
			}

			return false;
		}

		void Enter(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			owner->ChangeAnimationState(Player::AnimationState::Damage, 0.0f);
		}

		void Update(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			// No content.
		}

		void Exit(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{

		}
	}

	namespace Dedge
	{
		bool Transition(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			if (input->GetInputType() == PlayerInput::InputType::Dedge)
			{
				return true;
			}

			return false;
		}

		bool TransitionFromAttack(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			if ((input->GetInputType() == PlayerInput::InputType::Dedge) && owner->JudgeDedge())
			{
				return true;
			}

			return false;
		}

		bool TransitionFromDamage(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			// ダメージ中に回避に遷移
			if ((input->GetInputType() == PlayerInput::InputType::Dedge) && owner->GetModel()->IsPlaying() && (owner->GetModel()->GetLastTime() <= (15.0f / 60.0f)))
			{
				return true;
			}

			return false;
		}

		void Enter(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			owner->ChangeAnimationState(Player::AnimationState::Dedge, Player::DedgeSpeed);
			owner->JudgeDedgeIndex(input->GetSX(), input->GetSY());
		}

		void Update(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			// No content.
		}

		void Exit(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{

		}
	}

	namespace Death
	{
		bool Transition(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			if (owner->GetHP() <= 0 && owner->GetAlive())
			{
				return true;
			}

			return false;
		}

		void Enter(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			owner->ChangeAnimationState(Player::AnimationState::Death, 0.0f);
			owner->SetAlive(false);
		}

		void Update(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{
			// No content.
		}

		void Exit(std::shared_ptr<Player> owner, std::shared_ptr<PlayerInput> input)
		{

		}
	}
}