#include "pch.h"
#include "Phoenix/FND/Operation.h"


BOOL GetFileName(HWND hWnd, TCHAR* fname, int sz, const wchar_t* initDir, const wchar_t* ext, bool isOpen)
{
	if (isOpen)
	{
		OPENFILENAME o;
		fname[0] = _T('\0');
		ZeroMemory(&o, sizeof(o));
		o.lStructSize = sizeof(o);		// �\���̃T�C�Y
		o.hwndOwner = hWnd;				// �e�E�B���h�E�̃n���h��
		o.lpstrInitialDir = initDir;	// �����t�H���_�[
		o.lpstrFile = fname;			// �擾�����t�@�C������ۑ�����o�b�t�@
		o.nMaxFile = sz;				// �擾�����t�@�C������ۑ�����o�b�t�@�T�C�Y
		if (wcscmp(ext, L".bin") == 0)
		{
			o.lpstrFilter = _TEXT("bin�t�@�C��(*.bin)\0*.bin\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
			o.lpstrDefExt = _TEXT("bin");
		}
		else if (wcscmp(ext, L".json") == 0)
		{
			o.lpstrFilter = _TEXT("json�t�@�C��(*.json)\0*.json\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
			o.lpstrDefExt = _TEXT("json");
		}
		else if (wcscmp(ext, L".fbx") == 0)
		{
			o.lpstrFilter = _TEXT("fbx�t�@�C��(*.fbx)\0*.fbx\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
			o.lpstrDefExt = _TEXT("fbx");
		}
		else
		{
			o.lpstrFilter = _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
			o.lpstrDefExt = _TEXT("");
		}
		o.lpstrTitle = _TEXT("�t�@�C�����w��");
		o.nFilterIndex = 1;
#if 0 // �����t�@�C���̃p�X���擾
		o.Flags |= OFN_ALLOWMULTISELECT;
		o.Flags |= OFN_EXPLORER;
#endif
		return GetOpenFileName(&o);
	}
	else
	{
		OPENFILENAME s;
		fname[0] = _T('\0');
		ZeroMemory(&s, sizeof(s));
		s.lStructSize = sizeof(s);		// �\���̃T�C�Y
		s.hwndOwner = hWnd;				// �e�E�B���h�E�̃n���h��
		s.lpstrInitialDir = initDir;	// �����t�H���_�[
		s.lpstrFile = fname;			// �擾�����t�@�C������ۑ�����o�b�t�@
		s.nMaxFile = sz;				// �擾�����t�@�C������ۑ�����o�b�t�@�T�C�Y
		if (wcscmp(ext, L".bin") == 0)
		{
			s.lpstrFilter = _TEXT("bin�t�@�C��(*.bin)\0*.bin\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
			s.lpstrDefExt = _TEXT("bin");
		}
		else if (wcscmp(ext, L".json") == 0)
		{
			s.lpstrFilter = _TEXT("json�t�@�C��(*.json)\0*.json\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
			s.lpstrDefExt = _TEXT("json");
		}
		else
		{
			s.lpstrFilter = _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
			s.lpstrDefExt = _TEXT("");
		}
		s.lpstrTitle = _TEXT("�t�@�C����ۑ�");
		s.nFilterIndex = 1;
		return GetSaveFileName(&s);
	}

	return FALSE;
}


std::string GetModuleFilePass(bool keep)
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

	return result;
}

std::wstring GetModuleFilePassW(bool keep)
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

	std::wstring reconvert = StringToWString(result);

	return reconvert;
}

std::wstring StringToWString(std::string oString)
{
	// SJIS �� wstring
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, (wchar_t*)NULL, 0);

	// �o�b�t�@�̎擾
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];

	// SJIS �� wstring
	MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, cpUCS2, iBufferSize);

	// string�̐���
	std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

	// �o�b�t�@�̔j��
	delete[] cpUCS2;

	// �ϊ����ʂ�Ԃ�
	return(oRet);
}