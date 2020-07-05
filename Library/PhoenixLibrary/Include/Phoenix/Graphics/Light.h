#pragma once

#include "Phoenix/FND/Base.h"
#include "Phoenix/Math/AABB.h"
#include "Phoenix/Math/Color.h"
#include "Phoenix/Math/Vector.h"


namespace Phoenix
{
	namespace Graphics
	{
		static const u32 MaxDirLights = 2;
		static const u32 MaxBoxLights = 2;
		static const u32 MaxPointLights = 4;

		//****************************************************************************
		// �t�H�O
		//****************************************************************************
		struct ExpFog
		{
			s32 enabled = false;
			f32 density = 0.0f;
			f32 range = 0.0f;
			f32 fpad = 0.0f;
			Math::Color color = Math::Color::White;
		};

		//****************************************************************************
		// ���C�g���
		//****************************************************************************
		struct Light
		{
			Math::Color ambient;
			Math::Color diffuse;
			Math::Color specular;
		};

		//****************************************************************************
		// �f�B���N�V���i�����C�g
		//****************************************************************************
		struct DirLight : public Light
		{
			Math::Vector3 direction;
			f32 padding1;
		};

		//****************************************************************************
		// �{�b�N�X���C�g
		//****************************************************************************
		struct BoxLight : public DirLight
		{
			Math::Vector3 min;
			f32 padding2;
			Math::Vector3 max;
			f32 padding3;
			Math::Vector4 attenuation;
		};

		//****************************************************************************
		// �|�C���g���C�g
		//****************************************************************************
		struct PointLight : public Light
		{
			Math::Vector4 attenuation;
			Math::Vector4 position; //(xyz, radius)
		};

		//****************************************************************************
		// ���C�g��
		//****************************************************************************
		struct LightEnvironment
		{
			DirLight dirLights[MaxDirLights];
			BoxLight boxLights[MaxBoxLights];
			PointLight pointLights[MaxPointLights];
			u32 numDirLights;
			u32 numBoxLights;
			u32 numPointLights;
			u32 padding1;
		};

		//****************************************************************************
		// ���C�e�B���O�X�e�[�g����C���^�[�t�F�[�X
		//****************************************************************************
		class ILightingState : public FND::Base
		{
		public:
			// �S�Ẵ��C�g�o�^����
			virtual void ClearLight() = 0;

			// �f�B���N�V���i�����C�g�o�^
			virtual void RegisterDirLight(DirLight* light) = 0;

			// �f�B���N�V���i�����C�g�o�^����
			virtual void UnregisterDirLight(DirLight* light) = 0;

			// ���C���f�B���N�V���i�����C�g�擾
			virtual DirLight* GetProminentDirLight() = 0;

			// �f�t�H���g�f�B���N�V���i�����C�g�擾
			virtual DirLight* GetDefaultDirLight() = 0;

			// �{�b�N�X���C�g�o�^
			virtual void RegisterBoxLight(BoxLight* light) = 0;

			// �{�b�N�X���C�g�o�^����
			virtual void UnregisterBoxLight(BoxLight* light) = 0;

			// �|�C���g���C�g�o�^
			virtual void RegisterPointLight(PointLight* light) = 0;

			// �|�C���g���C�g�o�^����
			virtual void UnregisterPointLight(PointLight* light) = 0;

			// ���C�g���X�V
			virtual void UpdateLightEnvironment(LightEnvironment& env, const Math::AABB& bounds) = 0;
		};

	} // namespace Graphics
} // namespace Phoenix