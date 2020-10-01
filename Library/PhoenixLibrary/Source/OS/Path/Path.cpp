#include "pch.h"
#include <string>
#include "Phoenix/FND/STD.h"
#include "Phoenix/OS/Path.h"


namespace Phoenix
{
	namespace OS
	{
		//****************************************************************************
		// �t�@�C���p�X����N���X
		//****************************************************************************
		// �J�����g�f�B���N�g�����擾
		const char* Path::GetCurrentDirectory(char* buffer, int buffer_size)
		{
			if (buffer == nullptr)
			{
				static char s_string_buffer[256];
				buffer = s_string_buffer;
				buffer_size = sizeof(s_string_buffer);
			}
			::GetCurrentDirectoryA(buffer_size, buffer);
			FND::StrCat(buffer, buffer_size, "\\");

			return buffer;
		}

		// �t���p�X���擾
		const char* Path::GetFullPath(const char* filepath, char* buffer, int buffer_size)
		{
			if (buffer == nullptr)
			{
				static char s_string_buffer[256];
				buffer = s_string_buffer;
				buffer_size = sizeof(s_string_buffer);
			}
			::GetFullPathNameA(filepath, buffer_size, buffer, nullptr);

			return buffer;
		}
		const wchar_t* Path::GetFullPathW(const wchar_t* filePath, wchar_t* buffer, int bufferSize)
		{
			if (buffer == nullptr)
			{
				static wchar_t s_string_buffer[256];
				buffer = s_string_buffer;
				bufferSize = sizeof(s_string_buffer);
			}
			::GetFullPathNameW(filePath, bufferSize, buffer, nullptr);

			return buffer;
		}
		
		// �t�@�C�����擾
		const char* Path::GetFileName(const char* filePath)
		{
			if (filePath == nullptr)
			{
				return nullptr;
			}
			const char* p = filePath + FND::StrLen(filePath);

			while (p > filePath)
			{
				if ((*p == '\\') || (*p == '/'))
				{
					p++;
					break;
				}
				p--;
			}

			return p;
		}

		// �t�@�C���g���q�擾
		const char* Path::GetFileExtension(const char* filePath)
		{
			if (filePath == nullptr)
			{
				return nullptr;
			}

			const char* p = filePath + FND::StrLen(filePath);

			while (p > filePath)
			{
				if (*p == '.')
				{
					p++;
					break;
				}
				p--;
			}

			return p;
		}

		// �t�@�C���g���q�ύX
		const char* Path::ChangeFileExtension(const char* filePath, const char* extension, char* buffer, int bufferSize)
		{
			if (filePath == nullptr || extension == nullptr)
			{
				return nullptr;
			}
			if (buffer == nullptr)
			{
				static char s_string_buffer[256];
				buffer = s_string_buffer;
				bufferSize = sizeof(s_string_buffer);
			}
			FND::StrCpy(buffer, bufferSize, filePath);

			char* p = buffer + FND::StrLen(buffer);

			while (p > buffer)
			{
				if (*p == '.')
				{
					p++;
					break;
				}
				if (*p == '/' ||
					*p == '\\')
				{
					p = buffer;
					break;
				}
				p--;
			}
			if (p == buffer)
			{
				FND::StrCat(buffer, 256, ".");
				FND::StrCat(buffer, 256, extension);
			}
			else
			{
				FND::StrCpy(p, 256, extension);
			}

			return buffer;
		}

		// �t�@�C���g���q����
		bool Path::CheckFileExtension(const char* filePath, const char* extension)
		{
			const char* file_extension = GetFileExtension(filePath);
			if (file_extension == filePath)
			{
				return false;
			}
			char lhs[256], rhs[256];
			FND::StrCpy(lhs, 256, file_extension);
			FND::StrCpy(rhs, 256, extension);
			ToLower(lhs);
			ToLower(rhs);

			return FND::StrCmp(rhs, lhs) == 0;
		}

		// �f�B���N�g�����擾
		const char* Path::GetDirectoryName(const char* filePath, char* buffer, int bufferSize)
		{
			if (filePath == nullptr)
			{
				return nullptr;
			}

			if (buffer == nullptr)
			{
				static char s_string_buffer[256];
				buffer = s_string_buffer;
				bufferSize = sizeof(s_string_buffer);
			}
			FND::StrCpy(buffer, bufferSize, filePath);

			char* p = buffer + ::strlen(buffer);

			while (p > buffer)
			{
				if ((*p == '\\') || (*p == '/'))
				{
					p++;
					break;
				}
				p--;
			}
			*p = '\0';

			return buffer;
		}

		// ������,'/'��؂�ɃR���o�[�g
		const char* Path::ToLower(const char* filePath, char* buffer, int bufferSize)
		{
			if (filePath == nullptr)
			{
				return nullptr;
			}
			if (buffer == nullptr)
			{
				static char s_string_buffer[256];
				buffer = s_string_buffer;
				bufferSize = sizeof(s_string_buffer);
			}
			FND::StrCpy(buffer, bufferSize, filePath);

			int length = FND::StrLen(buffer);
			for (int i = 0; i < length; i++)
			{
				switch (buffer[i])
				{
				case '\\':
					buffer[i] = '/';
					break;
				default:						// ����(S-JIS)�łȂ���Ώ�������
					if ((i == 0) || (buffer[i - 1] < 0x80))
					{
						buffer[i] = static_cast<char>(FND::ToLower(buffer[i]));
					}
					break;
				}
			}
			return buffer;
		}

