#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// �X�g���[���V�[�N
		//****************************************************************************
		enum class StreamSeek
		{
			Top,		// �t�@�C���̐擪
			Current,	// �t�@�C���̌��݈ʒu
			End			// �t�@�C���̏I�[
		};

		//****************************************************************************
		// �t�@�C���A�N�Z�X
		//****************************************************************************
		enum class FileAccess
		{
			Read,	// �ǂݍ���
			Write,	// ��������
			Append	// �ǉ���������
		};

		//****************************************************************************
		// �X�g���[������C���^�[�t�F�[�X
		//****************************************************************************
		class IStream : public FND::Base
		{
		public:
			// �N���[�Y
			virtual void Close() = 0;

			// �ǂݍ���
			virtual s32 Read(void* buffer, s32 size) = 0;

			// ��������
			virtual s32 Write(const void* buffer, s32 size) = 0;

			// �|�C���^�ʒu�ݒ�
			virtual bool Seek(s32 offset, StreamSeek origin) = 0;

			// �|�C���^�ʒu�擾
			virtual s32 Tell() = 0;

			// �T�C�Y�擾
			virtual s32 GetSize() = 0;
		};

		//****************************************************************************
		// �t�@�C���X�g���[������C���^�[�t�F�[�X
		//****************************************************************************
		class IFileStream : public IStream
		{
		public:
			// �t�@�C������
			static std::unique_ptr<IFileStream> Create();

			// ������
			virtual bool Initialize(const char* basePath) = 0;

			// �I����
			virtual void Finalize() = 0;

			// ���݊m�F
			virtual bool Exists(const char* path) = 0;
			virtual bool ExistsW(const wchar_t* path) = 0;

			// �I�[�v��
			virtual bool Open(const char* path, FileAccess access) = 0;

			// �t�@�C���̍폜
			virtual bool Remove(const char* path) = 0;
		};

		//****************************************************************************
		// �������X�g���[������C���^�[�t�F�[�X
		//****************************************************************************
		class IMemoryStream : public IStream
		{
		public:
			// �t�@�C������
			static std::unique_ptr<IMemoryStream> Create();

			// ������
			virtual bool Initialize() = 0;

			// �I����
			virtual void Finalize() = 0;

			// �I�[�v��
			virtual bool Open(void* buffer, s32 size) = 0;
		};
	} // namespace OS
} // namespace Phoenix