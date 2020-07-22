#pragma once

#include <memory>
#include <vector>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/OS/Resource.h"
#include "Phoenix/Math/Color.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// テクスチャ次元タイプ
		//****************************************************************************
		enum class TextureDimension
		{
			Tex1D,		// 1Dテクスチャ
			Tex2D,		// 2Dテクスチャ
			Tex3D,		// 3Dテクスチャ
			Cube,		// キューブテクスチャ
		};

		//****************************************************************************
		// テクスチャフォーマット
		//****************************************************************************
		enum class TextureFormat
		{
			R8G8B8A8,		//!< 32bit R8G8B8A8
			B8G8R8A8,		//!< 32bit B8G8R8A8
			B4G4R4A4,		//!< 16bit B4G4R4A4
			B5G5R5A1,		//!< 16bit B5G5R5A1
			B5G6R5,			//!< 16bit B5G6R5
			A8,				//!< 8bit  A8
			R8,				//!< 8bit  R8
			BC1,			//!< 4bit  RGB
			BC2,			//!< 8bit  RGBA
			BC3,			//!< 8bit  RGBA

			UNKOWN			//!< プラットフォーム固有
		};

		//****************************************************************************
		// テクスチャ設定記述
		//****************************************************************************
		struct TextureDesc
		{
			u32 width;						// 幅。
			u32 height;						// 高さ(1Dテクスチャの場合は1)
			u32 depth;						// 深さ(1D,2Dテクスチャの場合は1)
			u32 arraySize;					// 配列数
			u32 mipLevels;					// ミップマップレベルの最大数(0の場合は最大になる)
			TextureDimension dimension;		// テクスチャ次元タイプ
			TextureFormat format;			// テクスチャフォーマット
			const void* sysMem;				// 初期化データへのポインタ
			u32	 sysMemSize;				// 初期化データサイズ
		};

		//****************************************************************************
		// マテリアル種類
		//****************************************************************************
		enum class MaterialType
		{
			Diffuse,
			Ambient,
			Specular,
			NormalMap,
			Bump,
			Emissive,
		};

		//****************************************************************************
		// テクスチャ操作インターフェース
		//****************************************************************************
		class ITexture : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<ITexture> Create();

			// 初期化
			virtual bool Initialize(IDevice* device, const TextureDesc& desc) = 0;

			// 初期化
			virtual bool Initialize(IDevice* device, const char* filename, MaterialType materialType, const Math::Color& color) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// ハンドル取得
			virtual void* Handle() = 0;
		};

		//****************************************************************************
		// テクスチャリソース操作インターフェース
		//****************************************************************************
		class ITextureResource : public OS::Resource
		{
		public:
			// テクスチャ取得
			virtual Graphics::ITexture* GetTexture() = 0;

		};

		//****************************************************************************
		// テクスチャリソース操作オブジェクト生成インターフェース
		//****************************************************************************
		class ITextureResourceFactory : public OS::IResourceFactory
		{
		public:
			// 生成
			static std::unique_ptr<ITextureResourceFactory> Create(Graphics::IDevice* device);
		};

		//****************************************************************************
		// STBテクスチャリソース操作オブジェクト生成インターフェース
		//****************************************************************************
		class IStbTextureResourceFactory : public OS::IResourceFactory
		{
		public:
			// 生成
			static std::unique_ptr<IStbTextureResourceFactory> Create(Graphics::IDevice* device);
		};

		//****************************************************************************
		// DDSテクスチャリソース操作オブジェクト生成インターフェース
		//****************************************************************************
		class IDdsTextureResourceFactory : public OS::IResourceFactory
		{
		public:
			// 生成
			static std::unique_ptr<IDdsTextureResourceFactory> Create(Graphics::IDevice* device);
		};

		//****************************************************************************
		// テクスチャデータ
		//****************************************************************************
		struct TextureData
		{
			u32 width;
			u32 height;
			u32 depth;
			u32 arraySize;
			u32 mipLevels;
			Graphics::TextureFormat format;
			Graphics::TextureDimension dimension;

			std::vector<u8> pixels;
		};
	} // namespace Graphics
} // namespace Phoenix