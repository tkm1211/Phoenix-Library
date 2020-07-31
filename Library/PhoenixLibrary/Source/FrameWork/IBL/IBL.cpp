#include "pch.h"
#include "Phoenix/FrameWork/IBL/IBL.h"
#include "Phoenix/FND/Util.h"


namespace Phoenix
{
	namespace FrameWork
	{
		std::unique_ptr<IBL> IBL::Create()
		{
			return std::make_unique<IBL>();
		}

		void IBL::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			skyIBL = FrameBuffer::Create();
			skyIBL->Initialize
			(
				graphicsDevice,
				64, 64, false, 1,
				Graphics::TextureFormatDx::R16G16B16A16_FLOAT,
				Graphics::TextureFormatDx::UNKNOWN,
				true,
				false,
				true,
				true,
				6
			);


		}

		void IBL::Finalize()
		{
			skyIBL.reset();
		}

		void IBL::Clear(Graphics::IGraphicsDevice* graphicsDevice, u32 index, float r, float g, float b, float a)
		{
			skyIBL->ClearRenderTargetView(graphicsDevice, index, r, g, b, a);
		}

		void IBL::Activate(Graphics::IGraphicsDevice* graphicsDevice, u32 index)
		{
			skyIBL->ActivateRenderTargetView(graphicsDevice, index);
		}

		void IBL::Deactivate(Graphics::IGraphicsDevice* graphicsDevice, u32 index)
		{
			skyIBL->Deactivate(graphicsDevice, index);
		}


		std::unique_ptr<SkyMap> SkyMap::Create()
		{
			return std::make_unique<SkyMap>();
		}

		void SkyMap::Initialize(Graphics::IGraphicsDevice* graphicsDevice, const s8* cubemapFilename)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();

			model = FrameWork::ModelObject::Create();
			model->Initialize(graphicsDevice);
			model->Load(graphicsDevice, "..\\Data\\Assets\\Model\\SkyMap\\sphere.fbx");

			texture = Graphics::ITexture::Create();
			texture->Initialize(device, cubemapFilename, Graphics::MaterialType::Diffuse, Math::Color(1.0f, 1.0f, 1.0f, 1.0f));

			Phoenix::Graphics::PhoenixInputElementDesc inputElementDesc[] =
			{
				// SemanticName	 SemanticIndex	Format												InputSlot	AlignedByteOffset	InputSlotClass										InstanceDataStepRate
				{"POSITION",	 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,	0,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 }
			};
			shader = Graphics::IShader::Create();
			shader->LoadVS
			(
				device,
				"skyMapVS.cso",
				inputElementDesc,
				Phoenix::FND::ArraySize(inputElementDesc)
			);
			shader->LoadPS
			(
				device,
				"skyMapPS.cso"
			);

			Graphics::PhoenixBufferDesc desc = {};
			desc.byteWidth = sizeof(ShaderConstants);
			desc.bindFlags = static_cast<u32>(Graphics::PhoenixBindFlag::ConstantBuffer);
			desc.cpuAccessFlags = static_cast<u32>(Graphics::PhoenixCPUAccessFlag::CPUAccessWrite);
			desc.usage = Graphics::PhoenixUsage::Dynamic;
			desc.miscFlags = 0;
			desc.structureByteStride = 0;

			constantBuffer = Graphics::IBuffer::Create();
			constantBuffer->Initialize(device, desc);

			vbKinds.emplace_back(Graphics::VertexBufferKind::Position);
		}

		void SkyMap::Finalize()
		{

		}

		void SkyMap::Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& world, const Math::Color& color)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();
			Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

			shader->Activate(device);
			{
				ShaderConstants cb = {};
				cb.color = color;
				context->UpdateSubresource(constantBuffer.get(), 0, 0, &cb, 0, 0);

				// メッシュ定数バッファ更新
				context->UpdateConstantBufferMesh(world);

				Graphics::ITexture* textures[] = { texture.get() };
				graphicsDevice->GetContext()->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, textures);

				Graphics::IModelResource* modelResource = model->GetModelResource();
				for (s32 i = 0; i < modelResource->GetMeshSize(); ++i)
				{
					if (0 < model->GetMeshNodes())
					{
						graphicsDevice->GetContext()->UpdateConstantBufferBone(model->GetBoneTransforms(i), model->GetBoneTransformCount(i));
					}

					Graphics::IMesh* mesh = modelResource->GetMesh(i);

					for (const Graphics::ModelData::Subset& subset : modelResource->GetModelData().meshes[i].subsets)
					{
						mesh->Draw(graphicsDevice->GetDevice(), vbKinds.data(), static_cast<u32>(vbKinds.size()), subset.startIndex, subset.indexCount, Graphics::PrimitiveTopology::TriangleList);
					}
				}
			}
			shader->Deactivate(device);
		}
	}
}