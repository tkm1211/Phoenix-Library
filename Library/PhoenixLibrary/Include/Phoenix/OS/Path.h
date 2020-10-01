#pragma once

namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// �t�@�C���p�X���샆�[�e�B���e�B
		//****************************************************************************
		class Path
		{
		public:
			// �J�����g�f�B���N�g�����擾
			static const char* GetCurrentDirectory(char* buffer = nullptr, int bufferSize = 0);

			// �t���p�X���擾
			static const char* GetFullPath(const char* filePath, char* buffer = nullptr, int bufferSize = 0);
			static const wchar_t* GetFullPathW(const wchar_t* filePath, wchar_t* buffer = nullptr, int bufferSize = 0);

			// �t���p�X����t�@�C�������擾
			static const char* GetFileName(const char* filePath);

			// �t���p�X����t�@�C���g���q���擾
			static const char* GetFileExtension(const char* filePath);

			// �t���p�X�̃t�@�C���g���q��ύX
			static const char* ChangeFileExtension(const char* filePath, const char* extension, char* buffer = nullptr, int bufferSize = 0);

			// �t���p�X����t�@�C���g���q���`�F�b�N
			static bool CheckFileExtension(const char* filePath, const char* extension);

			// �t���p�X����t�@�C�������������f�B���N�g���p�X���擾
			static const char* GetDirectoryName(const char* filePath, char* buffer = nullptr, int bufferSize = 0);

			// �t�@�C���p�X���������ɕϊ���'\\'��'/'�ϊ�
			static const char* ToLower(const char* filePath, char* buffer = nullptr, int bufferSize = 0);

			// ../���܂ރp�X���œK��
			static const char* Clean(const char* filePath, char* buffer = nullptr, int bufferSize = 0);

			// ���΃t�@�C���p�X�𐶐�
			static const char* GetRelativePath(const char* basePath, const char* targetPath, char* buffer = nullptr, int bufferSize = 0);

			// ���΃t�@�C���p�X�����΃t�@�C���p�X�𐶐�
			static const char* GetAbsolutePath(const char* basePath, const char* relativePath, char* buffer = nullptr, int bufferSize = 0);

			// �t�@�C���p�X����g���q�Ȃ��t�@�C�������擾
			static const char* GetFileNameWithoutExtension(const char* filePath, char* buffer = nullptr, int bufferSize = 0);

			// �t�@�C���p�X������
			static const char* Combine(const char* basePath, const char* targetPath, char* buffer = nullptr, int bufferSize = 0);

			// ���C���h�J�[�h�}�b�`���O
			// '*' �́A�����������ł��}�b�`���O����
			// '?' �́A�ꕶ�����ł��}�b�`���O����
			// �������C���h�J�[�h�̓X�y�[�X�܂��̓^�u�ŋ�؂邱��
			static bool IsMatchWildcard(const char* filePath, const char* wildcard);

			// �p�X�����ꂩ��r
			static bool Equal(const char* filePth1, const char* filePath2);

			// ���s�\�t�@�C�������s����Ă���ꏊ�܂Ńf�B���N�V���� �i��F"C:\\Desktop\\Project\\�j
			// Debug��Release�̃t�@�C���p�X�͊܂܂Ȃ��B�����A�܂ނ̂ł����keep��true�ɂ���Ƃ��̂܂܂̏�ԂŕԂ��B
			static const char* GetModuleFilePass(bool keep = false);

			// ���s�\�t�@�C�������s����Ă���ꏊ�܂Ńf�B���N�V���� �i��F"C:\\Desktop\\Project\\�j
			// backCount�̐����t�@�C���K�w��k��B
			static const char* GetModuleFilePass(int backCount);
		};

	} // namespace OS
} // namespace Phoenix