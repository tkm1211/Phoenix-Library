#include "pch.h"
#include "Phoenix/Math/Color.h"
#include "Phoenix/Math/Arithmetic.h"


namespace Phoenix
{
	namespace Math
	{
		// �X�^�e�B�b�N
		const Color Color::White = { 1.0f, 1.0f, 1.0f, 1.0f };
		const Color Color::Gray = { 0.5f, 0.5f, 0.5f, 1.0f };
		const Color Color::Black = { 0.0f, 0.0f, 0.0f, 1.0f };
		const Color Color::Red = { 1.0f, 0.0f, 0.0f, 1.0f };
		const Color Color::Green = { 0.0f, 1.0f, 0.0f, 1.0f };
		const Color Color::Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
		const Color Color::Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
		const Color Color::Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
		const Color Color::Magenta = { 1.0f, 1.0f, 0.0f, 1.0f };
		const Color Color::Pink = { 1.0f, 0.5f, 0.5f, 1.0f };

		//============================================================================
		// Color
		//----------------------------------------------------------------------------
		// �A�N�Z�X����
		f32&
			Color::operator () (u32 index)
		{
			return v[index];
		}

		f32
			Color::operator () (u32 index) const
		{
			return v[index];
		}

		// ���Z�q�̃L���X�e�B���O
		Color::operator f32* ()
		{
			return (f32*)&v[0];
		}

		Color::operator const f32* () const
		{
			return (const f32*)&v[0];
		}

		// �P�����Z�q
		Color
			Color::operator + () const
		{
			return (*this);
		}

		Color
			Color::operator - () const
		{
			Color ret;
			ret.r = -r;
			ret.g = -g;
			ret.b = -b;
			ret.a = -a;
			return ret;
		}

		// �񍀉��Z�q
		Color
			Color::operator + (const Color& color) const
		{
			Color ret;
			return *ColorAdd(&ret, this, &color);
		}

		Color
			Color::operator - (const Color& color) const
		{
			Color ret;
			return *ColorSubtract(&ret, this, &color);
		}

		Color
			Color::operator * (const Color& color) const
		{
			Color ret;
			return *ColorModulate(&ret, this, &color);
		}

		Color
			Color::operator * (const f32 f) const
		{
			Color ret;
			return *ColorScale(&ret, this, f);
		}

		Color
			Color::operator / (const f32 f) const
		{
			Color ret;
			return *ColorScale(&ret, this, 1.0f / f);
		}

		// ������Z�q
		Color
			Color::operator += (const Color& color)
		{
			return *ColorAdd(this, this, &color);
		}

		Color
			Color::operator -= (const Color& color)
		{
			return *ColorSubtract(this, this, &color);
		}

		Color
			Color::operator *= (const Color& color)
		{
			return *ColorModulate(this, this, &color);
		}

		Color
			Color::operator *= (const f32 f)
		{
			return *ColorScale(this, this, f);
		}

		// ��r���Z�q
		bool
			Color::operator == (const Color& color) const
		{
			if ((FabsF32(r - color.r) <= Epsilon)
				&& (FabsF32(g - color.g) <= Epsilon)
				&& (FabsF32(b - color.b) <= Epsilon)
				&& (FabsF32(a - color.a) <= Epsilon)
				) {
				return true;
			}

			return false;
		}

		bool
			Color::operator != (const Color& color) const
		{
			return !(*this == color);
		}

		//============================================================================
		//	�J���[�Z�p�֐�
		//----------------------------------------------------------------------------
		// 2 �̐F�l�����Z���āA�V�����F�l���쐬����B
		Color*									// [R] 2 �̐F�l�̍��v�ł��� Color �\���̂ւ̃|�C���^��Ԃ��B
			ColorAdd(
				Color* out,			// [O] ���Z���ʂł��� Color �\���̂ւ̃|�C���^�B
				const Color* c1,				// [I] �����̊�ɂȂ� Color �\���̂ւ̃|�C���^�B
				const Color* c2				// [I] �����̊�ɂȂ� Color �\���̂ւ̃|�C���^�B
			)
		{
			out->r = (c1->r) + (c2->r);
			out->g = (c1->g) + (c2->g);
			out->b = (c1->b) + (c2->b);
			out->a = (c1->a) + (c2->a);

			return out;
		}

		// 2 �̐F�l�����Z���āA�V�����F�l���쐬����B
		Color*									// [R] 2 �̐F�l�̍����ł��� Color �\���̂ւ̃|�C���^��Ԃ��B
			ColorSubtract(
				Color* out,			// [O] ���Z���ʂł��� Color �\���̂ւ̃|�C���^�B
				const Color* c1,				// [I] �����̊�ɂȂ� Color �\���̂ւ̃|�C���^�B
				const Color* c2				// [I] �����̊�ɂȂ� Color �\���̂ւ̃|�C���^�B
			)
		{
			out->r = (c1->r) - (c2->r);
			out->g = (c1->g) - (c2->g);
			out->b = (c1->b) - (c2->b);
			out->a = (c1->a) - (c2->a);

			return out;
		}

