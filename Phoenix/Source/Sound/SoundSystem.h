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
		sounds.insert(std::make_pair(type, Phoenix::FrameWork::CXAudio2::Create()));
		sounds.at(type)->Load(fileName, playType);
	}

	/// <summary>
	/// 指定の音を再生
	/// </summary>
	/// <param name="type"> 種類 </param>
	void Play(T type)
	{
		sounds.at(type)->PlayWAV();
	}

	/// <summary>
	/// 指定の音を一時停止
	/// </summary>
	/// <param name="type"> 種類 </param>
	void Pause(T type)
	{
		sounds.at(type)->PauseWAV();
	}

	/// <summary>
	/// 指定の音を停止
	/// </summary>
	/// <param name="type"> 種類 </param>
	void Stop(T type)
	{
		sounds.at(type)->StopWAV();
	}

	// すべての音を止める
	void AllSoundStop()
	{
		for (const auto& [key, value] : sounds)
		{
			value->StopWAV();
		}
	}
};