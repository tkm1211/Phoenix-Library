#pragma once

#include "Phoenix/Types.h"


namespace Phoenix
{
	namespace FND
	{
		//****************************************************************************
		// �A�T�[�g�o�̓C���^�[�t�F�[�X
		//****************************************************************************
		class IAssertWriter
		{
		public:
			// �A�T�[�g�o��
			virtual void Write(const char* file, s32 line, const char* message) = 0;
		};

		//****************************************************************************
		// �A�T�[�g����
		//****************************************************************************
		class Assert
		{
		private:
			static IAssertWriter* writer;

		public:
			// �A�T�[�g���C�^�[�ݒ�
			static void SetWriter(IAssertWriter* assertWriter);

			// �A�T�[�g�u���C�N
			static void Break(bool expression, const char* file, s32 line, const char* function);

			// �A�T�[�g�o��
			static void Print(const char* file, s32 line, const char* format, ...);
		};

	} // namespace FND
} // namespace Phoenix

// �}�N��
#if 	defined(PHOENIX_TARGET_RELEASE)
	#undef	PHOENIX_ASSERT
	#define	PHOENIX_ASSERT(...)				{}
	#undef	PHOENIX_ASSERT_MSG
	#define	PHOENIX_ASSERT_MSG(...)			{}		///< �A�T�[�g�`�F�b�N
#elif	defined(PHOENIX_TARGET_DEBUG)
	#undef	PHOENIX_ASSERT
	#define	PHOENIX_ASSERT(test)			{ Phoenix::FND::Assert::Break(test, __FILE__,__LINE__, __FUNCTION__); }
	#undef	PHOENIX_ASSERT_MSG
	#define	PHOENIX_ASSERT_MSG(test, ...)	{ if(!(test)){ Phoenix::FND::Assert::Print(__FILE__,__LINE__,__VA_ARGS__); PHOENIX_ASSERT(test); } }
#endif