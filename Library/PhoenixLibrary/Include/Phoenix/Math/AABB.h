#pragma once

#include "Phoenix/Math/Matrix.h"
#include "Phoenix/Math/Vector.h"


namespace Phoenix
{
	namespace Math
	{
		//****************************************************************************
		// �o�E���f�B���O�{�b�N�X
		//****************************************************************************
		class AABB
		{
		public:
			Vector3 min; // �ŏ��l
			Vector3 max; // �ő�l

			static const AABB Zero; // �[��

		public:
			AABB() {}
			AABB(const Vector3& min, const Vector3& max) { this->min = min; this->max = max; }
			~AABB() {}

		public:
			// AABB�͈̔͂����Z�b�g���܂�
			void Reset();

			// �^����ꂽ�s�������AABB�͈̔͂�ݒ肵�܂�
			void Transform(const Matrix& transform);

			// �^����ꂽAABB���܂ނ悤�ɔ͈͂��g�����܂�
			void Extend(const AABB& aabb);

			// �^����ꂽ�_���܂ނ悤�ɔ͈͂��g�����܂�
			void Extend(const Vector3& point);

			// �^����ꂽ�_���͈͓��Ɋ܂܂�Ă��邩���肵�܂�
			bool Contain(const Vector3& point);

			// AABB�̃R�[�i�[�ʒu���擾���܂�
			void GetCorners(Vector3 corners[8]);

			// AABB�̒��S�ʒu���擾���܂�
			Vector3 GetCenter() const { return (min + max) / 2.0f; }

			// ��
			bool IsEmpty() const { return min == max; }

			// ���Ƃ̌�������
			bool Intersect(const Math::Vector3& center, f32 radius) const;

			// AABB�Ƃ̌�������
			bool Intersect(const Math::AABB& aabb) const;
		};

	} // namespace Math
} // namespace Phoenix