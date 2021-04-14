#pragma once

#include <map>
#include <memory>
#include "Phoenix/Math/PhoenixMath.h"
#include "HTNPlanner.h"
#include "HTNDomain.h"


template<class Name, class State, class... Args>
class HTN
{
private:
	// �v�����i�[
	std::shared_ptr<HTNPlanner<Name, State, Args...>> planner;

	// �h���C��
	std::shared_ptr<HTNDomain<Name, State, Args...>> domain;

	// �v�������s��
	bool inProgress = false;

	// �v�����ԍ�
	Phoenix::s32 index = 0;

public:
	HTN() {}
	~HTN() {}

public:
	// ����
	static std::shared_ptr<HTN> Create()
	{
		return std::make_shared<HTN>();
	}

	// �R���X�g���N�^
	void Construct()
	{
		domain = HTNDomain<Name, State, Args...>::Create();

		planner = HTNPlanner<Name, State, Args...>::Create();
		planner->Construct(domain);
	}

	// ������
	void Initialize()
	{
		planner->Initialize();

		inProgress = false;
		index = 0;
	}

	// �X�V
	void Update(Args... args)
	{
		std::vector<Name> plan = planner->GetPlan();
		auto tasks = domain->GetTasks();

		// �v�����̃f�[�^������A�v�������s���̂Ƃ�
		if (!plan.empty() && inProgress)
		{
			// act���v�����Ō�̏ꍇ
			if (index == plan.size())
			{
				planner->ClearPlan(); // �v�������Ō�܂Ŏ��s���ꂽ��v�������e��������
			}
			else
			{
				// �^�X�N�̎��s�֐������݂��m�F
				auto it = tasks.find(plan.at(index));
				if (it != tasks.end())
				{
					// �Z�b�g�A�b�v�����s
					it->second.enter(args...);

					// �^�X�N�����s
					if (it->second.update(args...))
					{
						// �^�X�N���I������Ǝ��̃^�X�N��
						it->second.exit(args...);
						++index;
					}
				}
			}
		}
	}

	/// <summary>
	/// �v�����j���O
	/// </summary>
	/// <param name="worldState"> �Q�[����� </param>
	/// <param name="taskName"> ���s�������^�X�N�� </param>
	/// <returns> �v�����j���O�����������H </returns>
	bool Planning(State worldState, Name taskName)
	{
		return planner->Planning(worldState, taskName);
	}

	// �v���~�e�B�u�^�X�N��ǉ�
	void AddPrimitiveTask(Name name, std::function<bool(State*)> func)
	{
		domain->AddPrimitiveTask(name, func);
	}

	// �����^�X�N��ǉ�
	void AddCompoundTask(Name name, std::function<bool(State*, std::vector<Name>*)> func)
	{
		domain->AddCompoundTask(name, func);
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
		domain->AddTask(name, enter, update, exit);
	}

	// �v�������s����
	bool InProgress()
	{
		return inProgress;
	}

	// �v�����擾
	std::vector<Name> GetPlan()
	{
		return planner->GetPlan();
	}

	// �v����������
	void ClearPlan()
	{
		planner->ClearPlan();
	}
};