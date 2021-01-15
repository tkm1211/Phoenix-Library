#pragma once

#include <map>
#include <memory>
#include "Phoenix/Math/PhoenixMath.h"
#include "MetaAISystem.h"


enum class MetaType;
class MetaData;
class MetaAI
{
private:
	std::map<Phoenix::s32, std::shared_ptr<IMetaAISystem>> systems;

	bool canUpdate = false;
	Phoenix::s32 type = 0;
	Phoenix::s32 score = 0;
	Phoenix::s32 previousScore = 0;

	Phoenix::s32 notUpdatedTime = 0;

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
	void Update(Phoenix::f32 elapsedTime);

	/// <summary>
	/// �Q�[�����烁�^AI���N������֐�
	/// </summary>
	/// <param name="type"> : �������������^AI�̃^�C�v </param>
	/// <param name="score"> : �v���C���[�s���̍��v�X�R�A </param>
	void Sensor(Phoenix::s32 type, Phoenix::s32 score);

	/// <summary>
	/// �Q�[�����烁�^AI���N������֐�
	/// </summary>
	/// <typeparam name="T"> : AI�̌^ </typeparam>
	/// <typeparam name="U"> : ���^�f�[�^�̌^ </typeparam>
	/// <param name="type"> : �������������^AI�̃^�C�v </param>
	/// <param name="metaData"> : �Q�[�����烁�^AI�ŕK�v�ȃf�[�^ </param>
	template<class T, class U>
	void Sensor(Phoenix::s32 type, U metaData)
	{
		std::shared_ptr<T> ai = std::dynamic_pointer_cast<T>(systems[type]);
		if (ai)
		{
			this->type = type;
			ai->Sensor(metaData);
		}
	}

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