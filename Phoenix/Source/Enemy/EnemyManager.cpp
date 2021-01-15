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
	enemies.resize(EnemyRange);
	originEnemies.resize(EnemyRange);
	enemiesPos.resize(EnemyRange);
	enemiesState.resize(EnemyRange);
	enemiesType.resize(EnemyRange);

	for (Phoenix::s32 i = 0; i < EnemyRange; ++i)
	{
		enemiesPos.at(i) = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		enemiesState.at(i) = -1;
		enemiesType.at(i) = -1;
	}

	for (auto& enemy : originEnemies)
	{
		enemy = std::make_shared<Enemy>();
		enemy->Construct(graphicsDevice);
	}

	boss = Boss::Create();
	boss->Construct(graphicsDevice);

	enemiesUI = EnemiesUI::Create();
	enemiesUI->Initialize(graphicsDevice, EnemyRange);

	aliveEnemyCount = 0;
}

// ������
void EnemyManager::Initialize()
{
	//enemies.clear();

	for (Phoenix::s32 i = 0; i < EnemyRange; ++i)
	{
		enemiesPos.at(i) = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);
		enemiesState.at(i) = -1;
	}
}

// �I����
void EnemyManager::Finalize()
{
	for (auto& enemy : enemies)
	{
		if (!enemy) continue;

		enemy->Finalize();
		enemy.reset();
	}
	enemies.clear();
}

// �X�V
void EnemyManager::Update(bool onControl, Phoenix::f32 elapsedTime)
{
	Phoenix::s32 cnt = 0;
	for (auto enemy : enemies)
	{
		if (!enemy) continue;
		enemy->Update(onControl, elapsedTime);

		enemiesPos.at(cnt) = enemy->GetPosition();
		enemiesState.at(cnt) = static_cast<Phoenix::s32>(enemy->GetBattleState());
		enemiesType.at(cnt) = static_cast<Phoenix::s32>(enemy->GetTypeTag());

		++cnt;
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
	for (Phoenix::s32 i = 0; i < enemies.size(); ++i)
	{
		if (enemies.at(i))
		{
			if (enemies.at(i)->GetEnable()) continue;
		}

		if (tag == Enemy::TypeTag::Small)
		{
			enemies.at(i) = originEnemies.at(useOriginCnt);
			transform.SetScale({ 1.0f,1.0f,1.0f });
			++useOriginCnt;
		}
		else if (tag == Enemy::TypeTag::Large)
		{
			enemies.at(i) = boss;
			transform.SetScale({ 1.4f,1.4f,1.4f });
		}

		enemies.at(i)->Initialize();
		enemies.at(i)->SetEnable(true);
		enemies.at(i)->SetAlive(true);
		enemies.at(i)->SetDeath(false);
		enemies.at(i)->SetTransform(transform);
		enemies.at(i)->SetOwner(shared_from_this());
		enemies.at(i)->SetPlayer(player);

		enemiesUI->AddUI(i, enemies.at(i)->GetUI());

		++aliveEnemyCount;
		++battleEnemyCount; // TODO : delete.

		break;
	}
}

// �G�l�~�[�B����
void EnemyManager::ResetEnemies()
{
	for (auto enemy : originEnemies)
	{
		enemy->SetEnable(false);
		enemy->SetAlive(false);
		enemy->SetDeath(false);
		enemy->SetState(BattleEnemyState::Idle);
	}
	{
		boss->SetEnable(false);
		boss->SetAlive(false);
		boss->SetDeath(false);
		boss->SetState(BattleEnemyState::Idle);
	}

	for (Phoenix::s32 i = 0; i < enemies.size(); ++i)
	{
		enemies.at(i) = nullptr;
	}

	aliveEnemyCount = 0;
	battleEnemyCount = 0;
	useOriginCnt = 0;
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

// �G�l�~�[�B�̍��W���擾
std::vector<Phoenix::Math::Vector3>& EnemyManager::GetEnemiesPos()
{
	return enemiesPos;
}

// �G�l�~�[�B�̏�Ԃ��擾
std::vector<Phoenix::s32>& EnemyManager::GetEnemiesState()
{
	return enemiesState;
}

// �G�l�~�[�B�̎�ނ��擾
std::vector<Phoenix::s32>& EnemyManager::GetEnemiesType()
{
	return enemiesType;
}