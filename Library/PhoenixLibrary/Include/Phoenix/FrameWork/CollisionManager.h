#pragma once
// TODO : Add HIT 
#include <vector>
#include "Phoenix/FND/NonCopyable.h"
//#include "Phoenix/HIT/Manager.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// 前方宣言
		class MeshCollider;
		class SphereCollider;

		//****************************************************************************
		// コリジョンマネージャー
		//****************************************************************************
		class CollisionManager final : public FND::NonCopyable
		{
		private:
			//std::unique_ptr<hit::IManager> manager;
			std::vector<SphereCollider*> sphereColliders;
			std::vector<MeshCollider*> meshColliders;

		public:
			// 初期化
			bool Initialize();

			// 終了化
			void Finalize();

			// スフィアコライダー登録
			void RegisterSphereCollider(SphereCollider* collider);

			// スフィアコライダー登録解除
			void UnregisterSphereCollider(SphereCollider* collider);

			// メッシュコライダー登録
			void RegisterMeshCollider(MeshCollider* collider);

			// メッシュコライダー登録解除
			void UnregisterMeshCollider(MeshCollider* collider);

			// コライダー登録解除
			void Clear();

			// スフィアコライダーリスト取得
			const std::vector<SphereCollider*>& GetSphereColliders() const { return sphereColliders; }

			// メッシュコライダーリスト取得
			const std::vector<MeshCollider*>& GetMeshColliders() const { return meshColliders; }

			// 更新
			void Update();
		};
	} // namespace FrameWork
} // namespace Phoenix