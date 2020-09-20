#include "pch.h"
#include <list>
#include "ShaderDX11.h"
#include "Phoenix/OS/Path.h"
#include "Phoenix/FND/Util.h"
#include "../Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../Graphics/Context/Win/DirectX11/ContextDX11.h"
#include "../Source/Loader/ResourceManager.h"
#include "../Source/Graphics/Buffer/Win/DirectX11/BufferDX11.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�ŃO���t�B�b�N�X�f�o�C�X����I�u�W�F�N�g
		//****************************************************************************
		// ����
		std::unique_ptr<IShader> IShader::Create()
		{
			return std::make_unique<ShaderDX11>();
		}

		// �I����
		void ShaderDX11::Finalize()
		{
			FND::SafeRelease(inputLayout);
			FND::SafeRelease(pixelShader);
			FND::SafeRelease(geometryShader);
			FND::SafeRelease(vertexShader);
		}

		// ���_�V�F�[�_�[�ǂݍ���
		void ShaderDX11::LoadVS(IDevice* device, const char* csoNameOfVertexShader, PhoenixInputElementDesc* inputElementDesc, u32 numElements)
		{
#ifdef _WIN64
			const char* filename = Phoenix::OS::Path::Combine("..\\Data\\Shader\\", csoNameOfVertexShader);
#else
			const char* filename = Phoenix::OS::Path::Combine("..\\Data\\Shader\\", csoNameOfVertexShader);
#endif
			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			std::vector<D3D11_INPUT_ELEMENT_DESC> descList;
			for (u32 i = 0; i < numElements; i++)
			{
				D3D11_INPUT_ELEMENT_DESC desc = {};
				desc.SemanticName = static_cast<LPCSTR>(inputElementDesc[i].semanticName);
				desc.Format = static_cast<DXGI_FORMAT>(inputElementDesc[i].format);
				desc.InputSlotClass = static_cast<D3D11_INPUT_CLASSIFICATION>(inputElementDesc[i].inputSlotClass);
				desc.SemanticIndex = inputElementDesc[i].semanticIndex;
				desc.InputSlot = inputElementDesc[i].inputSlot;
				desc.AlignedByteOffset = inputElementDesc[i].alignedByteOffset;
				desc.InstanceDataStepRate = inputElementDesc[i].instanceDataStepRate;

				descList.emplace_back(desc);
			}

			std::unique_ptr<OS::IFileStream> file;
			file = OS::IFileStream::Create();
			file->Initialize(nullptr);

			const char* fullPass = OS::Path::GetFullPath(filename);
			if (file->Exists(fullPass)) ResourceManager::CreateVertexShaderAndInputLayout(d3dDevice, fullPass, &vertexShader, &inputLayout, descList.data(), numElements);
		}

		// �W�I���g���V�F�[�_�[�ǂݍ���
		void ShaderDX11::LoadGS(IDevice* device, const char* csoNameOfGeometryShader)
		{
#ifdef _WIN64
			const char* filename = Phoenix::OS::Path::Combine("..\\Data\\Shader\\", csoNameOfGeometryShader);
#else
			const char* filename = Phoenix::OS::Path::Combine("..\\Data\\Shader\\", csoNameOfGeometryShader);
#endif

			std::unique_ptr<OS::IFileStream> file;
			file = OS::IFileStream::Create();
			file->Initialize(nullptr);

			const char* fullPass = OS::Path::GetFullPath(filename);

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();
			if (file->Exists(fullPass)) ResourceManager::CreateGeometryShader(d3dDevice, fullPass, &geometryShader);
		}

		// �s�N�Z���V�F�[�_�[�ǂݍ���
		void ShaderDX11::LoadPS(IDevice* device, const char* csoNameOfPixelShader)
		{
#ifdef _WIN64
			const char* filename = Phoenix::OS::Path::Combine("..\\Data\\Shader\\", csoNameOfPixelShader);
#else
			const char* filename = Phoenix::OS::Path::Combine("..\\Data\\Shader\\", csoNameOfPixelShader);
#endif
			std::unique_ptr<OS::IFileStream> file;
			file = OS::IFileStream::Create();
			file->Initialize(nullptr);

			const char* fullPass = OS::Path::GetFullPath(filename);

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();
			if (file->Exists(fullPass)) ResourceManager::CreatePixelShader(d3dDevice, fullPass, &pixelShader);
		}

		// �V�F�[�_�[�J�n
		void ShaderDX11::Activate(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();

			d3dDeviceContext->IAGetInputLayout(&cachedInputLayout);
			d3dDeviceContext->VSGetShader(&cachedVertexShader, 0, 0);
			d3dDeviceContext->GSGetShader(&cachedGeometryShader, 0, 0);
			d3dDeviceContext->PSGetShader(&cachedPixelShader, 0, 0);

			if (vertexShader)
			{
				if (inputLayout)
				{
					d3dDeviceContext->IASetInputLayout(inputLayout);
				}
				d3dDeviceContext->VSSetShader(vertexShader, 0, 0);
			}
			if (geometryShader)
			{
				d3dDeviceContext->GSSetShader(geometryShader, 0, 0);
			}
			if (pixelShader)
			{
				d3dDeviceContext->PSSetShader(pixelShader, 0, 0);
			}
		}

		void ShaderDX11::ActivateVS(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();

			d3dDeviceContext->IAGetInputLayout(&cachedInputLayout);
			d3dDeviceContext->VSGetShader(&cachedVertexShader, 0, 0);

			d3dDeviceContext->IASetInputLayout(inputLayout);
			d3dDeviceContext->VSSetShader(vertexShader, 0, 0);
		}

		void ShaderDX11::ActivateGS(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();

			d3dDeviceContext->GSGetShader(&cachedGeometryShader, 0, 0);
			d3dDeviceContext->GSSetShader(geometryShader, 0, 0);
		}

		void ShaderDX11::ActivatePS(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();

			d3dDeviceContext->PSGetShader(&cachedPixelShader, 0, 0);
			d3dDeviceContext->PSSetShader(pixelShader, 0, 0);
		}

		// �V�F�[�_�[�I��
		void ShaderDX11::Deactivate(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();

			d3dDeviceContext->IASetInputLayout(cachedInputLayout);
			d3dDeviceContext->VSSetShader(cachedVertexShader, 0, 0);
			d3dDeviceContext->GSSetShader(cachedGeometryShader, 0, 0);
			d3dDeviceContext->PSSetShader(cachedPixelShader, 0, 0);
		}

		void ShaderDX11::DeactivateVS(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			d3dDeviceContext->IASetInputLayout(cachedInputLayout);
			d3dDeviceContext->VSSetShader(cachedVertexShader, 0, 0);
		}

		void ShaderDX11::DeactivateGS(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			d3dDeviceContext->GSSetShader(cachedGeometryShader, 0, 0);
		}

		void ShaderDX11::DeactivatePS(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			d3dDeviceContext->PSSetShader(cachedPixelShader, 0, 0);
		}

		//****************************************************************************
		// DirectX11�ŃR���s���[�g�V�F�[�_�[����I�u�W�F�N�g
		//****************************************************************************
		// ����
		std::unique_ptr<IComputeShader> IComputeShader::Create()
		{
			return std::make_unique<ComputeShaderDX11>();
		}

		// �I����
		void ComputeShaderDX11::Finalize()
		{
			FND::SafeRelease(computeShader);
		}

		// �R���s���[�g�V�F�[�_�[�ǂݍ���
		void ComputeShaderDX11::Load(IDevice* device, const char* csoNameOfComputeShader)
		{
#ifdef _WIN64
			const char* filename = Phoenix::OS::Path::Combine("..\\Data\\Shader\\", csoNameOfComputeShader);
#else
			const char* filename = Phoenix::OS::Path::Combine("..\\Data\\Shader\\", csoNameOfComputeShader);
#endif
			std::unique_ptr<OS::IFileStream> file;
			file = OS::IFileStream::Create();
			file->Initialize(nullptr);

			const char* fullPass = OS::Path::GetFullPath(filename);

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();
			if (file->Exists(fullPass)) ResourceManager::CreateComputeShader(d3dDevice, fullPass, &computeShader);
		}

		// �V�F�[�_�[�J�n
		void ComputeShaderDX11::Activate(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();

			d3dDeviceContext->CSGetShader(&cachedcomputeShader, 0, 0);
			d3dDeviceContext->CSSetShader(computeShader, 0, 0);
		}

		// �V�F�[�_�[�I��
		void ComputeShaderDX11::Deactivate(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			d3dDeviceContext->CSSetShader(cachedcomputeShader, 0, 0);
		}

		// �R���s���[�g�V�F�[�_�[���s
		void ComputeShaderDX11::Dispatch(IDevice* device, u32 x, u32 y, u32 z)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			d3dDeviceContext->Dispatch(x, y, z);
		}

		// �R���s���[�g�V�F�[�_�[���s
		void ComputeShaderDX11::DispatchIndirect(IDevice* device, IBuffer* buffer, u32 offset)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			ID3D11Buffer* d3dBuffer = static_cast<BufferDX11*>(buffer)->GetD3DBuffer();

			d3dDeviceContext->DispatchIndirect(d3dBuffer, offset);
		}
	} // namespace Graphics
} // namespace Phoenix