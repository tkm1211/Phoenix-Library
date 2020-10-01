#include "pch.h"
#include <process.h>
#include "Phoenix/FND/Assert.h"
#include "Phoenix/FND/Logger.h"
#include "FileStreamWin.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// Windows�Ńt�@�C���X�g���[������N���X
		//****************************************************************************
		// ����
		std::unique_ptr<IFileStream> IFileStream::Create()
		{
			return std::make_unique<FileStreamWin>();
		}

		// ������
		bool FileStreamWin::Initialize(const char* basePath)
		{
			if (basePath != nullptr)
			{
				::SetCurrentDirectoryA(basePath);
			}
			::GetCurrentDirectoryA(sizeof(this->basePath), this->basePath);

			return true;
		}

		// ���݊m�F
		bool FileStreamWin::Exists(const char* path)
		{
			::SetCurrentDirectoryA(basePath);

			FILE* fp = nullptr;
			errno_t err = fopen_s(&fp, path, "rb");
			if (fp != nullptr)
			{
				fclose(fp);
			}
			return err == 0;
		}
		bool FileStreamWin::ExistsW(const wchar_t* path)
		{
			::SetCurrentDirectoryA(basePath);

			FILE* fp = nullptr;
			errno_t err = _wfopen_s(&fp, path, L"rb");
			if (fp != nullptr)
			{
				fclose(fp);
			}
			return err == 0;
		}

		// �I�[�v��
		bool FileStreamWin::Open(const char* path, FileAccess access)
		{
			::SetCurrentDirectoryA(basePath);

			const char* mode;
			switch (access)
			{
			case FileAccess::Read:		mode = "rb";	break;
			case FileAccess::Write:		mode = "wb+";	break;
			case FileAccess::Append:	mode = "rb+";	break;
			default: return false;
			}

			errno_t err = fopen_s(&fp, path, mode);
			if (err != 0)
			{
				PHOENIX_LOG_OS_ERROR("�t�@�C����������܂���B: %s\n", path);
				return false;
			}
			return true;
		}

		// �N���[�Y
		void FileStreamWin::Close()
		{
			if (fp != nullptr)
			{
				fclose(fp);
				fp = nullptr;
			}
		}

		// �ǂݍ���
		s32 FileStreamWin::Read(void* buffer, s32 size)
		{
			PHOENIX_ASSERT_MSG(fp != nullptr, "�t�@�C���X�g���[���������ł��B\n");

			return static_cast<s32>(fread(buffer, 1, size, fp));
		}

		// ��������
		s32 FileStreamWin::Write(const void* buffer, s32 size)
		{
			PHOENIX_ASSERT_MSG(fp != nullptr, "�t�@�C���X�g���[���������ł��B\n");

			return static_cast<s32>(fwrite(buffer, 1, size, fp));
		}

		// �t�@�C���|�C���^�ʒu�ݒ�
		bool FileStreamWin::Seek(s32 offset, StreamSeek origin)
		{
			PHOENIX_ASSERT_MSG(fp != nullptr, "�t�@�C���X�g���[���������ł��B\n");

			int seek = 0;
			switch (origin)
			{
			case StreamSeek::Top:		seek = SEEK_SET; break;
			case StreamSeek::Current:	seek = SEEK_CUR; break;
			case StreamSeek::End:		seek = SEEK_END; break;
			}
			return  (0 == fseek(fp, offset, seek));
		}

		// �t�@�C���|�C���^�ʒu�擾
		s32 FileStreamWin::Tell()
		{
			PHOENIX_ASSERT_MSG(fp != nullptr, "�t�@�C���X�g���[���������ł��B\n");
			return static_cast<s32>(ftell(fp));
		}

		// �t�@�C���T�C�Y�擾
		s32 FileStreamWin::GetSize()
		{
			PHOENIX_ASSERT_MSG(fp != nullptr, "�t�@�C���X�g���[���������ł��B\n");

			long now = ftell(fp);
			fseek(fp, 0, SEEK_END);
			long pos = ftell(fp);
			fseek(fp, now, SEEK_SET);
			return static_cast<s32>(pos);
		}

		// �t�@�C���̍폜
		bool FileStreamWin::Remove(const char* path)
		{
			return (0 == remove(path));
		}

	}	// namespace os
}	// namespace ys