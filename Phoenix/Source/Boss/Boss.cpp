#include "Boss.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../Enemy/EnemyManager.h"


// ����
std::shared_ptr<Boss> Boss::Create()
{
	return std::make_shared<Boss>();
}

// �R���X�g���N�^
void Boss::Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	// ���f���ǂݍ���
	{
		model = std::make_unique<Phoenix::FrameWork::ModelObject>();
		model->Initialize(graphicsDevice);
		model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Enemy\\Boss02\\Idle\\Idle.fbx");
		model->SetHipID("Mutant:Hips");
	}

	// �A�j���[�V�����ǂݍ���
	Phoenix::s32 beginIndex, endIndex;
	{
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Walk\\Walk_Forward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Walk\\Walk_Backward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Walk\\Walk_Right.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Walk\\Walk_Left.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Run\\Mutant_Run.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Avoid\\Back_Step.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Damage\\Head_Hit.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Damage\\Head_Hit_Big.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Damage\\Flying_Back_Death.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Idle\\Getting_Up.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Death\\Mutant_Dying.fbx", -1);

		beginIndex = model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Attack\\Right\\Mutant_Swiping.fbx", -1);
					 model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Attack\\Left\\Hook_Punch.fbx", -1);
		endIndex   = model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Boss02\\Attack\\Rotate\\Right_Rotate.fbx", -1);

		Phoenix::s32 layerNum = 0;
		{
			layerNum = model->AddAnimationLayer();
			layerIndexList.insert(std::make_pair(LayerType::Base, layerNum));

			layerNum = model->AddAnimationLayer(29, 38);
			layerIndexList.insert(std::make_pair(LayerType::LowerBody, layerNum));
		}

		// �X�e�[�g�ǉ�
		Phoenix::s32 stateNum = 0;
		{
			auto AddState = [&](StateType type, Phoenix::u32 animationIndex, Phoenix::u32 layerIndex)
			{
				stateNum = model->AddAnimationStateToLayer(animationIndex, layerIndex);
				stateIndexList.insert(std::make_pair(type, stateNum));
			};

			// �x�[�X���C���[�ɃX�e�[�g�ǉ�
			layerNum = layerIndexList.at(LayerType::Base);
			{
				for (Phoenix::s32 i = beginIndex; i <= endIndex; ++i)
				{
					model->AddAnimationStateToLayer(i, layerNum);
				}

				AddState(StateType::Idle, 0, layerNum);
				AddState(StateType::Run, 5, layerNum);
				AddState(StateType::DamageSmall, 7, layerNum);
				AddState(StateType::DamageBig, 8, layerNum);
				AddState(StateType::KnockBack, 9, layerNum);
				AddState(StateType::GettingUp, 10, layerNum);
				AddState(StateType::Dedge, 6, layerNum);
				AddState(StateType::Death, 11, layerNum);
			}

			// �����g���C���[�Ƀu�����h�c���[�ǉ�
			layerNum = layerIndexList.at(LayerType::LowerBody);
			{
				Phoenix::s32 blendTreeIndex = model->AddBlendTreeToLayer(layerNum);
				model->AddBlendAnimationStateToBlendTree(0, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(1, Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(2, Phoenix::Math::Vector3(0.0f, -1.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(3, Phoenix::Math::Vector3(1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(4, Phoenix::Math::Vector3(-1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
			}
		}
	}

	// �R���W����������
	{
		collisionDatas.resize(3);

		collisionDatas.at(0).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(0).radius = 0.5f;
		collisionDatas.at(0).boneIndex = model->GetBoneIndex("Mutant:Hips");

		collisionDatas.at(1).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(1).radius = 0.5f;
		collisionDatas.at(1).boneIndex = model->GetBoneIndex("Mutant:RightHandIndex1");

		collisionDatas.at(2).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(2).radius = 1.3f;
		collisionDatas.at(2).boneIndex = model->GetBoneIndex("Mutant:LeftHand");
	}

	// �A�^�b�N�f�[�^����
	{
		auto SetAttackData = [&]
		(
			BossAttackState animState,
			Phoenix::s32 animIndex,

			Phoenix::f32 playSpeed,
			Phoenix::f32 playBeginTime,
			Phoenix::f32 playEndTime,

			Phoenix::s32 collisionNum,
			Phoenix::f32 collisionBeginTime,
			Phoenix::f32 collisionEndTime
		)
		{
			AttackData<BossAttackState> data;

			data.animState = animState;
			data.animIndex = animIndex;

			data.playSpeed = playSpeed;
			data.playBeginTime = playBeginTime == -1.0f ? -1.0f : playBeginTime / 60.0f;
			data.playEndTime = playEndTime == -1.0f ? -1.0f : playEndTime / 60.0f;

			data.collisionNum = collisionNum;
			data.collisionBeginTime = collisionBeginTime == -1.0f ? -1.0f : collisionBeginTime / 60.0f;
			data.collisionEndTime = collisionEndTime == -1.0f ? -1.0f : collisionEndTime / 60.0f;

			return data;
		};

		// ��U��
		{ // TODO : Add attack data.
			// �E�t�b�N
			{
				AttackDatas<BossAttackState> datas;

				datas.AddData(SetAttackData(BossAttackState::RightHook, 7, 1.0f, -1.0f, 150.0f, 2, 75.0f, 85.0f));

				attackDatasList.emplace_back(datas);
			}

			// ���t�b�N
			{
				AttackDatas<BossAttackState> datas;

				datas.AddData(SetAttackData(BossAttackState::LeftHook, 8, 1.0f, -1.0f, 96.0f, 1, 55.0f, 70.0f));

				attackDatasList.emplace_back(datas);
			}

			// ����]
			{
				AttackDatas<BossAttackState> datas;

				datas.AddData(SetAttackData(BossAttackState::LeftTurn, 8, 1.0f, -1.0f, -1.0f, 2, 55.0f, 65.0f));

				attackDatasList.emplace_back(datas);
			}
		}
	}

	// �g�����X�t�H�[���̏�����
	{
		transform = std::make_unique<Phoenix::FrameWork::Transform>();
	}

	// UI����
	{
		ui = EnemyUI::Create();
	}

	// AI�̏�����
	{
		currentMode = EnemyMode::Battle;

		// �ʏ탂�[�hAI
		//ordinaryAI = OrdinaryAIEnemyAI::Create();
		{
			//ordinaryAI->SetOwner(shared_from_this());
		}

		// �o�g�����[�hAI
		battleAI = BattleEnemyAI::Create();
		{
			std::weak_ptr<Boss> owner = downcasted_shared_from_this<Boss>();
			attackState = BattleEnemy::Attack<BossAttackState, Boss>::Create(owner);

			battleAI->SetOwner(owner);
			battleAI->SetUp();
			
			battleAI->AddState(BattleEnemy::Idle::Create());
			battleAI->AddState(BattleEnemy::Walk::Create(owner));
			battleAI->AddState(BattleBoss::Run::Create(owner));
			battleAI->AddState(BattleEnemy::Dedge::Create(owner));
			battleAI->AddState(BattleEnemy::DamageSmall::Create(owner));
			battleAI->AddState(BattleEnemy::DamageBig::Create(owner));
			battleAI->AddState(BattleEnemy::KnockBack::Create(owner));
			battleAI->AddState(BattleEnemy::GettingUp::Create(owner));
			battleAI->AddState(BattleEnemy::Guard::Create());
			battleAI->AddState(BattleEnemy::Death::Create());
			battleAI->AddState(attackState);
			
			attackState->AddAttack(BossAttackState::RightHook);
			attackState->AddAttack(BossAttackState::LeftHook);
			attackState->AddAttack(BossAttackState::LeftTurn);
		}
	}

	// ���̑��̃p�����[�^������
	{
		Initialize();
	}
}

// ������
void Boss::Initialize()
{
	// �ҋ@���[�V�����J�n
	{
		model->PlayAnimation(layerIndexList.at(LayerType::Base), stateIndexList.at(StateType::Idle), 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
	}

	// AI
	{
		battleAI->GoToState(BattleEnemyState::Idle);
	}

	// �p�����[�^�[
	{
		typeTag = TypeTag::Large;

		enable = false;
		alive = false;
		death = false;
		inBattle = false;

		newRotate = Phoenix::Math::Quaternion::Zero;

		life = LifeRange;
		lifeMax = LifeRange;
		radius = 1.0f;

		attackReceptionTimeCnt = 0.0f;
		attackAnimationSpeed = 0.0f;

		distanceToPlayer = 0.0f;

		moveSpeed = 0.0f;

		moveX = 0.0f;
		moveY = 0.0f;

		currentAttackState = BossAttackState::NoneState;
	}
}

// �I����
void Boss::Finalize()
{
	battleAI->CleanUp();
	battleAI.reset();

	collisionDatas.clear();
	transform.reset();
	model.reset();
}

// �X�V
void Boss::Update(bool onControl, Phoenix::f32 elapsedTime)
{
	// ���C�t���O�Ȃ�}�l�[�W���[�̐����G�l�~�[�J�E���g��������
	if (life <= 0 && alive)
	{
		alive = false;
		if (std::shared_ptr<EnemyManager> manager = owner.lock())
		{
			manager->SubAliveEnemyCount(1);
		}

		SetState(BattleEnemyState::Death, true);
		ChangeAnimation();
	}

	// ���S�A�j���[�V�����I�����ɑ��݂�����
	if (!alive)
	{
		if (!model->IsPlaying())
		{
			death = true;
		}
		else
		{
			model->UpdateTransform(elapsedTime / 60.0f);
		}
	}

	if (!enable) return;
	if (!alive) return;
	if (death) return;
	if (!onControl)
	{
		if (!model->IsPlaying())
		{
			SetState(BattleEnemyState::Idle);
		}
		model->UpdateTransform(elapsedTime / 60.0f);

		UpdateRotate(elapsedTime);
		UpdateTranslate(elapsedTime);
		UpdateTransform();
		UpdateCollision();

		return;
	}

	// AI�X�V
	{
		UpdateAI(elapsedTime);
	}

	// ��]
	{
		UpdateRotate(elapsedTime);
	}

	// �ړ�
	{
		UpdateTranslate(elapsedTime);
	}

	// �A�j���[�V�����̐؂�ւ�
	{
		ChangeAnimation();
	}

	// �U���A�j���[�V�����̐؂�ւ�
	{
		ChangeAttackAnimation();
	}

	// �A�j���[�V�����X�V
	{
		UpdateAnimation(elapsedTime);
	}

	// �g�����X�t�H�[���X�V
	{
		UpdateTransform();
	}

	// �R���W�����X�V
	{
		UpdateCollision();
	}

	// �v���C���[�Ƃ̋����v��
	{
		DistanceMeasurement();
	}

	// �A�^�b�N���蒆
	{
		AttackJudgment();
	}
}

// �`��
void Boss::Draw() {}

// GUI
void Boss::GUI(Phoenix::s32 index)
{

}

// �U���X�e�[�g���ڍs
void Boss::ChangeAttackAnimation()
{
	if (!changeAttackAnimation) return;

	Phoenix::s32 baseLayerIndex = layerIndexList.at(LayerType::Base);

	switch (changeAttackState)
	{
	case BossAttackState::RightHook:
		model->PlayAnimation(baseLayerIndex, 0, 1, 0.5f);
		model->SetLoopAnimation(false);
		model->SetSpeed(attackAnimationSpeed);
		if (0.0f <= playBeginTime)
		{
			model->SetBeginTime(playBeginTime);
			model->SetCurrentTime(playBeginTime);
		}
		if (0.0f < playEndTime)
		{
			model->SetEndTime(playEndTime);
		}

		break;

	case BossAttackState::LeftHook:
		model->PlayAnimation(baseLayerIndex, 1, 1, 0.5f);
		model->SetLoopAnimation(false);
		model->SetSpeed(attackAnimationSpeed);
		if (0.0f <= playBeginTime)
		{
			model->SetBeginTime(playBeginTime);
			model->SetCurrentTime(playBeginTime);
		}
		if (0.0f < playEndTime)
		{
			model->SetEndTime(playEndTime);
		}

		break;

	case BossAttackState::LeftTurn:
		model->PlayAnimation(baseLayerIndex, 2, 1, 0.5f);
		model->SetLoopAnimation(false);
		model->SetSpeed(attackAnimationSpeed);
		if (0.0f <= playBeginTime)
		{
			model->SetBeginTime(playBeginTime);
			model->SetCurrentTime(playBeginTime);
		}
		if (0.0f < playEndTime)
		{
			model->SetEndTime(playEndTime);
		}

		break;

	case BossAttackState::NoneState: break;

	default: break;
	}

	changeAttackAnimation = false;
	changeAttackState = BossAttackState::NoneState;
}

// �U���X�e�[�g��ύX
void Boss::SetAttackState(BossAttackState state)
{
	changeAttackAnimation = true;
	changeAttackState = state;
	currentAttackState = state;

	for (const auto& data : attackDatasList)
	{
		if (state == data.datas.at(0).animState)
		{
			attackAnimationSpeed = data.datas.at(0).playSpeed;
			playBeginTime = data.datas.at(0).playBeginTime;
			playEndTime = data.datas.at(0).playEndTime;
		}
	}
	attackReceptionTimeCnt = 0.0f;
}

// �U������
void Boss::AttackJudgment()
{
	if (battleAI->GetCurrentStateName() == BattleEnemyState::Attack)
	{
		auto Judgment = [&](Phoenix::s32 index)
		{
			if (isHit)
			{
				isAttackJudgment = false;
				return;
			}

			isAttackJudgment = true;
			attackCollisionIndex = index;
		};
		auto NoJudgment = [&]()
		{
			isAttackJudgment = false;
			isHit = false;
			attackCollisionIndex = -1;
		};

		//Phoenix::u32 index = static_cast<Phoenix::u32>(attackState);

		//// �����蔻��
		//if (attackDatasList.at(index).datas.at(attackComboState).collisionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= attackDatasList.at(index).datas.at(attackComboState).collisionEndTime)
		//{
		//	Judgment(attackDatasList.at(index).datas.at(attackComboState).collisionNum);
		//	attackPower = attackDatasList.at(index).receptionKey == AttackKey::WeakAttack ? 0 : 1;
		//}
		//else
		//{
		//	NoJudgment();
		//}

		bool hit = false;
		for (const auto& data : attackDatasList)
		{
			if (currentAttackState == data.datas.at(0).animState)
			{
				if (data.datas.at(0).collisionBeginTime <= attackReceptionTimeCnt && attackReceptionTimeCnt <= data.datas.at(0).collisionEndTime)
				{
					hit = true;
					Judgment(data.datas.at(0).collisionNum);
					if (currentAttackState == BossAttackState::RightHook)
					{
						attackPower = 0;
					}
					else if (currentAttackState == BossAttackState::LeftHook || currentAttackState == BossAttackState::LeftTurn)
					{
						attackPower = 1;
					}
				}
			}
		}
		if (!hit)
		{
			NoJudgment();
		}
	}
	else
	{
		isAttackJudgment = false;
	}
}

// �_���[�W
bool Boss::Damage(Phoenix::s32 damage)
{
	if (battleAI->GetCurrentStateName() == BattleEnemyState::Dedge) return false;

	bool downDamage = false;

	if (damage <= 10)
	{
		life -= damage;
		accumulationDamage += damage;

		SetMoveInput(0.0f, 0.0f);
		SetMoveSpeed(0.0f);

		downDamage = true;
	}
	else if (damage <= 20)
	{
		life -= damage;
		accumulationDamage += damage;

		// �~�σ_���[�W�̊m�F
		if (!AccumulationDamage(damage))
		{
			SetMoveInput(0.0f, 0.0f);
			SetMoveSpeed(0.0f);

			battleAI->GoToState(BattleEnemyState::DamageBig, true);

			changeAnimation = true;
			changeState = BattleEnemyState::DamageBig;
		}

		downDamage = true;
	}

	// ���C�t���O�Ȃ�}�l�[�W���[�̐����G�l�~�[�J�E���g��������
	if (life <= 0 && alive)
	{
		alive = false;
		if (std::shared_ptr<EnemyManager> manager = owner.lock())
		{
			manager->SubAliveEnemyCount(1);
		}

		SetState(BattleEnemyState::Death, true);
		ChangeAnimation();
	}

	return downDamage;
}

// �~�σ_���[�W
bool Boss::AccumulationDamage(Phoenix::s32 damage)
{
	accumulationDamage += damage;

	if (AccumulationMaxDamage <= accumulationDamage)
	{
		accumulationDamage = 0;

		SetMoveInput(0.0f, 0.0f);
		SetMoveSpeed(0.0f);

		battleAI->GoToState(BattleEnemyState::KnockBack, true);

		changeAnimation = true;
		changeState = BattleEnemyState::KnockBack;

		return true;
	}

	return false;
}

// �v���C���[�ɍU���������鋗���ɓ����Ă��邩�H
bool Boss::InDistanceHitByAttack()
{
	if (distanceToPlayer <= 2.5f)
	{
		return true;
	}

	return false;
}