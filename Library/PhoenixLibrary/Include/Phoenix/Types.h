#pragma once

#include "Phoenix/Target/Target.h"


namespace Phoenix
{
#if defined(PHOENIX_TARGET_WIN)
	typedef char					s8;
	typedef unsigned char			u8;
	typedef short					s16;
	typedef unsigned short			u16;
	typedef int						s32;
	typedef unsigned int			u32;
	typedef long long				s64;
	typedef unsigned long long		u64;
	typedef float					f32;
	typedef double					f64;
	typedef uintptr_t				uintPtr;
	// ※s8のsは「signed」、8は「8ビット」を示している。
#else
	ターゲットが宣言されていません。
#endif
}