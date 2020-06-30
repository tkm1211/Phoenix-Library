#pragma once

#include "Phoenix/FrameWork/Context/HierarchicalInsertionContext.h"
#include "Phoenix/FrameWork/Context/InstancingContext.h"
#include "Phoenix/FrameWork/Context/SelectionContext.h"
#include "Phoenix/FrameWork/Context/ViewingContext.h"
#include "Phoenix/FrameWork/Context/EnumerableContext.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �Q�[���R���e�L�X�g����C���^�[�t�F�[�X
		//****************************************************************************
		class IGameContext :
			public IHierarchicalInsertionContext,
			public IInstancingContext,
			public ISelectionContext,
			public IViewingContext,
			public IEnumerableContext
		{

		};
	} // namespace FrameWork
} // namespace Phoenix