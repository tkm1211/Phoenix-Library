#pragma once

#include "Phoenix/FND/Base.h"
#include "Phoenix/FrameWork/RenderableCollection.h"
#include "Phoenix/FrameWork/Object.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// 先行宣言
		class IGame;
		class GameObject;

		//****************************************************************************
		// コンポーネント
		//****************************************************************************
		class Component : public FND::Base
		{
		private:
			friend class GameObject;

			std::weak_ptr<GameObject> owner;
			bool enable = true;
			bool isStarted = false;

		public:
			Component() {}
			~Component() override {}

			// 名前取得
			virtual const char* GetName() = 0;

			// 子ゲームオブジェクト数取得
			virtual void  GetChildren(std::vector<IObject*>& objects) {}

			// 生成時に一度だけ呼ばれる
			virtual void Awake(IGame* game) {}

			// 描画ノードを収集
			virtual void GetRenderables(IGame* game, RenderableCollection& collection, bool force) {}

			// 行列更新処理
			virtual void UpdateTransform(IGame* game) {}

			// 開始処理
			virtual void Start(IGame* game) {}

			// フレーム毎の更新処理
			virtual void Update(IGame* game) {}

			// 有効状態を設定
			void SetEnable(bool enable) { this->enable = enable; }

			// 有効状態を取得
			bool IsEnable() const { return enable; }

			// オーナーを取得
			std::shared_ptr<GameObject> GetOwner() { return owner.lock(); }

			// 破棄時に呼ばれる
			virtual void OnDestroy(IGame* game) {}

			// シーンGUI表示
			virtual void OnSceneGUI(IGame* game) {}

			// インスペクターGUI表示
			//virtual void OnInspectorGUI(IGame* game);

		protected:
			// 表示判定
			bool IsVisible(IGame* game);

		private:
			// オーナー設定
			void SetOwner(std::shared_ptr<GameObject> gameObject) { owner = gameObject; }

			// スタート設定
			bool IsStarted() const { return isStarted; }
			void SetStart() { isStarted = true; }

		public:
			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};
	} // namespace FrameWork
} // namespace Phoenix