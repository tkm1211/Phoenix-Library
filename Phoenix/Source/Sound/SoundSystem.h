#pragma once

#include <map>
#include <memory>
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/FrameWork/Audio/Audio.h"


enum class SoundType
{
	BGM_Title,
	BGM_Game,

	SE_Player_Walk,
};

template<class T>
class SoundSystem
{
private:
	std::unordered_map<T, std::shared_ptr<Phoenix::FrameWork::CXAudio2>> sounds;

public:
	SoundSystem() {}
	~SoundSystem() { Finalize(); }

public:
	// ����
	static std::shared_ptr<SoundSystem> Create()
	{
		return std::make_shared<SoundSystem>();
	}

	// �R���X�g���N�^
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
	{
		//sounds.clear();
	}

	// ������
	void Initialize()
	{
		/*for (const auto& [key, value] : sounds)
		{
			value->StopWAV();
		}*/
	}

	// �I����
	void Finalize()
	{
		sounds.clear();
	}

	// �X�V
	void Update()
	{
		for (const auto& [key, value] : sounds)
		{
			value->Update();
		}
	}

	/// <summary>
	/// ���̒ǉ�
	/// </summary>
	/// <param name="type"> ��� </param>
	/// <param name="fileName"> �t�@�C���p�X </param>
	/// <param name="playType"> �Đ��`�� : 0 -> �ʏ�Đ��A1 -> ���[�v�Đ� </param>
	void AddSound(T type, const wchar_t* fileName, Phoenix::s32 playType)
	{
		sounds.insert(std::make_pair(type, Phoenix::FrameWork::CXAudio2::Create()));
		sounds.at(type)->Load(fileName, playType);
	}

	/// <summary>
	/// �w��̉����Đ�
	/// </summary>
	/// <param name="type"> ��� </param>
	void Play(T type)
	{
		sounds.at(type)->PlayWAV();
	}

	/// <summary>
	/// �w��̉����ꎞ��~
	/// </summary>
	/// <param name="type"> ��� </param>
	void Pause(T type)
	{
		sounds.at(type)->PauseWAV();
	}

	/// <summary>
	/// �w��̉����~
	/// </summary>
	/// <param name="type"> ��� </param>
	void Stop(T type)
	{
		sounds.at(type)->StopWAV();
	}

	// ���ׂẲ����~�߂�
	void AllSoundStop()
	{
		for (const auto& [key, value] : sounds)
		{
			value->StopWAV();
		}
	}
};