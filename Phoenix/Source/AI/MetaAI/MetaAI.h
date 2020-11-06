#pragma once

#include <map>
#include <memory>
#include "Phoenix/Math/PhoenixMath.h"
#include "MetaAISystem.h"


enum class MetaType;
class MetaAI
{
private:
	std::map<Phoenix::s32, std::shared_ptr<IMetaAISystem>> systems;

	bool canUpdate = false;
	Phoenix::s32 type = 0;
	Phoenix::s32 score = 0;
	Phoenix::s32 previousScore = 0;

public:
	MetaAI() {}
	~MetaAI() {}

public:
	// ����
	static std::unique_ptr<MetaAI> Create();

	// �R���X�g���N�^
	void Construct();

	// ������
	void Initialize();

	// �I����
	void Finalize();

	// �X�V
	void Update();

	/// <summary>
	/// �Q�[�����烁�^AI���N������֐�
	/// </summary>
	/// <param name="type"> : �������������^AI�̃^�C�v </param>
	/// <param name="score"> : �v���C���[�s���̍��v�X�R�A </param>
	void Sensor(Phoenix::s32 type, Phoenix::s32 score);

public:
	/// <summary>
	/// ���^AI�p�̃V�X�e���ǉ�
	/// </summary>
	/// <typeparam name="T"> : �V�X�e���̌^ </typeparam>
	/// <param name="type"> : �V�X�e���̎�� </param>
	template <class T>
	void AddSystem(Phoenix::s32 type)
	{
		systems.insert(std::make_pair(type, std::make_shared<T>()));
	}

	/// <summary>
	/// ���^AI�p�̃V�X�e���ǉ�
	/// </summary>
	/// <typeparam name="T">  : �V�X�e���̌^ </typeparam>
	/// <param name="type"> : �V�X�e���̎�� </param>
	/// <param name="system"> : �V�X�e���̎��� </param>
	template <class T>
	void AddSystem(Phoenix::s32 type, std::shared_ptr<T> system)
	{
		systems.insert(std::make_pair(type, system));
	}
};