		// �F�l���X�P�[�����O����B
		Color*									// [R] �X�P�[�����O���ꂽ�F�l�ł��� Color �\���̂ւ̃|�C���^��Ԃ��B
			ColorScale(
				Color* out,			// [O] ���Z���ʂł��� Color �\���̂ւ̃|�C���^�B
				const Color* c,				// [I] �����̊�ɂȂ� Color �\���̂ւ̃|�C���^�B
				f32					s				// [I] �X�P�[�����O�W���B�F�� 4D �x�N�g���̂悤�ɏ������āA������X�P�[�����O����Bs �̒l�ɐ����͂Ȃ��Bs �� 1 �̏ꍇ�A���ʂ̐F�͌��̐F�ł���B
			)
		{
			out->r = s * (c->r);
			out->g = s * (c->g);
			out->b = s * (c->b);
			out->a = s * (c->a);

			return out;
		}

		// 2 �̐F���u�����h����B
		Color*									// [R] �u�����h���ʂł��� Color �\���̂ւ̃|�C���^��Ԃ��B
			ColorModulate(
				Color* out,			// [O] ���Z���ʂł��� Color �\���̂ւ̃|�C���^�B
				const Color* c1,				// [I] �����̊�ɂȂ� Color �\���̂ւ̃|�C���^�B
				const Color* c2				// [I] �����̊�ɂȂ� Color �\���̂ւ̃|�C���^�B
			)
		{
			out->r = (c1->r) * (c2->r);
			out->g = (c1->g) * (c2->g);
			out->b = (c1->b) * (c2->b);
			out->a = (c1->a) * (c2->a);

			return out;
		}

		// �F�̃R���g���X�g�l�𒲐�����B
		Color*									// [R] �R���g���X�g�������ʂ� Color �\���̂ւ̃|�C���^��Ԃ��B
			ColorAdjustContrast(
				Color* out,			// [O] ���Z���ʂł��� Color �\���̂ւ̃|�C���^�B
				const Color* c,				// [I] �����̊�ɂȂ� Color �\���̂ւ̃|�C���^�B
				f32					s				// [I] �R���g���X�g�l�B���̃p�����[�^�́A50% �O���[�Ǝw��F c �̊ԂŐ��`��Ԃ���Bc �̒l�ɐ����͂Ȃ��B���̃p�����[�^�� 0 �̏ꍇ�A�Ԃ����F�� 50% �O���[�ł���B���̃p�����[�^�� 1 �̏ꍇ�A�Ԃ����F�͌��̐F�ł���B
			)
		{
			out->r = 0.5f + s * (c->r - 0.5f);
			out->g = 0.5f + s * (c->g - 0.5f);
			out->b = 0.5f + s * (c->b - 0.5f);
			out->a = c->a;

			return out;
		}

		// �F�̍ʓx�l�𒲐�����B
		Color*									// [R] �O�a�������ʂ� Color �\���̂ւ̃|�C���^��Ԃ��B
			ColorAdjustSaturation(
				Color* out,			// [O] ���Z���ʂł��� Color �\���̂ւ̃|�C���^�B
				const Color* c,				// [I] �����̊�ɂȂ� Color �\���̂ւ̃|�C���^�B
				f32					s				// [I] �O�a�l�B���̃p�����[�^�́A�O���[ �X�P�[���ɕϊ����ꂽ�F�ƌ��̐F c �̊ԂŐ��`��Ԃ���Bs �̒l�ɐ����͂Ȃ��Bs �� 0 �̏ꍇ�A�Ԃ����F�̓O���[�X�P�[�� �J���[�ł���Bs �� 1 �̏ꍇ�A�Ԃ����F�͌��̐F�ł���B
			)
		{
			f32 grey = c->r * 0.2125f + c->g * 0.7154f + c->b * 0.0721f;

			out->r = grey + s * (c->r - grey);
			out->g = grey + s * (c->g - grey);
			out->b = grey + s * (c->b - grey);
			out->a = c->a;

			return out;
		}

