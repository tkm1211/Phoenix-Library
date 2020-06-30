#pragma once

#include <vector>
#include "Phoenix/FrameWork/object.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// アイテム列挙操作インターフェース
		//****************************************************************************
		class IEnumerableContext
		{
		public:
			// アイテム列挙
			virtual const std::vector<IObject*>& GetItems() = 0;
		};

	} // namespace FrameWork
} // namespace Phoenix