#pragma once

#include <stdio.h>
#include "Phoenix/OS/Stream.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// Windows�Ńt�@�C���X�g���[������N���X
		//****************************************************************************
		class FileStreamWin final : public IFileStream
		{
		private:
			FILE* fp = nullptr;
			char basePath[256] = { 0 };

		public:
			~FileStreamWin() override { Finalize(); }

		public:
			// ������
			bool Initialize(const char* basePath) override;

			// �I����
			void Finalize() override { Close(); }

			// ���݊m�F
			bool Exists(const char* path) override;
			bool ExistsW(const wchar_t* path) override;

			// �I�[�v��
			bool Open(const char* path, FileAccess access) override;

			// �N���[�Y
			void Close() override;

			// �ǂݍ���
			s32 Read(void* buffer, s32 size) override;

			// ��������
			s32 Write(const void* buffer, s32 size) override;

			// �t�@�C���|�C���^�ʒu�ݒ�
			bool Seek(s32 offset, StreamSeek origin) override;

			// �t�@�C���|�C���^�ʒu�擾
			s32 Tell() override;

			// �t�@�C���T�C�Y�擾
			s32 GetSize() override;

			// �t�@�C���̍폜
			bool Remove(const char* path) override;
		};
	} // namespace OS
} // namespace Phoenix