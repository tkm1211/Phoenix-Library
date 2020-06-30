#pragma once

#include <functional>
#include <vector>
#include "Phoenix/FND/NonCopyable.h"
#include "Phoenix/Graphics/Shader.h"
#include "Phoenix/FrameWork/Material.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// �V�X�e���V�F�[�_�[�^�C�v
		enum class SystemShader
		{
			Wireframe,
			Normals,
			Line,
			Basic,
			Standard,
			SkyDome,

			TypeNum
		};

		using CreateMaterialFunction = std::function<std::shared_ptr<IMaterial>()>;

		//============================================================================
		// �V�F�[�_�[���
		//----------------------------------------------------------------------------
		struct ShaderInfo
		{
			std::string name;
			std::shared_ptr<Graphics::IShader> shader;
			ShaderID id;
			CreateMaterialFunction create_material;
		};

		//============================================================================
		// �V�F�[�_�[���C�u����
		//----------------------------------------------------------------------------
		class ShaderLibrary final : public FND::NonCopyable
		{
		public:
			ShaderLibrary() {}
			~ShaderLibrary() { Finalize(); }

			// ������
			bool Initialize(Graphics::IDevice* device, u32 shader_count);

			// �I����
			void Finalize();

			// �V�F�[�_�[�o�^
			template<class TShader, class TMaterial>
			ShaderID RegisterShader(Graphics::IDevice* device, const char* name)
			{
				return RegisterShader(device, name, TShader::Create(), []() { return std::make_shared<TMaterial>(); });
			}

			// �V�F�[�_�[�擾
			std::shared_ptr<Graphics::IShader>& GetShader(ShaderID shader_id);

			// �V�X�e���V�F�[�_�[�擾
			std::shared_ptr<Graphics::IShader>& GetShader(SystemShader type);

			// �V�F�[�_�[ID�擾
			ShaderID GetShaderId(SystemShader type) const;

			// �V�F�[�_�[���擾
			const ShaderInfo& GetShaderInfo(ShaderID shader_id) const { return m_shader_infos.at(shader_id); }
			const std::vector<ShaderInfo>& GetShaderInfos() const { return m_shader_infos; }

			// �}�e���A������
			std::shared_ptr<IMaterial> CreateMaterial(ShaderID shader_id);

		private:
			// �V�F�[�_�[�o�^
			ShaderID RegisterShader(Graphics::IDevice* device, const char* name, std::shared_ptr<Graphics::IShader> shader, CreateMaterialFunction create_material_function);

		private:
			std::vector<ShaderInfo>		m_shader_infos;
			ShaderID					m_system_shader_ids[static_cast<int>(SystemShader::TypeNum)] = { 0 };
		};

	}	// namespace fwk
}	// namespace ys