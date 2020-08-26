#pragma once

#include "Phoenix/Types.h"


namespace Phoenix
{
	namespace FND
	{
		//****************************************************************************
		// ���O�o�̓��x��
		//****************************************************************************
		enum class LogLevel
		{
			Vervose,	// �ڍ׏��
			Info,		// ���
			Warning,	// ���[�j���O
			Error,		// �G���[
			All			// �K���o��
		};

		//****************************************************************************
		// ���O�o�̓C���^�[�t�F�[�X
		//****************************************************************************
		class ILogWriter
		{
		public:
			// ���O�o��
			virtual void Write(LogLevel level, const char* message) = 0;
			virtual void Write(LogLevel level, const wchar_t* message) = 0;
		};

		//****************************************************************************
		// ���O�o�͑���
		//****************************************************************************
		class Logger
		{
		private:
			static ILogWriter*  s_writer;
			static u32			s_type_mask;
			static LogLevel		s_level;

		public:
			// ���O�̏o�͐���̂��߂̃}�X�N�l��ݒ�
			static void SetTypeMask(u32 mask) { s_type_mask = 1 | mask; }

			// ���O�o�̓��x����ݒ�
			static void SetLevel(LogLevel level) { s_level = level; }

			// ���O�o�̓��x�����擾
			static LogLevel GetLevel() { return s_level; }

			// ���O���C�^�[��ݒ�
			static void SetWriter(ILogWriter* writer) { s_writer = writer; }

			// ���O���o��
			static void Print(u32 type, LogLevel level, const char* format, ...);
			static void Print(u32 type, LogLevel level, const wchar_t* format, ...);
		};

	} // namespace FND
} // namespace Phoenix

// �}�N��
#define PHOENIX_LOG_TYPE_ALL		(1 << 0)
#define PHOENIX_LOG_TYPE_FND		(1 << 1)
#define PHOENIX_LOG_TYPE_OS			(1 << 2)
#define PHOENIX_LOG_TYPE_MATH		(1 << 3)
#define PHOENIX_LOG_TYPE_RHI		(1 << 4)
#define PHOENIX_LOG_TYPE_SND		(1 << 5)
#define PHOENIX_LOG_TYPE_FBX		(1 << 6)
#define PHOENIX_LOG_TYPE_DEV		(1 << 7)
#define PHOENIX_LOG_TYPE_FRAMEWORK	(1 << 8)
#define PHOENIX_LOG_TYPE_APP		(1 << 9)

#if defined(PHOENIX_TARGET_RELEASE) && 0
#define	PHOENIX_LOG_IMPLE(type, level, ...)		{}
#else
#define	PHOENIX_LOG_IMPLE(type, level, ...)		{ Phoenix::FND::Logger::Print(type, level, __VA_ARGS__); }
#endif

#define	PHOENIX_LOG_VERVOSE(...)			PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_ALL, Phoenix::FND::LogLevel::Vervose, __VA_ARGS__)
#define	PHOENIX_LOG(...)					PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_ALL, Phoenix::FND::LogLevel::Info, __VA_ARGS__)
#define	PHOENIX_LOG_WARNING(...)			PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_ALL, Phoenix::FND::LogLevel::Warning, __VA_ARGS__)
#define	PHOENIX_LOG_ERROR(...)				PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_ALL, Phoenix::FND::LogLevel::Error, __VA_ARGS__)

#define	PHOENIX_LOG_FND_VERVOSE(...)		PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_FND, Phoenix::FND::LogLevel::Vervose, __VA_ARGS__)
#define	PHOENIX_LOG_FND(...)				PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_FND, Phoenix::FND::LogLevel::Info, __VA_ARGS__)
#define	PHOENIX_LOG_FND_WARNING(...)		PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_FND, Phoenix::FND::LogLevel::Warning, __VA_ARGS__)
#define	PHOENIX_LOG_FND_ERROR(...)			PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_FND, Phoenix::FND::LogLevel::Error, __VA_ARGS__)

