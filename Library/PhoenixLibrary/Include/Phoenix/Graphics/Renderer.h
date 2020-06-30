#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"
#include "Phoenix/FND/Flag.h"
#include "Phoenix/Graphics/Context.h"
#include "Phoenix/Graphics/Mesh.h"
#include "Phoenix/Graphics/Renderable.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/OS/Display.h"
#include "Phoenix/Math/Color.h"
#include "Phoenix/Math/Vector.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// ラインレンダラ操作インターフェース
		//****************************************************************************
		class ILineRenderer : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<ILineRenderer> Create();

			// 初期化
			virtual bool Initialize(IDevice* device, u32 vertex_count = 1024) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// 頂点追加
			virtual void AddVertex(const Math::Vector3& position, const Math::Color& color = Math::Color::White) = 0;

			// 描画
			virtual void Draw(IContext* context, RenderFlag flags, PrimitiveTopology primitive_topology) = 0;
		};

		//============================================================================
		// シャドウマップレンダラ操作インターフェース
		//----------------------------------------------------------------------------
		class IShadowMapRenderer : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IShadowMapRenderer> Create();

			// 初期化
			virtual bool Initialize(IDevice* device) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// 開始
			virtual void Begin(IContext* context, const Math::AABB& rendered_area) = 0;

			// 描画フラグ設定
			virtual void SetRenderFlag(RenderFlag flags) = 0;

			// 描画
			virtual void Draw(const RenderableNode& node) = 0;

			// 終了
			virtual void End() = 0;
		};

	}	// namespace grp
}	// namespace ys