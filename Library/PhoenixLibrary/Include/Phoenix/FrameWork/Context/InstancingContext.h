#pragma once

#include <any>


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// インスタンシング操作インターフェース
		//****************************************************************************
		class IInstancingContext
		{
		public:
			// コピー可能か
			virtual bool CanCopy() = 0;

			// コピー
			virtual std::any Copy() = 0;

			// 挿入可能か
			virtual bool CanInsert(std::any& object) = 0;

			// 挿入
			virtual void Insert(std::any& object) = 0;

			// 削除可能か
			virtual bool CanDelete() = 0;

			// 削除
			virtual void Delete() = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix