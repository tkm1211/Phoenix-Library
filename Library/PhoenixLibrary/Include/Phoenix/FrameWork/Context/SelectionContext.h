#pragma once

#include <set>
#include <vector>
#include "Phoenix/Types.h"
#include "Phoenix/FrameWork/Object.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// 前方宣言
		class ISelectionEventListener;

		//****************************************************************************
		// 選択操作インターフェース
		//****************************************************************************
		class ISelectionContext
		{
		public:
			// 選択クリア
			virtual void Clear() = 0;

			// 選択設定
			virtual void Set(IObject* object) = 0;

			// 選択範囲設定
			virtual void SetRange(IObject* objects[], s32 count) = 0;

			// 選択追加
			virtual void Add(IObject* object) = 0;

			// 選択範囲追加
			virtual void AddRange(IObject* objects[], s32 count) = 0;

			// 選択解除
			virtual void Remove(IObject* object) = 0;

			// 選択範囲解除
			virtual void RemoveRange(IObject* objects[], s32 count) = 0;

			// 選択反転
			virtual void Toggle(IObject* object) = 0;

			// 選択範囲反転
			virtual void ToggleRange(IObject* objects[], s32 count) = 0;

			// 最後の選択オブジェクトを取得
			virtual IObject* LastSelected() = 0;

			// 指定オブジェクトが選択されているか確認
			virtual bool SelectionContains(IObject* object) = 0;

			// 選択数取得
			virtual s32 GetSelectionCount() = 0;

			// 選択イベントリスナー登録
			virtual void RegisterEventListener(ISelectionEventListener* listener) = 0;

			// 選択イベントリスナー登録解除
			virtual void UnregisterEventListener(ISelectionEventListener* listener) = 0;

			// 指定の型の最後の選択オブジェクトを取得
			template<class T>
			T* GetLastSelected()
			{
				return dynamic_cast<T*>(LastSelected());
			}

			// 指定の型の選択オブジェクトを全て取得
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
			// 選択オブジェクトを全て取得
			virtual std::set<IObject*>& GetSelections() = 0;
		};

		//****************************************************************************
		// 選択イベントリスナー
		//****************************************************************************
		class ISelectionEventListener
		{
		public:
			// 選択変更前に呼ばれる
			virtual void OnSelectionChanging(ISelectionContext* context) = 0;

			// 選択変更後に呼ばれる
			virtual void OnSelectionChanged(ISelectionContext* context) = 0;
		};

	} // namespace FrameWork
} // namespace Phoenix