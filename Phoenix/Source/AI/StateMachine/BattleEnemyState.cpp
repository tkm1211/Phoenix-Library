#include "BattleEnemyState.h"
#include "../../Enemy/Enemy.h"
#include "../../Boss/Boss.h"


namespace BattleEnemy
{
//#pragma region Idle
//	// ����
//	std::shared_ptr<Idle> Idle::Create()
//	{
//		return std::make_shared<Idle>();
//	}
//
//	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
//	void Idle::SetUp()
//	{
//		canChangeState = false;
//		timeCounter = 0.0f;
//	}
//
//	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
//	void Idle::CleanUp()
//	{
//
//	}
//
//	// �X�V
//	BattleEnemyState Idle::Update(Phoenix::f32 elapsedTime)
//	{
//		if (MaxCount <= timeCounter)
//		{
//			canChangeState = true;
//			timeCounter = 0;
//		}
//		else
//		{
//			timeCounter += 1.0f * elapsedTime;
//		}
//
//		return BattleEnemyState::NoneState;
//	}
//#pragma endregion
//
//#pragma region Walk
//	// ����
//	std::shared_ptr<Walk> Walk::Create(std::weak_ptr<Enemy> owner)
//	{
//		return std::make_shared<Walk>(owner);
//	}
//
//	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
//	void Walk::SetUp()
//	{
//		moveX = 0.0f;
//
//		Phoenix::s32 judge = rand() % 2;
//
//		if (judge)
//		{
//			moveX = -1.0f;
//		}
//		else
//		{
//			moveX = 1.0f;
//		}
//	}
//
//	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
//	void Walk::CleanUp()
//	{
//
//	}
//
//	// �X�V
//	BattleEnemyState Walk::Update(Phoenix::f32 elapsedTime)
//	{
//		std::shared_ptr<Enemy> obj = owner.lock();
//
//		if (obj)
//		{
//			if (obj->InDistanceHitByAttack())
//			{
//				obj->SetMoveInput(0.0f, 0.0f);
//				obj->SetMoveSpeed(0.0f);
//				//return BattleEnemyState::Attack;
//				return BattleEnemyState::Idle;
//			}
//			else if (!obj->InBattleTerritory())
//			{
//				obj->SetMoveInput(0.0f, 0.0f);
//				obj->SetMoveSpeed(0.0f);
//				return BattleEnemyState::Run;
//			}
//
//			obj->SetMoveInput(moveX, -1.0f);
//			obj->SetMoveSpeed(Speed);
//		}
//
//		return BattleEnemyState::NoneState;
//	}
//#pragma endregion
//
//#pragma region Run
//	// ����
//	std::shared_ptr<Run> Run::Create(std::weak_ptr<Enemy> owner)
//	{
//		return std::make_shared<Run>(owner);
//	}
//
//	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
//	void Run::SetUp()
//	{
//
//	}
//
//	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
//	void Run::CleanUp()
//	{
//
//	}
//
//	// �X�V
//	BattleEnemyState Run::Update(Phoenix::f32 elapsedTime)
//	{
//		std::shared_ptr<Enemy> obj = owner.lock();
//
//		if (obj)
//		{
//			if (obj->InBattleTerritory())
//			{
//				obj->SetMoveInput(0.0f, 0.0f);
//				obj->SetMoveSpeed(0.0f);
//				return BattleEnemyState::Idle;
//			}
//
//			obj->SetMoveInput(0.0f, -1.0f);
//			obj->SetMoveSpeed(Speed);
//		}
//
//		return BattleEnemyState::NoneState;
//	}
//#pragma endregion
//
//#pragma region Attack
//	// No Data.
//#pragma endregion
//
//#pragma region Dedge
//		// ����
//	std::shared_ptr<Dedge> Dedge::Create(std::weak_ptr<Enemy> owner)
//	{
//		return std::make_shared<Dedge>(owner);
//	}
//
//	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
//	void Dedge::SetUp()
//	{
//		std::shared_ptr<Enemy> obj = owner.lock();
//
//		if (obj)
//		{
//			obj->SetMoveSpeed(Speed);
//		}
//	}
//
//	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
//	void Dedge::CleanUp()
//	{
//
//	}
//
//	// �X�V
//	BattleEnemyState Dedge::Update(Phoenix::f32 elapsedTime)
//	{
//		std::shared_ptr<Enemy> obj = owner.lock();
//
//		if (obj)
//		{
//			if (!obj->GetModel()->IsPlaying())
//			{
//				obj->SetMoveInput(0.0f, 0.0f);
//				obj->SetMoveSpeed(0.0f);
//
//				return BattleEnemyState::Idle;
//			}
//
//#if 0
//			Phoenix::s32 input = rand() % 2;
//			if (input) obj->SetMoveInput(1.0f, 0.0f);
//			else if (!input) obj->SetMoveInput(-1.0f, 0.0f);
//#else
//			obj->SetMoveInput(0.0f, 1.0f);
//#endif
//
//			Phoenix::f32 speed = obj->GetMoveSpeed();
//			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * elapsedTime);
//			obj->SetMoveSpeed(speed);
//		}
//
//		return BattleEnemyState::NoneState;
//	}
//#pragma endregion
//
//#pragma region DamageSmall
//	// ����
//	std::shared_ptr<DamageSmall> DamageSmall::Create(std::weak_ptr<Enemy> owner)
//	{
//		return std::make_shared<DamageSmall>(owner);
//	}
//
//	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
//	void DamageSmall::SetUp()
//	{
//		canChangeState = false;
//
//		std::shared_ptr<Enemy> obj = owner.lock();
//		if (obj)
//		{
//			obj->SetMoveSpeed(Speed);
//		}
//	}
//
//	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
//	void DamageSmall::CleanUp()
//	{
//
//	}
//
//	// �X�V
//	BattleEnemyState DamageSmall::Update(Phoenix::f32 elapsedTime)
//	{
//		std::shared_ptr<Enemy> obj = owner.lock();
//
//		if (obj)
//		{
//			if (!obj->GetModel()->IsPlaying())
//			{
//				canChangeState = true;
//				return BattleEnemyState::Idle;
//			}
//
//			Phoenix::f32 speed = obj->GetMoveSpeed();
//			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * elapsedTime);
//			obj->SetMoveSpeed(speed);
//			obj->SetMoveInput(0.0f, 1.0f);
//		}
//
//		return BattleEnemyState::NoneState;
//	}
//#pragma endregion
//
//#pragma region DamageBig
//	// ����
//	std::shared_ptr<DamageBig> DamageBig::Create(std::weak_ptr<Enemy> owner)
//	{
//		return std::make_shared<DamageBig>(owner);
//	}
//
//	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
//	void DamageBig::SetUp()
//	{
//		canChangeState = false;
//
//		std::shared_ptr<Enemy> obj = owner.lock();
//
//		if (obj)
//		{
//			obj->SetMoveSpeed(Speed);
//		}
//	}
//
//	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
//	void DamageBig::CleanUp()
//	{
//
//	}
//
//	// �X�V
//	BattleEnemyState DamageBig::Update(Phoenix::f32 elapsedTime)
//	{
//		std::shared_ptr<Enemy> obj = owner.lock();
//
//		if (obj)
//		{
//			if (!obj->GetModel()->IsPlaying())
//			{
//				canChangeState = true;
//				return BattleEnemyState::Idle;
//			}
//
//			Phoenix::f32 speed = obj->GetMoveSpeed();
//			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * elapsedTime);
//			obj->SetMoveSpeed(speed);
//			obj->SetMoveInput(0.0f, 1.0f);
//		}
//
//		return BattleEnemyState::NoneState;
//	}
//#pragma endregion
//
//#pragma region KnockBack
//	// ����
//	std::shared_ptr<KnockBack> KnockBack::Create(std::weak_ptr<Enemy> owner)
//	{
//		return std::make_shared<KnockBack>(owner);
//	}
//
//	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
//	void KnockBack::SetUp()
//	{
//		canChangeState = false;
//
//		std::shared_ptr<Enemy> obj = owner.lock();
//
//		if (obj)
//		{
//			obj->SetMoveSpeed(Speed);
//		}
//	}
//
//	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
//	void KnockBack::CleanUp()
//	{
//
//	}
//
//	// �X�V
//	BattleEnemyState KnockBack::Update(Phoenix::f32 elapsedTime)
//	{
//		std::shared_ptr<Enemy> obj = owner.lock();
//
//		if (obj)
//		{
//			Phoenix::f32 speed = obj->GetMoveSpeed();
//
//			if (!obj->GetModel()->IsPlaying())
//			{
//				if (0.0006f < speed)
//				{
//					canChangeState = true;
//					return BattleEnemyState::KnockBack;
//				}
//				canChangeState = true;
//				return BattleEnemyState::GettingUp;
//			}
//
//			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.05f * elapsedTime);
//			obj->SetMoveSpeed(speed);
//			obj->SetMoveInput(0.0f, 1.0f);
//		}
//
//		return BattleEnemyState::NoneState;
//	}
//#pragma endregion
//
//#pragma region GettingUp
//	// ����
//	std::shared_ptr<GettingUp> GettingUp::Create(std::weak_ptr<Enemy> owner)
//	{
//		return std::make_shared<GettingUp>(owner);
//	}
//
//	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
//	void GettingUp::SetUp()
//	{
//		canChangeState = false;
//	}
//
//	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
//	void GettingUp::CleanUp()
//	{
//
//	}
//
//	// �X�V
//	BattleEnemyState GettingUp::Update(Phoenix::f32 elapsedTime)
//	{
//		std::shared_ptr<Enemy> obj = owner.lock();
//
//		if (obj)
//		{
//			if (!obj->GetModel()->IsPlaying())
//			{
//				canChangeState = true;
//				return BattleEnemyState::Idle;
//			}
//		}
//
//		return BattleEnemyState::NoneState;
//	}
//#pragma endregion
//
//#pragma region Guard
//	// ����
//	std::shared_ptr<Guard> Guard::Create()
//	{
//		return std::make_shared<Guard>();
//	}
//
//	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
//	void Guard::SetUp()
//	{
//
//	}
//
//	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
//	void Guard::CleanUp()
//	{
//
//	}
//
//	// �X�V
//	BattleEnemyState Guard::Update(Phoenix::f32 elapsedTime)
//	{
//		return BattleEnemyState::NoneState;
//	}
//#pragma endregion
//
//#pragma region Death
//	// ����
//	std::shared_ptr<Death> Death::Create()
//	{
//		return std::make_shared<Death>();
//	}
//
//	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
//	void Death::SetUp()
//	{
//
//	}
//
//	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
//	void Death::CleanUp()
//	{
//
//	}
//
//	// �X�V
//	BattleEnemyState Death::Update(Phoenix::f32 elapsedTime)
//	{
//		return BattleEnemyState::NoneState;
//	}
//#pragma endregion

