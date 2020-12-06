#pragma once

#define WIN32_WINNT 0x0510

#include <Windows.h>
#include <xaudio2.h>
#include <mmsystem.h>
#include <thread>
#include <mutex>
#include <vector>
#include <cassert>
#include <vector>
#include <list>

#include "WAVFileReader.h"
#include "Phoenix/Types.h"

#pragma comment(lib, "xaudio2.lib")


// TODO : re -> Audio�N���X���쐬
namespace Phoenix
{
	namespace FrameWork
	{
		class CXAudio2
		{
		public:
			//std::vector<SourceVoice> sourceVoice;
			std::vector<IXAudio2SourceVoice*> lpSourceVoice;
			std::unique_ptr<uint8_t[]> waveFile;
			wchar_t fName[256];
			WAVData waveData;
			XAUDIO2_BUFFER buffer = { 0 };
			bool playing;

			enum Type
			{
				normal,
				loop,
			};

		public:
			//CXAudio2(const wchar_t* fileName, int type);
			CXAudio2() {}
			~CXAudio2()
			{
				Release();
			}

		public:
			static std::unique_ptr<CXAudio2> Create();

			/// <summary>
			/// wave�t�@�C���̃��[�h
			/// </summary>
			/// <param name="fileName"> wave�t�@�C���p�X </param>
			/// <param name="type"> �Đ��`�� : 0 -> �ʏ�Đ��A1 -> ���[�v�Đ� </param>
			void Load(const wchar_t* fileName, int type);
			void Release()
			{
				for (auto& it : lpSourceVoice)
				{
					it->DestroyVoice();
				}
				lpSourceVoice.clear();
			}

		public:
			bool PlayWAV(bool onCopy);		// �Đ�
			bool PauseWAV();				// �ꎞ��~
			bool StopWAV();					// ��~
			bool Update();					// �X�V
			bool Copy();					// ����
			bool SetVolume(float volume);	// �{�����[������
			bool SetPitch(float pitch);		// �s�b�`����
			Phoenix::f32 GetVolume();		// �{�����[���擾
			Phoenix::f32 GetPitch();		// �s�b�`�擾
		};

		// TODO : re -> Factor��
		class CXAudio2Origin
		{
		public:
			IXAudio2* lpXAudio = nullptr;
			IXAudio2MasteringVoice* lpMasterVoice = nullptr;

		private:
			CXAudio2Origin();

		public:
			~CXAudio2Origin()
			{
				Release();
			}

			void Release()
			{
				if (lpMasterVoice) lpMasterVoice->DestroyVoice();

				if (lpXAudio)
				{
					lpXAudio->Release();
					lpXAudio = nullptr;
				}

				CoUninitialize();
			}

			static CXAudio2Origin* GetInstance()
			{
				static CXAudio2Origin instance;
				return &instance;
			}
		};

		#define pCXAudio2Origin  (CXAudio2Origin::GetInstance())
	}
}