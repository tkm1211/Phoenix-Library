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
	SE_Player_Attack_Punch_Swing,
	SE_Player_Attack_Punch_Hit_Right,
	SE_Player_Attack_Punch_Hit_Heavy,
	SE_Player_Attack_Kick_Swing,
	SE_Player_Attack_Kick_Hit_Right,
	SE_Player_Attack_Kick_Hit_Heavy,
	SE_Player_Dedge,
};

class Sound
{
private:
	std::shared_ptr<Phoenix::FrameWork::CXAudio2> sound;

	bool fadeIn = false;
	bool fadeOut = false;

	Phoenix::f32 maxVolume = 1.0f;
	Phoenix::f32 fadeVolume = 0.01f;

public:
	Sound() {}
	~Sound() { Finalize(); }

public:
	// ����
	static std::shared_ptr<Sound> Create()
	{
		return std::make_shared<Sound>();
	}

	// �R���X�g���N�^
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
	{
	}

	// ������
	void Initialize()
	{
		sound = Phoenix::FrameWork::CXAudio2::Create();

		fadeIn = false;
		fadeOut = false;

		maxVolume = 1.0f;
	}

	// �I����
	void Finalize()
	{
		sound.reset();
	}

	// �X�V
	void Update()
	{
		if (fadeIn)
		{
			Phoenix::f32 volume = sound->GetVolume() + (fadeVolume * maxVolume);
			sound->SetVolume(volume);

			if (maxVolume <= volume)
			{
				fadeIn = false;
			}
		}
		else if (fadeOut)
		{
			Phoenix::f32 volume = sound->GetVolume() - (fadeVolume * maxVolume);
			sound->SetVolume(volume);

			if (volume <= 0.0f)
			{
				fadeOut = false;
				sound->StopWAV();
			}
		}
		sound->Update();
	}

	/// <summary>
	/// ���̓ǂݍ���
	/// </summary>
	/// <param name="fileName"> �t�@�C���p�X </param>
	/// <param name="playType"> �Đ��`�� : 0 -> �ʏ�Đ��A1 -> ���[�v�Đ� </param>
	void Load(const wchar_t* fileName, Phoenix::s32 playType)
	{
		sound->Load(fileName, playType);
	}

	// �w��̉����Đ�
	void Play(bool fade, bool onCopy)
	{
		sound->PlayWAV(onCopy);
		fadeIn = fade;
		if (fadeIn)
		{
			sound->SetVolume(0.0f);
		}
		else
		{
			sound->SetVolume(maxVolume);
		}
	}

	// �w��̉����ꎞ��~
	void Pause()
	{
		sound->PauseWAV();
	}

	// �w��̉����~
	void Stop(bool fade)
	{
		if (!fade)
		{
			sound->StopWAV();
		}
		fadeOut = fade;
	}

	/// <summary>
	/// ���ʒ���
	/// </summary>
	/// <param name="volume"> 0.0 ~ 1.0 </param>
	void SetVolume(Phoenix::f32 volume)
	{
		sound->SetVolume(volume);
		maxVolume = volume;
	}

	/// <summary>
	/// �s�b�`����
	/// </summary>
	/// <param name="pitch"> 0.0 ~ 2.0 </param>
	void SetPicht(Phoenix::f32 pitch)
	{
		sound->SetPitch(pitch);
	}
};

template<class T>
class SoundSystem
{
private:
	std::unordered_map<T, std::shared_ptr<Sound>> sounds;

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
		sounds.insert(std::make_pair(type, Sound::Create()));
		sounds.at(type)->Initialize();
		sounds.at(type)->Load(fileName, playType);
	}

	/// <summary>
	/// �w��̉����Đ�
	/// </summary>
	/// <param name="type"> ��� </param>
	/// <param name="fade"> �t�F�[�h�C�� </param>
	/// <param name="onCopy"> �R�s�[���� </param>
	void Play(T type, bool fade = false, bool onCopy = false)
	{
		sounds.at(type)->Play(fade, onCopy);
	}

	/// <summary>
	/// �w��̉����ꎞ��~
	/// </summary>
	/// <param name="type"> ��� </param>
	void Pause(T type)
	{
		sounds.at(type)->Pause();
	}

	/// <summary>
	/// �w��̉����~
	/// </summary>
	/// <param name="type"> ��� </param>
	/// /// <param name="fade"> �t�F�[�h�A�E�g </param>
	void Stop(T type, bool fade = false)
	{
		sounds.at(type)->Stop(fade);
	}

	/// <summary>
	/// ���ʒ���
	/// </summary>
	/// <param name="volume"> 0.0 ~ 1.0 </param>
	void SetVolume(T type, Phoenix::f32 volume)
	{
		sounds.at(type)->SetVolume(volume);
	}

	// ���ׂẲ����~�߂�
	void AllSoundStop()
	{
		for (const auto& [key, value] : sounds)
		{
			value->Stop();
		}
	}
};