#include "pch.h"
#include "Phoenix\FrameWork\ComputeShader\GPUParticle.h"
#include "Phoenix\FrameWork\ComputeShader\ComputeShader.h"
#include "Phoenix/FND/Logger.h"
#include "Phoenix/FND/Util.h"
#include "Phoenix/FND/STD.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// 生成
		std::unique_ptr<GPUBuffer> GPUBuffer::Create()
		{
			return std::make_unique<GPUBuffer>();
		}

		// 初期化
		bool GPUBuffer::Initialize(Graphics::IDevice* device, u32 byteWidth, u32 structureByteStride, s32 miscFlags, void* initData)
		{
			buffer = Phoenix::Graphics::IBuffer::Create();
			srv = Phoenix::Graphics::ITexture::Create();
			uav = Phoenix::Graphics::ITexture::Create();

			if (!ComputeShaderBufferFactor::CreateStructuredBuffer(device, byteWidth, structureByteStride, miscFlags, initData, buffer.get()))
			{
				return false;
			}

			Phoenix::Graphics::PhoenixBufferDesc bufferDesc = {};
			Phoenix::FND::MemSet(&bufferDesc, 0, sizeof(bufferDesc));
			{
				buffer->GetDesc(&bufferDesc);
			}
			if (bufferDesc.bindFlags & static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixBindFlag::ShadowResource))
			{
				if (!ComputeShaderBufferFactor::CreateBufferSRV(device, buffer.get(), srv.get()))
				{
					return false;
				}
			}
			if (bufferDesc.bindFlags & static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixBindFlag::UnorderedAccess))
			{
				if (!ComputeShaderBufferFactor::CreateBufferUAV(device, buffer.get(), uav.get()))
				{
					return false;
				}
			}

			return true;
		}

		// 終了化
		void GPUBuffer::Finalize()
		{
			uav.reset();
			buffer.reset();
		}


		// 生成
		std::unique_ptr<GPUParticle> GPUParticle::Create()
		{
			return std::make_unique<GPUParticle>();
		}

		// 初期化
		bool GPUParticle::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();

			if (!CreateBuffers(device))
			{
				return false;
			}

			LoadShaders(device);

			return true;
		}

		// 終了化
		void GPUParticle::Finalize()
		{

		}

		// 更新
		void GPUParticle::UpdateCPU(Graphics::IGraphicsDevice* graphicsDevice, Math::Vector3 transform, float dt)
		{
			emit = FND::Max(0.0f, emit - floorf(emit));

			center = transform;

			//emit += count * dt;

			emit += burst;
			burst = 0;

			// Swap CURRENT alivelist with NEW alivelist
			std::swap(aliveList[0], aliveList[1]);
		}

		void GPUParticle::UpdateGPU(Graphics::IGraphicsDevice* graphicsDevice, Math::Matrix worldTransform, float dt)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();
			Graphics::IContext* context = graphicsDevice->GetContext();

			EmittedParticleCB cb = {};
			{
				cb.emitterWorld = worldTransform;
				cb.emitCount = static_cast<u32>(emit);
				cb.emitterMeshIndexCount = 0; // TODO : mesh index
				cb.emitterMeshVertexPositionStride = 0;
				cb.emitterRandomness = (rand() % 1000) * 0.001f;
				cb.particleLifeSpan = life;
				cb.particleNormalFactor = normalFactor;
				cb.particleRandomFactor = randomFactor;
				cb.particleScaling = scaleX;
				cb.particleSize = size;
				cb.particleMotionBlurAmount = motionBlurAmount;
				cb.particleRotation = rotation * Phoenix::Math::PI * 60;
				cb.particleColor = 0; // TODO : material color
				cb.particleEmissive = 0; // TODO : material emissive color w
				cb.emitterOpacity = 0; // TODO : material base color w
				cb.particleMass = mass;
				cb.emitterMaxParticleCount = particleMaxSize;
				cb.emitterFixedTimestep = fixedTimeStep;

				// SPH:
				/*cb.xSPH_h = SPH_h;
				cb.xSPH_h_rcp = 1.0f / SPH_h;
				cb.xSPH_h2 = SPH_h * SPH_h;
				cb.xSPH_h3 = cb.xSPH_h2 * SPH_h;
				const float h6 = cb.xSPH_h2 * cb.xSPH_h2 * cb.xSPH_h2;
				const float h9 = h6 * cb.xSPH_h3;
				cb.xSPH_poly6_constant = (315.0f / (64.0f * XM_PI * h9));
				cb.xSPH_spiky_constant = (-45.0f / (XM_PI * h6));
				cb.xSPH_K = SPH_K;
				cb.xSPH_p0 = SPH_p0;
				cb.xSPH_e = SPH_e;
				cb.xSPH_ENABLED = IsSPHEnabled() ? 1 : 0;*/
			}
			FrameTimeCB ftCB = {};
			{
				ftCB.frameTime = dt;
				ftCB.dummy[0] = 0.0f;
				ftCB.dummy[1] = 0.0f;
				ftCB.dummy[2] = 0.0f;
			}

			Phoenix::Graphics::IBuffer* psCBuffer[] =
			{
				emittedParticleCB.get(),
				frameTimeCB.get()
			};
			context->UpdateSubresource(emittedParticleCB.get(), 0, 0, &cb, 0, 0);
			context->UpdateSubresource(frameTimeCB.get(), 0, 0, &ftCB, 0, 0);
			context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Compute, 0, Phoenix::FND::ArraySize(psCBuffer), psCBuffer);

			Graphics::ITexture* uavTexture[] = 
			{
				particleBuffer->uav.get(),
				aliveList[0]->uav.get(),
				aliveList[1]->uav.get(),
				deadList->uav.get(),
				counterBuffer->uav.get(),
				indirectBuffers->uav.get(),
				distanceBuffer->uav.get()
			};
			context->SetUnorderedAccess(0, Phoenix::FND::ArraySize(uavTexture), uavTexture, nullptr);

			// kick off updating, set up state
			kickoffUpdateCS->Activate(device);
			{
				kickoffUpdateCS->Dispatch(device, 1, 1, 1);
			}
			kickoffUpdateCS->Deactivate(device);

			// emit the required amount if there are free slots in dead list
			emitCS->Activate(device);
			{
				emitCS->DispatchIndirect(device, indirectBuffers->buffer.get(), ARGUMENTBUFFER_OFFSET_DISPATCHEMIT);
			}
			emitCS->Deactivate(device);

			simulateCS->Activate(device);
			{
				simulateCS->DispatchIndirect(device, indirectBuffers->buffer.get(), ARGUMENTBUFFER_OFFSET_DISPATCHSIMULATION);
			}
			simulateCS->Deactivate(device);

			Graphics::ITexture* nullptrUAVTexture[] =
			{
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr
			};
			context->SetUnorderedAccess(0, Phoenix::FND::ArraySize(nullptrUAVTexture), nullptrUAVTexture, nullptr);

			finishUpdateCS->Activate(device);
			{
				Graphics::ITexture* texture[] =
				{
					counterBuffer->srv.get()
				};
				context->SetShaderResources(Graphics::ShaderType::Compute, 0, 1, texture);

				Graphics::ITexture* uavTexture[] =
				{
					indirectBuffers->uav.get()
				};
				context->SetUnorderedAccess(0, Phoenix::FND::ArraySize(uavTexture), uavTexture, nullptr);

				finishUpdateCS->Dispatch(device, 1, 1, 1);
			}
			finishUpdateCS->Deactivate(device);
		}

		void GPUParticle::Draw(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();
			Graphics::IContext* context = graphicsDevice->GetContext();
			ID3D11DeviceContext* d3dContext = static_cast<Graphics::DeviceDX11*>(device)->GetD3DContext();

			d3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// 定数バッファ設定
			{
				Phoenix::Graphics::IBuffer* vsCBuffer[] =
				{
					context->GetConstantBufferScene()
				};
				Phoenix::Graphics::IBuffer* psCBuffer[] =
				{
					nullptr
				};
				context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Vertex, 2, Phoenix::FND::ArraySize(vsCBuffer), vsCBuffer);
				context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Pixel, 0, Phoenix::FND::ArraySize(psCBuffer), psCBuffer);

				Phoenix::Graphics::ISampler* sampler[] =
				{
					context->GetSamplerState(Phoenix::Graphics::SamplerState::LinearWrap)
				};
				context->SetSamplers(Phoenix::Graphics::ShaderType::Pixel, 0, Phoenix::FND::ArraySize(sampler), sampler);

				// シーン定数バッファ更新
				context->UpdateConstantBufferScene(camera.GetView(), camera.GetProjection());
			}

			drawShader->Activate(device);
			{
				Graphics::ITexture* texture[] = 
				{
					particleBuffer->srv.get(),
					aliveList[1]->srv.get()
				};
				context->SetShaderResources(Graphics::ShaderType::Vertex, 0, 2, texture);

				context->DrawInstancedIndirect(indirectBuffers->buffer.get(), ARGUMENTBUFFER_OFFSET_DRAWPARTICLES);
			}
			drawShader->Deactivate(device);
		}

		void GPUParticle::Burst(int num)
		{
			burst += num;
		}

		void GPUParticle::Restart()
		{
			
		}
		bool GPUParticle::CreateBuffers(Graphics::IDevice* device)
		{
			particleBuffer = std::make_unique<GPUBuffer>();
			if (!particleBuffer->Initialize(device, sizeof(Particle) * particleMaxSize, sizeof(Particle), static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferStructured), nullptr))
			{
				return false;
			}

			aliveList[0] = std::make_unique<GPUBuffer>();
			if (!aliveList[0]->Initialize(device, sizeof(u32) * particleMaxSize, sizeof(u32), static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferStructured), nullptr))
			{
				return false;
			}
			aliveList[1] = std::make_unique<GPUBuffer>();
			if (!aliveList[1]->Initialize(device, sizeof(u32) * particleMaxSize, sizeof(u32), static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferStructured), nullptr))
			{
				return false;
			}

			deadList = std::make_unique<GPUBuffer>();
			{
				std::vector<u32> indices(particleMaxSize);
				for (u32 i = 0; i < particleMaxSize; ++i)
				{
					indices[i] = i;
				}
				if (!deadList->Initialize(device, sizeof(u32) * particleMaxSize, sizeof(u32), static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferStructured), indices.data()))
				{
					return false;
				}
			}

			distanceBuffer = std::make_unique<GPUBuffer>();
			{
				std::vector<f32> distances(particleMaxSize);
				std::fill(distances.begin(), distances.end(), 0.0f);
				if (!distanceBuffer->Initialize(device, sizeof(f32) * particleMaxSize, sizeof(f32), static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferStructured), distances.data()))
				{
					return false;
				}
			}

			ParticleCounters counters;
			counters.aliveCount = 0;
			counters.deadCount = particleMaxSize;
			counters.realEmitCount = 0;
			counters.aliveCount_afterSimulation = 0;

			counterBuffer = std::make_unique<GPUBuffer>();
			if (!counterBuffer->Initialize(device, sizeof(counters), sizeof(counters), static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferAllowsRAWViews), &counters))
			{
				return false;
			}

			indirectBuffers = std::make_unique<GPUBuffer>();
			if (!indirectBuffers->Initialize(device, sizeof(IndirectDispatchArgs) + sizeof(IndirectDispatchArgs) + sizeof(IndirectDrawArgsInstanced), 0, static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferAllowsRAWViews) | static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixResouceMiscFlag::ResouceMiscDrawindIrectArgs), nullptr))
			{
				return false;
			}

			emittedParticleCB = Phoenix::Graphics::IBuffer::Create();
			{
				Phoenix::Graphics::PhoenixBufferDesc desc = {};
				Phoenix::FND::MemSet(&desc, 0, sizeof(desc));
				desc.usage = Phoenix::Graphics::PhoenixUsage::Default;
				desc.bindFlags = static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixBindFlag::ConstantBuffer);
				desc.cpuAccessFlags = 0;
				desc.miscFlags = 0;
				desc.byteWidth = sizeof(EmittedParticleCB);
				desc.structureByteStride = 0;
				if (!emittedParticleCB->Initialize(device, desc))
				{
					return false;
				}
			}

			frameTimeCB = Phoenix::Graphics::IBuffer::Create();
			{
				Phoenix::Graphics::PhoenixBufferDesc desc = {};
				Phoenix::FND::MemSet(&desc, 0, sizeof(desc));
				desc.usage = Phoenix::Graphics::PhoenixUsage::Default;
				desc.bindFlags = static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixBindFlag::ConstantBuffer);
				desc.cpuAccessFlags = 0;
				desc.miscFlags = 0;
				desc.byteWidth = sizeof(FrameTimeCB);
				desc.structureByteStride = 0;
				if (!frameTimeCB->Initialize(device, desc))
				{
					return false;
				}
			}

			return true;
		}


		void GPUParticle::LoadShaders(Graphics::IDevice* device)
		{
			drawShader = Graphics::IShader::Create();

			kickoffUpdateCS = Graphics::IComputeShader::Create();
			finishUpdateCS = Graphics::IComputeShader::Create();
			emitCS = Graphics::IComputeShader::Create();
			emitCSVolume = Graphics::IComputeShader::Create();
			emitCSFromMesh = Graphics::IComputeShader::Create();
			sphpartitionCS = Graphics::IComputeShader::Create();
			sphpartitionoffsetsCS = Graphics::IComputeShader::Create();
			sphpartitionoffsetsresetCS = Graphics::IComputeShader::Create();
			sphdensityCS = Graphics::IComputeShader::Create();
			sphforceCS = Graphics::IComputeShader::Create();
			simulateCS = Graphics::IComputeShader::Create();
			simulateCSSorting = Graphics::IComputeShader::Create();
			simulateCSDepthCollisions = Graphics::IComputeShader::Create();
			simulateCSSortingDepthCollisions = Graphics::IComputeShader::Create();

			Phoenix::Graphics::PhoenixInputElementDesc inputElementDesc[] =
			{
				// SemanticName	 SemanticIndex	Format														InputSlot	AlignedByteOffset	InputSlotClass										InstanceDataStepRate
				{"POSITION",	 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,			0,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"TEXCOORD",	 0,				Phoenix::Graphics::PHOENIX_FORMAT_R32G32_FLOAT,				1,			0,					Phoenix::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
			};

			/*{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },*/

			drawShader->LoadVS(device, "EmittedParticleVS.cso", inputElementDesc, Phoenix::FND::ArraySize(inputElementDesc));
			drawShader->LoadPS(device, "EmittedParticlePS.cso");

			kickoffUpdateCS->Load(device, "KickoffUpdateCS.cso");
			finishUpdateCS->Load(device, "FinishUpdateCS.cso");
			emitCS->Load(device, "emitCS.cso");
			//emitCSVolume->Load(device, "emitCS_VOLUME.cso");
			//emitCSFromMesh->Load(device, "kickoffUpdateCS.cso");
			//sphpartitionCS->Load(device, "kickoffUpdateCS.cso");
			//sphpartitionoffsetsCS->Load(device, "kickoffUpdateCS.cso");
			//sphpartitionoffsetsresetCS->Load(device, "kickoffUpdateCS.cso");
			//sphdensityCS->Load(device, "kickoffUpdateCS.cso");
			//sphforceCS->Load(device, "kickoffUpdateCS.cso");
			simulateCS->Load(device, "SimulateCS.cso");
			//simulateCSSorting->Load(device, "kickoffUpdateCS.cso");
			//simulateCSDepthCollisions->Load(device, "kickoffUpdateCS.cso");
			//simulateCSSortingDepthCollisions->Load(device, "kickoffUpdateCS.cso");
		}


		// 生成
		std::unique_ptr<EmitParticle> EmitParticle::Create()
		{
			return std::make_unique<EmitParticle>();
		}

		// 初期化
		bool EmitParticle::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();

			// バッファ作成
			if (!CreateBuffers(device))
			{
				return false;
			}

			// シェーダー読み込み
			LoadShaders(device);

			// システムの初期化
			InitializeSystem(graphicsDevice);

			return true;
		}
		bool EmitParticle::InitializeSystem(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();
			Graphics::IContext* context = graphicsDevice->GetContext();

			Graphics::ITexture* uavTexture[] =
			{
				indirectArgs->uav.get(),
				paticleHeaders->uav.get()
			};
			context->SetUnorderedAccess(0, Phoenix::FND::ArraySize(uavTexture), uavTexture, nullptr);

			clearSystemCS->Activate(device);
			{
				clearSystemCS->Dispatch(device, 1, 1, 1);
			}
			clearSystemCS->Deactivate(device);

			clearParticleCS->Activate(device);
			{
				clearParticleCS->Dispatch(device, ceil(TotalParticleMax / (float)PARTICLE_PER_THREAD), 1, 1);
			}
			clearParticleCS->Deactivate(device);
		}

		// 終了化
		void EmitParticle::Finalize()
		{

		}

		// 更新
		void EmitParticle::UpdateCPU(Graphics::IGraphicsDevice* graphicsDevice, Math::Vector3 transform, float dt)
		{

		}

		void EmitParticle::UpdateGPU(Graphics::IGraphicsDevice* graphicsDevice, Math::Matrix worldTransform, float dt)
		{

		}

		void EmitParticle::Draw(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera)
		{

		}

		void EmitParticle::Burst(int num)
		{

		}

		void EmitParticle::Restart()
		{

		}

		bool EmitParticle::CreateBuffers(Graphics::IDevice* device)
		{
			indirectArgs = std::make_unique<GPUBuffer>();
			if (!indirectArgs->Initialize(device, sizeof(IndirectParticleNum) + sizeof(IndirectDispatchArgs) + sizeof(IndirectDispatchArgs) + sizeof(IndirectDrawArgsInstanced), 0, static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferAllowsRAWViews) | static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixResouceMiscFlag::ResouceMiscDrawindIrectArgs), nullptr))
			{
				return false;
			}

			emitterTable = std::make_unique<GPUBuffer>();
			if (!emitterTable->Initialize(device, sizeof(u32) * 2, sizeof(u32), static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferStructured), nullptr))
			{
				return false;
			}

			return true;
		}

		void EmitParticle::LoadShaders(Graphics::IDevice* device)
		{
			clearSystemCS = Graphics::IComputeShader::Create();
			clearParticleCS = Graphics::IComputeShader::Create();

			clearSystemCS->Load(device, "ClearSystemCS.cso");
			clearParticleCS->Load(device, "ClearParticleCS.cso");
		}
	}
}