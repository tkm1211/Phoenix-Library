#pragma once

#include "Phoenix/Types.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �t���[���^�C��
		//****************************************************************************
		struct FrameTime
		{
			f64					total_time = 0.0;		// ���v����
			f32					elapsed_time = 0.0f;	// �o�ߎ���
		};

		//****************************************************************************
		// �X�V�^�C�v
		//****************************************************************************
		enum class UpdateType
		{
			Editing,
			GamePlay,
			Pause
		};
	} // namespace FrameWork
} // namespace Phoenix