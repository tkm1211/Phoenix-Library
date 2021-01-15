#pragma once

#include "Phoenix/Math/PhoenixMath.h"


class IMetaAISystem
{
public:
	virtual ~IMetaAISystem() {}

public:
	// �R���X�g���N�^
	virtual void Construct() = 0;

	// ������
	virtual void Initialize() = 0;

	// �I����
	virtual void Finalize() = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="elapsedTime"> : �o�ߎ��� </param>
	virtual void Update(Phoenix::f32 elapsedTime) = 0;
};