#pragma once

#include "Phoenix/Types.h"
#include "Phoenix/FND/NonCopyable.h"


namespace Phoenix
{
	namespace FND
	{
		//****************************************************************************
		// �t���O����
		//****************************************************************************
		template <class T>
		class Flag
		{
		private:
			T flag = 0;

		public:
			// ���Z�q�̃L���X�e�B���O
			operator T () const { return flag; }

			// ���
			void operator = (T f) { flag = f; }
			void operator = (const Flag<T>& f) { flag = f.flag; }

			// ������Z�q
			Flag<T>& operator |= (T f) { flag |= f; return *this; }
			Flag<T>& operator &= (T f) { flag &= f; return *this; }
			Flag<T>& operator |= (const Flag<T>& f) { flag |= f.flag; return *this; }
			Flag<T>& operator &= (const Flag<T>& f) { flag &= f.flag; return *this; }

		public:
			// �t���O�𗧂Ă�
			void Set(T f) { flag |= f; }

			// �t���O�𗎂Ƃ�
			void Reset(T f) { flag &= ~f; }

			// �t���O�������Ă��邩�e�X�g����
			bool Test(T f) const { return ((flag & f) != 0); }

			// �t���O�𔽓]����
			void Toggle(T f) { flag ^= f; }

			// �t���O���N���A����
			void Clear() { flag = (T)0; }

			// �t���O��ݒ肷��
			void SetAll(T f) { flag = flag; }

			// �t���O���擾����
			T GetAll() const { return flag; }

			// �}�X�N�����t���O���擾����
			T GetMask(T mask) const { (T)(flag & mask); }

			// �t���O�T�C�Y���擾����
			T GetSize() const { return (T)sizeof(T) * 8; }
		};


		// ��`
		typedef Flag<u8>		Flag8;
		typedef Flag<u16>		Flag16;
		typedef Flag<u32>		Flag32;
		typedef Flag<u64>		Flag64;

	}	// namespace fnd
}	// namespace ys

// �h���N���X�p�R���X�g���N�^(64bit)
#define		FLAG64_CONSTRUCT( name )	\
public:\
	name(){}\
	name(Phoenix::u64 flag){ SetAll( flag ); }

// �h���N���X�p�R���X�g���N�^(32bit)
#define		FLAG32_CONSTRUCT( name )	\
public:\
	name(){}\
	name(Phoenix::u32 flag){ SetAll( flag ); }

// �h���N���X�p�R���X�g���N�^(16bit)
#define		FLAG16_CONSTRUCT( name )	\
public:\
	name(){}\
	name(Phoenix::u16 flag){ SetAll( flag ); }

// �h���N���X�p�R���X�g���N�^(8bit)
#define		FLAG8_CONSTRUCT( name )	\
public:\
	name(){}\
	name(Phoenix::u8 flag){ SetAll( flag ); }