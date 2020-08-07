#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Phoenix/Math/Color.h"
#include "Phoenix/Math/Quaternion.h"
#include "Phoenix/Math/Vector.h"
#include "Phoenix/OS/Resource.h"
#include "Phoenix/Graphics/Mesh.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// ���f���f�[�^
		//****************************************************************************
		struct ModelData
		{
			enum class SkinningMode
			{
				None,
				Rigid,
				Smooth
			};

			struct Node
			{
				std::string name;
				s32 parentIndex;
				Math::Vector3 translate;
				Math::Quaternion rotate;
				Math::Vector3 scale;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct Material
			{
				std::string name;
				std::vector<std::string> textureFilename;
				std::vector<Math::Color> color;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct Subset
			{
				u32 startIndex = 0;
				u32 indexCount = 0;
				u32 materialIndex = 0;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct Mesh
			{
				SkinningMode skinningMode;
				s32 nodeIndex;

				std::vector<Math::Vector3> positions;
				std::vector<Math::Vector3> normals;
				std::vector<Math::Vector3> tangents;
				std::vector<Math::Color> colors;
				std::vector<Math::Vector2> texcoords0;
				std::vector<Math::Vector2> texcoords1;
				std::vector<Math::Vector2> texcoords2;
				std::vector<Math::Vector2> texcoords3;
				std::vector<Math::Vector4> blendWeights0;
				std::vector<Math::Vector4> blendWeights1;
				std::vector<Math::Vector4_U8> blendIndices0;
				std::vector<Math::Vector4_U8> blendIndices1;
				std::vector<u16> indicesU16;
				std::vector<u32> indicesU32;
				std::vector<s32> nodeIndices;
				std::vector<Math::Matrix> offsetTransforms;
				std::vector<Subset> subsets;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			std::vector<Node> nodes;
			std::vector<Material> materials;
			std::vector<Mesh> meshes;

			template<class Archive>
			void serialize(Archive& archive, u32 version);

			// �V���A���C�Y
			static void Serialize(const ModelData& data, const char* filename);

			// �f�V���A���C�Y
			static bool Deserialize(ModelData& data, const char* filename);
		};

		//****************************************************************************
		// ���f�����\�[�X����C���^�[�t�F�[�X
		//****************************************************************************
		class IModelResource : public OS::Resource
		{
		public:
			// ���f���f�[�^�擾
			virtual const ModelData& GetModelData() = 0;

			// ���b�V���擾
			virtual IMesh* GetMesh(sizeT index) = 0;

			// ���b�V���T�C�Y�擾
			virtual sizeT GetMeshSize() = 0;
		};

		//****************************************************************************
		// ���f�����\�[�X����I�u�W�F�N�g�����C���^�[�t�F�[�X
		//****************************************************************************
		class IModelResourceFactory : public OS::IResourceFactory
		{
		public:
			// ����
			static std::unique_ptr<IModelResourceFactory> Create(Graphics::IDevice* device);
		};
	} // namespace Graphics
} // namespace Phoenix