#define	PHOENIX_LOG_OS_VERVOSE(...)			PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_OS, Phoenix::FND::LogLevel::Vervose, __VA_ARGS__)
#define	PHOENIX_LOG_OS(...)					PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_OS, Phoenix::FND::LogLevel::Info, __VA_ARGS__)
#define	PHOENIX_LOG_OS_WARNING(...)			PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_OS, Phoenix::FND::LogLevel::Warning, __VA_ARGS__)
#define	PHOENIX_LOG_OS_ERROR(...)			PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_OS, Phoenix::FND::LogLevel::Error, __VA_ARGS__)

#define	PHOENIX_LOG_MATH_VERVOSE(...)		PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_MATH, Phoenix::FND::LogLevel::Vervose, __VA_ARGS__)
#define	PHOENIX_LOG_MATH(...)				PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_MATH, Phoenix::FND::LogLevel::Info, __VA_ARGS__)
#define	PHOENIX_LOG_MATH_WARNING(...)		PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_MATH, Phoenix::FND::LogLevel::Warning, __VA_ARGS__)
#define	PHOENIX_LOG_MATH_ERROR(...)			PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_MATH, Phoenix::FND::LogLevel::Error, __VA_ARGS__)

#define	PHOENIX_LOG_GRP_VERVOSE(...)		PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_RHI, Phoenix::FND::LogLevel::Vervose, __VA_ARGS__)
#define	PHOENIX_LOG_GRP(...)				PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_RHI, Phoenix::FND::LogLevel::Info, __VA_ARGS__)
#define	PHOENIX_LOG_GRP_WARNING(...)		PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_RHI, Phoenix::FND::LogLevel::Warning, __VA_ARGS__)
#define	PHOENIX_LOG_GRP_ERROR(...)			PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_RHI, Phoenix::FND::LogLevel::Error, __VA_ARGS__)

#define	PHOENIX_LOG_SND_VERVOSE(...)		PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_SND, Phoenix::FND::LogLevel::Vervose, __VA_ARGS__)
#define	PHOENIX_LOG_SND(...)				PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_SND, Phoenix::FND::LogLevel::Info, __VA_ARGS__)
#define	PHOENIX_LOG_SND_WARNING(...)		PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_SND, Phoenix::FND::LogLevel::Warning, __VA_ARGS__)
#define	PHOENIX_LOG_SND_ERROR(...)			PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_SND, Phoenix::FND::LogLevel::Error, __VA_ARGS__)

#define	PHOENIX_LOG_FBX_VERVOSE(...)		PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_FBX, Phoenix::FND::LogLevel::Vervose, __VA_ARGS__)
#define	PHOENIX_LOG_FBX(...)				PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_FBX, Phoenix::FND::LogLevel::Info, __VA_ARGS__)
#define	PHOENIX_LOG_FBX_WARNING(...)		PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_FBX, Phoenix::FND::LogLevel::Warning, __VA_ARGS__)
#define	PHOENIX_LOG_FBX_ERROR(...)			PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_FBX, Phoenix::FND::LogLevel::Error, __VA_ARGS__)

#define	PHOENIX_LOG_DEV_VERVOSE(...)		PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_DEV, Phoenix::FND::LogLevel::Vervose, __VA_ARGS__)
#define	PHOENIX_LOG_DEV(...)				PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_DEV, Phoenix::FND::LogLevel::Info, __VA_ARGS__)
#define	PHOENIX_LOG_DEV_WARNING(...)		PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_DEV, Phoenix::FND::LogLevel::Warning, __VA_ARGS__)
#define	PHOENIX_LOG_DEV_ERROR(...)			PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_DEV, Phoenix::FND::LogLevel::Error, __VA_ARGS__)

#define	PHOENIX_LOG_FRAMEWORK_VERVOSE(...)	PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_FRAMEWORK, Phoenix::FND::LogLevel::Vervose, __VA_ARGS__)
#define	PHOENIX_LOG_FRAMEWORK(...)			PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_FRAMEWORK, Phoenix::FND::LogLevel::Info, __VA_ARGS__)
#define	PHOENIX_LOG_FRAMEWORK_WARNING(...)	PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_FRAMEWORK, Phoenix::FND::LogLevel::Warning, __VA_ARGS__)
#define	PHOENIX_LOG_FRAMEWORK_ERROR(...)	PHOENIX_LOG_IMPLE(PHOENIX_LOG_TYPE_FRAMEWORK, Phoenix::FND::LogLevel::Error, __VA_ARGS__)