	namespace Idle
	{
		bool Transition(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			return (aiData->changeState && (aiData->nextState == BattleEnemyState::Idle));
		}

		void Enter(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			owner->SetAnimation(BattleEnemyState::Idle);
			aiData->changeState = false;
		}

		void Update(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{

		}

		void Exit(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{

		}
	}
	
	namespace Walk
	{
		bool Transition(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			return (aiData->changeState && (aiData->nextState == BattleEnemyState::Walk));
		}

		void Enter(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			owner->SetAnimation(BattleEnemyState::Walk);
			aiData->changeState = false;
			aiData->runTime = 0.0f;
		}

		void Update(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			if (owner->InDistanceHitByAttack())
			{
				aiData->ChangeNextState();
			}

			// �v�����č\�z���邽�ߑҋ@��ԂɈڍs
			if (50.0f <= aiData->runTime)
			{
				aiData->ChangeNextState(BattleEnemyState::Idle);
			}

			owner->SetMoveInput(0.0f, -1.0f);
			owner->SetMoveSpeed(Enemy::WalkSpeed);

			aiData->UpdateRunTime();
		}

		void Exit(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			owner->SetMoveInput(0.0f, 0.0f);
			owner->SetMoveSpeed(0.0f);
		}
	}
	
	namespace Run
	{
		bool Transition(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			return (aiData->changeState && (aiData->nextState == BattleEnemyState::Run));
		}

		void Enter(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			owner->SetAnimation(BattleEnemyState::Run);
			aiData->changeState = false;
		}

		void Update(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			if (owner->InBattleTerritory())
			{
				aiData->ChangeNextState();
			}

			if (50.0f <= aiData->runTime)
			{
				//owner->SetChangeOfCourse(true);
			}
			
			owner->SetMoveInput(0.0f, -1.0f);
			owner->SetMoveSpeed(Enemy::RunSpeed);

			aiData->UpdateRunTime();
		}

		void Exit(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			owner->SetMoveInput(0.0f, 0.0f);
			owner->SetMoveSpeed(0.0f);
		}
	}
	
	namespace Attack
	{
		bool Transition(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			return (aiData->changeState && (aiData->nextState == BattleEnemyState::Attack));
		}

		void Enter(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			owner->SetAnimation(BattleEnemyState::Attack);
			aiData->changeState = false;
			aiData->attackIndex = 0;
		}

		void Update(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			if (!owner->GetModel()->IsPlaying() || aiData->attackIndex == 0)
			{
				if (owner->GetTypeTag() == Enemy::TypeTag::Small)
				{
					auto attackList = owner->GetAtackDatasList();

					if (attackList.size() <= aiData->attackIndex || !owner->InDistanceHitByAttack())
					{
						aiData->ChangeNextState();
						return;
					}

					auto currentAttack = attackList.at(aiData->attackIndex);
					++aiData->attackIndex;

					owner->SetAttackState(currentAttack.datas.at(0).animState);
					owner->UpdateNewRotate();
					owner->SetMoveSpeed(Enemy::AttackSpeed);
				}
				else if (owner->GetTypeTag() == Enemy::TypeTag::Large)
				{
					std::shared_ptr<Boss> boss = std::dynamic_pointer_cast<Boss>(owner); // TODO : dynamiccast���g��Ȃ��悤�ɃG�l�~�[�̍U���݌v��ύX
					auto attackList = boss->GetAtackDatasList();

					if (attackList.size() <= aiData->attackIndex || !boss->InDistanceHitByAttack())
					{
						aiData->ChangeNextState();
						return;
					}

					auto currentAttack = attackList.at(aiData->attackIndex);
					++aiData->attackIndex;

					boss->SetAttackState(currentAttack.datas.at(0).animState);
					boss->UpdateNewRotate();
					boss->SetMoveSpeed(Enemy::AttackSpeed);
				}
			}
			
			Phoenix::f32 speed = owner->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * aiData->elapsedTime);
			owner->SetMoveSpeed(speed);
			owner->SetMoveInput(0.0f, -1.0f);
		}

		void Exit(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			aiData->attackIndex = -1;
		}
	}
	
