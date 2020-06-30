#pragma once

#include <string>
#include "Phoenix/Graphics/Material.h"


namespace Phoenix
{
	namespace FrameWork
	{
		using ShaderID = s32;

		// 前方宣言
		class IGame;

		//****************************************************************************
		// マテリアル操作インターフェース
		//****************************************************************************
		class IMaterial
		{
		public:
			// マテリアル取得
			virtual Graphics::Material* GetMaterial() = 0;

			// 半透明か
			virtual bool HasTransparency() = 0;

			// セットアップ
			virtual void Setup(IGame* game) = 0;

			// インスペクターGUI表示
			virtual void OnInspectorGUI(IGame* game) = 0;
		};

		//****************************************************************************
		// マテリアル情報
		//****************************************************************************
		struct MaterialInfo
		{
			ShaderID shaderID = 0;
			std::string name;
			std::shared_ptr<IMaterial> material;

			// インスペクターGUI表示
			void OnInspectorGUI(IGame* game);

			template<class Archive>
			void serialize(Archive& archive, u32 version);

		private:
			bool open = false;
		};

		//****************************************************************************
		// ベーシックマテリアル
		//****************************************************************************
		class BasicMaterial : public IMaterial
		{
		private:
			Graphics::BasicMaterial	material;

		public:
			// マテリアル取得
			Graphics::Material* GetMaterial() override { return &material; }

			// 半透明か
			bool HasTransparency() override { return false; }

			// セットアップ
			void Setup(IGame* game) override {}

			// インスペクターGUI表示
			void OnInspectorGUI(IGame* game) override;

			// カラー設定
			void SetColor(const Math::Color& color) { material.color = color; }

			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};

		//****************************************************************************
		// スタンダードマテリアル
		//****************************************************************************
		class StandardMaterial : public IMaterial
		{
		private:
			Graphics::StandardMaterial material;
			std::string diffusemapFilename;
			std::string normalmapFilename;
			std::string specularmapFilename;
			std::shared_ptr<Graphics::ITextureResource>	diffusemapResource;
			std::shared_ptr<Graphics::ITextureResource>	normalmapResource;
			std::shared_ptr<Graphics::ITextureResource>	specularmapResource;

		public:
			// マテリアル取得
			Graphics::Material* GetMaterial() override { return &material; }

			// 半透明か
			bool HasTransparency() override { return false; }

			// セットアップ
			void Setup(IGame* game) override;

			// インスペクターGUI表示
			void OnInspectorGUI(IGame* game) override;

			// エミッシブカラー設定
			void SetEmissiveColor(const Math::Color& color) { material.emissive = color; }

			// ディフューズカラー設定
			void SetDiffuseColor(const Math::Color& color) { material.diffuse = color; }

			// スペキュラカラー設定
			void SetSpecularColor(const Math::Color& color) { material.specular = color; }

			// ディフューズマップテクスチャ読み込み
			void LoadDiffuseMap(IGame* game, const char* filename);

			// 法線マップテクスチャ読み込み
			void LoadNormalMap(IGame* game, const char* filename);

			// スペキュラマップテクスチャ読み込み
			void LoadSpecularMap(IGame* game, const char* filename);

		public:
			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};

		//****************************************************************************
		// スカイドームマテリアル
		//****************************************************************************
		class SkyDomeMaterial : public IMaterial
		{
		private:
			Graphics::SkyDomeMaterial material;
			std::string cubemapFilename;
			std::shared_ptr<Graphics::ITextureResource>	textureResource;

		public:
			// マテリアル取得
			Graphics::Material* GetMaterial() override { return &material; }

			// 半透明か
			bool HasTransparency() override { return false; }

			// セットアップ
			void Setup(IGame* game) override;

			// インスペクターGUI表示
			void OnInspectorGUI(IGame* game) override;

		private:
			// スカイドームテクスチャ読み込み
			void LoadCubeMap(IGame* game, const char* filename);

		public:
			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};
	} // namespace FrameWork
} // namespace Phoenix