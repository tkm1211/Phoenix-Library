#include "pch.h"
#include <stdio.h>
#include <assert.h>
#include "Phoenix/FND/assert.h"
#include "Phoenix/FND/logger.h"


namespace Phoenix
{
	namespace FND
	{
		// �X�^�e�B�b�N
		IAssertWriter* Assert::writer = nullptr;

		//****************************************************************************
		// �A�T�[�g����N���X
		//****************************************************************************
		// �A�T�[�g���C�^�[�ݒ�
		void Assert::SetWriter(IAssertWriter* assertWriter)
		{
			writer = assertWriter;
		}

		// �A�T�[�g�u���C�N
		void Assert::Break(bool expression, const char* file, s32 line, const char* function)
		{
			if (!expression)
			{
				char message[1024];
				::sprintf_s(message, sizeof(message), "file:%s\nline:%d\nfunction:%s", file, line, function);
				::MessageBoxA(NULL, message, "Assert", MB_OK);
			}
			assert(expression);
		}

		// �A�T�[�g�o��
		void Assert::Print(const char* file, s32 line, const char* format, ...)
		{
			char message[1024];
			va_list args;
			va_start(args, format);
			vsnprintf(message, sizeof(message), format, args);
			va_end(args);

#if		defined(PHOENIX_TARGET_WIN)
			::OutputDebugStringA(message);
			::puts(message);
#endif

			if (writer != nullptr)
			{
				writer->Write(file, line, message);
			}
		}
	}
}