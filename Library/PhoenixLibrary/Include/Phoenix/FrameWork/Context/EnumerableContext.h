#pragma once

#include <vector>
#include "Phoenix/FrameWork/object.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �A�C�e���񋓑���C���^�[�t�F�[�X
		//****************************************************************************
		class IEnumerableContext
		{
		public:
			// �A�C�e����
			virtual const std::vector<IObject*>& GetItems() = 0;
		};

	} // namespace FrameWork
} // namespace Phoenix