		// ../���܂ރp�X���œK��
		const char* Path::Clean(const char* filePath, char* buffer, int bufferSize)
		{
			if (filePath == nullptr)
			{
				return nullptr;
			}
			if (buffer == nullptr)
			{
				static char s_string_buffer[256];
				buffer = s_string_buffer;
				bufferSize = sizeof(s_string_buffer);
			}
			FND::StrCpy(buffer, bufferSize, filePath);

			char* p;

			// �܂� \\ �� / �ɓ���
			for (p = buffer; *p != '\0'; p++)
			{
				if (*p == '\\')
				{
					*p = '/';
				}
			}

			// �d�˃X���b�V���̐���
			for (p = buffer; *p != '\0'; p++)
			{
				if ((p[0] == '/') && (p[1] == '/'))
				{
					char* next = p + 2;
					while (*next == '/')
					{
						next++;
					}
					FND::StrCpy(p + 1, 256, next);
				}
			}

#if 0
			// /./
			for (p = buffer; *p != '\0'; p++)
			{
				if ((p[0] == '/') && (p[1] == '.') && (p[2] == '/'))
				{
					char* next = p + 3;
					FND::StrCpy(p + 1, 256, next);
					p--;
				}
			}
#endif

			// �h���C�u���^�[�𖳎����邽�߂ɁA�ړ�
			for (p = buffer; *p != '\0'; p++)
			{
				if (*p == ':')
				{
					p++;
					buffer = p;
					if (*p == '/')
					{
						p++;
						buffer = p;
						break;
					}
				}
			}

			// "/./" �܂��͐擪�� "./" ���J�b�g
			for (p = buffer; *p != '\0'; p++)
			{
				if ((p[0] == '.') && (p[1] == '/'))
				{
					if ((p == buffer) || (p[-1] == '/'))
					{
						FND::StrCpy(p, 256, p + 2);
					}
				}
			}

			// "/../" �����o�����璼�O�̃f�B���N�g�����Ɩ���
			char* prev_directory;
		_retry:
			prev_directory = p = buffer;
			while (1)
			{
				switch (*p)
				{
				case '\0':
					return buffer;
				case '/':
					if ((p[1] == '.') && (p[2] == '.') && (p[3] == '/'))
					{
						if (FND::StrNCmp(prev_directory, "../", 3) != 0)
						{
							FND::StrCpy(prev_directory, 256, p + 4);
							goto _retry;
						}
					}
					// ���O�̃f�B���N�g���ʒu�X�V
					prev_directory = p + 1;
					break;
				}
				p++;
			}
		}

		// ���΃p�X����
		const char* Path::GetRelativePath(const char* basePath, const char* targetPath, char* buffer, int bufferSize)
		{
			if (buffer == nullptr)
			{
				static char s_string_buffer[256];
				buffer = s_string_buffer;
				bufferSize = sizeof(s_string_buffer);
			}

			char base_string[2][256];
			char* base_string_a = base_string[0];
			char* base_string_b = base_string[1];

			// ��r�̂��߂ɏ��������E�œK��
			Clean(ToLower(basePath), base_string[0], sizeof(base_string[0]));
			Clean(ToLower(targetPath), base_string[1], sizeof(base_string[1]));

			// �p�X�̋��ʕ�����r
			int	str_index = -1;
			int	i, l = FND::StrLen(base_string[0]);
			for (i = 0; i < l; i++)
			{
				if (base_string[0][i] != base_string[1][i]) break;
				if (base_string[0][i] == '/') str_index = i;
			}

			str_index++;
			base_string_a += str_index;
			base_string_b += str_index;

			*buffer = '\0';
			if (str_index)	// ���ʕ���������Ȃ炳���̂ڂ�
			{
				while (*base_string_a != '\0')
				{
					if (*base_string_a == '/')
					{
						if (bufferSize < 3)
						{
							return nullptr;
						}
						FND::StrCat(buffer, bufferSize, "../");
						bufferSize -= 3;
					}
					base_string_a++;
				}
			}
			if (bufferSize < FND::StrLen(base_string_b))
			{
				return nullptr;
			}
			FND::StrCat(buffer, bufferSize, base_string_b);

			return buffer;
		}

		// ��΃p�X����
		const char* Path::GetAbsolutePath(const char* basePath, const char* relativePath, char* buffer, int bufferSize)
		{
			if (buffer == nullptr)
			{
				static char s_string_buffer[256];
				buffer = s_string_buffer;
				bufferSize = sizeof(s_string_buffer);
			}

			char dir[256];
			FND::SplitPath(basePath, nullptr, 0, dir, 256, nullptr, 0, nullptr, 0);
			FND::MakePath(buffer, bufferSize, nullptr, dir, relativePath, nullptr);

			return buffer;
		}

