#pragma once

#include "Phoenix/Math/PhoenixMath.h"


class IMetaAISystem
{
public:
	IMetaAISystem() {}
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
	/// <param name="score"> : �v���C���[�s���̍��v�X�R�A </param>
	virtual void Update(Phoenix::s32 score) = 0;
};