	namespace Dedge
	{
		bool Transition(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			return (aiData->changeState && (aiData->nextState == BattleEnemyState::Dedge));
		}

		void Enter(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			owner->SetAnimation(BattleEnemyState::Dedge);
			owner->SetMoveSpeed(Enemy::DedgeSpeed);
			aiData->changeState = false;
		}

		void Update(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			if (!owner->GetModel()->IsPlaying())
			{
				aiData->ChangeNextState();
			}

			owner->SetMoveInput(0.0f, 1.0f);

			Phoenix::f32 speed = owner->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * aiData->elapsedTime);
			owner->SetMoveSpeed(speed);
		}

		void Exit(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			owner->SetMoveInput(0.0f, 0.0f);
			owner->SetMoveSpeed(0.0f);
		}
	}
	
	namespace DamageSmall
	{
		bool Transition(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			return (aiData->changeState && (aiData->nextState == BattleEnemyState::DamageSmall));
		}

		void Enter(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			owner->SetAnimation(BattleEnemyState::DamageSmall);
			owner->SetMoveSpeed(Enemy::DamageSpeed);
			aiData->changeState = false;
		}

		void Update(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			if (!owner->GetModel()->IsPlaying())
			{
				aiData->ChangeNextState();
			}

			Phoenix::f32 speed = owner->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * aiData->elapsedTime);
			owner->SetMoveSpeed(speed);
			owner->SetMoveInput(0.0f, 1.0f);
		}

