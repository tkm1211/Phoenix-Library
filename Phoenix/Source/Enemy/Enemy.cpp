#include "Enemy.h"
#include "EnemyManager.h"
#include "../Player/Player.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../AI/StateMachine/BattleEnemyState.h"


// ����
std::shared_ptr<Enemy> Enemy::Create()
{
	return std::make_shared<Enemy>();
}

// �R���X�g���N�^
void Enemy::Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	// ���f���ǂݍ���
	{
		model = std::make_unique<Phoenix::FrameWork::ModelObject>();
		model->Initialize(graphicsDevice);
		model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Enemy\\Enemy\\Idle\\Ready_Idle.fbx"); // "..\\Data\\Assets\\Model\\Enemy\\Idle\\Idle.fbx"  // "..\\Data\\Assets\\Model\\Boss\\Mutant\\Idle\\Mutant_Roaring.fbx"
	}

	// �A�j���[�V�����ǂݍ���
	Phoenix::s32 beginIndex, endIndex;
	{
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Forward\\Walk_Forward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Back\\Walk_Backward.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Right\\Walk_Right.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Left\\Walk_Left.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Run\\Running.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Dedge\\Back_Step.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Damage\\Head_Hit.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Damage\\Head_Hit_Big.fbx", -1);

		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Death\\Dying_Backwards.fbx", -1);

		beginIndex = model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Attack\\Cross_Punch.fbx", -1);
		model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Attack\\Punching.fbx", -1);
		endIndex = model->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Attack\\Hook_Punch.fbx", -1);

		/*model->AddAnimationLayer(0); // 0
		model->AddAnimationLayer(0, 56, 65); // 1

		model->AddAnimationLayer(5); // 2
		model->AddAnimationLayer(6); // 3

		model->AddAnimationLayer(7); // 4
		model->AddAnimationLayer(8); // 5

		model->AddAnimationLayer(9); // 6

		model->AddAnimationLayer(10); // 7
		model->AddAnimationLayer(11); // 8
		model->AddAnimationLayer(12); // 9

		model->AddBlendAnimationToLayer(1, 1, Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f));
		model->AddBlendAnimationToLayer(2, 1, Phoenix::Math::Vector3(0.0f, -1.0f, 0.0f));
		model->AddBlendAnimationToLayer(3, 1, Phoenix::Math::Vector3(1.0f, 0.0f, 0.0f));
		model->AddBlendAnimationToLayer(4, 1, Phoenix::Math::Vector3(-1.0f, 0.0f, 0.0f));*/

		Phoenix::s32 layerNum = 0;
		{
			layerNum = model->AddAnimationLayer();
			layerIndexList.insert(std::make_pair(LayerType::Base, layerNum));

			layerNum = model->AddAnimationLayer(56, 65);
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
				AddState(StateType::Dedge, 6, layerNum);
				AddState(StateType::Death, 9, layerNum);
			}

			// �����g���C���[�Ƀu�����h�c���[�ǉ�
			layerNum = layerIndexList.at(LayerType::LowerBody);
			{
				Phoenix::s32 blendTreeIndex = model->AddBlendTreeToLayer(layerNum);
				model->AddBlendAnimationStateToBlendTree(0, Phoenix::Math::Vector3( 0.0f,  0.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(1, Phoenix::Math::Vector3( 0.0f,  1.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(2, Phoenix::Math::Vector3( 0.0f, -1.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(3, Phoenix::Math::Vector3( 1.0f,  0.0f, 0.0f), layerNum, blendTreeIndex);
				model->AddBlendAnimationStateToBlendTree(4, Phoenix::Math::Vector3(-1.0f,  0.0f, 0.0f), layerNum, blendTreeIndex);
			}
		}
	}

	// �R���W����������
	{
		collisionDatas.resize(4);

		collisionDatas.at(0).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(0).radius = 0.5f;
		collisionDatas.at(0).boneIndex = model->GetBoneIndex("Hips");

		collisionDatas.at(1).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(1).radius = 0.25f;
		collisionDatas.at(1).boneIndex = model->GetBoneIndex("RightHandIndex1");

		collisionDatas.at(2).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(2).radius = 0.25f;
		collisionDatas.at(2).boneIndex = model->GetBoneIndex("LeftHandIndex1");

		collisionDatas.at(3).pos = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		collisionDatas.at(3).radius = 0.25f;
		collisionDatas.at(3).boneIndex = model->GetBoneIndex("RightFoot");
	}

	// �A�^�b�N�f�[�^����
	{
		auto SetAttackData = [&]
		(
			EnemyAttackState animState,
			Phoenix::s32 animIndex,

			Phoenix::f32 playSpeed,
			Phoenix::f32 playBeginTime,
			Phoenix::f32 playEndTime,

			Phoenix::s32 collisionNum,
			Phoenix::f32 collisionBeginTime,
			Phoenix::f32 collisionEndTime
		)
		{
			AttackData data;

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
		{
			// �E�X�g���[�g
			{
				AttackDatas datas;

				datas.AddData(SetAttackData(EnemyAttackState::WeakRight, 7, 1.5f, -1.0f, -1.0f, 1, 56.0f, 71.0f));

				attackDatasList.emplace_back(datas);
			}

			// ���X�g���[�g
			{
				AttackDatas datas;

				datas.AddData(SetAttackData(EnemyAttackState::WeakLeft, 8, 1.5f, -1.0f, -1.0f, 2, 20.0f, 40.0f));

				attackDatasList.emplace_back(datas);
			}

			// �E�t�b�N
			{
				AttackDatas datas;

				datas.AddData(SetAttackData(EnemyAttackState::StrongRight, 9, 1.5f, -1.0f, -1.0f, 1, 60.0f, 72.0f));

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
			std::shared_ptr<Enemy> owner = shared_from_this();
			std::shared_ptr<AI::BattleEnemy::Attack<EnemyAttackState>> attackState = AI::BattleEnemy::Attack<EnemyAttackState>::Create(owner);

			battleAI->SetOwner(owner);
			battleAI->SetUp();

			battleAI->AddState(AI::BattleEnemy::Idle::Create());
			battleAI->AddState(AI::BattleEnemy::Walk::Create(owner));
			battleAI->AddState(AI::BattleEnemy::Run::Create(owner));
			battleAI->AddState(AI::BattleEnemy::Dedge::Create(owner));
			battleAI->AddState(AI::BattleEnemy::DamageSmall::Create(owner));
			battleAI->AddState(AI::BattleEnemy::DamageBig::Create(owner));
			battleAI->AddState(AI::BattleEnemy::Guard::Create());
			battleAI->AddState(AI::BattleEnemy::Death::Create());
			battleAI->AddState(attackState);

			//attackState->AddAttack(EnemyAttackState::WeakRight);
			//attackState->AddAttack(EnemyAttackState::WeakLeft);
			attackState->AddAttack(EnemyAttackState::StrongRight);
			//attackState->AddAttack(EnemyAttackState::StrongLeft);
		}
	}

	// ���̑��̃p�����[�^������
	{
		Initialize();
	}
}

// ������
void Enemy::Initialize()
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
		enable = false;
		alive = false;
		death = false;
		inBattle = false;

		newRotate = Phoenix::Math::Quaternion::Zero;

		life = LifeRange;
		radius = 0.75f;

		attackReceptionTimeCnt = 0.0f;
		attackAnimationSpeed = 0.0f;

		distanceToPlayer = 0.0f;

		moveSpeed = 0.0f;

		moveX = 0.0f;
		moveY = 0.0f;

		currentAttackState = EnemyAttackState::NoneState;
	}
}

// �I����
void Enemy::Finalize()
{
	battleAI->CleanUp();
	battleAI.reset();

	collisionDatas.clear();
	transform.reset();
	model.reset();
}

// �X�V
void Enemy::Update(bool onControl, Phoenix::f32 elapsedTime)
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

// �g�����X�t�H�[���X�V
void Enemy::UpdateTransform()
{
	transform->Update();
}

// ���W�X�V
void Enemy::UpdateTranslate(Phoenix::f32 elapsedTime)
{
	if (0.0f < moveSpeed) // 0�ȉ��̎��Ɍv�Z�����Ă��Ӗ��������̂ŕ��򂵂Ă���B
	{
		Phoenix::f32 cameraY = 0.0f;
		{
			Phoenix::Math::Vector3 dir = transform->GetTranslate() - player->GetPosition();
			dir.y = 0.0f;
			dir = Phoenix::Math::Vector3Normalize(dir);

			cameraY = atan2f(dir.x, dir.z);
		}

		Phoenix::f32 rotateY = 0.0f;
		{
			float len = sqrtf(moveX * moveX + moveY * moveY);

			if (len <= 0)
			{
				moveX = 0;
				moveY = 0;
			}

			float mag = 1 / len;

			moveX *= mag;
			moveY *= mag;

			rotateY = cameraY + atan2f(moveX, moveY);
		}

		Phoenix::Math::Vector3 pos = transform->GetTranslate();
		{
			pos.x += sinf(rotateY) * (moveSpeed * elapsedTime);
			pos.z += cosf(rotateY) * (moveSpeed * elapsedTime);
		}

		transform->SetTranslate(pos);
	}
}

// ��]�X�V
void Enemy::UpdateRotate(Phoenix::f32 elapsedTime)
{
	if (battleAI->GetCurrentStateName() == BattleEnemyState::Idle || battleAI->GetCurrentStateName() == BattleEnemyState::Walk || battleAI->GetCurrentStateName() == BattleEnemyState::Run)
	{
		UpdateNewRotate();
	}

	Phoenix::Math::Quaternion rotate = transform->GetRotate();
	rotate = Phoenix::Math::QuaternionSlerp(rotate, newRotate, 0.17f * elapsedTime);

	transform->SetRotate(rotate);
}

// �A�j���[�V�����X�V
void Enemy::UpdateAnimation(Phoenix::f32 elapsedTime)
{
	if (battleAI->GetCurrentStateName() == BattleEnemyState::Walk)
	{
		model->SetBlendRate(Phoenix::Math::Vector3(-moveX, moveY, 0.0f));
	}

	model->UpdateTransform(elapsedTime / 60.0f);
	attackReceptionTimeCnt += attackAnimationSpeed * elapsedTime / 60.0f;
}

// AI�X�V
void Enemy::UpdateAI(Phoenix::f32 elapsedTime)
{
	BattleEnemyState nextBattleState = BattleEnemyState::NoneState;

	switch (currentMode)
	{
	case EnemyMode::Ordinary:
		break;

	case EnemyMode::Battle:
		nextBattleState = battleAI->Update(elapsedTime);
		/*if (nextBattleState == BattleEnemyState::Idle && stackAttackRight)
		{
			stackAttackRight = false;
			if (battleAI->GetCurrentStateName() == BattleEnemyState::Attack)
			{
				battleAI->GoToState(BattleEnemyState::Idle);
			}
			else
			{
				battleAI->GoToState(BattleEnemyState::Attack);
			}
			SetMoveInput(0.0f, 0.0f);
			SetMoveSpeed(0.0f);
		}
		else if (nextBattleState == BattleEnemyState::Attack)
		{
			battleAI->GoToState(BattleEnemyState::Attack);
			SetMoveInput(0.0f, 0.0f);
			SetMoveSpeed(0.0f);
		}
		else */

		if (nextBattleState != BattleEnemyState::NoneState)
		{
			if (battleAI->GetCurrentStateName() != nextBattleState)
			{
				SetState(nextBattleState);
			}
			SetMoveInput(0.0f, 0.0f);
			SetMoveSpeed(0.0f);
		}

		break;

	default: break;
	}
}

// �����蔻��X�V
void Enemy::UpdateCollision()
{
	Phoenix::Math::Matrix systemUnitTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	systemUnitTransform._11 = systemUnitTransform._22 = systemUnitTransform._33 = 0.01f;

	auto nodes = model->GetNodes();
	for (auto& data : collisionDatas)
	{
		Phoenix::Math::Matrix bone = nodes->at(data.boneIndex).worldTransform;
		bone *= systemUnitTransform * transform->GetWorldTransform();
		data.pos = Phoenix::Math::Vector3(bone._41, bone._42, bone._43);
	}
}

// UI�X�V
void Enemy::UpdateUI(Phoenix::Math::Vector2 pos)
{
	Phoenix::f32 hp = static_cast<Phoenix::f32>(life);
	hp = hp <= 0 ? 0 : hp;

	ui->Update((hp / LifeRange) * 100.0f);
	ui->SetExit(alive);
	ui->SetPos(pos);
}

// �U������
void Enemy::AttackJudgment()
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
					if (currentAttackState == EnemyAttackState::WeakRight || currentAttackState == EnemyAttackState::WeakLeft)
					{
						attackPower = 0;
					}
					else if (currentAttackState == EnemyAttackState::StrongRight || currentAttackState == EnemyAttackState::StrongLeft)
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

// �v���C���[�Ƃ̋����v��
void Enemy::DistanceMeasurement()
{
	distanceToPlayer = Phoenix::Math::Vector3Length(player->GetPosition() - transform->GetTranslate());
}

// �v���C���[�̍U������ɂ��邩����
bool Enemy::JudgePlayerAttackRange()
{
	Phoenix::Math::Vector3 dir = GetPosition() - player->GetPosition();
	dir = Phoenix::Math::Vector3Normalize(dir);
	dir.y = 0.0f;

	Phoenix::Math::Quaternion rotate = player->GetRotate();
	Phoenix::Math::Matrix m = Phoenix::Math::MatrixRotationQuaternion(&rotate);
	Phoenix::Math::Vector3 forward = Phoenix::Math::Vector3(m._31, m._32, m._33);
	forward.y = 0.0f;

	Phoenix::f32 angle;
	angle = acosf(Phoenix::Math::Vector3Dot(dir, forward));

	if (1e-8f < fabs(angle))
	{
		angle /= 0.01745f;

		if (angle <= 45.0f)
		{
			return true;
		}
	}

	return false;
}

// �V���ȉ�]�l�̍X�V
void Enemy::UpdateNewRotate()
{
	Phoenix::Math::Vector3 dir = Phoenix::Math::Vector3Normalize(player->GetPosition() - GetPosition());
	float len = sqrtf(dir.x * dir.x + dir.z * dir.z);

	if (len <= 0)
	{
		dir.x = 0;
		dir.z = 0;
	}

	float mag = 1 / len;

	dir.x *= mag;
	dir.z *= mag;

	Phoenix::f32 angleY = atan2f(dir.x, dir.z);

	newRotate = Phoenix::Math::QuaternionRotationAxis(Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), angleY);
}

// �`��
void Enemy::Draw() {}

// GUI
void Enemy::GUI(Phoenix::s32 index)
{
	std::string name = std::string("Enemy") + std::to_string(index);
	if (ImGui::TreeNode(name.c_str()))
	{
		transform->GUI();
		ImGui::TreePop();
	}
}

// �L�����ݒ�
void Enemy::SetEnable(bool enable)
{
	this->enable = enable;
}

// ���݃t���O�ݒ�
void Enemy::SetAlive(bool alive)
{
	this->alive = alive;
}

// ���S�t���O�ݒ�
void Enemy::SetDeath(bool death)
{
	this->death = death;
}

// �퓬���t���O�ݒ�
void Enemy::SetInBattle(bool inBattle)
{
	this->inBattle = inBattle;
	currentMode = inBattle ? EnemyMode::Battle : EnemyMode::Ordinary;
}

// �g�����X�t�H�[���̐ݒ�
void Enemy::SetTransform(Phoenix::FrameWork::Transform transform)
{
	this->transform->SetTranslate(transform.GetTranslate());
	this->transform->SetRotate(transform.GetRotate());
	this->transform->SetScale(transform.GetScale());
}

// �g�����X���[�g�̐ݒ�
void Enemy::SetTranslate(Phoenix::Math::Vector3 translate)
{
	this->transform->SetTranslate(translate);
}

// �V���ȉ�]�l��ݒ�
void Enemy::SetNewRotate(Phoenix::Math::Quaternion newRotate)
{
	this->newRotate = newRotate;
}

// �G�l�~�[�}�l�[�W���[�̐ݒ�
void Enemy::SetOwner(std::shared_ptr<EnemyManager> owner)
{
	this->owner = owner;
}

// �X�e�[�g��ύX
void Enemy::SetState(BattleEnemyState state, bool forcedChange)
{
	Phoenix::s32 check = battleAI->GoToState(state, forcedChange);

	if (check != -1)
	{
		changeAnimation = true;
		changeState = state;
	}
}

// �U�����𔭍s
bool Enemy::SetAttackRight(bool stackAttackRight)
{
	if (battleAI->GetCurrentStateName() == BattleEnemyState::Idle)
	{
		battleAI->GoToState(BattleEnemyState::Attack, true);
		return true;
	}
	else if (stackAttackRight)
	{
		this->stackAttackRight = stackAttackRight;
	}

	return false;
}

// �U���X�e�[�g��ύX
void Enemy::SetAttackState(EnemyAttackState state)
{
	changeAttackAnimation = true;
	changeAttackState = state;
	currentAttackState = state;

	for (const auto& data : attackDatasList)
	{
		if (state == data.datas.at(0).animState)
		{
			attackAnimationSpeed = data.datas.at(0).playSpeed;
		}
	}
	attackReceptionTimeCnt = 0.0f;
}

// �v���C���[��ݒ�
void Enemy::SetPlayer(std::shared_ptr<Player> player)
{
	this->player = player;
	DistanceMeasurement();
}

// �ړ��X�s�[�h�ݒ�
void Enemy::SetMoveSpeed(Phoenix::f32 speed)
{
	moveSpeed = speed;
}

// �ړ������̎w��
void Enemy::SetMoveInput(Phoenix::f32 moveX, Phoenix::f32 moveY)
{
	this->moveX = moveX;
	this->moveY = moveY;
}

// �A�j���[�V�������ڍs
void Enemy::ChangeAnimation()
{
	if (!changeAnimation) return;

	Phoenix::s32 baseLayerIndex = layerIndexList.at(LayerType::Base);
	Phoenix::s32 lowerBodyLayerIndex = layerIndexList.at(LayerType::LowerBody);

	switch (changeState)
	{
	case BattleEnemyState::Idle:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::Idle), 1, 0.2f);
		model->SetLoopAnimation(true);
		break;

	case BattleEnemyState::Walk:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::Idle), 1, 0.2f);
		model->SimultaneousPlayBlendTreeAniamation(lowerBodyLayerIndex, 0, 1, 0.2f);
		model->SetLoopAnimation(true);
		model->SetBlendLoopAnimation(true);
		break;

	case BattleEnemyState::Run:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::Run), 1, 0.2f);
		model->SetLoopAnimation(true);
		break;

	case BattleEnemyState::Attack: break;

	case BattleEnemyState::Dedge:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::Dedge), 0, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(2.0f);
		break;

	case BattleEnemyState::DamageSmall:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::DamageSmall), 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetEndTime(43.0f / 60.0f);
		break;

	case BattleEnemyState::DamageBig:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::DamageBig), 1, 0.2f);
		model->SetLoopAnimation(false);
		break;

	case BattleEnemyState::Guard:
		model->PlayAnimation(baseLayerIndex, 0, 1, 0.2f);
		model->SetLoopAnimation(false);
		break;

	case BattleEnemyState::Death:
		model->PlayAnimation(baseLayerIndex, stateIndexList.at(StateType::Death), 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetBeginTime(55.0f / 60.0f);
		model->SetCurrentTime(42.0f / 60.0f);
		break;

	case BattleEnemyState::NoneState: break;

	default: break;
	}

	changeAnimation = false;
	changeState = BattleEnemyState::NoneState;
}

