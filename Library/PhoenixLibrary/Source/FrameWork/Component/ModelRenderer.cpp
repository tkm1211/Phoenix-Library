#include "pch.h"
#include "Phoenix/FrameWork/Component/ModelRenderer.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/FND/Util.h"


namespace Phoenix
{
	namespace FrameWork
	{
		void ModelRenderer::Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera, Graphics::IBuffer* buffers[], u32 size)
		{
			Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

			Phoenix::Graphics::IBuffer* vsCBuffer[] =
			{
				context->GetConstantBufferScene(),
				context->GetConstantBufferMesh(),
				context->GetConstantBufferBone()
			};
			//Phoenix::Graphics::IBuffer* psCBuffer[] =
			//{
			//	context->GetConstantBufferScene(),// temp
			//	context->GetConstantBufferMesh(),// temp
			//	context->GetConstantBufferBone()// temp
			//};

			context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Vertex, 0, Phoenix::FND::ArraySize(vsCBuffer), vsCBuffer);
			context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Pixel, 0, size, buffers);

			Phoenix::Graphics::ISampler* sampler[] =
			{
				context->GetSamplerState(Phoenix::Graphics::SamplerState::LinearWrap)
			};

			context->SetSamplers(Phoenix::Graphics::ShaderType::Pixel, 0, Phoenix::FND::ArraySize(sampler), sampler);

			// シーン定数バッファ更新
			{
				context->UpdateConstantBufferScene(camera.GetView(), camera.GetProjection());
			}
		}

		void ModelRenderer::Draw(Graphics::IGraphicsDevice* graphicsDevice, ModelObject* model, bool skining)
		{
			Graphics::IModelResource* modelResource = model->GetModelResource();
			Graphics::ModelData modelData = modelResource->GetModelData();
			const std::vector<FrameWork::ModelObject::Node>& nodes = model->GetNodes();

			for (s32 i = 0; i < modelResource->GetMeshSize(); i++)
			{
				graphicsDevice->GetContext()->UpdateConstantBufferBone(model->GetBoneTransforms(i), model->GetBoneTransformCount(i));

				if (skining)
				{
					static Graphics::VertexBufferKind vbKinds[] =
					{
						Graphics::VertexBufferKind::Position,
						//Graphics::VertexBufferKind::Normal,
						//Graphics::VertexBufferKind::Tangent,
						Graphics::VertexBufferKind::TexCoord0,
						Graphics::VertexBufferKind::BlendWeight0,
						Graphics::VertexBufferKind::BlendWeight1,
						Graphics::VertexBufferKind::BlendIndex0,
						Graphics::VertexBufferKind::BlendIndex1
					};

					Graphics::IMesh* mesh = modelResource->GetMesh(i);
					Graphics::ModelData::Mesh meshData = modelData.meshes[i];

					for (Graphics::ModelData::Subset& subset : meshData.subsets)
					{
						mesh->Draw(graphicsDevice->GetDevice(), vbKinds, FND::ArraySize(vbKinds), subset.startIndex, subset.indexCount, Graphics::PrimitiveTopology::TriangleList);
					}
				}
				else
				{
					static Graphics::VertexBufferKind vbKinds[] =
					{
						Graphics::VertexBufferKind::Position,
						//Graphics::VertexBufferKind::Normal,
						//Graphics::VertexBufferKind::Tangent,
						Graphics::VertexBufferKind::TexCoord0
					};

					Graphics::IMesh* mesh = modelResource->GetMesh(i);
					Graphics::ModelData::Mesh meshData = modelData.meshes[i];

					for (Graphics::ModelData::Subset& subset : meshData.subsets)
					{
						mesh->Draw(graphicsDevice->GetDevice(), vbKinds, FND::ArraySize(vbKinds), subset.startIndex, subset.indexCount, Graphics::PrimitiveTopology::TriangleList);
					}
				}
			}
		}

		void ModelRenderer::End(Graphics::IGraphicsDevice* graphicsDevice)
		{

		}
	}
}