#include "pch.h"
#include "Phoenix/FrameWork/Shader/StandardShader.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FND/STD.h"
#include "../Source/Graphics/Light/LightingState.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// ����
		std::unique_ptr<IShader> StandardShader::Create()
		{
			return std::make_unique<StandardShader>();
		}

		// ������
		bool StandardShader::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Phoenix::Graphics::PhoenixInputElementDesc inputElementDesc[] =
			{
				// SemanticName	 SemanticIndex	Format													InputSlot	AlignedByteOffset	InputSlotClass										InstanceDataStepRate
				{"POSITION",	 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,		0,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"NORMAL",		 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,		1,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"TANGENT",		 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,		2,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"TEXCOORD",	 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32_FLOAT,			3,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
			};

			shader = Graphics::IShader::Create();
			shader->LoadVS
			(
				graphicsDevice->GetDevice(),
				"C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Library\\PhoenixLibrary\\Build\\vs2019\\obj\\PhoenixLib_HLSL\\x86\\Debug\\StandardVS.cso",
				inputElementDesc,
				Phoenix::FND::ArraySize(inputElementDesc)
			);
			shader->LoadPS
			(
				graphicsDevice->GetDevice(),
				"C:\\Users\\2180082.MAETEL\\Desktop\\Phoenix\\Library\\PhoenixLibrary\\Build\\vs2019\\obj\\PhoenixLib_HLSL\\x86\\Debug\\StandardPS.cso"
			);

			cbScene = Phoenix::Graphics::IBuffer::Create();
			cbMaterial = Phoenix::Graphics::IBuffer::Create();
			{
				Phoenix::Graphics::PhoenixBufferDesc desc = {};
				Phoenix::FND::MemSet(&desc, 0, sizeof(desc));
				desc.usage = Phoenix::Graphics::PhoenixUsage::Default;
				desc.bindFlags = static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixBindFlag::ConstantBuffer);
				desc.cpuAccessFlags = 0;
				desc.miscFlags = 0;
				desc.byteWidth = sizeof(CbScene);
				desc.structureByteStride = 0;
				if (!cbScene->Initialize(graphicsDevice->GetDevice(), desc))
				{
					return false;
				}

				desc.byteWidth = sizeof(CbMaterial);
				if (!cbMaterial->Initialize(graphicsDevice->GetDevice(), desc))
				{
					return false;
				}
			}

			vbKinds.emplace_back(Graphics::VertexBufferKind::Position);
			vbKinds.emplace_back(Graphics::VertexBufferKind::Normal);
			vbKinds.emplace_back(Graphics::VertexBufferKind::Tangent);
			vbKinds.emplace_back(Graphics::VertexBufferKind::TexCoord0);

			light = std::make_unique<Graphics::LightingState>();

			return true;
		}

		// �I����
		void StandardShader::Finalize()
		{
			vbKinds.clear();
			cbMaterial.reset();
			cbScene.reset();
			light.reset();
			shader.reset();
		}

		// �V�F�[�_�[�J�n
		void StandardShader::Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera)
		{
			Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

			Phoenix::Graphics::IBuffer* vsCBuffer[] =
			{
				context->GetConstantBufferScene(),
				context->GetConstantBufferMesh(),
				context->GetConstantBufferBone()
			};
			Phoenix::Graphics::IBuffer* psCBuffer[] =
			{
				cbScene.get(),
				cbMaterial.get()
			};
			context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Vertex, 0, Phoenix::FND::ArraySize(vsCBuffer), vsCBuffer);
			context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Pixel, 0, Phoenix::FND::ArraySize(psCBuffer), psCBuffer);

			Phoenix::Graphics::ISampler* sampler[] =
			{
				context->GetSamplerState(Phoenix::Graphics::SamplerState::LinearWrap)
			};
			context->SetSamplers(Phoenix::Graphics::ShaderType::Pixel, 0, Phoenix::FND::ArraySize(sampler), sampler);

			// ���_�V�F�[�_�[�p�V�[���萔�o�b�t�@�X�V
			{
				context->UpdateConstantBufferScene(camera.GetView(), camera.GetProjection());
			}

			// �s�N�Z���V�F�[�_�[�p�V�[���萔�o�b�t�@�X�V
			{
				CbScene constantBufferSnene = {};
				constantBufferSnene.eye = camera.GetEye();
				context->UpdateSubresource(cbScene.get(), 0, 0, &constantBufferSnene, 0, 0);
			}

			shader->Activate(graphicsDevice->GetDevice());
		}

		// �`��
		void StandardShader::Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model)
		{
			Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

			Graphics::IModelResource* modelResource = model->GetModelResource();
			Graphics::ModelData modelData = modelResource->GetModelData();

			CbMaterial cb = {};
			light->UpdateLightEnvironment(cb.lightEnv, Math::AABB());
			cb.emissive = Math::Color(0.0f, 0.0f, 0.0f, 1.0f);
			cb.diffuse = Math::Color(1.0f, 1.0f, 1.0f, 1.0f);
			cb.specular = Math::Color(0.1f, 0.1f, 0.1f, 1.0f);
			context->UpdateSubresource(cbMaterial.get(), 0, 0, &cb, 0, 0);

			// ���b�V���萔�o�b�t�@�X�V
			context->UpdateConstantBufferMesh(worldTransform);

			for (s32 i = 0; i < modelResource->GetMeshSize(); ++i)
			{
				if (0 < model->GetMeshNodes())
				{
					graphicsDevice->GetContext()->UpdateConstantBufferBone(model->GetBoneTransforms(i), model->GetBoneTransformCount(i));
				}

				Graphics::IMesh* mesh = modelResource->GetMesh(i);
				Graphics::ModelData::Mesh meshData = modelData.meshes[i];

				for (Graphics::ModelData::Subset& subset : meshData.subsets)
				{
					u32 size = model->GetTextureSize(subset.materialIndex);
					for (u32 j = 0; j < size; ++j)
					{
						Graphics::ITexture* texture[] = { model->GetTexture(subset.materialIndex, j) };
						graphicsDevice->GetContext()->SetShaderResources(Graphics::ShaderType::Pixel, j, 1, texture);
					}
					mesh->Draw(graphicsDevice->GetDevice(), GetVectexBuferKinds(), GetVectexBuferKindsSize(), subset.startIndex, subset.indexCount, Graphics::PrimitiveTopology::TriangleList);
				}
			}
		}

		// �V�F�[�_�[�I��
		void StandardShader::End(Graphics::IGraphicsDevice* graphicsDevice)
		{
			shader->Deactivate(graphicsDevice->GetDevice());
		}
	}
}