// �U���X�e�[�g���ڍs
void Enemy::ChangeAttackAnimation()
{
	if (!changeAttackAnimation) return;

	Phoenix::s32 baseLayerIndex = layerIndexList.at(LayerType::Base);

	switch (changeAttackState)
	{
	case EnemyAttackState::WeakRight:
		model->PlayAnimation(baseLayerIndex, 0, 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(attackAnimationSpeed);

		break;

	case EnemyAttackState::WeakLeft:
		model->PlayAnimation(baseLayerIndex, 1, 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(attackAnimationSpeed);

		break;

	case EnemyAttackState::StrongRight:
		model->PlayAnimation(baseLayerIndex, 2, 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(attackAnimationSpeed);

		break;

	case EnemyAttackState::StrongLeft:
		model->PlayAnimation(baseLayerIndex, 0, 1, 0.2f);
		model->SetLoopAnimation(false);
		model->SetSpeed(attackAnimationSpeed);

		break;

	case EnemyAttackState::NoneState: break;

	default: break;
	}

	changeAttackAnimation = false;
	changeAttackState = EnemyAttackState::NoneState;
}

void Enemy::Damage(int damage)
{
	if (battleAI->GetCurrentStateName() == BattleEnemyState::Dedge) return;

	life -= damage;
	if (damage <= 10)
	{
		SetMoveInput(0.0f, 0.0f);
		SetMoveSpeed(0.0f);

		battleAI->GoToState(BattleEnemyState::DamageSmall, true);

		changeAnimation = true;
		changeState = BattleEnemyState::DamageSmall;
	}
	else if (damage <= 20)
	{
		SetMoveInput(0.0f, 0.0f);
		SetMoveSpeed(0.0f);

		battleAI->GoToState(BattleEnemyState::DamageBig, true);

		changeAnimation = true;
		changeState = BattleEnemyState::DamageBig;
	}
}

// �L���t���O�擾
bool Enemy::GetEnable()
{
	return enable;
}

// ���݃t���O�擾
bool Enemy::GetAlive()
{
	return alive;
}

// ���S�t���O�擾
bool Enemy::GetDeath()
{
	return death;
}

// �퓬���t���O�擾
bool Enemy::GetInBattle()
{
	return inBattle;
}

// �g�����X�t�H�[���̎擾
Phoenix::FrameWork::Transform Enemy::GetTransform()
{
	return *transform.get();
}

// �o�g�����[�h�̃X�e�[�g�擾
BattleEnemyState Enemy::GetBattleState()
{
	return battleAI->GetCurrentStateName();
}

// �v���C���[�̐퓬�G���A�ɐN��������
bool Enemy::InBattleTerritory()
{
	if (distanceToPlayer <= 5.0f)
	{
		return true;
	}

	return false;
}

// �v���C���[�ɍU���������鋗���ɓ����Ă��邩�H
bool Enemy::InDistanceHitByAttack()
{
	if (distanceToPlayer <= 1.5f)
	{
		return true;
	}

	return false;
}