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

			// Camera
			{
				Math::Vector3 pos = Math::Vector3(0.0f, 0.0f, 0.0f);
				Math::Vector4 tar[] = { Math::Vector4(1, 0, 0, 0), Math::Vector4(-1, 0, 0, 0), Math::Vector4(0, 1, 0, 0), Math::Vector4(0, -1, 0, 0), Math::Vector4(0, 0, 1, 0), Math::Vector4(0, 0, -1, 0) };
				Math::Vector4 up[] = { Math::Vector4(0, 1, 0, 0), Math::Vector4(0, 1, 0, 0), Math::Vector4(0, 0, -1, 0), Math::Vector4(0, 0, 1, 0), Math::Vector4(0, 1, 0, 0), Math::Vector4(0, 1, 0, 0) };

				for (int i = 0; i < cameraSize; ++i)
				{
					Math::Vector4 dir = Math::Vector4Rotate(tar[i], Math::QuaternionIdentity());

					captureCamera[i] = std::make_unique<Phoenix::Graphics::Camera>();
					captureCamera[i]->SetLookAt(pos, Math::Vector3(dir), Math::Vector3(up[i]));
					captureCamera[i]->SetPerspective(0.5f * 3.141592f, 1.0f, 0.1f, 100.0f);
				}
			}
		}

		void IBL::Finalize()
		{
			skyIBL.reset();
		}

		void IBL::Clear(Graphics::IGraphicsDevice* graphicsDevice, float r, float g, float b, float a)
		{
			for (int i = 0; i < 6; ++i)
			{
				skyIBL->ClearRenderTargetView(graphicsDevice, i, r, g, b, a);
			}
		}

		void IBL::Activate(Graphics::IGraphicsDevice* graphicsDevice)
		{
			skyIBL->ActivateAllRenderTargetView(graphicsDevice);
		}

		void IBL::Deactivate(Graphics::IGraphicsDevice* graphicsDevice)
		{
			skyIBL->Deactivate(graphicsDevice);
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

		void SkyMap::Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& world, const Graphics::Camera& camera, const Math::Vector4& lightDirection, const Math::Color& color)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();
			Phoenix::Graphics::IContext* context = graphicsDevice->GetContext();

			shader->Activate(device);
			{
				Phoenix::Graphics::IBuffer* vsCBuffer[] =
				{
					context->GetConstantBufferScene(),
					context->GetConstantBufferMesh()
				};
				Phoenix::Graphics::IBuffer* psCBuffer[] =
				{
					constantBuffer.get()
				};
				context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Vertex, 0, Phoenix::FND::ArraySize(vsCBuffer), vsCBuffer);
				context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Pixel, 0, Phoenix::FND::ArraySize(psCBuffer), psCBuffer);

				Phoenix::Graphics::ISampler* sampler[] =
				{
					context->GetSamplerState(Phoenix::Graphics::SamplerState::PointClamp)
				};
				context->SetSamplers(Phoenix::Graphics::ShaderType::Pixel, 0, Phoenix::FND::ArraySize(sampler), sampler);

				// �V�[���萔�o�b�t�@�X�V
				context->UpdateConstantBufferScene(camera.GetView(), camera.GetProjection());

				// ���b�V���萔�o�b�t�@�X�V
				context->UpdateConstantBufferMesh(world);

				ShaderConstants cb = {};
				cb.lightDirection = lightDirection;
				cb.color = color;
				context->UpdateSubresource(constantBuffer.get(), 0, 0, &cb, 0, 0);

				Graphics::ITexture* textures[] = { texture.get() };
				graphicsDevice->GetContext()->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, textures);

				Graphics::IModelResource* modelResource = model->GetModelResource();
				for (u32 i = 0; i < modelResource->GetMeshSize(); ++i)
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