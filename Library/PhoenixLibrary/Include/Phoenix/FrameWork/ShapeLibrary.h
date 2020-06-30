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
		// �V�F�C�v�^�C�v
		//****************************************************************************
		enum class ShapeType
		{
			Sphere,				// ��
			Cube,				// ������
			Cylinder,			// �~��
			Cone,				// �~��
			Torus,				// ��
			Bounds,				// �o�E���f�B���O�{�b�N�X
			Axis,				// ��
			QuadLine,			// �l�p�`(���C��)
			SphereLine,			// ��(���C��)
			Camera,				// �J����
			DirectionalLight,	// ���s��

			TypeNum
		};

		//****************************************************************************
		// �V�F�C�v���C�u����
		//****************************************************************************
		class ShapeLibrary : public FND::NonCopyable
		{
		private:
			//std::shared_ptr<Graphics::IShape> shapes[static_cast<int>(ShapeType::TypeNum)];

		public:
			ShapeLibrary() {}
			~ShapeLibrary() { Finalize(); }

			// ������
			bool Initialize(Graphics::IDevice* device);

			// �I����
			void Finalize();

			// ���b�V���擾
			//Graphics::IMesh* GetMesh(ShapeType shape_type) const { return shapes[static_cast<int>(shape_type)]->GetMesh(); }
		};

	}	// namespace fwk
}	// namespace ys