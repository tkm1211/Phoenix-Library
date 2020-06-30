#pragma once

#include "Phoenix/Types.h"
#include <cereal/cereal.hpp>


namespace Phoenix
{
	namespace Math
	{
		//============================================================================
		// �J���[
		//----------------------------------------------------------------------------
		class Color
		{
		public:
			union {
				struct {
					f32			r;					// ��
					f32			g;					// ��
					f32			b;					// ��
					f32			a;					// �A���t�@
				};
				f32				v[4];
			};

			static const Color	White;				// ��
			static const Color	Gray;				// �D
			static const Color	Black;				// ��
			static const Color	Red;				// ��
			static const Color	Green;				// ��
			static const Color	Blue;				// ��
			static const Color	Yellow;				// ��
			static const Color	Cyan;				// ��
			static const Color	Magenta;			// �Ԏ�
			static const Color	Pink;				// ��

		public:
			Color() : Color(White) {}
			Color(f32 cr, f32 cg, f32 cb, f32 ca) { r = cr; g = cg; b = cb; a = ca; }
			Color(const Color& c) { r = c.r; g = c.g; b = c.b; a = c.a; }
			Color(const f32* f) { r = f[0]; g = f[1]; b = f[2]; a = f[3]; }

		public:
			// �A�N�Z�X����
			f32& operator () (u32 index);
			f32  operator () (u32 index) const;

			// ���Z�q�̃L���X�e�B���O
			operator f32* ();
			operator const f32* () const;

			// �P�����Z�q
			Color operator + () const;
			Color operator - () const;

			// �񍀉��Z�q
			Color operator + (const Color&) const;
			Color operator - (const Color&) const;
			Color operator * (const Color&) const;
			Color operator * (const f32) const;
			Color operator / (const f32) const;

			// ������Z�q
			Color operator += (const Color&);
			Color operator -= (const Color&);
			Color operator *= (const Color&);
			Color operator *= (const f32);

			// ��r���Z�q
			bool operator == (const Color&) const;
			bool operator != (const Color&) const;

		};


		//============================================================================
		// �J���[�Z�p�֐�
		//----------------------------------------------------------------------------
		// 2�̐F�l�����Z���āA�V�����F�l���쐬����B
		Color* ColorAdd(Color* out, const Color* c1, const Color* c2);

		// 2�̐F�l�����Z���āA�V�����F�l���쐬����B
		Color* ColorSubtract(Color* out, const Color* c1, const Color* c2);

		// �F�l���X�P�[�����O����B
		Color* ColorScale(Color* out, const Color* c, f32 s);

		// 2�̐F���u�����h����B
		Color* ColorModulate(Color* out, const Color* c1, const Color* c2);

		// �F�̃R���g���X�g�l�𒲐�����B
		Color* ColorAdjustContrast(Color* out, const Color* c, f32 s);

		// �F�̍ʓx�l�𒲐�����B
		Color* ColorAdjustSaturation(Color* out, const Color* c, f32 s);

		// ���`��Ԃ��g�p���ĐF�l���쐬����B
		Color* ColorLerp(Color* out, const Color* c1, const Color* c2, f32 s);

		// ����F�l�ɑ΂��镉�̐F�l���쐬����B
		Color* ColorNegative(Color* out, const Color* c);

		// �F��32bitARGB�l�Ƃ��Ď擾���܂��B
		u32 ColorARGB32(const Color* c);

		// �F��32bitARGB�l����擾���܂��B
		Color* ColorARGB32(Color* out, u32 argb32);

		// �F��32bitABGR�l�Ƃ��Ď擾���܂��B
		u32 ColorABGR32(const Color* c);

		// �F��32bitABGR�l����擾���܂��B
		Color* ColorABGR32(Color* out, u32 abgr32);


		template<class Archive>
		void serialize(Archive& archive, Color& c)
		{
			archive(cereal::make_nvp("r", c.r), cereal::make_nvp("g", c.g), cereal::make_nvp("b", c.b), cereal::make_nvp("a", c.a));
		}

	}	// namespace math
}	// namespace ys