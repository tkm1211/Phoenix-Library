#pragma once

#include <memory>
#include "Phoenix\Graphics\GraphicsDevice.h"
#include "Phoenix\Graphics\Shader.h"
#include "Phoenix\Graphics\Camera.h"
#include "Phoenix\Math\PhoenixMath.h"


namespace Phoenix
{
	namespace FrameWork
	{
		class GPUBuffer
		{
		public:
			std::unique_ptr<Graphics::IBuffer> buffer;
			std::unique_ptr<Graphics::ITexture> srv;
			std::unique_ptr<Graphics::ITexture> uav;

		public:
			GPUBuffer() {}
			~GPUBuffer() {}

		public:
			// 生成
			static std::unique_ptr<GPUBuffer> Create();

			// 初期化
			bool Initialize(Graphics::IDevice* device, u32 byteWidth, u32 structureByteStride, s32 miscFlags, void* initData);

			// 終了化
			void Finalize();
		};

		class GPUParticle
		{
		private:
			struct IndirectDispatchArgs
			{
				u32 threadGroupCountX = 0;
				u32 threadGroupCountY = 0;
				u32 threadGroupCountZ = 0;
			};
			struct IndirectDrawArgsInstanced
			{
				u32 vertexCountPerInstance = 0;
				u32 instanceCount = 0;
				u32 startVertexLocation = 0;
				u32 startInstanceLocation = 0;
			};

			struct Particle
			{
				Math::Vector3 position;
				f32 mass;
				Math::Vector3 force;
				f32 rotationalVelocity;
				Math::Vector3 velocity;
				f32 maxLife;
				Math::Vector2 sizeBeginEnd;
				f32 life;
				u32 colorMirror;
			};

			struct ParticleCounters
			{
				u32 aliveCount;
				u32 deadCount;
				u32 realEmitCount;
				u32 aliveCount_afterSimulation;
			};

			struct EmittedParticleCB
			{
				Math::Matrix emitterWorld;

				u32	emitCount;
				u32	emitterMeshIndexCount;
				u32	emitterMeshVertexPositionStride;
				f32	emitterRandomness;

				f32	particleSize;
				f32	particleScaling;
				f32	particleRotation;
				u32	particleColor;

				f32 particleRandomFactor;
				f32 particleNormalFactor;
				f32 particleLifeSpan;
				f32 particleLifeSpanRandomness;

				f32	particleMass;
				f32	particleMotionBlurAmount;
				f32	emitterOpacity;
				u32	emitterMaxParticleCount;

				f32 SPH_h;				// smoothing radius
				f32 SPH_h_rcp;			// 1.0f / smoothing radius
				f32 SPH_h2;				// smoothing radius ^ 2
				f32 SPH_h3;				// smoothing radius ^ 3

				f32 SPH_poly6_constant;	// precomputed Poly6 kernel constant term
				f32 SPH_spiky_constant;	// precomputed Spiky kernel function constant term
				f32 SPH_K;				// pressure constant
				f32 SPH_p0;				// reference density

				f32 SPH_e;					// viscosity constant
				u32 SPH_ENABLED;			// is SPH enabled?
				f32 emitterFixedTimestep;	// we can force a fixed timestep (>0) onto the simulation to avoid blowing up
				f32 particleEmissive;
			};

			struct FrameTimeCB
			{
				f32 frameTime;
				f32 dummy[3];
			};

		private:
			static const u32 emitterMaxSize = 256;
			static const u32 particleMaxSize = 1000;

			static const u32 ARGUMENTBUFFER_OFFSET_DISPATCHEMIT = 0;
			static const u32 ARGUMENTBUFFER_OFFSET_DISPATCHSIMULATION = ARGUMENTBUFFER_OFFSET_DISPATCHEMIT + (3 * 4);
			static const u32 ARGUMENTBUFFER_OFFSET_DRAWPARTICLES = ARGUMENTBUFFER_OFFSET_DISPATCHSIMULATION + (3 * 4);

		private:
			std::unique_ptr<Graphics::IShader> drawShader;

			std::unique_ptr<Graphics::IComputeShader> kickoffUpdateCS;
			std::unique_ptr<Graphics::IComputeShader> finishUpdateCS;
			std::unique_ptr<Graphics::IComputeShader> emitCS;
			std::unique_ptr<Graphics::IComputeShader> emitCSVolume;
			std::unique_ptr<Graphics::IComputeShader> emitCSFromMesh;
			std::unique_ptr<Graphics::IComputeShader> sphpartitionCS;
			std::unique_ptr<Graphics::IComputeShader> sphpartitionoffsetsCS;
			std::unique_ptr<Graphics::IComputeShader> sphpartitionoffsetsresetCS;
			std::unique_ptr<Graphics::IComputeShader> sphdensityCS;
			std::unique_ptr<Graphics::IComputeShader> sphforceCS;
			std::unique_ptr<Graphics::IComputeShader> simulateCS;
			std::unique_ptr<Graphics::IComputeShader> simulateCSSorting;
			std::unique_ptr<Graphics::IComputeShader> simulateCSDepthCollisions;
			std::unique_ptr<Graphics::IComputeShader> simulateCSSortingDepthCollisions;

