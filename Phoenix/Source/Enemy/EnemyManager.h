#pragma once

#include <vector>
#include <memory>
#include "Enemy.h"
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/FrameWork/Component/Transform.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "../UI/EnemiesUI.h"
#include "../Boss/Boss.h"


//class Enemy;
class Player;
class EnemyManager : public std::enable_shared_from_this<EnemyManager>
{
public:
	static constexpr Phoenix::s32 EnemyRange = 5;

private:
	std::vector<std::shared_ptr<Enemy>> enemies;
	std::vector<std::shared_ptr<Enemy>> originEnemies;
	std::shared_ptr<Boss> boss;

	Phoenix::s32 aliveEnemyCount = 0;
	Phoenix::s32 battleEnemyCount = 0;

	std::shared_ptr<Player> player;
	std::shared_ptr<EnemiesUI> enemiesUI;

	Phoenix::s32 useOriginCnt = 0;

public:
	EnemyManager() {}
	~EnemyManager()
	{
		Finalize(); 
	}

public:
	// ����
	static std::shared_ptr<EnemyManager> Create();

	// �R���X�g���N�^
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice);

	// ������
	void Initialize();

	// �I����
	void Finalize();

	// �X�V
	void Update(bool onControl, Phoenix::f32 elapsedTime);

	// UI�X�V
	void UpdateUI(Phoenix::s32 index);

	// �`��
	void Draw();

	// GUI
	void GUI();

public:
	// �G�l�~�[�ǉ�
	void AddEnemy(Enemy::TypeTag tag, Phoenix::FrameWork::Transform transform);

	// �G�l�~�[�B����
	void ResetEnemies();

	// �G�l�~�[�����ő吔����J�E���g�_�E��
	void SubAliveEnemyCount(Phoenix::s32 sub);

	// �w��̃G�l�~�[�ɍU�����𔭍s
	bool SetAttackRight(Phoenix::s32 enemyIndex, bool stackAttackRight);

	// �w��̃G�l�~�[���o�g�����[�h�ɕύX
	void SetBattleEnemy(Phoenix::s32 enemyIndex);

	// �v���C���[��ݒ�
	void SetPlayer(std::shared_ptr<Player> player);

public:
	// �G�l�~�[�B���擾
	std::vector<std::shared_ptr<Enemy>>& GetEnemies();

	// �G�l�~�[���E�ő吔
	Phoenix::s32 GetEnemyRange();

	// �G�l�~�[�����ő吔
	Phoenix::s32 GetAliveEnemyCount();

	// �o�g�����̃G�l�~�[�ő吔
	Phoenix::s32 GetBattleEnemyCount();

	// �G�l�~�[UI���擾
	std::shared_ptr<EnemiesUI> GetEnemiesUI();
};