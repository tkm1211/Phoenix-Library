#pragma once

#include <Windows.h>
#include <tchar.h>
#include <string>


// �t�@�C�������w�肷��R�����_�C�A���O��\��
BOOL GetFileName(HWND hWnd, TCHAR* fname, int sz, const wchar_t* initDir, const wchar_t* ext, bool isOpen = true);

// ���s�\�t�@�C�������s����Ă���ꏊ�܂Ńf�B���N�V���� �i��F"C:\\Desktop\\Project\\�j
// Debug��Release�̃t�@�C���p�X�͊܂܂Ȃ��B�����A�܂ނ̂ł����keep��true�ɂ���Ƃ��̂܂܂̏�ԂŕԂ��B
std::string GetModuleFilePass(bool keep = false);

// ���s�\�t�@�C�������s����Ă���ꏊ�܂Ńf�B���N�V���� �i��FL"C:\\Desktop\\Project\\�j
// Debug��Release�̃t�@�C���p�X�͊܂܂Ȃ��B�����A�܂ނ̂ł����keep��true�ɂ���Ƃ��̂܂܂̏�ԂŕԂ��B
std::wstring GetModuleFilePassW(bool keep = false);

// string��wstring�֕ϊ�����
std::wstring StringToWString(std::string oString);