#pragma once

#include <vector>
#include "Phoenix/FND/Flag.h"
#include "Phoenix/Math/AABB.h"
#include "Phoenix/Math/Vector.h"
#include "Phoenix/Math/Matrix.h"
#include "Phoenix/Math/Quaternion.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// 前方宣言
		class IGame;

		//****************************************************************************
		// トランスフォームフラグ
		//****************************************************************************
		struct TransformationFlag : public FND::Flag32
		{
			static const u32 None = 0;				// なし
			static const u32 Translate = (1 << 0);	// 移動
			static const u32 Rotate = (1 << 1);		// 回転F
			static const u32 Scale = (1 << 2);		// スケール
			static const u32 All = 0xFFFFFFFF;

			FLAG32_CONSTRUCT(TransformationFlag)
		};

		//****************************************************************************
		// トランスフォーム操作インターフェース
		//****************************************************************************
		class ITransform
		{
		public:
			// スケール値を設定
			virtual void SetScale(const Math::Vector3& scale) = 0;

			// 回転値を設定
			virtual void SetRotate(const Math::Quaternion& rotate) = 0;

			// 移動値を設定
			virtual void SetTranslate(const Math::Vector3& translate) = 0;

			// ピボット値を設定
			//virtual void SetPivot(const math::Vector3& pivot) = 0;

			// スケール値を取得
			virtual const Math::Vector3& GetScale() = 0;

			// 回転値を取得
			virtual const Math::Quaternion& GetRotate() = 0;

			// 移動値を取得
			virtual const Math::Vector3& GetTranslate() = 0;

			// ローカルトランスフォームを取得
			virtual const Math::Matrix& GetLocalTransform() = 0;

			// ワールドトランスフォームを取得
			virtual const Math::Matrix& GetWorldTransform() = 0;

			// トランスフォームフラグを取得
			virtual TransformationFlag GetTransformationFlag() = 0;
		};

		//****************************************************************************
		// オブジェクト操作インターフェース
		//****************************************************************************
		class IObject : public ITransform
		{
		public:
			// 名前設定
			virtual void SetName(const char* name) = 0;

			// 名前取得
			virtual const char* GetName() = 0;

			// アクティブ状態を設定
			virtual void SetActive(bool active) = 0;

			// アクティブ状態を取得
			virtual bool IsActive() = 0;

			// 親ゲームオブジェクトを取得
			virtual IObject* GetParent() = 0;

			// 子ゲームオブジェクト数取得
			virtual const std::vector<IObject*>& GetChildren() = 0;

			// 子を追加できるか判定
			virtual bool CanAddChild(IObject* child) = 0;

			// 子を追加
			virtual bool AddChild(IObject* child) = 0;

			// バウンディングボックスを取得
			virtual const Math::AABB& GetBounds() = 0;
		};

	}	// namespace fwk
}	// namespace ys
