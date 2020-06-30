#pragma once

#include "Phoenix/Types.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// フレームタイム
		//****************************************************************************
		struct FrameTime
		{
			f64					total_time = 0.0;		// 合計時間
			f32					elapsed_time = 0.0f;	// 経過時間
		};

		//****************************************************************************
		// 更新タイプ
		//****************************************************************************
		enum class UpdateType
		{
			Editing,
			GamePlay,
			Pause
		};
	} // namespace FrameWork
} // namespace Phoenix