		// ���`��Ԃ��g�p���ĐF�l���쐬����B
		Color*									// [R] ���`��Ԍ��ʂ� Color �\���̂ւ̃|�C���^��Ԃ��B
			ColorLerp(
				Color* out,			// [O] ���Z���ʂł��� Color �\���̂ւ̃|�C���^�B
				const Color* c1,			// [I] �����̊�ɂȂ� Color �\���̂ւ̃|�C���^�B
				const Color* c2,			// [I] �����̊�ɂȂ� Color �\���̂ւ̃|�C���^�B
				f32					s				// [I] �F c1 �� c2 �� 4D �x�N�g���Ƃ��ď������A�����̊Ԃ���`��Ԃ���p�����[�^�Bs �̒l�ɐ����͂Ȃ��B
			)
		{
			out->r = (1.0f - s) * (c1->r) + s * (c2->r);
			out->g = (1.0f - s) * (c1->g) + s * (c2->g);
			out->b = (1.0f - s) * (c1->b) + s * (c2->b);
			out->a = (1.0f - s) * (c1->a) + s * (c2->a);

			return out;
		}

		// ����F�l�ɑ΂��镉�̐F�l���쐬����B
		Color*									// [R] �F�l�̕��̐F�ł��� Color �\���̂ւ̃|�C���^��Ԃ��B
			ColorNegative(
				Color* out,			// [O] ���Z���ʂł��� Color �\���̂ւ̃|�C���^�B
				const Color* c				// [I] �����̊�ɂȂ� Color �\���̂ւ̃|�C���^�B
			)
		{
			out->r = 1.0f - c->r;
			out->g = 1.0f - c->g;
			out->b = 1.0f - c->b;
			out->a = c->a;

			return out;
		}

		// �F��32bitARGB�l�Ƃ��Ď擾
		u32										// [R] 32bitARGB�l
			ColorARGB32(
				const Color* c				// [I] �����̊�ɂȂ� Color �\���̂ւ̃|�C���^�B
			)
		{
			// 32bit�J���[�ɕϊ�
			u8 cr = c->r >= 1.0f ? 0xff : (u8)(c->r * 255.0f + 0.5f);
			u8 cg = c->g >= 1.0f ? 0xff : (u8)(c->g * 255.0f + 0.5f);
			u8 cb = c->b >= 1.0f ? 0xff : (u8)(c->b * 255.0f + 0.5f);
			u8 ca = c->a >= 1.0f ? 0xff : (u8)(c->a * 255.0f + 0.5f);
			return ((ca << 24) | (cr << 16) | (cg << 8) | cb);
		}

		// �F��32bitARGB�l�Ƃ��Ď擾
		Color*									// [R] �ϊ���̐F�ł��� Color �\���̂ւ̃|�C���^��Ԃ��B
			ColorARGB32(
				Color* out,			// [O] ���Z���ʂł��� Color �\���̂ւ̃|�C���^�B
				u32					col32			// [I] �����̊�ɂȂ� 32bitARGB�l
			)
		{
			const f32 f = 1.0f / 255.0f;
			out->r = f * (f32)(u8)(col32 >> 16);
			out->g = f * (f32)(u8)(col32 >> 8);
			out->b = f * (f32)(u8)(col32 >> 0);
			out->a = f * (f32)(u8)(col32 >> 24);

			return out;
		}

		// �F��32bitABGR�l�Ƃ��Ď擾
		u32										// [R] 32bitABGR�l
			ColorABGR32(
				const Color* c				// [I] �����̊�ɂȂ� Color �\���̂ւ̃|�C���^�B
			)
		{
			// 32bit�J���[�ɕϊ�
			u8 cr = c->r >= 1.0f ? 0xff : (u8)(c->r * 255.0f + 0.5f);
			u8 cg = c->g >= 1.0f ? 0xff : (u8)(c->g * 255.0f + 0.5f);
			u8 cb = c->b >= 1.0f ? 0xff : (u8)(c->b * 255.0f + 0.5f);
			u8 ca = c->a >= 1.0f ? 0xff : (u8)(c->a * 255.0f + 0.5f);
			return ((ca << 24) | (cb << 16) | (cg << 8) | cr);
		}

		// �F��32bitABGR�l�Ƃ��Ď擾
		Color*									// [R] �ϊ���̐F�ł��� Color �\���̂ւ̃|�C���^��Ԃ��B
			ColorABGR32(
				Color* out,			// [O] ���Z���ʂł��� Color �\���̂ւ̃|�C���^�B
				u32					col32			// [I] �����̊�ɂȂ� 32bitABGR�l
			)
		{
			const f32 f = 1.0f / 255.0f;
			out->r = f * (f32)(u8)(col32 >> 0);
			out->g = f * (f32)(u8)(col32 >> 8);
			out->b = f * (f32)(u8)(col32 >> 16);
			out->a = f * (f32)(u8)(col32 >> 24);

			return out;
		}


	}	// namespace math
}	// namespace ys