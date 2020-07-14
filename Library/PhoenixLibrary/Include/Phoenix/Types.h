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
	typedef size_t					sizeT;
	// ��s8��s�́usigned�v�A8�́u8�r�b�g�v�������Ă���B
#else
	�^�[�Q�b�g���錾����Ă��܂���B
#endif
}