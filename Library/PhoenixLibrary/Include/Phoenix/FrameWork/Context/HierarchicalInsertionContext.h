#pragma once

#include <any>
#include "Phoenix/FrameWork/Object.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �K�w�}���C�x���g���X�i�[
		//****************************************************************************
		class IHierarchicalInsertionEventListener
		{
		public:
			// �}���O�ɌĂ΂��
			virtual void OnInserting(IObject* parent, IObject* child) = 0;

			// �}����ɌĂ΂��
			virtual void OnInserted(IObject* parent, IObject* child) = 0;
		};

		//****************************************************************************
		// �K�w�}������C���^�[�t�F�[�X
		//****************************************************************************
		class IHierarchicalInsertionContext
		{
		public:
			// �}���\��
			virtual bool CanInsert(std::any& parent, std::any& child) = 0;

			// �}��
			virtual void Insert(std::any& parent, std::any& child) = 0;

			// �C�x���g���X�i�[�o�^
			virtual void RegisterEventListener(IHierarchicalInsertionEventListener* listener) = 0;

			// �C�x���g���X�i�[�o�^����
			virtual void UnregisterEventListener(IHierarchicalInsertionEventListener* listener) = 0;
		};

	} // namespace FrameWork
} // namespace Phoenix