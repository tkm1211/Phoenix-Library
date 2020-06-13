#include "pch.h"
#include <stdio.h>
#include "Phoenix/FND/Logger.h"


namespace Phoenix
{
	namespace FND
	{
		// �X�^�e�B�b�N
		ILogWriter* Logger::s_writer = nullptr;
		u32			Logger::s_type_mask = 0xFFFFFF;
		LogLevel	Logger::s_level = LogLevel::All;

		//****************************************************************************
		// ���O����N���X
		//****************************************************************************
		// ���O�o��
		void Logger::Print(u32 type, LogLevel level, const char* format, ...)
		{
			if (type & s_type_mask)
			{
				if (level == LogLevel::All || level < s_level)
				{
					char message[1024];
					va_list args;
					va_start(args, format);
					vsnprintf_s(message, sizeof(message), format, args);
					va_end(args);

#if	defined(PHOENIX_TARGET_WIN)
					::OutputDebugStringA(message);
					::puts(message);
#endif
					if (s_writer != nullptr)
					{
						s_writer->Write(level, message);
					}
				}
			}
		}

		// ���O�o��
		void Logger::Print(u32 type, LogLevel level, const wchar_t* format, ...)
		{
			if (type & s_type_mask)
			{
				if (level == LogLevel::All || level < s_level)
				{
					wchar_t message[1024];
					va_list args;
					va_start(args, format);
					vswprintf_s(message, sizeof(message) / sizeof(message[0]), format, args);
					va_end(args);

#if	defined(PHOENIX_TARGET_WIN)
					::OutputDebugStringW(message);
					::_putws(message);
#endif
					if (s_writer != nullptr)
					{
						s_writer->Write(level, message);
					}
				}
			}
		}
	}
}