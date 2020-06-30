#pragma once

#include <set>
#include <vector>
#include "Phoenix/Types.h"
#include "Phoenix/FrameWork/Object.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// �O���錾
		class ISelectionEventListener;

		//****************************************************************************
		// �I�𑀍�C���^�[�t�F�[�X
		//****************************************************************************
		class ISelectionContext
		{
		public:
			// �I���N���A
			virtual void Clear() = 0;

			// �I��ݒ�
			virtual void Set(IObject* object) = 0;

			// �I��͈͐ݒ�
			virtual void SetRange(IObject* objects[], s32 count) = 0;

			// �I��ǉ�
			virtual void Add(IObject* object) = 0;

			// �I��͈͒ǉ�
			virtual void AddRange(IObject* objects[], s32 count) = 0;

			// �I������
			virtual void Remove(IObject* object) = 0;

			// �I��͈͉���
			virtual void RemoveRange(IObject* objects[], s32 count) = 0;

			// �I�𔽓]
			virtual void Toggle(IObject* object) = 0;

			// �I��͈͔��]
			virtual void ToggleRange(IObject* objects[], s32 count) = 0;

			// �Ō�̑I���I�u�W�F�N�g���擾
			virtual IObject* LastSelected() = 0;

			// �w��I�u�W�F�N�g���I������Ă��邩�m�F
			virtual bool SelectionContains(IObject* object) = 0;

			// �I�𐔎擾
			virtual s32 GetSelectionCount() = 0;

			// �I���C�x���g���X�i�[�o�^
			virtual void RegisterEventListener(ISelectionEventListener* listener) = 0;

			// �I���C�x���g���X�i�[�o�^����
			virtual void UnregisterEventListener(ISelectionEventListener* listener) = 0;

			// �w��̌^�̍Ō�̑I���I�u�W�F�N�g���擾
			template<class T>
			T* GetLastSelected()
			{
				return dynamic_cast<T*>(LastSelected());
			}

			// �w��̌^�̑I���I�u�W�F�N�g��S�Ď擾
			template<class T>
			void GetSelection(std::vector<T*>& items)
			{
				const std::set<IObject*>& selections = GetSelections();
				for (auto it = selections.begin(); it != selections.end(); ++it)
				{
					T* item = dynamic_cast<T*>(*it);
					if (item != nullptr)
					{
						items.emplace_back(item);
					}
				}
			}

		protected:
			// �I���I�u�W�F�N�g��S�Ď擾
			virtual std::set<IObject*>& GetSelections() = 0;
		};

		//****************************************************************************
		// �I���C�x���g���X�i�[
		//****************************************************************************
		class ISelectionEventListener
		{
		public:
			// �I��ύX�O�ɌĂ΂��
			virtual void OnSelectionChanging(ISelectionContext* context) = 0;

			// �I��ύX��ɌĂ΂��
			virtual void OnSelectionChanged(ISelectionContext* context) = 0;
		};

	} // namespace FrameWork
} // namespace Phoenix