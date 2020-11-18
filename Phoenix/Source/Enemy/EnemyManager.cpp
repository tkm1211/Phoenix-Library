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

	for (auto& enemy : enemies)
	{
		enemy = std::make_shared<Enemy>();
		enemy->Construct(graphicsDevice);
	}

	enemiesUI = EnemiesUI::Create();
	enemiesUI->Initialize(graphicsDevice, EnemyRange);

	aliveEnemyCount = 0;
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
void EnemyManager::Update(bool onControl)
{
	for (auto enemy : enemies)
	{
		enemy->Update(onControl);
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

	enemiesUI->Update(index, (hp / enemies.at(index)->LifeRange) * 100.0f);
}

// �`��
void EnemyManager::Draw()
{

}

// �G�l�~�[�ǉ�
void EnemyManager::AddEnemy(Phoenix::FrameWork::Transform transform)
{
	Phoenix::s32 index = 0;
	for (auto enemy : enemies)
	{
		if (!enemy->GetEnable())
		{
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