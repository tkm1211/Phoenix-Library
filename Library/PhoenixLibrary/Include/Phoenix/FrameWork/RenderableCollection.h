#pragma once

#include <vector>
#include <map>
#include "Phoenix/Graphics/Renderable.h"
#include "Phoenix/Math/AABB.h"
#include "Phoenix/FrameWork/Object.h"
#include "Phoenix/FrameWork/RenderState.h"
#include "Phoenix/FrameWork/ShaderLibrary.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// 前方宣言
		class IGame;
		class GameObject;

		struct RenderableNode : public Graphics::RenderableNode
		{
			IObject* object = nullptr;
			GameObject* owner = nullptr;
			f32 distance = 0.0f;
		};


		// 描画ノードリスト
		using RenderableNodeList = std::vector<RenderableNode>;

		//****************************************************************************
		// 描画ノードコレクション
		//****************************************************************************
		class RenderableCollection : public FND::Base
		{
		protected:
			Math::AABB bounds;

		private:
			GlobalRenderFlag globalRenderFlags = 0;

		public:
			// 描画ノード追加
			virtual void Add(const RenderableNode& node, IGame* game, Graphics::RenderFlag renderFlags, ShaderID shaderID) = 0;

			// コレクションをクリア
			virtual void Clear() = 0;

			// バウンディングボックスの取得
			const Math::AABB& GetBounds() const { return bounds; }

			// バウンディングボックスのクリア
			void ClearBounds() { bounds.Reset(); }
		};

		//****************************************************************************
		// 描画ノードコレクション(ソートなし)
		//****************************************************************************
		class RenderableSet : public RenderableCollection
		{
		private:
			RenderableNodeList renderables;
			bool skipSelected = false;

		public:
			// 描画ノード追加
			void Add(const RenderableNode& node, IGame* game, Graphics::RenderFlag renderFlags, ShaderID shaderID) override;

			// コレクションをクリア
			void Clear() override;

			// 描画ノードリストを取得
			RenderableNodeList& GetNodeList() { return renderables; }

			// 選択されているノードは追加されないように設定
			void SetSkipSelected(bool skipSelected) { this->skipSelected = skipSelected; }
		};

		//****************************************************************************
		// 描画ノードコレクション(ソートあり)
		//****************************************************************************
		class RenderableSorter : public RenderableCollection
		{
		public:
			// 描画設定毎にまとめた描画ノードデータ
			struct Bucket
			{
				ShaderID shaderID;					// シェーダーID
				Graphics::RenderFlag renderFlags;	// 描画フラグ
				RenderableNodeList renderables;		// 描画ノードリスト
			};

		private:
			using BucketMap = std::map<u32, Bucket>;
			using BucketKeys = std::vector<u32>;

			BucketMap buckets;
			BucketKeys bucketKeys;

		public:
			// 描画ノード追加
			void Add(const RenderableNode& node, IGame* game, Graphics::RenderFlag renderFlags, ShaderID shaderID) override;

			// コレクションをクリア
			void Clear() override;

			// バケット数を取得
			s32 GetBucketCount() const;

			// バケットを取得
			Bucket& GetBucket(s32 index);

		private:
			Bucket& GetOrMakeBucket(Graphics::RenderFlag renderFlags, ShaderID shaderID);
		};
	} // namespace FrameWork
} // namespace Phoenix