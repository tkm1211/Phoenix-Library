#include "EnemyManager.h"
#include "Enemy.h"


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
	enemies.clear();
}

// �X�V
void EnemyManager::Update()
{
	for (auto enemy : enemies)
	{
		enemy->Update();
	}
}

// �`��
void EnemyManager::Draw()
{

}

// �G�l�~�[�ǉ�
void EnemyManager::AddEnemy(Phoenix::FrameWork::Transform transform)
{
	for (auto enemy : enemies)
	{
		if (!enemy->GetEnable())
		{
			enemy->SetEnable(true);
			enemy->SetAlive(true);
			enemy->SetTransform(transform);
			enemy->SetOwner(shared_from_this());
			break;
		}
	}
}

// �G�l�~�[�����ő吔����J�E���g�_�E��
void EnemyManager::SubAliveEnemyCount(Phoenix::s32 sub)
{
	aliveEnemyCount -= sub;
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