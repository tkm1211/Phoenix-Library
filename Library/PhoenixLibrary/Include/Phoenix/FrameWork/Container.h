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
		// コンテナ
		//****************************************************************************
		class Container final : public FND::NonCopyable
		{
		private:
			std::vector<std::unique_ptr<Composition>> compositions;
			uintPtr instanceHandle = 0;

		public:
			~Container() { Finalize(); }

			// コンストラクション
			void Construct(uintPtr instanceHandle);

			// 初期化
			bool Initialize();

			// 終了化
			void Finalize();

			// インスタンスハンドル取得
			uintPtr GetInstanceHandle() const { return instanceHandle; }

			// 指定の型のコンポジションを取得
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

			// 指定の型のコンポジションを全て取得
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

			// 指定の型のコンポジションを登録
			template<class T, class... Args>
			void RegisterComposition(Args... args)
			{
				compositions.emplace_back(std::make_unique<T>(args...));
			}
		};

	}	// namespace fwk
}	// namespace ys