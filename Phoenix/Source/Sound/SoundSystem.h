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
	// 生成
	static std::shared_ptr<Sound> Create()
	{
		return std::make_shared<Sound>();
	}

	// コンストラクタ
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
	{
	}

	// 初期化
	void Initialize()
	{
		sound = Phoenix::FrameWork::CXAudio2::Create();

		fadeIn = false;
		fadeOut = false;

		maxVolume = 1.0f;
	}

	// 終了化
	void Finalize()
	{
		sound.reset();
	}

	// 更新
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
	/// 音の読み込み
	/// </summary>
	/// <param name="fileName"> ファイルパス </param>
	/// <param name="playType"> 再生形態 : 0 -> 通常再生、1 -> ループ再生 </param>
	void Load(const wchar_t* fileName, Phoenix::s32 playType)
	{
		sound->Load(fileName, playType);
	}

	// 指定の音を再生
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

	// 指定の音を一時停止
	void Pause()
	{
		sound->PauseWAV();
	}

	// 指定の音を停止
	void Stop(bool fade)
	{
		if (!fade)
		{
			sound->StopWAV();
		}
		fadeOut = fade;
	}

	/// <summary>
	/// 音量調整
	/// </summary>
	/// <param name="volume"> 0.0 ~ 1.0 </param>
	void SetVolume(Phoenix::f32 volume)
	{
		sound->SetVolume(volume);
		maxVolume = volume;
	}

	/// <summary>
	/// ピッチ調整
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
	// 生成
	static std::shared_ptr<SoundSystem> Create()
	{
		return std::make_shared<SoundSystem>();
	}

	// コンストラクタ
	void Construct(Phoenix::Graphics::IGraphicsDevice* graphicsDevice)
	{
		//sounds.clear();
	}

	// 初期化
	void Initialize()
	{
		/*for (const auto& [key, value] : sounds)
		{
			value->StopWAV();
		}*/
	}

	// 終了化
	void Finalize()
	{
		sounds.clear();
	}

	// 更新
	void Update()
	{
		for (const auto& [key, value] : sounds)
		{
			value->Update();
		}
	}

	/// <summary>
	/// 音の追加
	/// </summary>
	/// <param name="type"> 種類 </param>
	/// <param name="fileName"> ファイルパス </param>
	/// <param name="playType"> 再生形態 : 0 -> 通常再生、1 -> ループ再生 </param>
	void AddSound(T type, const wchar_t* fileName, Phoenix::s32 playType)
	{
		sounds.insert(std::make_pair(type, Sound::Create()));
		sounds.at(type)->Initialize();
		sounds.at(type)->Load(fileName, playType);
	}

	/// <summary>
	/// 指定の音を再生
	/// </summary>
	/// <param name="type"> 種類 </param>
	/// <param name="fade"> フェードイン </param>
	/// <param name="onCopy"> コピー許可 </param>
	void Play(T type, bool fade = false, bool onCopy = false)
	{
		sounds.at(type)->Play(fade, onCopy);
	}

	/// <summary>
	/// 指定の音を一時停止
	/// </summary>
	/// <param name="type"> 種類 </param>
	void Pause(T type)
	{
		sounds.at(type)->Pause();
	}

	/// <summary>
	/// 指定の音を停止
	/// </summary>
	/// <param name="type"> 種類 </param>
	/// /// <param name="fade"> フェードアウト </param>
	void Stop(T type, bool fade = false)
	{
		sounds.at(type)->Stop(fade);
	}

	/// <summary>
	/// 音量調整
	/// </summary>
	/// <param name="volume"> 0.0 ~ 1.0 </param>
	void SetVolume(T type, Phoenix::f32 volume)
	{
		sounds.at(type)->SetVolume(volume);
	}

	// すべての音を止める
	void AllSoundStop()
	{
		for (const auto& [key, value] : sounds)
		{
			value->Stop();
		}
	}
};