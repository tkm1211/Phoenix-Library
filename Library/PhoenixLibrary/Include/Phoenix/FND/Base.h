#pragma once

#include "NonCopyable.h"


namespace Phoenix
{
	namespace FND
	{
		//****************************************************************************
		// すべてのクラスの基底となるクラス
		//****************************************************************************
		class Base : public NonCopyable
		{
		public:
			Base() {}
			virtual ~Base() {}
		};

	}	// namespace FND
}	// namespace Phoenix