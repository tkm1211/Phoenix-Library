#pragma once
// TODO : ADD Graphics::Shape
#include "Phoenix/FND/NonCopyable.h"
#include "Phoenix/Graphics/Device.h"
//#include "Phoenix/Graphics/Shape.h"
#include "Phoenix/Graphics/Mesh.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// シェイプタイプ
		//****************************************************************************
		enum class ShapeType
		{
			Sphere,				// 球
			Cube,				// 立方体
			Cylinder,			// 円柱
			Cone,				// 円錐
			Torus,				// 輪
			Bounds,				// バウンディングボックス
			Axis,				// 軸
			QuadLine,			// 四角形(ライン)
			SphereLine,			// 球(ライン)
			Camera,				// カメラ
			DirectionalLight,	// 平行光

			TypeNum
		};

		//****************************************************************************
		// シェイプライブラリ
		//****************************************************************************
		class ShapeLibrary : public FND::NonCopyable
		{
		private:
			//std::shared_ptr<Graphics::IShape> shapes[static_cast<int>(ShapeType::TypeNum)];

		public:
			ShapeLibrary() {}
			~ShapeLibrary() { Finalize(); }

			// 初期化
			bool Initialize(Graphics::IDevice* device);

			// 終了化
			void Finalize();

			// メッシュ取得
			//Graphics::IMesh* GetMesh(ShapeType shape_type) const { return shapes[static_cast<int>(shape_type)]->GetMesh(); }
		};

	}	// namespace fwk
}	// namespace ys