#include "pch.h"
#include "Phoenix/FrameWork/Shader/BasicSkinShader.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FND/STD.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// 生成
		std::unique_ptr<IShader> BasicSkinShader::Create()
		{
			return std::make_unique<BasicSkinShader>();
		}

		// 初期化
		bool BasicSkinShader::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Phoenix::Graphics::PhoenixInputElementDesc inputElementDesc[] =
			{
				// SemanticName	 SemanticIndex	Format														InputSlot	AlignedByteOffset	InputSlotClass										InstanceDataStepRate
				{"POSITION",	 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,			0,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"TEXCOORD",	 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32_FLOAT,				1,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"BLENDWEIGHT",	 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32A32_FLOAT,		2,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"BLENDWEIGHT",	 1,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32A32_FLOAT,		3,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"BLENDINDICES", 0,				Phoenix::Graphics::PHOENIX_FORMAT_R8G8B8A8_UINT,			4,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"BLENDINDICES", 1,				Phoenix::Graphics::PHOENIX_FORMAT_R8G8B8A8_UINT,			5,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
			};

			shader = Graphics::IShader::Create();
			shader->LoadVS
			(
				graphicsDevice->GetDevice(),
				"BasicVSSkin.cso",
				inputElementDesc,
				Phoenix::FND::ArraySize(inputElementDesc)
			);
			shader->LoadPS
			(
				graphicsDevice->GetDevice(),
				"BasicPS.cso"
			);

			cbMatrial = Phoenix::Graphics::IBuffer::Create();
			{
				Phoenix::Graphics::PhoenixBufferDesc desc = {};
				Phoenix::FND::MemSet(&desc, 0, sizeof(desc));
				desc.usage = Phoenix::Graphics::PhoenixUsage::Default;
				desc.bindFlags = static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixBindFlag::ConstantBuffer);
				desc.cpuAccessFlags = 0;
				desc.miscFlags = 0;
				desc.byteWidth = sizeof(CbMaterial);
				desc.structureByteStride = 0;
				if (!cbMatrial->Initialize(graphicsDevice->GetDevice(), desc))
				{
					return false;
				}
			}

			vbKinds.emplace_back(Graphics::VertexBufferKind::Position);
			vbKinds.emplace_back(Graphics::VertexBufferKind::TexCoord0);
			vbKinds.emplace_back(Graphics::VertexBufferKind::BlendWeight0);
			vbKinds.emplace_back(Graphics::VertexBufferKind::BlendWeight1);
			vbKinds.emplace_back(Graphics::VertexBufferKind::BlendIndex0);
			vbKinds.emplace_back(Graphics::VertexBufferKind::BlendIndex1);

			return true;
		}

		// 終了化
		void BasicSkinShader::Finalize()
		{
			vbKinds.clear();
			cbMatrial.reset();
			shader.reset();
		}

		// シェーダー開始
		void BasicSkinShader::Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera)
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
				cbMatrial.get()
			};
			context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Vertex, 0, Phoenix::FND::ArraySize(vsCBuffer), vsCBuffer);
			context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Pixel, 0, Phoenix::FND::ArraySize(psCBuffer), psCBuffer);

			Phoenix::Graphics::ISampler* sampler[] =
			{
				context->GetSamplerState(Phoenix::Graphics::SamplerState::LinearWrap)
			};
			context->SetSamplers(Phoenix::Graphics::ShaderType::Pixel, 0, Phoenix::FND::ArraySize(sampler), sampler);

			// シーン定数バッファ更新
			context->UpdateConstantBufferScene(camera.GetView(), camera.GetProjection());

			// メッシュ定数バッファ更新
			CbMaterial cb = {};
			cb.color = { 5.0f, 5.0f, 5.0f, 1.0f };
			context->UpdateSubresource(cbMatrial.get(), 0, 0, &cb, 0, 0);

			shader->Activate(graphicsDevice->GetDevice());
		}

		// 描画
		void BasicSkinShader::Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model)
		{
			Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();
			Graphics::IModelResource* modelResource = model->GetModelResource();

			// メッシュ定数バッファ更新
			context->UpdateConstantBufferMesh(worldTransform);

			for (u32 i = 0; i < modelResource->GetMeshSize(); ++i)
			{
				if (0 < model->GetMeshNodes())
				{
					graphicsDevice->GetContext()->UpdateConstantBufferBone(model->GetBoneTransforms(i), model->GetBoneTransformCount(i));
				}

				Graphics::IMesh* mesh = modelResource->GetMesh(i);

				for (const Graphics::ModelData::Subset& subset : modelResource->GetModelData().meshes[i].subsets)
				{
					u32 size = static_cast<u32>(model->GetTextureSize(subset.materialIndex));
					for (u32 j = 0; j < size; ++j)
					{
						Graphics::ITexture* texture[] = { model->GetTexture(subset.materialIndex, j) };
						graphicsDevice->GetContext()->SetShaderResources(Graphics::ShaderType::Pixel, j, 1, texture);
					}
					mesh->Draw(graphicsDevice->GetDevice(), GetVectexBuferKinds(), static_cast<u32>(GetVectexBuferKindsSize()), subset.startIndex, subset.indexCount, Graphics::PrimitiveTopology::TriangleList);
				}
			}
		}

		// シェーダー終了
		void BasicSkinShader::End(Graphics::IGraphicsDevice* graphicsDevice)
		{
			shader->Deactivate(graphicsDevice->GetDevice());
		}
	}
}