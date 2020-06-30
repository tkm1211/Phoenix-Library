#pragma once

#include "Phoenix/OS/Stream.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// �������X�g���[������N���X
		//****************************************************************************
		class MemoryStream final : public IMemoryStream
		{
		private:
			u8* buffer = nullptr;
			u8* current = nullptr;
			s32 size;

		public:
			~MemoryStream() override { Finalize(); }

		public:
			// ������
			bool Initialize() override { return true; }

			// �I����
			void Finalize() override { Close(); }

			// �I�[�v��
			bool Open(void* buffer, s32 size) override;

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
		};
	} // namespace OS
} // namespace Phoenix