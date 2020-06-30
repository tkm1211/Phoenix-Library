#pragma once

#include <memory>
#include <vector>
#include "Phoenix/Types.h"
#include "Phoenix/FND/NonCopyable.h"
#include "Phoenix/FrameWork/Composition.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �R���e�i
		//****************************************************************************
		class Container final : public FND::NonCopyable
		{
		private:
			std::vector<std::unique_ptr<Composition>> compositions;
			uintPtr instanceHandle = 0;

		public:
			~Container() { Finalize(); }

			// �R���X�g���N�V����
			void Construct(uintPtr instanceHandle);

			// ������
			bool Initialize();

			// �I����
			void Finalize();

			// �C���X�^���X�n���h���擾
			uintPtr GetInstanceHandle() const { return instanceHandle; }

			// �w��̌^�̃R���|�W�V�������擾
			template<class T>
			T* GetComposition()
			{
				for (auto&& composition : compositions)
				{
					T* p = dynamic_cast<T*>(composition.get());
					if (p != nullptr)
					{
						return p;
					}
				}
				return nullptr;
			}

			// �w��̌^�̃R���|�W�V������S�Ď擾
			template<class T>
			void GetCompositions(std::vector<T*>& compositions)
			{
				for (auto&& composition : this->compositions)
				{
					T* p = dynamic_cast<T*>(composition.get());
					if (p != nullptr)
					{
						compositions.emplace_back(p);
					}
				}
			}

			// �w��̌^�̃R���|�W�V������o�^
			template<class T, class... Args>
			void RegisterComposition(Args... args)
			{
				compositions.emplace_back(std::make_unique<T>(args...));
			}
		};

	}	// namespace fwk
}	// namespace ys