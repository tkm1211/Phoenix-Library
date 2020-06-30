#pragma once

#include <any>
#include "Phoenix/FrameWork/Object.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// 階層挿入イベントリスナー
		//****************************************************************************
		class IHierarchicalInsertionEventListener
		{
		public:
			// 挿入前に呼ばれる
			virtual void OnInserting(IObject* parent, IObject* child) = 0;

			// 挿入後に呼ばれる
			virtual void OnInserted(IObject* parent, IObject* child) = 0;
		};

		//****************************************************************************
		// 階層挿入操作インターフェース
		//****************************************************************************
		class IHierarchicalInsertionContext
		{
		public:
			// 挿入可能か
			virtual bool CanInsert(std::any& parent, std::any& child) = 0;

			// 挿入
			virtual void Insert(std::any& parent, std::any& child) = 0;

			// イベントリスナー登録
			virtual void RegisterEventListener(IHierarchicalInsertionEventListener* listener) = 0;

			// イベントリスナー登録解除
			virtual void UnregisterEventListener(IHierarchicalInsertionEventListener* listener) = 0;
		};

	} // namespace FrameWork
} // namespace Phoenix