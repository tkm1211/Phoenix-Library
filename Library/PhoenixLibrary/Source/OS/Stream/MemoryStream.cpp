#include "pch.h"
#include "Phoenix/FND/Logger.h"
#include "Phoenix/FND/STD.h"
#include "MemoryStream.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// �������X�g���[������N���X
		//****************************************************************************
		// ����
		std::unique_ptr<IMemoryStream> IMemoryStream::Create()
		{
			return std::make_unique<MemoryStream>();
		}

		bool MemoryStream::Open(void* buffer, s32 size)
		{
			buffer = current = reinterpret_cast<u8*>(buffer);
			size = size;
			return false;
		}

		// �N���[�Y
		void MemoryStream::Close()
		{
			buffer = nullptr;
			current = nullptr;
			size = 0;
		}

		// �ǂݍ���
		s32 MemoryStream::Read(void* buffer, s32 size)
		{
			s32 remain = size - static_cast<s32>(current - static_cast<u8*>(buffer));
			s32 read = (remain < size) ? remain : size;

			FND::MemCpy(buffer, current, read);
			current += read;

			return read;
		}

		// ��������
		s32 MemoryStream::Write(const void* buffer, s32 size)
		{
			s32 remain = size - static_cast<s32>(current - static_cast<const u8*>(buffer));
			s32 write = (remain < size) ? remain : size;

			FND::MemCpy(current, buffer, write);
			current += write;

			return write;
		}

		// �t�@�C���|�C���^�ʒu�ݒ�
		bool MemoryStream::Seek(s32 offset, StreamSeek origin)
		{
			switch (origin)
			{
			case StreamSeek::Top:
			{
				s32	pos = offset;
				if (pos < 0 || pos > size)
				{
					PHOENIX_LOG_OS_ERROR("�t�@�C���V�[�N�Ɏ��s���܂����B\n");
					return false;
				}

				current = buffer + offset;
				break;
			}
			case StreamSeek::Current:
			{
				s32	pos = static_cast<s32>(current - buffer) + offset;
				if (pos < 0 || pos > size)
				{
					PHOENIX_LOG_OS_ERROR("�t�@�C���V�[�N�Ɏ��s���܂����B\n");
					return false;
				}

				current += offset;
				break;
			}
			case StreamSeek::End:
			{
				s32 pos = size + offset;
				if (pos < 0 || pos > size)
				{
					PHOENIX_LOG_OS_ERROR("�t�@�C���V�[�N�Ɏ��s���܂����B\n");
					return false;
				}

				current = buffer + pos;
				break;
			}
			}

			return true;
		}

		// �t�@�C���|�C���^�ʒu�擾
		s32 MemoryStream::Tell()
		{
			return static_cast<s32>(current - buffer);
		}

		// �t�@�C���T�C�Y�擾
		s32 MemoryStream::GetSize()
		{
			return size;
		}

	} // namespace OS
} // namespace Phoenix