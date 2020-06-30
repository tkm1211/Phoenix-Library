#pragma once

#include <set>
#include <vector>
#include "Phoenix/Math/Vector.h"
#include "Phoenix/FrameWork/Object.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// 前方宣言
		class IViewingEventListener;

		//****************************************************************************
		// ビュー操作インターフェース
		//****************************************************************************
		class IViewingContext
		{
		public:
			// 現在のビューで指定のオブジェクトをフレーム内におさめれるか
			virtual bool CanFrame(const std::vector<IObject*>& items) = 0;

			// 現在のビューに指定のオブジェクトをフレーム内におさめる
			virtual void Frame(const std::vector<IObject*>& items) = 0;

			// ビュー操作イベントリスナー登録
			virtual void RegisterEventListener(IViewingEventListener* listener) = 0;

			// ビュー操作イベントリスナー登録解除
			virtual void UnregisterEventListener(IViewingEventListener* listener) = 0;
		};

		//****************************************************************************
		// ビュー操作イベントリスナー
		//****************************************************************************
		class IViewingEventListener
		{
		public:
			// 指定のオブジェクトをフレーム内におさめる時に呼ばれる
			virtual void OnFrame(Math::Vector3& center, f32 radius) = 0;
		};

	} // namespace FrameWork
} // namespace Phoenix