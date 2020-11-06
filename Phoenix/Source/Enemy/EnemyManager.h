#pragma once

#include <vector>
#include <memory>
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/FrameWork/Component/Transform.h"


class Enemy;
class EnemyManager : public std::enable_shared_from_this<EnemyManager>
{
private:
	static constexpr Phoenix::s32 EnemyRange = 5;

private:
	std::vector<std::shared_ptr<Enemy>> enemies;
	Phoenix::s32 aliveEnemyCount = 0;
	Phoenix::s32 battleEnemyCount = 0;

public:
	EnemyManager() {}
	~EnemyManager() {}

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
	void Update();

	// �`��
	void Draw();

	// GUI
	void GUI();

public:
	// �G�l�~�[�ǉ�
	void AddEnemy(Phoenix::FrameWork::Transform transform);

public:
	// �G�l�~�[�����ő吔����J�E���g�_�E��
	void SubAliveEnemyCount(Phoenix::s32 sub);

public:
	// �G�l�~�[�B���擾
	std::vector<std::shared_ptr<Enemy>>& GetEnemies();

	// �G�l�~�[���E�ő吔
	Phoenix::s32 GetEnemyRange();

	// �G�l�~�[�����ő吔
	Phoenix::s32 GetAliveEnemyCount();

	// �o�g�����̃G�l�~�[�ő吔
	Phoenix::s32 GetBattleEnemyCount();
};