		void Exit(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{

		}
	}
	
	namespace DamageBig
	{
		bool Transition(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			return (aiData->changeState && (aiData->nextState == BattleEnemyState::DamageBig));
		}

		void Enter(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			owner->SetAnimation(BattleEnemyState::DamageBig);
			owner->SetMoveSpeed(Enemy::DamageSpeed);
			aiData->changeState = false;
		}

		void Update(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			if (!owner->GetModel()->IsPlaying())
			{
				aiData->ChangeNextState();
			}

			Phoenix::f32 speed = owner->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.25f * aiData->elapsedTime);
			owner->SetMoveSpeed(speed);
			owner->SetMoveInput(0.0f, 1.0f);
		}

		void Exit(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{

		}
	}
	
	namespace KnockBack
	{
		bool Transition(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			return (aiData->changeState && (aiData->nextState == BattleEnemyState::KnockBack));
		}

		void Enter(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			owner->SetAnimation(BattleEnemyState::KnockBack);
			owner->SetMoveSpeed(Enemy::KnockBackSpeed);
			aiData->changeState = false;
		}

		void Update(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			if (!owner->GetModel()->IsPlaying())
			{
				aiData->changeState = true;
				aiData->nextState = BattleEnemyState::GettingUp;
			}

			if (owner->GetHitWall())
			{
				owner->SetMoveSpeed(0.0f);
			}
			
			Phoenix::f32 speed = owner->GetMoveSpeed();
			speed = Phoenix::Math::f32Lerp(speed, 0.0f, 0.05f * aiData->elapsedTime);
			owner->SetMoveSpeed(speed);
			owner->SetMoveInput(0.0f, 1.0f);
		}

