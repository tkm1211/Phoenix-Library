#pragma once

#include "Phoenix/Math/Color.h"
#include "Phoenix/Graphics/Texture.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �}�e���A��
		//****************************************************************************
		struct Material
		{
		};

		//****************************************************************************
		// �x�[�V�b�N�}�e���A��
		//****************************************************************************
		struct BasicMaterial : public Material
		{
			Math::Color color = Math::Color::White;
		};

		//****************************************************************************
		// �X�^���_�[�h�}�e���A��
		//****************************************************************************
		struct StandardMaterial : public Material
		{
			ITexture* diffusemap = nullptr;
			ITexture* normalmap = nullptr;
			ITexture* specularmap = nullptr;
			Math::Color emissive = Math::Color::Black;
			Math::Color diffuse = Math::Color::White;
			Math::Color specular = Math::Color::Black;
		};

		//****************************************************************************
		// �X�J�C�h�[���}�e���A��
		//****************************************************************************
		struct SkyDomeMaterial : public Material
		{
			ITexture* cubemap = nullptr;
		};

	} // namespace Graphics
} // namespace Phoenix