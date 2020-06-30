#pragma once

#include "Phoenix/FND/NonCopyable.h"
#include "Phoenix/FND/Flag.h"
#include "Phoenix/Math/Color.h"
#include "Phoenix/Graphics/Material.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// 表示フラグモード
		//****************************************************************************
		enum class DisplayFlagModes
		{
			None,		// 表示しない
			Always,		// 常に表示
			Selection,	// 選択されたオブジェクトのみ表示
		};

		//****************************************************************************
		// レンダービュー
		//****************************************************************************
		enum class RenderView
		{
			Scene,
			Game
		};

		//****************************************************************************
		// グローバル描画切り替えを判定するフラグ
		//****************************************************************************
		struct GlobalRenderFlag : public FND::Flag32
		{
			static const u32 None = 0;						// なし
			static const u32 Textured = (1 << 0);			// テクスチャ表示のON/OFF
			static const u32 Lit = (1 << 1);				// ライティング処理のON/OFF
			static const u32 RenderBackFace = (1 << 2);		// 背面表示のON/OFF
			static const u32 Solid = (1 << 3);				// ソリッド表示のON/OFF
			static const u32 WireFrame = (1 << 4);			// ワイヤーフレーム表示のON/OFF
			static const u32 Shadows = (1 << 5);			// 影表示のON/OFF
			static const u32 RenderNormals = (1 << 6);		// 法線表示のON/OFF
			static const u32 Collision = (1 << 7);			// コリジョン表示のON/OFF
			static const u32 All = 0xFFFFFFFF;				// リソースをマップ可能にし、CPU がそのリソースの内容を読み取れるようにします。

			FLAG32_CONSTRUCT(GlobalRenderFlag)
		};

		//****************************************************************************
		// カリングマスクフラグ
		//****************************************************************************
		struct CullingMaskFlag : public FND::Flag32
		{
			static const u32 Nothing = 0;
			static const u32 Everything = 0xFFFFFFFF;

			FLAG32_CONSTRUCT(CullingMaskFlag)
		};

		//****************************************************************************
		// レンダーステート操作オブジェクト
		//****************************************************************************
		class RenderState final : public FND::NonCopyable
		{
		private:
			union
			{
				u32 globalRenderFlags
					= GlobalRenderFlag::Solid
					| GlobalRenderFlag::Textured
					| GlobalRenderFlag::Lit
					| GlobalRenderFlag::Shadows;
				GlobalRenderFlag globalRenderFlag;
			};
			union
			{
				u32 cullingMaskFlags = CullingMaskFlag::Everything;
				CullingMaskFlag cullingMaskFlag;
			};

			RenderView renderView = RenderView::Scene;

			Graphics::BasicMaterial selectionMaterial;
			Graphics::BasicMaterial wireframeMaterial;
			Graphics::BasicMaterial collisionMaterial;
			Graphics::BasicMaterial defaultMaterial;

			DisplayFlagModes displayBounds = DisplayFlagModes::Selection;
			DisplayFlagModes displayBones = DisplayFlagModes::Selection;

		public:
			RenderState()
			{
				SetSelectionColor(Math::Color(0.25f, 1.00f, 0.63f, 1.00f));
				SetWireframeColor(Math::Color(0.00f, 0.00f, 0.55f, 1.00f));
				SetCollisionColor(Math::Color(0.70f, 0.00f, 0.00f, 1.00f));
			}

			// グローバル描画切り替えフラグ取得
			GlobalRenderFlag GetGlobalRenderFlags() const { return globalRenderFlags; }

			// グローバル描画切り替えフラグを設定
			void SetGlobalRenderFlags(GlobalRenderFlag flags) { globalRenderFlags = flags; }

			// カリングマスクフラグ取得
			CullingMaskFlag GetCullingMaskFlags() const { return cullingMaskFlags; }

			// カリングマスクフラグを設定
			void SetCullingMaskFlags(CullingMaskFlag flags) { cullingMaskFlags = flags; }

			// レンダービュー取得
			RenderView GetRenderView() const { return renderView; }

			// レンダービュー設定
			void SetRenderView(RenderView render_view) { render_view = render_view; }

			// 選択色を設定
			void SetSelectionColor(const Math::Color& color) { selectionMaterial.color = color; }

			// ワイヤーフレーム色を設定
			void SetWireframeColor(const Math::Color& color) { wireframeMaterial.color = color; }

			// コリジョン色を設定
			void SetCollisionColor(const Math::Color& color) { collisionMaterial.color = color; }

			// 選択マテリアルを取得
			const Graphics::BasicMaterial* GetSelectionMaterial() const { return &selectionMaterial; }

			// ワイヤーフレームマテリアルを取得
			const Graphics::BasicMaterial* GetWireframeMaterial() const { return &wireframeMaterial; }

			// コリジョンマテリアルを取得
			const Graphics::BasicMaterial* GetCollisionMaterial() const { return &collisionMaterial; }

			// デフォルトマテリアルを取得
			const Graphics::BasicMaterial* GetDefaultMaterial() const { return &defaultMaterial; }

			// バウンディングボックス表示モード取得
			DisplayFlagModes GetDisplayBounds() const { return displayBounds; }

			// バウンディングボックス表示モード設定
			void SetDisplayBounds(DisplayFlagModes mode) { displayBounds = mode; }

			// ボーン表示モード取得
			DisplayFlagModes GetDisplayBones() const { return displayBones; }

			// ボーン表示モード設定
			void SetDisplayBones(DisplayFlagModes mode) { displayBones = mode; }

		public:
			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};
	} // namespace FrameWork
} // namespace Phoenix