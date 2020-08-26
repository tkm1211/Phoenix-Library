#pragma once

#include <memory>
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/Graphics/Buffer.h"
#include "Phoenix/FrameWork/ComputeShader/ComputeShader.h"


namespace Phoenix
{
	namespace FrameWork
	{
		struct SortData
		{
			f32 key;	// �\�[�g�������l
			u32 index;	// �ꏏ�Ƀ\�[�g�����Index
		};

		class BitonicSort
		{
		public:
			static const u32 threadNumX = 64;

		private:
			struct ConstantBuffer
			{
				f32 inc;
				f32 dir;
				f32 dummy[2];
			};

		private:
			std::unique_ptr<Graphics::IComputeShader> shader;
			std::unique_ptr<Graphics::IBuffer> buffer;
			std::unique_ptr<Graphics::ITexture> bufferUAV;

			std::unique_ptr<Graphics::IBuffer> constantBuffer;

		public:
			BitonicSort() {}
			~BitonicSort() { Finalize(); }

		public:
			// ����
			static std::unique_ptr<BitonicSort> Create();

			// ������
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			// �I����
			void Finalize();

			/// <summary>
			/// �o�C�g�j�b�N�\�[�g�̎��s�֐�
			/// </summary>
			/// <param name="graphicsDevice"> �O���t�B�b�N�X�f�o�C�X </param>
			/// <param name="data"> �f�[�^�z��̐擪�A�h���X </param>
			/// <param name="dataMaxSize"> �f�[�^�z��̍ő�T�C�Y (���� : 64�ȉ�) </param>
			void Run(Graphics::IGraphicsDevice* graphicsDevice, SortData* data, u32 dataMaxSize);

		private:
			/// <summary>
			/// �o�b�t�@�̍쐬�֐�
			/// </summary>
			/// <param name="device"> �f�o�C�X </param>
			/// <param name="data"> �f�[�^�z��̐擪�A�h���X </param>
			/// <param name="dataMaxSize"> �f�[�^�z��̍ő�T�C�Y </param>
			bool Set(Graphics::IDevice* device, SortData* data, u32 dataMaxSize);
		};
	}
}