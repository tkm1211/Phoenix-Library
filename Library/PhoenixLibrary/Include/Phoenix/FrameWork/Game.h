#pragma once

#include "Phoenix/OS/Resource.h"
#include "Phoenix/OS/Display.h"
#include "Phoenix/Graphics/Context.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/Graphics/Renderer.h"
#include "Phoenix/Math/Matrix.h"
#include "Phoenix/Math/Vector.h"
#include "Phoenix/FrameWork/Context/GameContext.h"
#include "Phoenix/FrameWork/CameraRegistry.h"
#include "Phoenix/FrameWork/CollisionManager.h"
#include "Phoenix/FrameWork/frameTime.h"
#include "Phoenix/FrameWork/GameObjectArchive.h"
#include "Phoenix/FrameWork/Input.h"
#include "Phoenix/FrameWork/Object.h"
#include "Phoenix/FrameWork/RenderState.h"
#include "Phoenix/FrameWork/RenderSurface.h"
#include "Phoenix/FrameWork/ShaderLibrary.h"
#include "Phoenix/FrameWork/ShapeLibrary.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// 前方宣言
		class GameObject;

		//****************************************************************************
		// ヒット記録
		//****************************************************************************
		struct HitRecord
		{
			IObject* object;
			GameObject* owner;
			u32 index;
			f32 distance;
			Math::Vector3 hitPt;
			Math::Vector3 normal;
			Math::Vector3 nearestVertex;
			bool hasNormal;
			bool hasNearestVertex;
		};

		//****************************************************************************
		// ゲーム操作インターフェース
		//****************************************************************************
		class IGame
		{
		public:
			// ゲームコンテキスト取得
			virtual std::shared_ptr<IGameContext> GetGameContext() = 0;

			// ディスプレイ取得
			virtual OS::IDisplay* GetDisplay() = 0;

			// リソースマネージャー取得
			virtual OS::IResourceManager* GetResourceManager() = 0;

			// グラフィックスデバイス取得
			virtual Graphics::IDevice* GetGraphicsDevice() = 0;

			// グラフィックスコンテキスト取得
			virtual Graphics::IContext* GetGraphicsContext() = 0;

			// ラインレンダラ取得
			virtual Graphics::ILineRenderer* GetLineRenderer() = 0;

			// 描画ステート取得
			virtual RenderState* GetRenderState() = 0;

			// レンダーサーフェイス取得
			virtual RenderSurface* GetRenderSurface() = 0;

			// カメラレジストリ取得
			virtual CameraRegistry* GetCameraRegistry() = 0;

			// コリジョンマネージャー取得
			virtual CollisionManager* GetCollisionManager() = 0;

			// インプット取得
			virtual Input* GetInput() = 0;

			// シェーダーライブラリ取得
			virtual ShaderLibrary* GetShaderLibrary() = 0;

			// シェイプライブラリ取得
			virtual ShapeLibrary* GetShapeLibrary() = 0;

			// フレームタイム取得
			virtual const FrameTime& GetFrameTime() = 0;

			// 更新タイプ取得
			virtual UpdateType GetUpdateType() = 0;

			// シリアライズ
			virtual void Serialize(std::shared_ptr<GameObject> gameObject, std::ostream& stream, GameObjectArchive archive) = 0;

			// デシリアライズ
			virtual std::shared_ptr<GameObject> Deserialize(std::istream& stream, GameObjectArchive archive) = 0;
		};
	} // namespace FrameWork
} // namespace Phoenix