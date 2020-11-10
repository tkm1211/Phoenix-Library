#include "Enemy.h"
#include "EnemyManager.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"


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
		model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Enemy\\Idle\\Idle.fbx"); // "..\\Data\\Assets\\Model\\Enemy\\Idle\\Idle.fbx"  // "..\\Data\\Assets\\Model\\Boss\\Mutant\\Idle\\Mutant_Roaring.fbx"
		model->AddAnimationLayer(0);
	}

	// �A�j���[�V�����ǂݍ���
	{
		//model->LoadAnimation("..\\Data\\Assets\\Model\\Boss\\Mutant\\Run\\Mutant_Run.fbx", -1);
		//model->AddAnimationLayer(1);
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

	// �g�����X�t�H�[���̏�����
	{
		transform = std::make_unique<Phoenix::FrameWork::Transform>();
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
			battleAI->SetOwner(shared_from_this());
			battleAI->SetUp();
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
		model->PlayAnimation(0, 1);
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
		inBattle = false;

		life = LifeRange;
		radius = 0.5f;
	}
}

// �I����
void Enemy::Finalize()
{
	battleAI.reset();

	collisionDatas.clear();
	transform.reset();
	model.reset();
}

// �X�V
void Enemy::Update()
{
	// AI�X�V
	{
		BattleEnemyState nextBattleState = BattleEnemyState::NoneState;

		switch (currentMode)
		{
		case EnemyMode::Ordinary:
			break;

		case EnemyMode::Battle:
			nextBattleState = battleAI->Update();
			if (nextBattleState != BattleEnemyState::NoneState)
			{
				changeAnimation = true;
				changeState = nextBattleState;
				battleAI->GoToState(nextBattleState);
			}

			break;

		default: break;
		}
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
		model->UpdateTransform(1 / 60.0f);
	}

	// �g�����X�t�H�[���X�V
	{
		transform->Update();
	}

	// �R���W�����X�V
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

	// ���C�t���O�Ȃ�}�l�[�W���[�̐����G�l�~�[�J�E���g��������
	if (life <= 0)
	{
		if (std::shared_ptr<EnemyManager> manager = owner.lock())
		{
			manager->SubAliveEnemyCount(1);
		}
	}
}

// UI�X�V
void Enemy::UpdateUI()
{

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

// �G�l�~�[�}�l�[�W���[�̐ݒ�
void Enemy::SetOwner(std::shared_ptr<EnemyManager> owner)
{
	this->owner = owner;
}

// �U�����𔭍s
void Enemy::SetAttackRight()
{
	battleAI->GoToState(BattleEnemyState::Attack);
}

// �U���X�e�[�g��ύX
void Enemy::SetAttackState(EnemyAttackState state)
{
	changeAttackAnimation = true;
	changeAttackState = state;
}

// �A�j���[�V�������ڍs
void Enemy::ChangeAnimation()
{
	if (!changeAnimation) return;

	switch (changeState)
	{
	case BattleEnemyState::Idle:
		model->PlayAnimation(0, 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(true);
		break;

	case BattleEnemyState::Attack: break;

	case BattleEnemyState::Dedge:
		model->PlayAnimation(0, 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
		break;

	case BattleEnemyState::Guard:
		model->PlayAnimation(0, 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);
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

	switch (changeAttackState)
	{
	case EnemyAttackState::WeakRight:
		model->PlayAnimation(0, 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);

		break;

	case EnemyAttackState::WeakLeft:
		model->PlayAnimation(0, 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);

		break;

	case EnemyAttackState::StrongRight:
		model->PlayAnimation(0, 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);

		break;

	case EnemyAttackState::StrongLeft:
		model->PlayAnimation(0, 1);
		model->UpdateTransform(1 / 60.0f);
		model->SetLoopAnimation(false);

		break;

	case EnemyAttackState::NoneState: break;

	default: break;
	}

	changeAttackAnimation = false;
	changeAttackState = EnemyAttackState::NoneState;
}

void Enemy::Damage(int damage)
{
	life -= damage;
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