			std::unique_ptr<GPUBuffer> particleBuffer;
			std::unique_ptr<GPUBuffer> aliveList[2];
			std::unique_ptr<GPUBuffer> deadList;
			std::unique_ptr<GPUBuffer> distanceBuffer;

			std::unique_ptr<GPUBuffer> counterBuffer;
			std::unique_ptr<GPUBuffer> indirectBuffers;

			std::unique_ptr<Graphics::IBuffer> emittedParticleCB;
			std::unique_ptr<Graphics::IBuffer> frameTimeCB;

		private:
			float fixedTimeStep = 1.0f / 60.0f; // -1 : variable timestep; >=0 : fixed timestep

			f32 emit = 0.0f;
			s32 burst = 0;
			Math::Vector3 center;

			f32 size = 0.1f;
			f32 randomFactor = 1.0f;
			f32 normalFactor = 1.0f;
			f32 count = 0.0f;
			f32 life = 10.0f;
			f32 randomLife = 1.0f;
			f32 scaleX = 1.0f;
			f32 scaleY = 1.0f;
			f32 rotation = 0.0f;
			f32 motionBlurAmount = 0.0f;
			f32 mass = 1.0f;

		public:
			GPUParticle() {}
			~GPUParticle() { Finalize(); }

		public:
			// 生成
			static std::unique_ptr<GPUParticle> Create();

			// 初期化
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			// 終了化
			void Finalize();

			// 更新
			void UpdateCPU(Graphics::IGraphicsDevice* graphicsDevice, Math::Vector3 transform, float dt);
		
			void UpdateGPU(Graphics::IGraphicsDevice* graphicsDevice, Math::Matrix worldTransform, float dt);

			void Draw(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera);

			void Burst(int num);

			void Restart();

			bool CreateBuffers(Graphics::IDevice* device);

			void LoadShaders(Graphics::IDevice* device);
		};

		class EmitParticle
		{
		private:
			struct IndirectParticleNum
			{
				u32 particleNum;
				u32 prevParticleNum;
			};
			struct IndirectDispatchArgs
			{
				u32 threadGroupCountX = 0;
				u32 threadGroupCountY = 0;
				u32 threadGroupCountZ = 0;
			};
			struct IndirectDrawArgsInstanced
			{
				u32 vertexCountPerInstance = 0;
				u32 instanceCount = 0;
				u32 startVertexLocation = 0;
				u32 startInstanceLocation = 0;
			};

			struct emitterHeader
			{
				u32 emitHead;
				u32 emitSize;
				u32 particleHead;
				u32 particleSize;
			};
			struct particleHeader
			{
				u32 tag; // alive + emitterID + index
				f32 depth;
			};

		private:
			static const u32 IA_PARTICLE_COUNTER = 0;
			static const u32 IA_PREV_PARTICLE_COUNTER = IA_PARTICLE_COUNTER + 4;

			static const u32 PARTICLE_PER_THREAD = 256; // Dispatch(ceil(TotalParticleMax / (float)PARTICLE_PER_THREAD), 1 ,1)

			static const u32 INVALID_TAG = 0xffffffff;

			static const u32 TotalEmitterMax = 256;
			static const u32 TotalParticleMax = 1024;

		private:
			std::unique_ptr<GPUBuffer> indirectArgs;

			std::unique_ptr<GPUBuffer> emitterTable;
			std::unique_ptr<GPUBuffer> emitterHeaders;
			std::unique_ptr<GPUBuffer> emitterBinary;

			std::unique_ptr<GPUBuffer> paticleHeaders;

			std::unique_ptr<Graphics::IComputeShader> clearSystemCS;
			std::unique_ptr<Graphics::IComputeShader> clearParticleCS;

		private:
			u32 emitterTable[TotalEmitterMax];

		public:
			EmitParticle() {}
			~EmitParticle() {}

		public:
			// 生成
			static std::unique_ptr<EmitParticle> Create();

			// 初期化
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice);
			bool InitializeSystem(Graphics::IGraphicsDevice* graphicsDevice);

			// 終了化
			void Finalize();

			// 更新
			void UpdateCPU(Graphics::IGraphicsDevice* graphicsDevice, Math::Vector3 transform, float dt);

			void UpdateGPU(Graphics::IGraphicsDevice* graphicsDevice, Math::Matrix worldTransform, float dt);

			void Draw(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera);

			void Burst(int num);

			void Restart();

			bool CreateBuffers(Graphics::IDevice* device);

			void LoadShaders(Graphics::IDevice* device);
		};
	}
}