		// �g���q�Ȃ��t�@�C�����擾
		const char* Path::GetFileNameWithoutExtension(const char* filePath, char* buffer, int bufferSize)
		{
			if (buffer == nullptr)
			{
				static char s_string_buffer[256];
				buffer = s_string_buffer;
				bufferSize = sizeof(s_string_buffer);
			}

			const char* fname = GetFileName(filePath);
			if (fname == nullptr)
			{
				return nullptr;
			}

			char* p = buffer;

			bufferSize--;
			while ((*fname != '.') && (*fname != '\0') && (bufferSize > 0))
			{
				*p = static_cast<char>(FND::ToLower(*fname));
				p++;
				fname++;
				bufferSize--;
			}
			*p = '\0';

			return buffer;
		}

		// �t�@�C���p�X������
		const char* Path::Combine(const char* basePath, const char* targetPath, char* buffer, int bufferSize)
		{
			if (buffer == nullptr)
			{
				static char s_string_buffer[256];
				buffer = s_string_buffer;
				bufferSize = sizeof(s_string_buffer);
			}

			FND::StrCpy(buffer, bufferSize, basePath);
			size_t length = FND::StrLen(buffer);
			if (buffer[length - 1] != '\\' && buffer[length - 1] != '/')
			{
				FND::StrCat(buffer, bufferSize, "/");
			}
			FND::StrCat(buffer, bufferSize, targetPath);
			Clean(buffer, buffer, bufferSize);

			return buffer;
		}


		// ���C���h�J�[�h�}�b�`���O�T�u
		static bool IsMatchWildcardSub(const char* filename, const char* wildcard, int length)
		{
			while (1)
			{
				switch (*wildcard)
				{
				case '*':
					// '*' �̎��̕������炪�Ώ�
					do
					{
						wildcard++;
						length--;
					} while (*wildcard == '*');

					// '*' ���I�[�Ȃ� true �m��
					if (length <= 0)
					{
						return true;
					}

					// ���Ƃ͂P�����ÂċA�`�F�b�N
					while (*filename != '\0')
					{
						if (IsMatchWildcardSub(filename, wildcard, length))
						{
							return true;
						}
						filename++;
					}
					return false;

				case '?':
					if (*filename == '\0')
					{
						return false;
					}
					break;

				default:
					if ((*filename == '\0') && (length <= 0))
					{
						return true;
					}
					if (*filename != *wildcard)
					{
						return false;
					}
					break;
				}
				filename++;
				wildcard++;
				length--;
			}
		}

		// ���C���h�J�[�h�}�b�`���O
		bool Path::IsMatchWildcard(const char* filePath, const char* wildcard)
		{
			const char* p = wildcard;

			while (1)
			{
				// �擪����
				while ((*p == ' ') || (*p == '\t'))
				{
					p++;
				}

				// ��������
				if (*p == '\0') break;

				// ��������
				int length = 0;
				while ((p[length] != ' ') && (p[length] != '\t') && (p[length] != '\0'))
				{
					length++;
				}

				// �}�b�`���O
				if (IsMatchWildcardSub(filePath, p, length))
				{
					return true;
				}
				p += length;
			}

			return false;
		}

		// �p�X�����ꂩ��r
		bool Path::Equal(const char* filePath1, const char* filePath2)
		{
			char a[256], b[256];
			GetFullPath(filePath1, a, sizeof(a));
			GetFullPath(filePath2, b, sizeof(b));

			return FND::StrCmp(a, b) == 0;
		}

		// ���s�\�t�@�C�������s����Ă���ꏊ�܂Ńf�B���N�V���� �i��F"C:\\Desktop\\Project\\�j
		// Debug��Release�̃t�@�C���p�X�͊܂܂Ȃ��B�����A�܂ނ̂ł����keep��true�ɂ���Ƃ��̂܂܂̏�ԂŕԂ��B
		const char* Path::GetModuleFilePass(bool keep)
		{
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			std::string::size_type pos = std::string(buffer).find_last_of("\\/");
			std::string result = std::string(buffer).substr(0, pos);

			if (!keep)
			{
				std::string::size_type pos2 = result.find_last_of("\\/");
				std::string result2 = result.substr(pos2 + 1, pos);
				if (result2 == std::string("Debug") || result2 == std::string("Release"))
				{
					result = std::string(buffer).substr(0, pos2);
				}
			}
			result += std::string("\\");

			return result.c_str();
		}

		// ���s�\�t�@�C�������s����Ă���ꏊ�܂Ńf�B���N�V���� �i��F"C:\\Desktop\\Project\\�j
		// backCount�̐����t�@�C���K�w��k��B
		const char* Path::GetModuleFilePass(int backCount)
		{
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);

			std::string result = std::string(buffer);
			for (int i = 0; i < backCount; ++i)
			{
				std::string::size_type pos = std::string(result).find_last_of("\\/");
				result = std::string(result).substr(0, pos);
			}
			result += std::string("\\");

			return result.c_str();
		}
	}	// namespace os
}	// namespace ys

