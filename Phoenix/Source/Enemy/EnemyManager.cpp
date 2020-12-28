#include "EnemyManager.h"
#include "Enemy.h"
#include "../Player/Player.h"
//#include "../UI/EnemiesUI.h"


// ����
std::shared_ptr<EnemyManager> EnemyManager::Create()
{
	return std::make_shared<EnemyManager>();
}

// �R���X�g���N�^
void EnemyManager::Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	CreateBossModel(graphicsDevice);

	enemies.resize(EnemyRange);

	for (auto& enemy : enemies)
	{
		enemy = std::make_shared<Enemy>();
		enemy->Construct(graphicsDevice);
		enemy->SetBossModel(bossModel);
	}

	enemiesUI = EnemiesUI::Create();
	enemiesUI->Initialize(graphicsDevice, EnemyRange);

	aliveEnemyCount = 0;
}

// �{�X���f���ǂݍ���
void EnemyManager::CreateBossModel(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
{
	// ���f���ǂݍ���
	{
		bossModel = std::make_unique<Phoenix::FrameWork::ModelObject>();
		bossModel->Initialize(graphicsDevice);
		bossModel->Load(graphicsDevice, "..\\Data\\Assets\\Model\\Enemy\\Boss\\Idle\\Ready_Idle.fbx");
	}

	// �A�j���[�V�����ǂݍ���
	Phoenix::s32 beginIndex, endIndex;
	{
		bossModel->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Forward\\Walk_Forward.fbx", -1);
		bossModel->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Back\\Walk_Backward.fbx", -1);
		bossModel->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Right\\Walk_Right.fbx", -1);
		bossModel->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Walk\\Left\\Walk_Left.fbx", -1);

		bossModel->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Run\\Running.fbx", -1);

		bossModel->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Dedge\\Back_Step.fbx", -1);

		bossModel->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Damage\\Head_Hit.fbx", -1);
		bossModel->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Damage\\Head_Hit_Big.fbx", -1);

		bossModel->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Death\\Dying_Backwards.fbx", -1);

		beginIndex = bossModel->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Attack\\Cross_Punch.fbx", -1);
		bossModel->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Attack\\Punching.fbx", -1);
		endIndex = bossModel->LoadAnimation("..\\Data\\Assets\\Model\\Enemy\\Enemy\\Attack\\Hook_Punch.fbx", -1);

		Phoenix::s32 layerNum = 0;
		{
			layerNum = bossModel->AddAnimationLayer();
			layerNum = bossModel->AddAnimationLayer(56, 65);
		}

		// �X�e�[�g�ǉ�
		Phoenix::s32 stateNum = 0;
		{
			auto AddState = [&](Enemy::StateType type, Phoenix::u32 animationIndex, Phoenix::u32 layerIndex)
			{
				stateNum = bossModel->AddAnimationStateToLayer(animationIndex, layerIndex);
			};

			// �x�[�X���C���[�ɃX�e�[�g�ǉ�
			layerNum = 0;
			{
				for (Phoenix::s32 i = beginIndex; i <= endIndex; ++i)
				{
					bossModel->AddAnimationStateToLayer(i, layerNum);
				}

				AddState(Enemy::StateType::Idle, 0, layerNum);
				AddState(Enemy::StateType::Run, 5, layerNum);
				AddState(Enemy::StateType::DamageSmall, 7, layerNum);
				AddState(Enemy::StateType::DamageBig, 8, layerNum);
				AddState(Enemy::StateType::Dedge, 6, layerNum);
				AddState(Enemy::StateType::Death, 9, layerNum);
			}

			// �����g���C���[�Ƀu�����h�c���[�ǉ�
			layerNum = 1;
			{
				Phoenix::s32 blendTreeIndex = bossModel->AddBlendTreeToLayer(layerNum);
				bossModel->AddBlendAnimationStateToBlendTree(0, Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
				bossModel->AddBlendAnimationStateToBlendTree(1, Phoenix::Math::Vector3(0.0f, 1.0f, 0.0f), layerNum, blendTreeIndex);
				bossModel->AddBlendAnimationStateToBlendTree(2, Phoenix::Math::Vector3(0.0f, -1.0f, 0.0f), layerNum, blendTreeIndex);
				bossModel->AddBlendAnimationStateToBlendTree(3, Phoenix::Math::Vector3(1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
				bossModel->AddBlendAnimationStateToBlendTree(4, Phoenix::Math::Vector3(-1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
			}
		}
	}
}

// ������
void EnemyManager::Initialize()
{
	//enemies.clear();
}

// �I����
void EnemyManager::Finalize()
{
	for (auto& enemy : enemies)
	{
		enemy->Finalize();
		enemy.reset();
	}
	enemies.clear();
}

// �X�V
void EnemyManager::Update(bool onControl, Phoenix::f32 elapsedTime)
{
	for (auto enemy : enemies)
	{
		enemy->Update(onControl, elapsedTime);
	}
}

// UI�X�V
void EnemyManager::UpdateUI(Phoenix::s32 index)
{
	if (index <= -1 || enemies.size() <= index)
	{
		enemiesUI->Update(index, 0.0f);
		return;
	}

	Phoenix::s32 life = enemies.at(index)->GetLife();

	Phoenix::f32 hp = static_cast<Phoenix::f32>(life);
	hp = hp <= 0 ? 0 : hp;

	enemiesUI->Update(index, (hp / enemies.at(index)->GetLifeMax()) * 100.0f);
}

// �`��
void EnemyManager::Draw()
{

}

// �G�l�~�[�ǉ�
void EnemyManager::AddEnemy(Enemy::TypeTag tag, Phoenix::FrameWork::Transform transform)
{
	Phoenix::s32 index = 0;
	for (auto enemy : enemies)
	{
		if (!enemy->GetEnable())
		{
			enemy->SetTypeTag(tag);
			enemy->Initialize();
			enemy->SetEnable(true);
			enemy->SetAlive(true);
			enemy->SetDeath(false);
			enemy->SetTransform(transform);
			enemy->SetOwner(shared_from_this());
			enemy->SetPlayer(player);

			enemiesUI->AddUI(index, enemy->GetUI());

			++aliveEnemyCount;
			++battleEnemyCount; // TODO : delete.

			break;
		}

		++index;
	}
}

// �G�l�~�[�B����
void EnemyManager::ResetEnemies()
{
	for (auto enemy : enemies)
	{
		enemy->SetEnable(false);
		enemy->SetAlive(false);
		enemy->SetDeath(false);
		enemy->SetState(BattleEnemyState::Idle);
	}
}

// �G�l�~�[�����ő吔����J�E���g�_�E��
void EnemyManager::SubAliveEnemyCount(Phoenix::s32 sub)
{
	aliveEnemyCount -= sub;
}

// �w��̃G�l�~�[�ɍU�����𔭍s
bool EnemyManager::SetAttackRight(Phoenix::s32 enemyIndex, bool stackAttackRight)
{
	return enemies.at(enemyIndex)->SetAttackRight(stackAttackRight);
}

// �w��̃G�l�~�[���o�g�����[�h�ɕύX
void EnemyManager::SetBattleEnemy(Phoenix::s32 enemyIndex)
{
	enemies.at(enemyIndex)->SetInBattle(true);
}

// �v���C���[��ݒ�
void EnemyManager::SetPlayer(std::shared_ptr<Player> player)
{
	this->player = player;
}

// GUI
void EnemyManager::GUI()
{
	if (ImGui::TreeNode("EnemyManager"))
	{
		Phoenix::s32 index = 0;

		for (auto enemy : enemies)
		{
			if (enemy->GetEnable())
			{
				enemy->GUI(index);
			}

			++index;
		}
		ImGui::TreePop();
	}
}

// �G�l�~�[�B���擾
std::vector<std::shared_ptr<Enemy>>& EnemyManager::GetEnemies()
{
	return enemies;
}

// �G�l�~�[���E�ő吔
Phoenix::s32 EnemyManager::GetEnemyRange()
{
	return EnemyRange;
}

// �G�l�~�[�����ő吔
Phoenix::s32 EnemyManager::GetAliveEnemyCount()
{
	return aliveEnemyCount;
}

// �o�g�����̃G�l�~�[�ő吔
Phoenix::s32 EnemyManager::GetBattleEnemyCount()
{
	return battleEnemyCount;
}

// �G�l�~�[UI���擾
std::shared_ptr<EnemiesUI> EnemyManager::GetEnemiesUI()
{
	return enemiesUI;
}