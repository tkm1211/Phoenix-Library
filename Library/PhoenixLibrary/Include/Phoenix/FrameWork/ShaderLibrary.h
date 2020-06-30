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
		// システムシェーダータイプ
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
		// シェーダー情報
		//----------------------------------------------------------------------------
		struct ShaderInfo
		{
			std::string name;
			std::shared_ptr<Graphics::IShader> shader;
			ShaderID id;
			CreateMaterialFunction create_material;
		};

		//============================================================================
		// シェーダーライブラリ
		//----------------------------------------------------------------------------
		class ShaderLibrary final : public FND::NonCopyable
		{
		public:
			ShaderLibrary() {}
			~ShaderLibrary() { Finalize(); }

			// 初期化
			bool Initialize(Graphics::IDevice* device, u32 shader_count);

			// 終了化
			void Finalize();

			// シェーダー登録
			template<class TShader, class TMaterial>
			ShaderID RegisterShader(Graphics::IDevice* device, const char* name)
			{
				return RegisterShader(device, name, TShader::Create(), []() { return std::make_shared<TMaterial>(); });
			}

			// シェーダー取得
			std::shared_ptr<Graphics::IShader>& GetShader(ShaderID shader_id);

			// システムシェーダー取得
			std::shared_ptr<Graphics::IShader>& GetShader(SystemShader type);

			// シェーダーID取得
			ShaderID GetShaderId(SystemShader type) const;

			// シェーダー情報取得
			const ShaderInfo& GetShaderInfo(ShaderID shader_id) const { return m_shader_infos.at(shader_id); }
			const std::vector<ShaderInfo>& GetShaderInfos() const { return m_shader_infos; }

			// マテリアル生成
			std::shared_ptr<IMaterial> CreateMaterial(ShaderID shader_id);

		private:
			// シェーダー登録
			ShaderID RegisterShader(Graphics::IDevice* device, const char* name, std::shared_ptr<Graphics::IShader> shader, CreateMaterialFunction create_material_function);

		private:
			std::vector<ShaderInfo>		m_shader_infos;
			ShaderID					m_system_shader_ids[static_cast<int>(SystemShader::TypeNum)] = { 0 };
		};

	}	// namespace fwk
}	// namespace ys