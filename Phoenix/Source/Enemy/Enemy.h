#pragma once

#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/FrameWork/Object/Object.h"
#include "Phoenix/FrameWork/Component/Transform.h"
#include "EnemyBattleState.h"


class EnemyManager;
class Enemy
{
private:
	static constexpr Phoenix::s32 LifeRange = 50;

public:
	enum class TypeTag
	{
		Small,  // �G��
		Medium, // ���{�X
		Large   // �{�X
	};

private:
	bool enable = false;
	bool alive = false;
	bool inBattle = false;

	std::unique_ptr<Phoenix::FrameWork::ModelObject> model;
	std::unique_ptr<Phoenix::FrameWork::Transform> transform;
	std::vector<Phoenix::FrameWork::CollisionData> collisionDatas;

	std::weak_ptr<EnemyManager> owner;

	Phoenix::s32 life = 0;
	Phoenix::f32 radius = 0.0f;

public:
	Enemy() {}
	~Enemy() { Finalize(); }

public:
	// ����
	static std::shared_ptr<Enemy> Create();

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
	void GUI(Phoenix::s32 index);

public:
	// �L���t���O�ݒ�
	void SetEnable(bool enable);

	// ���݃t���O�ݒ�
	void SetAlive(bool alive);

	// �퓬���t���O�ݒ�
	void SetInBattle(bool inBattle);

	// �g�����X�t�H�[���̐ݒ�
	void SetTransform(Phoenix::FrameWork::Transform transform);

	// �G�l�~�[�}�l�[�W���[�̐ݒ�
	void SetOwner(std::shared_ptr<EnemyManager> owner);

public:
	// �L���t���O�擾
	bool GetEnable();

	// ���݃t���O�擾
	bool GetAlive();

	// �퓬���t���O�擾
	bool GetInBattle();

	// �g�����X�t�H�[���̎擾
	Phoenix::FrameWork::Transform GetTransform();


	Phoenix::FrameWork::ModelObject* GetModel() { return model.get(); }

	const std::vector<Phoenix::FrameWork::CollisionData>* GetCollisionDatas() { return &collisionDatas; }

	Phoenix::Math::Matrix GetWorldMatrix() { return transform->GetWorldTransform(); }

	Phoenix::Math::Vector3 GetPosition() { return transform->GetTranslate(); }

	Phoenix::f32 GetRadius() { return radius; }
};