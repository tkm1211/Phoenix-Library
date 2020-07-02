#include "pch.h"
#include "Phoenix/FrameWork/Renderer/ModelRenderer.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/FND/Util.h"


namespace Phoenix
{
	namespace FrameWork
	{
		void ModelRenderer::Begin(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, const Graphics::Camera& camera)
		{
			Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

			// メッシュ定数バッファ更新
			context->UpdateConstantBufferMesh(worldTransform);

			// シーン定数バッファ更新
			context->UpdateConstantBufferScene(camera.GetView(), camera.GetProjection());
		}

		void ModelRenderer::Draw(Graphics::IGraphicsDevice* graphicsDevice, ModelObject* model, IShader* shader)
		{
			Graphics::IModelResource* modelResource = model->GetModelResource();
			Graphics::ModelData modelData = modelResource->GetModelData();

			for (u32 i = 0; i < model->GetMaterialSize(); ++i)
			{
				Graphics::ITexture* texture[] = { model->GetTexture(i) };
				graphicsDevice->GetContext()->SetShaderResources(Graphics::ShaderType::Pixel, i, 1, texture);
			}

			for (s32 i = 0; i < modelResource->GetMeshSize(); i++)
			{
				graphicsDevice->GetContext()->UpdateConstantBufferBone(model->GetBoneTransforms(i), model->GetBoneTransformCount(i));

				Graphics::IMesh* mesh = modelResource->GetMesh(i);
				Graphics::ModelData::Mesh meshData = modelData.meshes[i];

				for (Graphics::ModelData::Subset& subset : meshData.subsets)
				{
					mesh->Draw(graphicsDevice->GetDevice(), shader->GetVectexBuferKinds(), shader->GetVectexBuferKindsSize(), subset.startIndex, subset.indexCount, Graphics::PrimitiveTopology::TriangleList);
				}
			}
		}

		void ModelRenderer::End(Graphics::IGraphicsDevice* graphicsDevice)
		{
			// No data.
		}
	}
}