		void Exit(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{

		}
	}
	
	namespace GettingUp
	{
		bool Transition(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			return (aiData->changeState && (aiData->nextState == BattleEnemyState::GettingUp));
		}

		void Enter(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			owner->SetAnimation(BattleEnemyState::GettingUp);
			aiData->changeState = false;
		}

		void Update(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			if (!owner->GetModel()->IsPlaying() && !owner->GetChangeAnimation())
			{
				aiData->changeState = true;
				aiData->nextState = BattleEnemyState::Idle;
			}
		}

		void Exit(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{

		}
	}
	
	namespace Death
	{
		bool Transition(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			return (owner->GetLife() <= 0);
		}

		void Enter(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{
			owner->SetAnimation(BattleEnemyState::Death);
			aiData->changeState = false;
		}

		void Update(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{

		}

		void Exit(std::shared_ptr<Enemy> owner, std::shared_ptr<BattleEnemy::AIData> aiData)
		{

		}
	}
}

namespace BattleBoss
{
#pragma region Walk
	// ����
	std::shared_ptr<Walk> Walk::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<Walk>(owner);
	}

	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
	void Walk::SetUp()
	{
		moveX = 0.0f;

		Phoenix::s32 judge = rand() % 2;

		if (judge)
		{
			moveX = -1.0f;
		}
		else
		{
			moveX = 1.0f;
		}
	}

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	void Walk::CleanUp()
	{

	}

	// �X�V
	BattleEnemyState Walk::Update(Phoenix::f32 elapsedTime)
	{
		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			if (obj->InDistanceHitByAttack())
			{
				obj->SetMoveInput(0.0f, 0.0f);
				obj->SetMoveSpeed(0.0f);
				//return BattleEnemyState::Attack;
				return BattleEnemyState::Idle;
			}
			else if (!obj->InBattleTerritory())
			{
				obj->SetMoveInput(0.0f, 0.0f);
				obj->SetMoveSpeed(0.0f);
				return BattleEnemyState::Run;
			}

			obj->SetMoveInput(0.0f, -1.0f);
			obj->SetMoveSpeed(Speed);
		}

		return BattleEnemyState::NoneState;
	}
#pragma endregion

#pragma region Run
	// ����
	std::shared_ptr<Run> Run::Create(std::weak_ptr<Enemy> owner)
	{
		return std::make_shared<Run>(owner);
	}

	// ��Ԃɓ������Ƃ��ɌĂ΂��֐�
	void Run::SetUp()
	{

	}

	// ���̏�ԂɈڂ�O�ɌĂ΂��֐�
	void Run::CleanUp()
	{

	}

	// �X�V
	BattleEnemyState Run::Update(Phoenix::f32 elapsedTime)
	{
		std::shared_ptr<Enemy> obj = owner.lock();

		if (obj)
		{
			if (obj->InDistanceHitByAttack())
			{
				obj->SetMoveInput(0.0f, 0.0f);
				obj->SetMoveSpeed(0.0f);
				return BattleEnemyState::Idle;
			}

			obj->SetMoveInput(0.0f, -1.0f);
			obj->SetMoveSpeed(Speed);
		}

		return BattleEnemyState::NoneState;
	}
#pragma endregion
}