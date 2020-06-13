#pragma once

#include "Phoenix/Types.h"


namespace Phoenix
{
	namespace FND
	{
		// 4bit�؂�グ
		template<class T>
		inline T RoundUp4(const T& v) { return (((v)+(const T)3) & ~(const T)3); }
		// 4bit�؂�̂�
		template<class T>
		inline T RoundDown4(const T& v) { return ((v) & ~(const T)3); }
		// 8bit�؂�グ
		template<class T>
		inline T RoundUp8(const T& v) { return (((v)+(const T)7) & ~(const T)7); }
		// 8bit�؂�̂�
		template<class T>
		inline T RoundDown8(const T& v) { return ((v) & ~(const T)7); }
		// 16bit�؂�グ
		template<class T>
		inline T RoundUp16(const T& v) { return (((v)+(const T)15) & ~(const T)15); }
		// 16bit�؂�̂�
		template<class T>
		inline T RoundDown16(const T& v) { return ((v) & ~(const T)15); }
		// 32bit�؂�グ
		template<class T>
		inline T RoundUp32(const T& v) { return (((v)+(const T)31) & ~(const T)31); }
		// 32bit�؂�̂�
		template<class T>
		inline T RoundDown32(const T& v) { return ((v) & ~(const T)31); }
		// 64bit�؂�グ
		template<class T>
		inline T RoundUp64(const T& v) { return (((v)+(const T)63) & ~(const T)63); }
		// 64bit�؂�̂�
		template<class T>
		inline T RoundDown64(const T& v) { return ((v) & ~(const T)63); }
		// 128bit�؂�グ
		template<class T>
		inline T RoundUp128(const T& v) { return (((v)+(const T)127) & ~(const T)127); }
		// 128bit�؂�̂�
		template<class T>
		inline T RoundDown128(const T& v) { return ((v) & ~(const T)127); }
		// 256�؂�グ
		template<class T>
		inline T RoundUp256(const T& v) { return (((v)+(const T)255) & ~(const T)255); }
		// 256bit�؂�̂�
		template<class T>
		inline T RoundDown256(const T& v) { return ((v) & ~(const T)255); }
		// 512�؂�グ
		template<class T>
		inline T RoundUp512(const T& v) { return (((v)+(const T)511) & ~(const T)511); }
		// 512bit�؂�̂�
		template<class T>
		inline T RoundDown512(const T& v) { return ((v) & ~(const T)511); }
		// 1024�؂�グ
		template<class T>
		inline T RoundUp1024(const T& v) { return (((v)+(const T)1023) & ~(const T)1023); }
		// 1024bit�؂�̂�
		template<class T>
		inline T RoundDown1024(const T& v) { return ((v) & ~(const T)1023); }
		// 2048�؂�グ
		template<class T>
		inline T RoundUp2048(const T& v) { return (((v)+(const T)2047) & ~(const T)2047); }
		// 2048bit�؂�̂�
		template<class T>
		inline T RoundDown2048(const T& v) { return ((v) & ~(const T)2047); }
		// 4096�؂�グ
		template<class T>
		inline T RoundUp4096(const T& v) { return (((v)+(const T)4095) & ~(const T)4095); }
		// 4096bit�؂�̂�
		template<class T>
		inline T RoundDown4096(const T& v) { return ((v) & ~(const T)4095); }
		// 8192�؂�グ
		template<class T>
		inline T RoundUp8192(const T& v) { return (((v)+(const T)8191) & ~(const T)8191); }
		// 8192�؂�̂�
		template<class T>
		inline T RoundDown8192(const T& v) { return ((v) & ~(const T)8191); }

		// �ᑬ n �؂�グ
		template<class T>
		inline T RoundUp(const T& n, const T& v)
		{
			T	v = n * (((v)+n) % n);
			return v;
		}

		// �ᑬ n �؂�̂�
		template<class T>
		inline T RoundDown(const T& n, const T& v)
		{
			T	v = n * ((v) % n);
			return v;
		}

		// ��I��
		template<class T>
		inline T Max(const T& a, const T& b)
		{
			return ((a) > (b)) ? (a) : (b);
		}

		// ���I��
		template<class T>
		inline T Min(const T& a, const T& b)
		{
			return ((a) < (b)) ? (a) : (b);
		}

		// �N�����v
		template<class T>
		inline T Clamp(const T& x, const T& min, const T& max)
		{
			return Min(max, Max(min, x));
		}

		// �z��v�f���擾
		template<class T, s32 n>
		inline s32 ArraySize(T(&)[n])
		{
			return n;
		}

		// ���S���
		template<class T>
		inline void SafeRelease(T*& p)
		{
			if (p != nullptr)
			{
				p->Release();
				p = nullptr;
			}
		}

		// ���S�I����
		template<class T>
		inline void SafeFinalize(T& p)
		{
			if (p != nullptr)
			{
				p->Finalize();
				p = nullptr;
			}
		}

		// ���S���
		template<class T>
		inline void SafeDelete(T*& p)
		{
			if (p != nullptr)
			{
				delete p;
				p = nullptr;
			}
		}

		// ���S���
		template<class T>
		inline void SafeDeleteArray(T*& p)
		{
			if (p != nullptr)
			{
				delete[] p;
				p = nullptr;
			}
		}

	} // namespace FND
} // namespace Phoenix