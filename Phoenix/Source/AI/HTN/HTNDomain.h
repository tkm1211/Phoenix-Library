#pragma once

#include <map>
#include <memory>
#include <vector>
#include "Phoenix/Math/PhoenixMath.h"


template<class Name, class State, class... Args>
class HTNDomain
{
public:
	struct Task
	{
		// �������֐�
		std::function<void(Args...)> enter;

		// ���s�֐�
		std::function<bool(Args...)> update;

		// �I�����֐�
		std::function<void(Args...)> exit;
	};

private:
	// �L�[���̓v���~�e�B�u�^�X�N���A�l�̓v���~�e�B�u�^�X�N
	std::unordered_map<Name, std::function<bool(State*)>> primitive;

	// �L�[���͕����^�X�N���A�l�͕����^�X�N
	std::unordered_multimap<Name, std::function<bool(State*, std::vector<Name>*)>> compound;

	// �^�X�N�̊֐��Ǘ�
	std::map<Name, Task> tasks;

public:
	HTNDomain() {}
	~HTNDomain() {}

public:
	// ����
	static std::shared_ptr<HTNDomain> Create()
	{
		return std::make_shared<HTNDomain>();
	}

	// �v���~�e�B�u�^�X�N��ǉ�
	void AddPrimitiveTask(Name name, std::function<bool(State*)> func)
	{
		primitive.insert(std::make_pair(name, func));
	}

	// �����^�X�N��ǉ�
	void AddCompoundTask(Name name, std::function<bool(State*, std::vector<Name>*)> func)
	{
		compound.insert(std::make_pair(name, func));
	}

	/// <summary>
	/// �^�X�N�̏������֐��Ǝ��s�֐��ƏI�����֐���ǉ�
	/// </summary>
	/// <param name="name"> �^�X�N�� </param>
	/// <param name="enter"> �^�X�N�̏������֐�(Arg... : �ϒ�����) </param>
	/// <param name="update"> �^�X�N�̎��s�֐�(bool : �^�X�N�I���ʒm�AArg... : �ϒ�����) </param>
	/// <param name="exit"> �^�X�N�̏I�����֐�(Arg... : �ϒ�����) </param>
	void AddTask(Name name, std::function<void(Args...)> enter, std::function<bool(Args...)> update, std::function<void(Args...)> exit)
	{
		tasks.insert(std::make_pair(name, Task()));
		tasks.at(name).enter = enter;
		tasks.at(name).update = update;
		tasks.at(name).exit = exit;
	}

	// �v���~�e�B�u�^�X�N�̊֐������s
	bool RunPrimitiveFunction(Name taskName, State* state)
	{
		return primitive.at(taskName)(state);
	}

	// �����^�X�N���擾
	std::unordered_multimap<Name, std::function<bool(State*, std::vector<Name>*)>> GetCompound()
	{
		return compound;
	}

	// �^�X�N�̊֐��Ǘ����擾
	std::map<Name, Task> GetTasks()
	{
		return tasks;
	}
	
	// �v���~�e�B�u�^�X�N�̐����擾
	Phoenix::sizeT GetPrimitiveSize(Name taskName)
	{
		return primitive.count(taskName);
	}

	// �����^�X�N�̐����擾
	Phoenix::sizeT GetCompoundSize(Name taskName)
	{
		return compound.count(taskName);
	}
};