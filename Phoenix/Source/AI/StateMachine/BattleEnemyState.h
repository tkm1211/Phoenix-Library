#pragma once

#include "Phoenix/FrameWork/StateMachine/StateMachine.h"
#include "../../Enemy/EnemyState.h"


class Enemy;
namespace BattleEnemy
{
#pragma region Idle
	class Idle : public State<BattleEnemyState>
	{
	private:
		const Phoenix::f32 MaxCount = 100.0f;

	private:
		Phoenix::f32 timeCounter = 0.0f;

	public:
		Idle() : State<BattleEnemyState>(BattleEnemyState::Idle) {}
		~Idle() {}

	public:
		// ����
		static std::shared_ptr<Idle> Create();

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void SetUp() override;

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void CleanUp() override;

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="elapsedTime"> �o�ߎ��� </param>
		/// <returns> ���̈ڍs����X�e�[�gID </returns>
		BattleEnemyState Update(Phoenix::f32 elapsedTime) override;
	};
#pragma endregion

#pragma region Walk
	class Walk : public State<BattleEnemyState>
	{
	private:
		static constexpr Phoenix::f32 Speed = 0.045f;

	private:
		std::weak_ptr<Enemy> owner;
		Phoenix::f32 moveX = 0.0f;

	public:
		Walk(std::weak_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::Walk), owner(owner) {}
		~Walk() {}

	public:
		// ����
		static std::shared_ptr<Walk> Create(std::weak_ptr<Enemy> owner);

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void SetUp() override;

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void CleanUp() override;

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="elapsedTime"> �o�ߎ��� </param>
		/// <returns> ���̈ڍs����X�e�[�gID </returns>
		BattleEnemyState Update(Phoenix::f32 elapsedTime) override;
	};
#pragma endregion

#pragma region Run
	class Run : public State<BattleEnemyState>
	{
	private:
		static constexpr Phoenix::f32 Speed = 0.1f;

	private:
		std::weak_ptr<Enemy> owner;

	public:
		Run(std::weak_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::Run), owner(owner) {}
		~Run() {}

	public:
		// ����
		static std::shared_ptr<Run> Create(std::weak_ptr<Enemy> owner);

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void SetUp() override;

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void CleanUp() override;

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="elapsedTime"> �o�ߎ��� </param>
		/// <returns> ���̈ڍs����X�e�[�gID </returns>
		BattleEnemyState Update(Phoenix::f32 elapsedTime) override;
	};
#pragma endregion

#pragma region Attack
	template <class T, class U>
	class Attack : public State<BattleEnemyState>
	{
	private:
		static constexpr Phoenix::f32 Speed = 0.1f;

	private:
		std::weak_ptr<U> owner;

		Phoenix::s32 index = 0;
		T currentAttack = T();
		std::vector<T> attackList;

	public:
		Attack(std::weak_ptr<U> owner) : State<BattleEnemyState>(BattleEnemyState::Attack), owner(owner) {}
		~Attack() {}

	public:
		// ����
		static std::shared_ptr<Attack<T, U>> Create(std::weak_ptr<U> owner)
		{
			return std::make_shared<Attack<T, U>>(owner);
		}

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void SetUp() override
		{
			index = 0;
		}

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void CleanUp() override
		{
			//attackList.clear();
		}

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="elapsedTime"> �o�ߎ��� </param>
		/// <returns> ���̈ڍs����X�e�[�gID </returns>
		BattleEnemyState Update(Phoenix::f32 elapsedTime) override
		{
			std::shared_ptr<U> obj = owner.lock();
			if (obj)
			{
				if (!obj->GetModel()->IsPlaying() || index == 0)
				{
					if (attackList.size() <= index || !obj->InDistanceHitByAttack())
					{
						return BattleEnemyState::Idle;
					}

					currentAttack = attackList.at(index);
					++index;

					obj->SetAttackState(currentAttack);
					obj->UpdateNewRotate();
					obj->SetMoveSpeed(Speed);
				}

				Phoenix::f32 speed = obj->GetMoveSpeed();
				speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * elapsedTime);
				obj->SetMoveSpeed(speed);
				obj->SetMoveInput(0.0f, -1.0f);
			}

			return BattleEnemyState::NoneState;
		}

	public:
		/// <summary>
		/// �U���ǉ�
		/// </summary>
		/// <param name="id"> �U��ID </param>
		void AddAttack(T id)
		{
			for (const auto& attack : attackList)
			{
				if (id == attack) return;
			}

			attackList.emplace_back(id);
		}
	};
#pragma endregion

#pragma region Dedge
	class Dedge : public State<BattleEnemyState>
	{
	private:
		static constexpr Phoenix::f32 Speed = 0.35f;

	private:
		std::weak_ptr<Enemy> owner;

	public:
		Dedge(std::weak_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::Dedge), owner(owner) {}
		~Dedge() {}

	public:
		// ����
		static std::shared_ptr<Dedge> Create(std::weak_ptr<Enemy> owner);

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void SetUp() override;

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void CleanUp() override;

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="elapsedTime"> �o�ߎ��� </param>
		/// <returns> ���̈ڍs����X�e�[�gID </returns>
		BattleEnemyState Update(Phoenix::f32 elapsedTime) override;
	};
#pragma endregion

