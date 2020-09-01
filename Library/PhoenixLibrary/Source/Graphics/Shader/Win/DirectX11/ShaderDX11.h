#pragma once

#include "Phoenix/Graphics/Shader.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�ŃV�F�[�_�[����I�u�W�F�N�g
		//****************************************************************************
		class ShaderDX11 final : public IShader
		{
		private:
			ID3D11InputLayout* inputLayout = nullptr;
			ID3D11VertexShader* vertexShader = nullptr;
			ID3D11GeometryShader* geometryShader = nullptr;
			ID3D11PixelShader* pixelShader = nullptr;

		private:
			ID3D11InputLayout* cachedInputLayout = nullptr;
			ID3D11VertexShader* cachedVertexShader = nullptr;
			ID3D11GeometryShader* cachedGeometryShader = nullptr;
			ID3D11PixelShader* cachedPixelShader = nullptr;

		public:
			ShaderDX11() {}
			~ShaderDX11() { Finalize(); }

		public:
			// ������
			bool Initialize() override { return true; }

			// �I����
			void Finalize() override;

			// ���_�V�F�[�_�[�ǂݍ���
			void LoadVS(IDevice* device, const char* csoNameOfVertexShader, PhoenixInputElementDesc* inputElementDesc, u32 numElements) override;

			// �W�I���g���V�F�[�_�[�ǂݍ���
			void LoadGS(IDevice* device, const char* csoNameOfGeometryShader) override;

			// �s�N�Z���V�F�[�_�[�ǂݍ���
			void LoadPS(IDevice* device, const char* csoNameOfPixelShader) override;

			// �V�F�[�_�[�J�n
			void Activate(IDevice* device) override;
			void ActivateVS(IDevice* device) override;
			void ActivateGS(IDevice* device) override;
			void ActivatePS(IDevice* device) override;

			// �V�F�[�_�[�I��
			void Deactivate(IDevice* device) override;
			void DeactivateVS(IDevice* device) override;
			void DeactivateGS(IDevice* device) override;
			void DeactivatePS(IDevice* device) override;
		};

		//****************************************************************************
		// DirectX11�ŃR���s���[�g�V�F�[�_�[����I�u�W�F�N�g
		//****************************************************************************
		class ComputeShaderDX11 : public IComputeShader
		{
		private:
			ID3D11ComputeShader* computeShader = nullptr;

		private:
			ID3D11ComputeShader* cachedcomputeShader = nullptr;

		public:
			ComputeShaderDX11() {}
			~ComputeShaderDX11() { Finalize(); }

		public:
			// ������
			bool Initialize() override { return true; }

			// �I����
			void Finalize() override;

			// �R���s���[�g�V�F�[�_�[�ǂݍ���
			void Load(IDevice* device, const char* csoNameOfComputeShader) override;

			// �V�F�[�_�[�J�n
			void Activate(IDevice* device) override;

			// �V�F�[�_�[�I��
			void Deactivate(IDevice* device) override;

			// �R���s���[�g�V�F�[�_�[���s
			void Dispatch(IDevice* device, u32 x, u32 y, u32 z) override;

			// �R���s���[�g�V�F�[�_�[���s
			void DispatchIndirect(IDevice* device, IBuffer* buffer, u32 offset) override;
		};
	} // namespace Graphics
} // namespace Phoenix