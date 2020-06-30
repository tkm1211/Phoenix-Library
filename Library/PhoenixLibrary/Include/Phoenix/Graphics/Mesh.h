#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Phoenix/FND/Base.h"
#include "Phoenix/Math/AABB.h"
#include "Phoenix/Math/Color.h"
#include "Phoenix/Math/vector.h"
#include "Phoenix/Math/Matrix.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/Graphics/Buffer.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// ���_�f�[�^����ʂɃ����_�����O������@
		//****************************************************************************
		enum class PrimitiveTopology
		{
			PointList,			// �|�C���g���X�g
			LineList,			// ���C�����X�g
			LineStrip,			// ���C���X�g���b�v
			TriangleList,		// �g���C�A���O�����X�g
			TriangleStrip,		// �g���C�A���O���X�g���b�v

			TypeNum
		};

		// ���_�o�b�t�@���
		enum class VertexBufferKind
		{
			Position,
			Normal,
			Tangent,
			Color,
			TexCoord0,
			TexCoord1,
			TexCoord2,
			TexCoord3,
			BlendWeight0,
			BlendWeight1,
			BlendIndex0,
			BlendIndex1,

			TypeNum
		};

		//****************************************************************************
		// ���b�V���̐ݒ�L�q
		//****************************************************************************
		struct MeshDesc
		{
			PrimitiveTopology primitiveTopology = PrimitiveTopology::TriangleList;	// �v���~�e�B�u�g�|���W�[

			u32 vertexCount = 0;								// ���_��
			u32 indexCount = 0;									// �C���f�b�N�X��
			const u16* indicesU16 = nullptr;					// 16bit�C���f�b�N�X�f�[�^
			const u32* indicesU32 = nullptr;					// 32bit�C���f�b�N�X�f�[�^
			const Math::Vector3* positions = nullptr;			// ���W�f�[�^
			const Math::Vector3* normals = nullptr;				// �@���f�[�^
			const Math::Vector3* tangents = nullptr;			// �ڐ��f�[�^
			const Math::Color* colors = nullptr;				// �F�f�[�^
			const Math::Vector2* texcoords0 = nullptr;			// �e�N�X�`�����W�f�[�^0
			const Math::Vector2* texcoords1 = nullptr;			// �e�N�X�`�����W�f�[�^1
			const Math::Vector2* texcoords2 = nullptr;			// �e�N�X�`�����W�f�[�^2
			const Math::Vector2* texcoords3 = nullptr;			// �e�N�X�`�����W�f�[�^3
			const Math::Vector4* blendWeights0 = nullptr;		// �u�����h�E�F�C�g�f�[�^0
			const Math::Vector4* blendWeights1 = nullptr;		// �u�����h�E�F�C�g�f�[�^1
			const Math::Vector4_U8* blendIndices0 = nullptr;	// �u�����h�C���f�b�N�X�f�[�^0
			const Math::Vector4_U8* blendIndices1 = nullptr;	// �u�����h�C���f�b�N�X�f�[�^1

			// ���_�ڐ����v�Z
			void ComputeTangents();
		};

		//****************************************************************************
		// ���b�V������C���^�[�t�F�[�X
		//****************************************************************************
		class IMesh : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IMesh> Create();

			// ������
			virtual bool Initialize(IDevice* device, const MeshDesc& desc) = 0;

			// �I����
			virtual void Finalize() = 0;

			// �`��
			virtual void Draw(IDevice* device, VertexBufferKind vbKind[], u32 kindNum, u32 start, u32 count, PrimitiveTopology primitiveTopology) = 0;

			// �o�E���f�B���O�{�b�N�X�擾
			virtual const Math::AABB& GetBounds() = 0;

			// ���b�V���ݒ�L�q�擾
			virtual MeshDesc& GetDesc() = 0;

			// �o�E���f�B���O�{�b�N�X�v�Z
			virtual void ComputeBounds() = 0;

			// �o�E���f�B���O�{�b�N�X�v�Z
			virtual void ComputeBounds(Math::AABB& bounds, const Math::Matrix boneTransforms[]) = 0;

			// �X�L�j���O���_�v�Z
			virtual void ComputeSkinnedVertices(Math::Vector3 vertices[], const Math::Matrix boneTransforms[]) = 0;
		};
	} // namespace Graphics
} // namespace Phoenix