#pragma region DamageSmall
	class DamageSmall : public State<BattleEnemyState>
	{
	private:
		static constexpr Phoenix::f32 Speed = 0.1f;

	private:
		std::weak_ptr<Enemy> owner;

	public:
		DamageSmall(std::weak_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::DamageSmall), owner(owner) {}
		~DamageSmall() {}

	public:
		// ����
		static std::shared_ptr<DamageSmall> Create(std::weak_ptr<Enemy> owner);

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void SetUp() override;

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void CleanUp() override;

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="elapsedTime"> �o�ߎ��� </param>
		/// <returns> ���̈ڍs����X�e�[�gID </returns>
		BattleEnemyState Update(Phoenix::f32 elapsedTime) override;
	};
#pragma endregion

#pragma region DamageBig
	class DamageBig : public State<BattleEnemyState>
	{
	private:
		static constexpr Phoenix::f32 Speed = 0.1f;

	private:
		std::weak_ptr<Enemy> owner;

	public:
		DamageBig(std::weak_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::DamageBig), owner(owner) {}
		~DamageBig() {}

	public:
		// ����
		static std::shared_ptr<DamageBig> Create(std::weak_ptr<Enemy> owner);

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void SetUp() override;

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void CleanUp() override;

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="elapsedTime"> �o�ߎ��� </param>
		/// <returns> ���̈ڍs����X�e�[�gID </returns>
		BattleEnemyState Update(Phoenix::f32 elapsedTime) override;
	};
#pragma endregion

#pragma region KnockBack
	class KnockBack : public State<BattleEnemyState>
	{
	private:
		static constexpr Phoenix::f32 Speed = 0.5f;

	private:
		std::weak_ptr<Enemy> owner;

	public:
		KnockBack(std::weak_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::KnockBack), owner(owner) {}
		~KnockBack() {}

	public:
		// ����
		static std::shared_ptr<KnockBack> Create(std::weak_ptr<Enemy> owner);

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void SetUp() override;

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void CleanUp() override;

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="elapsedTime"> �o�ߎ��� </param>
		/// <returns> ���̈ڍs����X�e�[�gID </returns>
		BattleEnemyState Update(Phoenix::f32 elapsedTime) override;
	};
#pragma endregion

#pragma region GettingUp
	class GettingUp : public State<BattleEnemyState>
	{
	private:
		static constexpr Phoenix::f32 Speed = 0.1f;

	private:
		std::weak_ptr<Enemy> owner;

	public:
		GettingUp(std::weak_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::GettingUp), owner(owner) {}
		~GettingUp() {}

	public:
		// ����
		static std::shared_ptr<GettingUp> Create(std::weak_ptr<Enemy> owner);

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void SetUp() override;

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void CleanUp() override;

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="elapsedTime"> �o�ߎ��� </param>
		/// <returns> ���̈ڍs����X�e�[�gID </returns>
		BattleEnemyState Update(Phoenix::f32 elapsedTime) override;
	};
#pragma endregion

#pragma region Guard
	class Guard : public State<BattleEnemyState>
	{
	private:


	public:
		Guard() : State<BattleEnemyState>(BattleEnemyState::Guard) {}
		~Guard() {}

	public:
		// ����
		static std::shared_ptr<Guard> Create();

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void SetUp() override;

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void CleanUp() override;

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="elapsedTime"> �o�ߎ��� </param>
		/// <returns> ���̈ڍs����X�e�[�gID </returns>
		BattleEnemyState Update(Phoenix::f32 elapsedTime) override;
	};
#pragma endregion

#pragma region Death
	class Death : public State<BattleEnemyState>
	{
	private:


	public:
		Death() : State<BattleEnemyState>(BattleEnemyState::Death) {}
		~Death() {}

	public:
		// ����
		static std::shared_ptr<Death> Create();

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void SetUp() override;

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void CleanUp() override;

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="elapsedTime"> �o�ߎ��� </param>
		/// <returns> ���̈ڍs����X�e�[�gID </returns>
		BattleEnemyState Update(Phoenix::f32 elapsedTime) override;
	};
#pragma endregion
}

namespace BattleBoss
{
#pragma region Walk
	class Walk : public State<BattleEnemyState>
	{
	private:
		static constexpr Phoenix::f32 Speed = 0.045f;

	private:
		std::weak_ptr<Enemy> owner;
		Phoenix::f32 moveX = 0.0f;

	public:
		Walk(std::weak_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::Walk), owner(owner) {}
		~Walk() {}

	public:
		// ����
		static std::shared_ptr<Walk> Create(std::weak_ptr<Enemy> owner);

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void SetUp() override;

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void CleanUp() override;

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="elapsedTime"> �o�ߎ��� </param>
		/// <returns> ���̈ڍs����X�e�[�gID </returns>
		BattleEnemyState Update(Phoenix::f32 elapsedTime) override;
	};
#pragma endregion

#pragma region Run
	class Run : public State<BattleEnemyState>
	{
	private:
		static constexpr Phoenix::f32 Speed = 0.1f;

	private:
		std::weak_ptr<Enemy> owner;

	public:
		Run(std::weak_ptr<Enemy> owner) : State<BattleEnemyState>(BattleEnemyState::Run), owner(owner) {}
		~Run() {}

	public:
		// ����
		static std::shared_ptr<Run> Create(std::weak_ptr<Enemy> owner);

		// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
		void SetUp() override;

		// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
		void CleanUp() override;

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="elapsedTime"> �o�ߎ��� </param>
		/// <returns> ���̈ڍs����X�e�[�gID </returns>
		BattleEnemyState Update(Phoenix::f32 elapsedTime) override;
	};
#pragma endregion
}