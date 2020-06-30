#pragma once

#include "Phoenix/FND/Flag.h"
#include "Phoenix/Math/color.h"
#include "Phoenix/Graphics/Camera.h"
#include "Phoenix/Graphics/Material.h"
#include "Phoenix/FrameWork/Component.h"
#include "Phoenix/FrameWork/RenderState.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// カメラクリアフラグ
		//****************************************************************************
		enum class CameraClearFlags
		{
			SkyBox,
			SolidColor,
			Depth,
			Nothing
		};

		//****************************************************************************
		// カメラタイプ
		//****************************************************************************
		enum class CameraType
		{
			Main,
			Sub
		};

		//****************************************************************************
		// ビューポート矩形
		//****************************************************************************
		struct ViewportRect
		{
			f32					x;
			f32					y;
			f32					w;
			f32					h;

			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};

		//****************************************************************************
		// カメラ
		//****************************************************************************
		class Camera : public Component
		{
		private:
			using super = Component;

			Graphics::Camera camera;
			Graphics::StandardMaterial material;
			CameraType cameraType = CameraType::Sub;
			CameraClearFlags clearFlags = CameraClearFlags::SolidColor;
			union
			{
				u32 cullingMaskFlags = CullingMaskFlag::Everything;
				CullingMaskFlag cullingMaskFlag;
			};
			f32 fov = 3.14f / 4.0f;
			f32 aspect = 16.0f / 9.0f;
			f32 nearZ = 0.1f;
			f32 farZ = 1000.0f;
			f32 depth = 0.0f;
			ViewportRect viewportRect = { 0.0f, 0.0f, 1.0f, 1.0f };
			Math::Color backgroundColor = Math::Color::Blue;

		public:
			Camera() {}
			~Camera() override {}

		public:
			// 名前取得
			const char* GetName() override { return "Camera"; }

			// 生成時に一度だけ呼ばれる
			void Awake(IGame* game) override;

			// 描画ノード収集
			void GetRenderables(IGame* game, RenderableCollection& collection, bool force) override;

			// 行列更新処理
			void UpdateTransform(IGame* game) override;

			// 開始処理
			void Start(IGame* game) override;

			// 破棄時に呼ばれる
			void OnDestroy(IGame* game) override;

			// シーンGUI表示
			void OnSceneGUI(IGame* game) override;

			// インスペクターGUI表示
			void OnInspectorGUI(IGame* game) override;

			// カメラタイプ取得
			CameraType GetCameraType() const { return cameraType; }

			// カメラタイプ設定
			void SetCameraType(CameraType type) { cameraType = type; }

			// クリアフラグ取得
			CameraClearFlags GetClearFlags() const { return clearFlags; }

			// カリングマスクフラグ取得
			CullingMaskFlag GetCullingMaskFlags() const { return cullingMaskFlags; }

			// 背景色取得
			const Math::Color& GetBackgroundColor() const { return backgroundColor; }

			// レンダリング順序取得
			f32 GetDepth() const { return depth; }

			// レンダリング順序設定
			void SetDepth(f32 depth) { this->depth = depth; }

			// ビューポート矩形
			const ViewportRect& GetViewportRect() const { return viewportRect; }

			// ビュー行列取得
			const Math::Matrix& GetView() const { return camera.GetView(); }

			// プロジェクション行列取得
			const Math::Matrix& GetProjection() const { return camera.GetProjection(); }

			// 視点位置取得
			const Math::Vector3& GetEye() const { return camera.GetEye(); }

			// 注視点位置取得
			const Math::Vector3& GetFocus() const { return camera.GetFocus(); }

			// 前ベクトル取得
			const Math::Vector3& GetFront() const { return camera.GetFront(); }

			// 上ベクトル取得
			const Math::Vector3& GetUp() const { return camera.GetUp(); }

			// 右ベクトル取得
			const Math::Vector3& GetRight() const { return camera.GetRight(); }

			// 視点から注視点の距離取得
			f32 GetDistanceFromLookAt() const { return camera.GetDistanceFromLookAt(); }

			// 視野角取得
			f32 GetFov() const { return camera.GetFovY(); }

			// 視野角設定
			void SetFov(f32 fov) { camera.SetFovY(fov); }

			// アスペクト比取得
			f32 GetAspect() const { return camera.GetAspect(); }

			// アスペクト比設定
			void SetAspect(f32 aspect) { camera.SetAspect(aspect); }

			// ニアクリップ値取得
			f32 GetNearZ() const { return camera.GetNearZ(); }

			// ニアクリップ値設定
			void SetNearZ(f32 near_z) { camera.SetNearZ(near_z); }

			// ファークリップ値取得
			f32 GetFarZ() const { return camera.GetFarZ(); }

			// ファークリップ値設定
			void SetFarZ(f32 far_z) { camera.SetFarZ(far_z); }

		public:
			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};
	} // namespace FrameWork
} // namespace Phoenix