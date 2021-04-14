#pragma once

#include <map>
#include <memory>
#include "HTNDomain.h"
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/FND/STD.h"


template<class Name, class State, class... Args>
class HTNPlanner
{
private:
	// �h���C��
	std::shared_ptr<HTNDomain<Name, State, Args...>> domain;

	// �v�����j���O���ʊi�[
	std::vector<Name> plan;

public:
	HTNPlanner() {}
	~HTNPlanner() {}

public:
	// ����
	static std::shared_ptr<HTNPlanner> Create()
	{
		return std::make_shared<HTNPlanner>();
	}

	// �R���X�g���N�^
	void Construct(std::shared_ptr<HTNDomain<Name, State, Args...>> domain)
	{
		this->domain = domain;
	}

	// ������
	void Initialize()
	{
		ClearPlan();
	}

	/// <summary>
	/// �v�����j���O
	/// </summary>
	/// <param name="worldState"> �Q�[����� </param>
	/// <param name="taskName"> ���s�������^�X�N�� </param>
	/// <returns> �v�����j���O�����������H </returns>
	bool Planning(State worldState, Name taskName)
	{
		std::vector<Name> taskNames;
		taskNames.emplace_back(taskName);

		return Internal(worldState, taskNames, true);
	}

	// �v�������擾
	std::vector<Name> GetPlan()
	{
		return plan;
	}

	// �v����������
	void ClearPlan()
	{
		plan.clear();
	}

private:
	// �v�����j���O����
	bool Internal(State worldState, std::vector<Name> taskNames, bool compound)
	{
		// taskNames����ł���ΏI��
		if (taskNames.begin() == taskNames.end())
		{
			return true;
		}

		// taskNames�̍ŏ��̃^�X�N���R�s�[���AtaskNames�̍ŏ��̃f�[�^���폜
		Name taskName = taskNames.at(0);
		taskNames.erase(taskNames.begin());

		// taskName�Ɠ���L�[������Ύ��s
		if (domain->GetPrimitiveSize(taskName) == 1 && !compound)
		{
			// �V�����X�e�[�g�Ɍ��݂̃X�e�[�g���R�s�[����i�ċA�����Ȃ̂ŃX�e�[�g��ێ�����K�v������j
			State newState;
			Phoenix::FND::MemCpy(&newState, &worldState, sizeof(State));

			// taskName�Ɠ������O�̃v���~�e�B�u�֐������s
			bool result = domain->RunPrimitiveFunction(taskName, &newState);
			if (result)
			{
				// �v�����j���O�ɒǉ�
				plan.emplace_back(taskName);

				// ���̃v����������
				return Internal(newState, taskNames, false);
			}
			else
			{
				return false; //�v���~�e�B�u�����s�o���Ȃ������B
			}
		}
		// taskName�Ɠ������O�̃v���~�e�B�u�^�X�N��������Ε����^�X�N�Ɠ������O�̃L�[��1�ȏ゠�邩����
		else if (domain->GetCompoundSize(taskName) > 0 && compound)
		{
			// taskName�Ɠ����L�[�����f�[�^�ƏI�������C�e���[�^���擾
			for (auto it : domain->GetCompound())
			{
				// �^�X�N�l�[���Ɠ����^�X�N�̂Ƃ��������������s����
				if (it.first == taskName)
				{
					// �^�X�N�i�[�p�ϐ��錾
					std::vector<Name> newTask;

					// �֐����s���ĐV�����K�v�ɂȂ�^�X�N���i�[����
					bool result = it.second(&worldState, &newTask); //�X�e�[�g������newTask�ɉ��ʃv�������i�[

					//it.second�͊֐��|�C���^��&state�i�X�e�[�g�j, &newTask�i�^�X�N�̖��O������j�������Ɏ���
					if (result)
					{
						// newTask��tasks�̎c��𖖔��ɒǉ���������֐�
						ArrayMerge(&newTask, taskNames);

						// newTask�̓��e���炳��ɉ��ʊK�w�̃v�����j���O���s��
						result = Internal(worldState, newTask, false);
						if (result != false)
						{
							return result;
						}
					}
				}
			}
		}

		return false;
	}

	// �z����}�[�W������
	void ArrayMerge(std::vector<Name>* tasks, std::vector<Name> task)
	{
		for (auto it : task)
		{
			tasks->emplace_back(it);
		}
	}
};