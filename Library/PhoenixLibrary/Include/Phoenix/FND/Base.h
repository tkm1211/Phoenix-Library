#pragma once

#include "NonCopyable.h"


namespace Phoenix
{
	namespace FND
	{
		//****************************************************************************
		// ���ׂẴN���X�̊��ƂȂ�N���X
		//****************************************************************************
		class Base : public NonCopyable
		{
		public:
			Base() {}
			virtual ~Base() {}
		};

	}	// namespace FND
}	// namespace Phoenix